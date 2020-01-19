/** $Id: solvers.cpp 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
**/

#include <stdio.h>
#include <exception>

#define ARMA_DONT_PRINT_ERRORS
#include <armadillo>

#define USE_GLSOLVERS
#include "residential.h"
#include "solvers.h"

using namespace arma;

static void exception(const char *format, ...)
{
	char *message = NULL;
	va_list ptr;
	va_start(ptr,format);
	vasprintf(&message,format,ptr);
	va_end(ptr);
	throw message ? message : "memory allocation failure";
}

static void debug(const char *format, ...)
{
	char *message = NULL;
	va_list ptr;
	va_start(ptr,format);
	vasprintf(&message,format,ptr);
	va_end(ptr);
	gl_debug("(solvers) %s", message ? message : "memory allocation failure");
}

static void verbose(const char *format, ...)
{
	char *message = NULL;
	va_list ptr;
	va_start(ptr,format);
	vasprintf(&message,format,ptr);
	va_end(ptr);
	gl_verbose("(solvers) %s", message ? message : "memory allocation failure");
}

static void dump_array(const char *t, double *p, size_t N)
{
	cerr << t << "[" << N << "] = [ "; 
	for ( size_t n = 0 ; n < N; n++ ) 
	{
		cerr << (n>0?", ":"") << p[n]; 
	}
	cerr << "]" << endl;
}

static void dump_matrix(const char *t, mat x)
{
	cerr << t 
	     << "[" 
	     << x.n_rows 
	     << "x" 
	     << x.n_cols 
	     << "] = " 
	     << endl
	     << x 
	     << endl;
}

//////////////////////////////////////////////////////////////////////////////
//
// ETP Solver -- single-zone thermal model solver
//
//////////////////////////////////////////////////////////////////////////////


double e2solve(double a, 
               double n, 
               double b, 
               double m, 
               double c, 
               double p, 
               double *e)
{
	static gld_solver *etp = NULL;
	static struct etpdata 
	{
		double t,a,n,b,m,c,p,e;
		unsigned int i;
	} data;

	// load the solver if not yet loaded
	if ( etp==NULL )
	{
		etp = new gld_solver("etp");
		int version;
		if ( etp->get("version",&version,NULL) == 0 || version != 1 )
		{
			exception("incorrect ETP solver version (version=%d)",version);
		}
		if ( etp->get("init",&data,NULL) == 0 )
		{
			exception("unable to initialize ETP solver data");
		}
		data.i = 100;
	}

	// solve the given problem
	data.t = 0;
	data.a = a;
	data.b = b;
	data.c = c;
	data.n = n;
	data.m = m;
	data.p = p;
	if ( etp->solve(&data) )
	{
		if ( e != NULL )
		{
			*e = data.e;
		}
		return data.t;
	}
	else
	{
		return NaN;
	}
}

//////////////////////////////////////////////////////////////////////////////
//
// MSOLVER -- multizone thermal model solver
//
// TODO: move this to the core so it can be shared with commercial module
//
//////////////////////////////////////////////////////////////////////////////

// status update flags
#define SU_NONE 0x0000
#define SU_A    0x0001
#define SU_B1   0x0002
#define SU_B2   0x0004
#define SU_Q    0x0008
#define SU_T    0x0010
#define SU_U    0x0020

typedef struct s_parameter_map 
{
	const char *name;
	double **array;
	int size;
	int modifies;
} PARAMETERMAP;

class MSolver
{
public:
	MSolver(void);
	~MSolver(void);
private:
	int N;
	mat A, B1, B2;
	mat T, q, u, Tset, dT;
	mat Ainv, B1inv, B2inv, Aeig;
	mat Tbal, Teq;
	int modified;
	msolver *data;
	bool enable_dump;
	bool enable_debug;
	bool enable_verbose;
public:
	void init(int size);
	PARAMETERMAP *get_map(const char *name=NULL);
	inline size_t get_mapsize() { return mapsize; };
	inline int get_size(void) { return N; };
	inline void set_modified(int su) { modified |= su; debug("modified |= 0x%04X -> 0x%04x",su,modified); };
	void update(bool force = false);
	void solve(double dt);
	inline msolver *get_data(void) { return data; };
	void dump(const char *heading=NULL);
	inline void set_enable_dump(bool state) { enable_dump = state; };
	inline void set_enable_debug(bool state) { enable_debug = state; };
	inline void set_enable_verbose(bool state) { enable_verbose = state; };
	inline bool is_enable_dump(void) { return enable_dump; };
	inline bool is_enable_debug(void) { return enable_debug; };
	inline bool is_enable_verbose(void) { return enable_verbose; };
private:
	PARAMETERMAP *map;
	size_t mapsize;
};

