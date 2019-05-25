/* 	File: complex.h 
	Copyright (C) 2008, Battelle Memorial Institute

	The complex number implementation is specifically designed to use in power systems
	calculations.  Hence it supports some things that a peculiar to power systems engineering.
	In particular, the imaginary portion can be identified using 'i', 'j', or 'd' or 'r' (for polar
	coordinates).
 */

#ifndef _COMPLEX_H
#define _COMPLEX_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

/*	Typedef: CNOTATION
	
	See <e_complexnotation>

	Enum: e_complexnotation
	I = 'i' - rectangular notation using 'i'
	J = 'j' - rectangular notation using 'j'
	A = 'd' - polar notation using degrees
	R = 'r' - polar notation using radian

	Specifies complex number notation to use in <complex> class.
 */
typedef enum e_complexnotation
{
	I='i',
	J='j',
	A='d', 
	R='r',
} CNOTATION; 

/*	Define: CNOTATION_DEFAULT
 
 		Specifies the notation to use by default.

 	Caveat: Never set this to CNOTATION(A)
 */
#define CNOTATION_DEFAULT (J) 
#define PI (3.1415926535897932384626433832795)
#define E (2.71828182845905)

#include <math.h>
#include "platform.h"

#ifndef __cplusplus

/*	Structure: s_complex
	r - real part
	i - imaginary part
	f - notation
 */
