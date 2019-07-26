/** lock.cpp
	Copyright (C) 2008 Battelle Memorial Institute
	
	@file lock.h
	@defgroup locking Locking memory
	@ingroup module_api

	Memory locking is implemented using compare and exchange methods.  
	Any time more than one object can concurrently write to the same
	region of memory, it is necessary to implement locking to prevent
	one object from overwriting the changes made by another.  
 @{	  
 **/

#include "gldcore.h"

/* TODO: remove these when reentrant code is completed */
extern GldMain *my_instance;

//#define LOCKTRACE // enable this to trace locking events back to variables
#define MAXSPIN 0x7fffffff

/** Determine locking method 
 **/
#define METHOD0 /* locking method as of 3.0 */

/************************************************************************************** 
   IMPORTANT NOTE: it is vital that the platform specific implementations be tested for
   first because some compilers support __sync_bool_compare_and_swap on platforms that
   have a native (often better) implementation of atomic operations.
 **************************************************************************************/
#if defined(__APPLE__)
	//#include <libkern/OSAtomic.h>
	#include <atomic>
	//#define atomic_compare_and_swap(thevalue, oldvalue, newvalue) OSAtomicCompareAndSwap32Barrier(oldvalue, newvalue, (volatile int32_t *) thevalue)
	#define atomic_compare_and_swap(thevalue, oldvalue, newvalue) std::atomic_compare_exchange_strong((std::atomic<LOCKVAR>*)thevalue,(LOCKVAR*)&oldvalue,(LOCKVAR)newvalue)
	//#define atomic_increment(ptr) OSAtomicIncrement32Barrier((volatile int32_t *) ptr)
	#define atomic_increment(ptr) std::atomic_fetch_add((std::atomic<LOCKVAR>*)ptr,1)
#elif defined(WIN32) && !defined __MINGW32__
	#include <intrin.h>
	#pragma intrinsic(_InterlockedCompareExchange)
	#pragma intrinsic(_InterlockedIncrement)
	#define atomic_compare_and_swap(dest, comp, xchg) (_InterlockedCompareExchange((volatile LOCKVAR *) dest, xchg, comp) == comp)
	#define atomic_increment(ptr) _InterlockedIncrement((volatile LOCKVAR *) ptr)
	#ifndef inline
		#define inline __inline
	#endif
#elif defined HAVE___SYNC_BOOL_COMPARE_AND_SWAP
	#define atomic_compare_and_swap __sync_bool_compare_and_swap
	#ifdef HAVE___SYNC_ADD_AND_FETCH
		#define atomic_increment(ptr) __sync_add_and_fetch((volatile LOCKVAR *)ptr, 1)
	#else
		static inline LOCKVAR atomic_increment(LOCKVAR *ptr)
		{
			LOCKVAR value;
			do {
				value = *(volatile LOCKVAR *)ptr;
			} while (!__sync_bool_compare_and_swap((volatile LOCKVAR*)ptr, value, value + 1));
			return value;
		}
	#endif
#else
	#error "Locking is not supported on this system"
#endif

/** Enable lock trace 
 **/
#ifdef LOCKTRACE // this code should only be used in case of mystery lock timeouts
typedef struct s_locklist {
	const char *name;
	LOCKVAR *lock;
	LOCKVAR last_value;
	struct s_locklist *next;
} LOCKLIST;
LOCKLIST *locklist = NULL;

/** Register a lock trace
 **/
void register_lock(const char *name, LOCKVAR *lock)
{
	LOCKLIST *item = (LOCKLIST*)malloc(sizeof(LOCKLIST));
	item->name = name;
	item->lock = lock;
	item->last_value = *lock;
	if ( *lock!=0 )
		printf("%s lock initial value not zero (%d)\n", name, *lock);
	item->next = locklist;
	locklist = item;
}
/** Check a lock trace
 **/