MSolver::MSolver()
{
	N = 0;
	data = new msolver;
	memset(data,0,sizeof(msolver));
	data->solver = this;
	modified = SU_NONE;
	map = NULL;
	mapsize = 0;
	enable_dump = false;
	enable_debug = false;
	enable_verbose = false;
}

MSolver::~MSolver(void)
{
	if ( data )
	{
		if ( data->U ) delete [] data->U;
		if ( data->C ) delete [] data->C;
		if ( data->q ) delete [] data->q;
		if ( data->T ) delete [] data->T;
		if ( data->u ) delete [] data->u;
		if ( data->a ) delete [] data->a;
		if ( data->umin ) delete [] data->umin;
		if ( data->umax ) delete [] data->umax;
		delete data;
	}
}

PARAMETERMAP *MSolver::get_map(const char *name)
{
	if ( name == NULL )
	{
		return map;
	}
	for ( size_t m = 0 ; m < get_mapsize() ; m++ )
	{
		if ( strcmp(name,map[m].name) == 0 )
		{
			return map + m;
		}
	}
	exception("MSolver::get_map(name='%s'): parameter name not found");
	return NULL;
}

void MSolver::init(int size)
{
	if ( N != 0 )
		exception("cannot reinitialize solver (old size %d, new size %d)", N, size);
	N = size;
	A = mat(size,size).zeros();
	B1 = mat(size,size).zeros();
	B2 = mat(size,size).zeros();
	T = mat(size,1).zeros();
	q = mat(size,1).zeros();
	u = mat(size,1).zeros();
	Tset = mat(size,1).zeros();
	dT = mat(size,1).zeros();
	PARAMETERMAP tmp[] = {
		{"U",    &(data->U),    N*(N+1)/2, SU_A|SU_B1},
		{"C",    &(data->C),    N,         SU_A|SU_B1|SU_B2},
		{"T",    &(data->T),    N+1,       SU_NONE},
		{"q",    &(data->q),    N,         SU_Q},
		{"a",    &(data->a),    1,         SU_B1|SU_B2},
		{"umin", &(data->umin), N-1,       SU_U},
		{"umax", &(data->umax), N-1,       SU_U},
		{"Tset", &(data->Tset), N-1,       SU_T},
		{"dT",   &(data->dT),	N,         SU_NONE},
	};
	mapsize = sizeof(tmp)/sizeof(tmp[0]);
	map = new PARAMETERMAP[mapsize];
	memcpy(map,tmp,sizeof(tmp));
}

