/** $Id: solvers.cpp 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
**/

#include <armadillo>

#define USE_GLSOLVERS

#include "residential.h"
#include "solvers.h"

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
			throw "incorrect ETP solver version";
		}
		if ( etp->get("init",&data,NULL) == 0 )
		{
			throw "unable to initialize ETP solver data";
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

using namespace arma;

// status update flags
#define SU_NONE 0x0000
#define SU_A    0x0001
#define SU_B1   0x0002
#define SU_B2   0x0004
#define SU_Q    0x0008
#define SU_T    0x0010
#define SU_U    0x0020

class MSolver
{
public:
	MSolver(void);
	~MSolver(void);
private:
	int N;
	mat A, B1, B2;
	mat T, q, u, Tset;
	mat Ainv, B1inv, B2inv, Aeig;
	mat umin, umax;
	mat Tbal, Teq;
	int modified;
	msolver *data;
public:
	void init(int size);
	inline int get_size(void) { return N; };
	inline void set_modified(int su) { modified |= su; };
	void update(bool force = false);
	void solve(double dt);
	inline msolver *get_data(void) { return data; };
};

MSolver::MSolver()
{
	data = new msolver;
	memset(data,0,sizeof(data[0]));
	data->U = NULL;
	data->C = NULL;
	data->q = NULL;
	data->T = NULL;
	data->u = NULL;
	data->a = NULL;
	data->umin = NULL;
	data->umax = NULL;
	data->solver = this;
	modified = false;
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

void MSolver::init(int size)
{
	if ( N != 0 )
		throw "cannot reinitialize solver";
	N = size;
	A = mat(size,size);
	B1 = mat(size,size);
	B2 = mat(size,size);
	T = mat(size,1);
	q = mat(size,1);
	u = mat(size,1);
	Tset = mat(size,1);
}
void MSolver::update(bool force)
{
	// build A if needed
	if ( (modified&SU_A) | force)
	{
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
		Ainv = inv(A);
		Aeig = diagmat(arma::real(eig_gen(A)));
	}

	// build B1 if needed
	if ( (modified&SU_B1) | force )
	{
		for ( int i = 0 ; i < N ; i++ )
		{
			B1(i,0) = data->U[i] / data->C[i];
			if ( i < N-1 )
			{
				B1(i,i+1) = data->a[0] / data->C[i];
			}
			else
			{
				B1(N-1,i+1) = (1-data->a[0]) / data->C[N-1];
			}
		}
		B1inv = inv(B1);
	}

	// build B2 if needed
	if ( (modified&SU_B2) | force )
	{
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
		B2inv = inv(B2);
	}

	// build q if needed
	if ( (modified&SU_Q) | force )
	{
		q[0] = data->T[0];
		for ( int i = 1 ; i < N ; i++ )
		{
			q(i,0) = data->q[i-1];
		}	
	}

	if ( (modified&(SU_A|SU_B1|SU_Q)) | force )
	{
		Tbal = -Ainv*B1*q;
	}

	// update the control input if needed
	if ( (modified&(SU_B2|SU_A|SU_T|SU_B1|SU_Q)) | force )
	{
		u = -B2inv*(A*Tset+B1*q);
		modified |= SU_U;
	}

	// constrain the control input to capacity limits (should use clamp but that doesn't compile)
	if ( (modified&SU_U) | force )
	{
		for ( int i = 0 ; i < N ; i++ )
		{
			if ( umin[i] > umax[i] ) { continue; }
			if ( u[i] < umin[i] ) { u[i] = umin[i]; }
			else if ( u[i] > umax[i] ) { u[i] = umax[i]; }
		}
	}

	if ( (modified&(SU_A|SU_B1|SU_Q|SU_B2|SU_U)) | force )
	{
		Teq = Tbal - Ainv*B2*u;
	}
	modified = SU_NONE;
}

void MSolver::solve(double dt)
{
	T = Teq + (exp(Aeig*dt)-mat(4,4).eye())*T;
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
	else if ( strcmp(op,"set") == 0 || strcmp(op,"copy") == 0 )
	{
		data = va_arg(ptr,msolver*);
		solver = (MSolver*)(data->solver);
		assert(solver->get_data()==data);
		const char *param = va_arg(ptr,const char*);

		// first time call must be N to initialize solver
		if ( strcmp(op,"set") == 0 && strcmp(param,"N") == 0 )
		{
			solver->init(va_arg(ptr,int));
		}

		// N must be 2 or more
		int N = solver->get_size();
		if ( N < 2 )
		{
			throw "msolver N must be greater than 1";
		}

		// parameter update map
		struct s_parameter_map 
		{
			const char *name;
			double *array;
			int size;
			bool modifies;
		} map[] = {
			{"U",    data->U,    N*(N+1)/2, SU_A|SU_B1},
			{"C",    data->C,    N,         SU_A|SU_B1|SU_B2},
			{"T",    data->T,    N+1,       SU_NONE},
			{"q",    data->q,    N,         SU_Q},
			{"a",    data->a,    1,         SU_B1|SU_B2},
			{"umin", data->umin, N-1,       SU_U},
			{"umax", data->umin, N-1,       SU_U},
			{"Tset", data->Tset, N-1,       SU_T},
		}, *p;

		// process changes to N
		if ( strcmp(op,"set") == 0 ) 
		{
			if ( strcmp(param,"N") == 0 )
			{
				for ( p = map ; p < map + sizeof(map)/sizeof(map[0]) ; p++ )
				{
					if ( p->array != NULL )
						delete [] p->array;
					p->array = new double[p->size];
				}
				data->N = N;
			}
			else
			{
				// process changes to a single value
				N = va_arg(ptr,int);
				double value = va_arg(ptr,double);

				// find mapping
				for ( p = map ; p < map + sizeof(map)/sizeof(map[0]) ; p++ )
				{
					if ( strcmp(param,p->name) == 0 )
					{
						// copy single entry
						if ( N >= 0 && N < p->size )
						{
							p->array[N] = value;
							solver->set_modified(p->modifies);
							break;
						}
						else
						{
							throw "msolver parameter index out of range";
						}
					}
				}
			}
		}
		else // copy
		{
			// find mapping
			for ( p = map ; p < map + sizeof(map)/sizeof(map[0]) ; p++ )
			{
				if ( strcmp(param,p->name) == 0 )
				{
					for ( int n = 0 ; n < p->size ; n++ )
					{
						p->array[N] = va_arg(ptr,double);
					}
					solver->set_modified(p->modifies);
					break;
				}
			}
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
		delete data;
		data = NULL;
	}
	else 
	{
		throw "msolve operation is invalid";
	}

	va_end(ptr);
	return data;
}