void check_lock(LOCKVAR *lock, bool write, bool unlock)
{
	LOCKLIST *item;

	// lock locklist
	static unsigned int check_lock=0;
	unsigned int timeout = MAXSPIN;
	LOCKVAR value;
	do {
		value = check_lock;
		if ( timeout--==0 ) 
			throw_exception("check lock timeout");
	} while ((value&1) || !atomic_compare_and_swap(&check_lock, value, value + 1));

	for ( item=locklist ; item!=NULL ; item=item->next )
	{
		if ( item->lock==lock )
			break;
	}
	if ( item==NULL )
	{
		printf("%s %slock(%p) = %d (unregistered)\n", 
			write?"write":"read ", 
			unlock?"un":"  ",
			lock,
			*lock);
		register_lock("unregistered",lock);
	}
	else 
	{
		bool no_lock = unlock&&((*lock&1)!=1);
//		bool damage = abs(item->last_value-*lock)>1;
//		if ( damage ) // found a registered lock that was damaged
//			printf("%s %slock(%p) = %d (%s) - possible damage (last=%d)\n", write?"write":"read ", unlock?"un":"  ",lock,*lock,item->name, item->last_value);
		if ( no_lock ) // found a registered lock that was not locked
			printf("%s %slock(%p) = %d (%s) - no lock detected (last=%d)\n", write?"write":"read ", unlock?"un":"  ",lock,*lock,item->name, item->last_value);
		item->last_value = *lock;
	}

	// unlock locklist
	atomic_increment(&check_lock);
}
#else
#define check_lock(X,Y,Z)
void register_lock(LOCKVAR *lock)
{
	// do nothing
}
#endif

#if defined METHOD0 
/**********************************************************************************
 * SINGLE LOCK METHOD
 **********************************************************************************/

/* This locking method uses a standard spinlock.
   The lock/unlock operations work as follows:
   (1) a lock is attempted when the low bit of the lock value is 0
   (2) an atomic compare-and-swap (CAS) operation is performed to take the lock by setting the low bit to 1
   (3) if the CAS operation fails, the lock process starts over at (1)
   (4) to unlock the lock value is incremented (which clears the low bit and increments the lock count).
 */
/** Read lock
 **/
extern "C" void rlock(LOCKVAR *lock)
{
	LOCKVAR timeout = MAXSPIN;
	LOCKVAR value;
	check_lock(lock,false,false);
	atomic_increment(&my_instance->rlock_count);
	do {
		value = (*lock);
		atomic_increment(&my_instance->rlock_spin);
		if ( timeout--==0 ) 
			throw_exception("read lock timeout");
	} while ((value&1) || !atomic_compare_and_swap(lock, value, value + 1));
}
/** Write lock 
 **/
extern "C" void wlock(LOCKVAR *lock)
{
	LOCKVAR timeout = MAXSPIN;
	LOCKVAR value;	
	check_lock(lock,true,false);
	atomic_increment(&my_instance->wlock_count);
	do {
		value = (*lock);
		atomic_increment(&my_instance->wlock_spin);
		if ( timeout--==0 ) 
			throw_exception("write lock timeout");
	} while ((value&1) || !atomic_compare_and_swap(lock, value, value + 1));
}
/** Read unlock
 **/
extern "C" void runlock(LOCKVAR *lock)
{
	check_lock(lock,false,true);
	atomic_increment(lock);
}
/** Write unlock
 **/
extern "C" void wunlock(LOCKVAR *lock)
{
	check_lock(lock,true,true);
	atomic_increment(lock);
}

#elif defined METHOD1 
/**********************************************************************************
 * WEAK R/W LOCK METHOD
 **********************************************************************************/

/* This locking method uses a modified double spinlock
   The read lock operation works as follows:
   (1) a lock is attempted when the low bit is set to zero
   (2) an atomic compare-and-swap (CAS) operation is performed to take the lock by setting the high bit to 1
   (3) if the CAS operation fails, the lock process starts over at (1)
   (4) to unlock the lock value is set to zero (which clears all the bits)
   The write lock operation works as follows:
   (1) a lock is attempted when both the low and high bits are zero
   (2) an atomic CAS operation is performed to take the lock by setting the low bit to 1
   (3) to unlock the lock value is set to zero (which clears all the bits)
 */