void MSolver::update(bool force)
{
	debug("MSolver::update(force=%s): modified = 0x%04X", force?"true":"false", modified);
	const char *error = NULL;
	try
	{
		// build A if needed
		if ( (modified&SU_A) | force)
		{
			if ( enable_debug ) debug("A outdated");
			int n = N;
			for ( int i = 0 ; i < N ; i++ )
			{
				A(i,i) = - data->U[i];
				for ( int j = i+1 ; j < N ; j++ )
				{
					A(i,i) -= data->U[n];
					A(i,j) = data->U[n] / data->C[i];
					A(j,i) = data->U[n] / data->C[j];
					n++;
				}
				A(i,i) /= data->C[i];
			}
			if ( enable_dump ) dump_matrix("updating A",A);			
			Ainv = inv(A);
			if ( enable_dump ) dump_matrix("updating Ainv",Ainv);
			Aeig = diagmat(arma::real(eig_gen(A)));
			if ( enable_dump ) dump_matrix("updating Aeig",Aeig);
		}

		// build B1 if needed
		if ( (modified&SU_B1) | force )
		{
			if ( enable_debug ) debug("B1 outdated");
			for ( int i = 0 ; i < N ; i++ )
			{
				B1(i,0) = data->U[i] / data->C[i];
				if ( i < N-1 )
				{
					B1(i,i+1) = data->a[0] / data->C[i];
					B1(N-1,i+1) = (1-data->a[0]) / data->C[N-1];
				}
			}
			if ( enable_dump ) dump_matrix("updating B1",B1);
			B1inv = inv(B1);
			if ( enable_dump ) dump_matrix("updating B1inv",B1inv);
		}

		// build B2 if needed
		if ( (modified&SU_B2) | force )
		{
			if ( enable_debug ) debug("B2 outdated");
			for ( int i = 0 ; i < N ; i++ )
			{
				if ( i < N-1 )
				{
					B2(i,i) = data->a[0] / data->C[i];
					B2(N-1,i) = (1-data->a[0]) / data->C[N-1];
				}
				else
				{
					B2(N-1,N-1) = 1 / data->C[N-1];
				}
			}
			if ( enable_dump ) dump_matrix("updating B2",B2);
			B2inv = inv(B2);
			if ( enable_dump ) dump_matrix("updating B2inv",B2inv);
		}

		// build q if needed
		if ( (modified&SU_Q) | force )
		{
			if ( enable_debug ) debug("q outdated");
			q[0] = data->T[0];
			for ( int i = 1 ; i < N ; i++ )
			{
				q(i,0) = data->q[i-1];
			}	
			if ( enable_dump ) dump_matrix("updating q",q);
		}

		if ( (modified&(SU_A|SU_B1|SU_Q)) | force )
		{
			if ( enable_debug ) debug("Tbal outdated");
			Tbal = -Ainv*B1*q;
			if ( enable_dump ) dump_matrix("updating Tbal",Tbal);
		}

		// update the control input if needed
		if ( (modified&(SU_B2|SU_A|SU_T|SU_B1|SU_Q)) | force )
		{
			if ( enable_debug ) debug("u outdated");
			u = -B2inv*(A*Tset+B1*q);
			if ( enable_dump ) dump_matrix("updating u",u);
			modified |= SU_U;
		}

		// constrain the control input to capacity limits (should use clamp but that doesn't compile)
		if ( (modified&SU_U) | force )
		{
			if ( enable_debug ) debug("u outdating");
			for ( int i = 0 ; i < N ; i++ )
			{
				if ( data->umin[i] > data->umax[i] ) 
				{ 
					continue; 
				}
				if ( u[i] < data->umin[i] ) 
				{ 
					u[i] = data->umin[i]; 
				}
				else if ( u[i] > data->umax[i] ) 
				{ 
					u[i] = data->umax[i]; 
				}
			}
			if ( enable_dump ) dump_matrix("updating u",u);
		}

		if ( (modified&(SU_A|SU_B1|SU_Q|SU_B2|SU_U)) | force )
		{
			if ( enable_debug ) debug("Teq outdated");
			Teq = Tbal - Ainv*B2*u;
			if ( enable_dump ) dump_matrix("updating Teq",Teq);
		}
		modified = SU_NONE;
	}
	catch (std::exception &msg)
	{
		dump(msg.what());
		error = msg.what();
	}
	if ( error )
	{
		exception("MSolver::update() failed on error %s",error);
	}
}

void MSolver::solve(double dt)
{
	const char *error = NULL;
	try
	{
		if ( enable_debug ) debug("updating T");
		dT = T - (Teq + (exp(Aeig*dt)-mat(4,4).eye())*T);
		if ( enable_dump ) dump_matrix("dT",dT);
		T += dT;
		if ( enable_dump ) dump_matrix("T",T);
	}
	catch (std::exception &msg)
	{
		dump(msg.what());
		error = msg.what();
	}
	if ( error )
	{
		exception("MSolver::solve() failed on error %s",error);
	}
}

void MSolver::dump(const char *heading)
{
	if ( enable_debug ) debug("dumping MSolver data");
	cerr << "MSolver dump " << (heading?heading:"with no context") << "" << endl;
	cerr << "  N = " << N << endl;
	cerr << "MSolver input data:" << endl;
	for ( PARAMETERMAP *p = get_map() ; p < get_map()+get_mapsize() ; p++ )
	{
		dump_array(p->name,*(p->array),p->size);
	}
	cerr << "MSolver solution data:" << endl;
	dump_matrix("A",A);
	dump_matrix("B1",B1);
	dump_matrix("B2",B2);
	dump_matrix("T",T);
	dump_matrix("q",q);
	dump_matrix("u",u);
	dump_matrix("Tset",Tset);
}