typedef struct s_complex {
#else
/*	Class: complex
 */
class complex { 
private:
#endif
	double r; /**< the real part */
	double i; /**< the imaginary part */
	CNOTATION f; /**< the default notation to use */
#ifndef __cplusplus
} complex;

/*	Function: complex_set_polar

	Parameters:
	X - complex value
	M - magnitude
	A - angle
 */
inline DEPRECATED double complex_set_polar(X,M,A) { return ((X).r=((M)*cos(A)),(X).i=((M)*sin(A)),(X)); };

/* Function: complex_set_power_factor

	Parameters:
	X - complex value
	M - magnitude
	P - power factor
 */
inline DEPRECATED double complex_set_power_factor(X,M,P) { return complex_set_polar((X),(M)/(P),acos(P)); };

/* Function: complex_get_mag

	Parameters:
	X - complex value
 */
inline DEPRECATED double complex_get_mag(X) { return (sqrt((X).r*(X).r + (X).i*(X).i)); };

/* Function: complex_get_arg

	Parameters:
	X - complex value
 */
inline DEPRECATED double complex_get_arg(X) { return ((X).r==0 ? ( (X).i > 0 ? PI/2 : ((X).i<0 ? -PI/2 : 0) ) : ( (X).r>0 ? atan((X).i/(X).r) : PI+atan((X).i/(X).r) )); };

/* Function: complex_set_rect

	Parameters:
	C - complex value
	X - real part
	Y - imaginary part
 */
inline DEPRECATED double complex_set_rect(C,X,Y) { return ((C).r=(X),(C).i=(Y)); };

/* Function: complex_set_real

	Parameters:
	C - complex value
	X - real part
 */
inline DEPRECATED double complex_set_real(C,X) { return ((C).r=(X)); };

/* Function: complex_set_imag

	Parameters:
	C - complex value
	Y - imaginary part
 */
inline DEPRECATED double complex_set_imag(C,Y) { return ((C).i=(Y)); };

/* Function: complex_set_mag

	Parameters:
	C - complex value
	M - magnitude
 */
inline DEPRECATED double complex_set_mag(C,M) {}; // TODO

/* Function: complex_set_arg

	Parameters:
	C - complex value
	R - angle (in radians)
 */
inline DEPRECATED double complex_set_arg(C,R) {}; // TODO

/* Function: complex_set_ang

	Parameters:
	C - complex value
	D - angle (in degrees)
 */
inline DEPRECATED double complex_set_ang(C,D) {}; // TODO

#else
public:
	// Constructor: complex
	// Create a zero complex number
	inline complex() 
	{
		r = 0;
		i = 0;
		f = CNOTATION_DEFAULT;
	};

	// Constructor: complex
	// Create a complex number with real part only
	inline complex(double re) 
	{
		r = re;
		i = 0;
		f = CNOTATION_DEFAULT;
	};

	// Constructor: complex
	// Create a complex number real and imaginary parts
	inline complex(double re, double im, CNOTATION nf=CNOTATION_DEFAULT)
	{
		assert(nf!=A || nf!=R);
		f = nf;
		r = re;
		i = im;
	};
	
	// Method: operator =
	// Copy complex values
	inline complex &operator = (complex x) 
	{
		r = x.r; 
		i = x.i; 
		f = x.f; 
		return *this;
	};

	// Method: operator =
	// Copy real value
	inline complex &operator = (double x) 
	{
		r = x; 
		i = 0; 
		f = CNOTATION_DEFAULT;
		return *this;
	};

	// Method: Re
	inline double & Re(void) 
	{
		return r;
	};

	// Method: Re
	inline double Re(double x)
	{
		r = x;
		return r;
	};

	// Method: Im
	inline double & Im(void) 
	{
		return i;
	};

	// Method: Im
	inline double Im(double y)
	{
		i = y;
		return i;
	};

	// Method: Notation
	inline CNOTATION & Notation(void) /**< access to notation */
	{
		return f;
	};

	// Method: Notation
	inline CNOTATION Notation(CNOTATION s)
	{
		f = s;
		return f;
	};

	// Method: Mag
	inline double Mag(void) const /**< compute magnitude */
	{
		return sqrt(r*r+i*i);
	};

	// Method: Mag
	inline double Mag(double m)  /**< set magnitude */
	{
		double old = sqrt(r*r+i*i);
		r *= m/old;
		i *= m/old;
		return m;
	};

	// Method: Arg
	inline double Arg(void) const /**< compute angle */
	{
		if (r==0)
		{
			if (i>0)
				return PI/2;
			else if (i==0)
				return 0;
			else
				return -PI/2;
		}
		else if (r>0)
			return atan(i/r);
		else
			return PI+atan(i/r);
	};

	// Method: Ang
	inline double Ang(void)
	{
		return Arg()*180.0/PI;
	}

	// Method: Arg
	inline double Arg(double a)  /**< set angle */
	{
		SetPolar(Mag(),a,f);
		return a;
	};

	// Method: Ang
	inline double Ang(double a)
	{
		SetPolar(Mag(),a*PI/180.0);
		return a;
	}

	// Method: Log
	inline complex Log(void) const /**< compute log */
	{ 
		return complex(log(Mag()),Arg(),f);
	};

	// Method: SetReal
	inline void SetReal(double v) /**< set real part */
	{
		r = v;
	};

	// Method: SetImag
	inline void SetImag(double v) /**< set imaginary part */
	{
		i = v;
	};

	// Method: SetNotation
	inline void SetNotation(CNOTATION nf) /**< set notation */
	{
		f = nf;
	}

	// Method: SetRect
	inline void SetRect(double rp, double ip, CNOTATION nf=CNOTATION_DEFAULT) /**< set rectangular value */
	{
		r = rp;
		i = ip;
		f = nf;
	};

	// Method: SetPolar
	inline void SetPolar(double m, double a, CNOTATION nf=A) /**< set polar values */
	{
		r = (m*cos(a)); 
		i = (m*sin(a));
		f = nf;
	};

	// Method: operator -
	inline complex operator - (void) /**< change sign */
	{
		return complex(-r,-i,f);
	};

	// Method: operator ~
	inline complex operator ~ (void) /**< complex conjugate */
	{ 
		return complex(r,-i,f);
	};

	// Method: operator +=
	inline complex &operator += (double x) /**< add a double to the real part */
	{
		r += x; 
		return *this;
	};

	// Method: operator -=
	inline complex &operator -= (double x) /**< subtract a double from the real part */
	{
		r -= x; 
		return *this;
	};

	// Method: operator *=
	inline complex &operator *= (double x) /**< multiply a double to real part */
	{
		r *= x; 
		i *= x; 
		return *this;
	};

	// Method: operator /=
	inline complex &operator /= (double x) /**< divide into the real part */
	{
		r /= x; 
		i /= x;
		return *this;
	};

	// Method: operator ^=
	inline complex &operator ^= (double x) /**< raise to a real power */
	{ 
		double lm = log(Mag()), a = Arg(), b = exp(x*lm), c = x*a; 
		r = (b*cos(c)); 
		i = (b*sin(c)); 
		return *this;
	};

	// Method: operator +=
	inline complex &operator += (complex x) /**< add a complex number */
	{
		r += x.r; 
		i += x.i; 
		return *this;
	};

	// Method: operator -=
	inline complex &operator -= (complex x)  /**< subtract a complex number */
	{
		r -= x.r; 
		i -= x.i; 
		return *this;
	};

	// Method: operator *=
	inline complex &operator *= (complex x)  /**< multip[le by a complex number */
	{
		double pr=r; 
		r = pr * x.r - i * x.i; 
		i = pr * x.i + i * x.r; 
		return *this;
	};

	// Method: operator /=
	inline complex &operator /= (complex y)  /**< divide by a complex number */
	{
		double xr=r;
		double a = y.r*y.r+y.i*y.i;
		r = (xr*y.r+i*y.i)/a; 
		i = (i*y.r-xr*y.i)/a;
		return *this;
	};

	// Method: operator ^=
	inline complex &operator ^= (complex x) /**< raise to a complex power */
	{ 
		double lm = log(Mag()), a = Arg(), b = exp(x.r*lm-x.i*a), c = x.r*a+x.i*lm; 
		r = (b*cos(c)); 
		i = (b*sin(c)); 
		return *this;
	};

	// Method: operator +
	inline complex operator + (double y) /**< double sum */
	{
		complex x(*this);
		return x+=y;
	};

	// Method: operator -
	inline complex operator - (double y) /**< double subtract */
	{
		complex x(*this);
		return x-=y;
	};

	// Method: operator *
	inline complex operator * (double y) /**< double multiply */
	{
		complex x(*this);
		return x*=y;
	};

	// Method: operator /
	inline complex operator / (double y) /**< double divide */
	{
		complex x(*this);
		return x/=y;
	};

	// Method: operator ^
	inline complex operator ^ (double y) /**< double power */
	{
		complex x(*this);
		return x^=y;
	};

	// Method: operator +
	inline complex operator + (complex y) /**< complex sum */
	{
		complex x(*this);
		return x+=y;
	};

	// Method: operator -
	inline complex operator - (complex y) /**< complex subtract */
	{
		complex x(*this);
		return x-=y;
	};

	// Method: operator *
	inline complex operator * (complex y) /**< complex multiply */
	{
		complex x(*this);
		return x*=y;
	};

	// Method: operator /
	inline complex operator / (complex y) /**< complex divide */
	{
		complex x(*this);
		return x/=y;
	};

	// Method: operator ^
	inline complex operator ^ (complex y) /**< complex power */
	{
		complex x(*this);
		return x^=y;
	};

	// Method: SetPowerFactor
	inline complex SetPowerFactor(double mag, /**< magnitude of power */
		double pf, /**< power factor */
		CNOTATION n=J) /** notation */
	{
		SetPolar(mag/pf, acos(pf),n);
		return *this;
	}

	// Method: IsZero
	inline bool IsZero(double err=0.0) /**< zero test */
	{
		return Mag()<=err;
	}

	// Method: operator ==
	// Compare magnitude
	inline bool operator == (double m)	{ return Mag()==m; };

	// Method: operator !=
	// Compare magnitude
	inline bool operator != (double m)	{ return Mag()!=m; };

	// Method: operator <
	// Compare magnitude
	inline bool operator < (double m)	{ return Mag()<m; };

	// Method: operator <=
	// Compare magnitude
	inline bool operator <= (double m)	{ return Mag()<=m; };

	// Method: 
	// Compare magnitude
	inline bool operator > (double m)	{ return Mag()>m; };

	// Method: operator >=
	// Compare magnitude
	inline bool operator >= (double m)	{ return Mag()>=m; };

	// Method: operator ==
	// Compare angle
	inline bool operator == (complex y)	{ return fmod(y.Arg()-Arg(),2*PI)==0.0;};

	// Method: operator !=
	// Compare angle
	inline bool operator != (complex y)	{ return fmod(y.Arg()-Arg(),2*PI)!=0.0;};

	// Method: operator <
	// Compare angle
	inline bool operator < (complex y)	{ return fmod(y.Arg()-Arg(),2*PI)<PI;};

	// Method: operator <=
	// Compare angle
	inline bool operator <= (complex y)	{ return fmod(y.Arg()-Arg(),2*PI)<=PI;};

	// Method: operator >
	// Compare angle
	inline bool operator > (complex y)	{ return fmod(y.Arg()-Arg(),2*PI)>PI;};

	// Method: operator >=
	// Compare angle
	inline bool operator >= (complex y)	{ return fmod(y.Arg()-Arg(),2*PI)>=PI;};

	// Method: IsFinite
	inline bool IsFinite(void) { return isfinite(r) && isfinite(i); };
};
#endif

// Function: complex_from_string
int complex_from_string(void *c, const char *str);

// Function: complex_get_part
double complex_get_part(void *c, const char *name);

// Function: complex_set_part
int complex_set_part(void *c, const char *name, const char *value);

#endif
 /**@}**/