extern "C" void rlock(LOCKVAR *lock)
{
	LOCKVAR timeout = MAXSPIN;
	LOCKVAR value;
	atomic_increment(&my_instance->get_exec()->rlock_count);
	do {
		value = (*lock);
		if ( timeout--==0 ) 
			throw_exception("read lock timeout");
	} while ((value&1) || !atomic_compare_and_swap(lock, value, value|0x80000000));
}
extern "C" void wlock(LOCKVAR *lock)
{
	LOCKVAR timeout = MAXSPIN;
	LOCKVAR value;
	atomic_increment(&my_instance->get_exec()->wlock_count);
	do {
		value = (*lock);
		if ( timeout--==0 ) 
			throw_exception("write lock timeout");
	} while ((value&0x80000001) || !atomic_compare_and_swap(lock, value, value + 1));
}
extern "C" void wunlock(LOCKVAR *lock)
{
	*lock = 0;
}
extern "C" void runlock(LOCKVAR *lock)
{
	*lock = 0;
}

#elif defined METHOD2
/**********************************************************************************
 * STRONG R/W LOCK METHOD
 **********************************************************************************/

#define WBIT 0x80000000
#define RBITS 0x7FFFFFFF

extern "C" void rlock(LOCKVAR *lock)
{
	LOCKVAR timeout = MAXSPIN;
	LOCKVAR test;
	my_instance->get_exec()->rlock_count++;
	// 1. Wait for exclusive write lock to be released, if any
	// 2. Increment reader counter
	do {
		test = *lock;
		if ( timeout--==0 ) 
			throw_exception("read lock timeout");
	} while (test & WBIT || !atomic_compare_and_swap(lock, test, test + 1));
}

extern "C" void wlock(LOCKVAR *lock)
{
	LOCKVAR timeout = MAXSPIN;
	LOCKVAR test;
	my_instance->get_exec()->wlock_count++;
	// 1. Wait for exclusive write lock to be released, if any
	// 2. Take exclusive write lock
	do {
		test = *lock;
		if ( timeout--==0 ) 
			throw_exception("write lock timeout");
	} while (test & WBIT || !atomic_compare_and_swap(lock, test, test | WBIT));
	// 3. Wait for readers to complete before proceeding
	while ((*lock) & RBITS);
}

extern "C" void runlock(LOCKVAR *lock)
{
	LOCKVAR test;

	// Decrement the reader counter
	do {
		test = *lock;
	} while (!atomic_compare_and_swap(lock, test, test - 1));
}

extern "C" void wunlock(LOCKVAR *lock)
{
	LOCKVAR test;

	// Release write lock
	do {
		test = *lock;
	} while (!atomic_compare_and_swap(lock, test, test & RBITS));
}

#elif defined METHOD3
/**********************************************************************************
 * SEQLOCK METHOD
 **********************************************************************************/

// Start a loop for reading values
#define rlock(lock) {          \
	LOCKVAR lock_tmp;      \
	do {                        \
		lock_tmp = *lock;

// End the loop, checking that no writes occurred or are in progress
#define runlock(lock) } while (lock_tmp != *(lock) || lock_tmp & 1);   \
}

inline void wlock(LOCKVAR *lock)
{
	LOCKVAR test;

	// 1. Wait for exclusive write lock to be released, if any
	// 2. Take exclusive write lock
	do {
		test = *lock;
	} while (test & 1 || !atomic_compare_and_swap(lock, test, test + 1));
}

inline void wunlock(LOCKVAR *lock)
{
	LOCKVAR test;
	// Release write lock
	do {
		test = *lock;
	} while (!atomic_compare_and_swap(lock, test, test + 1));
}

/** @} **/
#endif
