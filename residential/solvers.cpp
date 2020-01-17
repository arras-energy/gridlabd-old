/** $Id: solvers.cpp 4738 2014-07-03 00:55:39Z dchassin $
	Copyright (C) 2008 Battelle Memorial Institute
**/

#define USE_GLSOLVERS

#include "residential.h"
#include "solvers.h"

using namespace arma;

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

class MSolver
{
public:
	MSolver(msolver *);
	~MSolver(void);
public:
	void update();
	void solve();
private:
	mat A, B1, B2, C, D1, D2;
	mat T, q, u;
};

msolver *msolve(int op, ...)
{
	msolver *result = NULL;
	MSolver *solver = NULL;
	va_list ptr;
	va_start(ptr,op);

	switch (op)
	{
	case MSO_NEW:
		result = new msolver;
	case MSO_CREATE:
		result = va_arg(ptr,msolver*);
		solver = new MSolver(result);
		result->data = (void*)solver;
	case MSO_DELETE:
		result = va_arg(ptr,msolver*);
		solver = (MSolver*)result;
		delete solver;
		delete result;
		result = NULL;
	case MSO_UPDATE:
		result = va_arg(ptr,msolver*);
		solver = (MSolver*)result;
		solver->update();
	case MSO_SOLVE:
		result = va_arg(ptr,msolver*);
		solver = (MSolver*)result;
		solver->solve();
	default:
		throw "msolve operation is invalid";
	}

	va_end(ptr);
	return result;
}

MSolver::MSolver(msolver *init)
{
	// TODO
}

MSolver::~MSolver(void)
{
	// TODO
}

void MSolver::update(void)
{
	// TODO
}

void MSolver::solve(void)
{
	// TODO
}