msolver *msolve(const char *op, ...)
{
	msolver *data = NULL;
	MSolver *solver = NULL;
	va_list ptr;
	va_start(ptr,op);

	if ( strcmp(op,"new") == 0 )
	{
		solver = new MSolver();
		data = solver->get_data();
	}
	else if ( strcmp(op,"get") == 0 )
	{
		data = va_arg(ptr,msolver*);
		solver = (MSolver*)(data->solver);
		assert(solver->get_data()==data);
		const char *param = va_arg(ptr,const char *);
		PARAMETERMAP *map = solver->get_map(param);
		double **ref = va_arg(ptr,double**);
		*ref = *(map->array);
	}
	else if ( strcmp(op,"set") == 0 || strcmp(op,"copy") == 0 )
	{
		data = va_arg(ptr,msolver*);
		solver = (MSolver*)(data->solver);
		assert(solver->get_data()==data);
		const char *param = va_arg(ptr,const char*);

		// process changes to N
		if ( strcmp(op,"set") == 0 ) 
		{
			if ( strcmp(param,"N") == 0 )
			{
				data->N = va_arg(ptr,int);
				if ( data->N < 2 )
				{
					exception("msolver N must be greater than 1 (N=%d)",solver->get_size());
				}
				solver->init(data->N);
				for ( PARAMETERMAP *map = solver->get_map() ; map < solver->get_map() + solver->get_mapsize() ; map++ )
				{
					if ( *(map->array) != NULL )
						delete [] *(map->array);
					*(map->array) = new double[map->size];
				}
				verbose("msolver(op='set',param='%s',value=%d): ok",param,data->N);
			}
			else if ( strcmp(param,"dump") == 0 )
			{
				int value = va_arg(ptr,int);
				solver->set_enable_dump(value?true:false);
				verbose("msolver(op='set',param='%s',value=%d): enable_dump <- %s",param,value,solver->is_enable_dump()?"true":"false");
			}
			else if ( strcmp(param,"debug") == 0 )
			{
				int value = va_arg(ptr,int);
				solver->set_enable_debug(value?true:false);
				verbose("msolver(op='set',param='%s',value=%d): enable_debug <- %s",param,value,solver->is_enable_debug()?"true":"false");
			}
			else if ( strcmp(param,"verbose") == 0 )
			{
				int value = va_arg(ptr,int);
				solver->set_enable_verbose(value?true:false);
				verbose("msolver(op='set',param='%s',value=%d): enable_verbose <- %s",param,value,solver->is_enable_verbose()?"true":"false");
			}
			else
			{
				// find mapping
				PARAMETERMAP *map = solver->get_map(param);

				// process changes to a single value
				int n = 0;
				if ( map->size > 1 )
				{
					n = va_arg(ptr,int);
				}
				double value = va_arg(ptr,double);

				// copy single entry
				if ( n >= 0 && n < map->size )
				{
					(*(map->array))[n] = value;
					solver->set_modified(map->modifies);
				}
				else
				{
					exception("msolver(op='set',param='%s',index=%d,value=%g): index out of bound (size=%d)",param,n,value,map->size);
				}
				verbose("msolver(op='set',param='%s',index=%d,value=%g): ok",param,n,value);
			}
		}
		else // copy
		{
			// find mapping
			PARAMETERMAP *map = solver->get_map(param);

			// copy entire array
			for ( int n = 0 ; n < map->size ; n++ )
			{
				double value = va_arg(ptr,double);
				(*(map->array))[n] = value;
				verbose("msolver(op='copy',param='%s',index=%d,value=%g): ok",param,n,value);
			}
			solver->set_modified(map->modifies);
			verbose("msolver(op='copy',param='%s',range=%d..%d): ok",param,0,map->size-1);
		}
	}
	else if ( strcmp(op,"update") == 0 )
	{
		data = va_arg(ptr,msolver*);
		solver = (MSolver*)(data->solver);
		assert(solver->get_data()==data);
		solver->update();
	}
	else if ( strcmp(op,"solve") == 0 )
	{
		data = va_arg(ptr,msolver*);
		solver = (MSolver*)(data->solver);
		assert(solver->get_data()==data);
		solver->update();
		double dt = va_arg(ptr,double);
		solver->solve(dt);
	}
	else if ( strcmp(op,"delete") == 0 )
	{
		data = va_arg(ptr,msolver*);
		solver = (MSolver*)(data->solver);
		assert(solver->get_data()==data);
		delete solver;
		data = NULL;
	}
	else 
	{
		exception("msolve(op='%s'): operation is invalid ",op);
	}

	va_end(ptr);
	return data;
}

