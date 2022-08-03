/* transform.cpp
 * Copyright (C) 2008, Battelle Memorial Institute
 */

#include "gldcore.h"

SET_MYCONTEXT(DMC_TRANSFORM)

static TRANSFORM *schedule_xformlist=NULL;

/****************************************************************
 * GridLAB-D Variable Handling for transform functions
 ****************************************************************/
GLDVAR *gldvar_create(unsigned int dim)
{
	GLDVAR *vars = NULL;
	vars = (GLDVAR*)malloc(sizeof(GLDVAR)*dim);
	memset(vars,0,sizeof(GLDVAR)*dim);
	return vars;
}
int gldvar_isset(GLDVAR *var, unsigned int n)
{
	return var[n].addr!=NULL;
}
void gldvar_set(GLDVAR *var, unsigned int n, void *addr, PROPERTY *prop)
{
	var[n].addr = addr;
	var[n].prop = prop;
}
void gldvar_unset(GLDVAR *var, unsigned int n)
{
	var[n].addr = var[n].prop = NULL;
}
void *gldvar_getaddr(GLDVAR *var, unsigned int n)
{
	return var[n].addr;
}
void *gldvar_getprop(GLDVAR *var, unsigned int n)
{
	return var[n].prop;
}
PROPERTYTYPE gldvar_gettype(GLDVAR *var, unsigned int n)
{
	return var[n].prop->ptype;
}
const char *gldvar_getname(GLDVAR *var, unsigned int n)
{
	return var[n].prop->name;
}
const char *gldvar_getstring(GLDVAR *var, unsigned int n, char *buffer, int size)
{
	if ( gldvar_isset(var,n) )
	{
		property_write(var[n].prop, var[n].addr, buffer,size);
		return buffer;
	}
	else
		return strncpy(buffer,"NULL",size);
}
UNIT *gldvar_getunits(GLDVAR *var, unsigned int n)
{
	if ( gldvar_isset(var,n) )
		return var[n].prop->unit;
	else
		return NULL;
}

/****************************************************************
 * Transform handling
 ****************************************************************/
TRANSFORM *transform_getnext(TRANSFORM *xform)
{
	return xform?xform->next:schedule_xformlist;
}

TRANSFERFUNCTION *tflist = NULL; ///< transfer function list
int write_term(char *buffer,size_t size, double a,char *x,int n,bool first)
{
	int len = 0;
	if ( fabs(a)>1e-6 ) // non-zero
	{
		if ( n==0 ) // z^0 term
		{
			if ( fabs(a-1)<1e-6 )
				snprintf(buffer+len,size-len-1,"%s",first?"1":"+1");
			else if ( fabs(-a-1)<1e-6 )
				snprintf(buffer+len,size-len-1,"%s","-1");
			else
				snprintf(buffer+len,size-len-1,first?"%+.4f":"%+.4f",a);
			len = strlen(buffer);
		}
		else // z^n term
		{
			if ( fabs(a-1)>1e-6 ) // non-unitary coefficient
				snprintf(buffer+len,size-len-1,"%+.4f",a);
			else if ( fabs(-a-1)<1e-6 ) // -1 coefficient
				snprintf(buffer+len,size-len-1,"-");
			len = strlen(buffer);
			snprintf(buffer+len,size-len-1,"%s",x); // domain variable
			len = strlen(buffer);
			if ( n>1 ) // higher-order
				snprintf(buffer+len,size-len-1,"^%d",n);
			len = strlen(buffer);
		}
	}
	return len;
}

static size_t dump_vector(double *x, size_t n, char *buffer, size_t maxlen, const char *format="%+7.4g")
{
	snprintf(buffer,maxlen,"[ ");
	size_t pos = strlen(buffer);
	for ( size_t i = 0 ; i < n && pos < maxlen-10; i++ )
	{
		if ( i > 0 )
		{
			pos += snprintf(buffer+pos,maxlen-pos,"%s",", ");
			pos = strlen(buffer);
		} 
		pos += snprintf(buffer+pos,maxlen-pos,format, x[i]);
		pos = strlen(buffer);
	}
	pos += snprintf(buffer+pos,maxlen-pos," ]");		
	pos = strlen(buffer);
	return pos;
}

static size_t dump_function(double *a, size_t n, double *b, size_t m, char *buffer, size_t maxlen)
{
	snprintf(buffer,maxlen,"(");
	size_t pos = strlen(buffer);
	for ( unsigned int i = 0 ; i < n && pos < maxlen-10 ; i++ )
	{
		if ( b[i] == 0.0 )
		{
			continue;
		}
		if ( i == 0 )
		{
			pos += snprintf(buffer+pos,maxlen-pos,"%.4g",b[i]);
			pos = strlen(buffer);
		}
		else 
		{
			if ( b[0] != 1.0 )
			{
				pos += snprintf(buffer+pos,maxlen-pos,"%+.4g",b[i]);
				pos = strlen(buffer);
			}
			else
			{
				pos += snprintf(buffer+pos,maxlen-pos,"%s"," +");	
				pos = strlen(buffer);
			}
			pos += snprintf(buffer+pos,maxlen-pos,"z^-%d", i);
			pos = strlen(buffer);
		}
	}
	pos += snprintf(buffer+pos,maxlen-pos,") / (");
	pos = strlen(buffer);
	for ( unsigned int i = 0 ; i < n && pos < maxlen-10 ; i++ )
	{
		if ( a[i] == 0.0 )
		{
			continue;
		}
		if ( i == 0 )
		{
			pos += snprintf(buffer+pos,maxlen-pos,"%.4g",a[i]);
			pos = strlen(buffer);
		}
		else 
		{
			if ( a[i] != 1.0 )
			{
				pos += snprintf(buffer+pos,maxlen-pos,"%+.4g",a[i]);
				pos = strlen(buffer);
			}
			else
			{
				pos += snprintf(buffer+pos,maxlen-pos,"%s","+");	
				pos = strlen(buffer);
			}
			pos += snprintf(buffer+pos,maxlen-pos,"z^-%d", i);
			pos = strlen(buffer);
		}
	}
	pos += snprintf(buffer+pos,maxlen-pos,")");
	pos = strlen(buffer);
	return pos;
}

int transfer_function_add(char *name,		///< transfer function name
						  char *domain,		///< domain variable name
						  double timestep,	///< timestep (seconds)
						  double timeskew,	///< timeskew (seconds)
						  unsigned int n,		///< denominator order
						  double *a,			///< denominator coefficients
						  unsigned int m,		///< numerator order
						  double *b)			///< numerator coefficients
{
	TRANSFERFUNCTION *tf = (TRANSFERFUNCTION*)malloc(sizeof(TRANSFERFUNCTION));
	if ( tf == NULL )
	{
		output_error("transfer_function_add(): memory allocation failure");
		return 0;
	}
	memset(tf,0,sizeof(TRANSFERFUNCTION));
	strncpy(tf->name,name,sizeof(tf->name)-1);
	strncpy(tf->domain,domain,sizeof(tf->domain)-1);
	tf->timestep = timestep;
	tf->timeskew = timeskew;
	tf->n = n;
	tf->a = (double*)malloc(sizeof(double)*n);
	if ( tf->a == NULL )
	{
		output_error("transfer_function_add(): memory allocation failure");
		return 0;
	}
	for ( unsigned int i = 0 ; i < n ; i++ )
	{
		tf->a[i] = a[n-i-1];
	}
	tf->m = m;
	tf->b = (double*)malloc(sizeof(double)*n);
	if ( tf->b == NULL )
	{
		output_error("transfer_function_add(): memory allocation failure");
		return 0;
	}
	for ( unsigned int i = 0 ; i < m ; i++ )
	{
		tf->b[i+n-m] = b[m-i-1];
	}
	if ( m < n )
	{
		memset(tf->b,0,sizeof(double)*(n-m));
	}
	tf->next = tflist;
	tflist = tf;

	// debugging output
	IN_MYCONTEXT
	{
		char buffer[1024];
		dump_function(a,n,b,m,buffer,sizeof(buffer)-1);
		output_debug("%s(%s) = %s",name,domain,buffer);
	}
	return 1;
}

TRANSFERFUNCTION *transform_find_filter(const char *name)
{
	TRANSFERFUNCTION *tf;
	for ( tf = tflist ; tf != NULL ; tf = tf->next )
	{
		if ( strcmp(tf->name,name) == 0 )
		{
			return tf;
		}
	}
	return NULL;
}

int transfer_function_constrain(char *tfname, unsigned int64 flags, unsigned int64 nbits, double minimum, double maximum)
{
	TRANSFERFUNCTION *tf = transform_find_filter(tfname);
	if ( tf==NULL )
	{
		output_error("transfer_function_constrain(name='%s',...): transfer function not found",tfname);
		return 0;
	}
	tf->flags = flags;
	tf->resolution = (maximum-minimum)/pow(2.0,(double)nbits);
	tf->minimum = minimum;
	tf->maximum = maximum;
	if ( tf->flags != FC_NONE )
	{
		if ( tf->flags&FC_MINIMUM && tf->flags&FC_MAXIMUM && tf->minimum >= tf->maximum )
		{
			output_error("transfer function '%s' constraint minimum=%lg is not less than maximum=%lg",tfname,minimum,maximum);
			return 0;
		}
		else if ( tf->flags&FC_RESOLUTION && ( nbits == 0 || nbits > 64 ) )
		{
			output_error("transfer function '%s' constraint resolution=%lld bits is not valid",tfname,nbits);
			return 0;
		}
		else
		{
			IN_MYCONTEXT output_debug("transfer function '%s' constraint to range (%lg,%lg( with resolution %lg (%lld bits, flags=0x%llx)",tfname,minimum,maximum,tf->resolution, nbits, flags);
		}
	}
	return 1;
}

TRANSFORMSOURCE get_source_type(PROPERTY *prop)
{
	/* TODO extend this to support multiple sources */
	switch ( prop->ptype ) {
	case PT_double: return XS_DOUBLE;
	case PT_complex: return XS_COMPLEX;
	case PT_loadshape: return XS_LOADSHAPE;
	case PT_enduse: return XS_ENDUSE;
	case PT_random: return XS_RANDOMVAR;
	default:
		output_error("tranform/get_source_type(PROPERTY *prop='%s'): unsupported source property type '%s'",
			prop->name,property_getspec(prop->ptype)->name);
		return XS_UNKNOWN;
	}
}

static bool read_vector(const char *name, double *vector, size_t len)
{
	char values[1024];
	if ( global_getvar(name,values,sizeof(values)) == NULL )
	{
		output_error("transform.cpp/read_vector(name='%s',vector=%x,len=%ld): global variable not found",name,vector,len);
		return false;
	}
	else
	{
		IN_MYCONTEXT output_debug("transform.cpp/read_vector(name='%s',vector=%x,len=%ld): value = '%s'",name,vector,len,values);
	}
	char *p = values;
	int n;
	for ( n = len-1 ; n >= 0 && p != (char*)1 ; n--,p++ )
	{
		vector[n] = atof(p);
		p = strchr(p,',');
	}
	if ( n >= 0 )
	{
		output_error("transform.cpp/read_vector(name='%s',vector=%x,len=%ld): too few values to read (missing %d values)",name,vector,len,n+1);
		return false;
	}
	else if ( p != (char*)1 )
	{
		output_warning("transform.cpp/read_vector(name='%s',vector=%x,len=%ld): too many values to read ('%s' not scanned)",name,vector,len,p+1);
	}
	IN_MYCONTEXT
	{
		char buffer[1024];
		dump_vector(vector,len,buffer,sizeof(buffer));
		output_debug("transform.cpp/read_vector(name='%s',vector=%x,len=%ld): --> %s",name,vector,len,buffer);
	}
	return true;
}

int transform_add_filter(OBJECT *target_obj,		/* pointer to the target object (lhs) */
						 PROPERTY *target_prop,	/* pointer to the target property */
						 char *filter,			/* filter name to use */
						 OBJECT *source_obj,		/* object containing source value (rhs) */
						 PROPERTY *source_prop,		/* schedule object associated with target value, used if stype == XS_SCHEDULE */
						 const char *input_name,		/* name of global containing initial input vector */
						 const char *state_name)      /* name of global containing initial state vector */
{
	char buffer1[1024], buffer2[1024];
	TRANSFORM *xform;
	TRANSFERFUNCTION *tf;

	// find the filter
	tf = transform_find_filter(filter);
	if ( tf == NULL )
	{
		output_error("transform_add_filter(source='%s:%s',filter='%s',target='%s:%s'): transfer function not defined",
			object_name(target_obj,buffer1,sizeof(buffer1)),target_prop->name,filter, object_name(source_obj,buffer2,sizeof(buffer2)),source_prop->name);
		return 0;
	}

	// allocate memory for the transform
	xform = (TRANSFORM*)malloc(sizeof(TRANSFORM));
	if ( xform == NULL )
	{
		output_error("transform_add_filter(source='%s:%s',filter='%s',target='%s:%s'): memory allocation failure",
			object_name(target_obj,buffer1,sizeof(buffer1)),target_prop->name,filter, object_name(source_obj,buffer2,sizeof(buffer2)),source_prop->name);
		return 0;
	}
	xform->x = (double*)malloc(sizeof(double)*(tf->n));
	xform->u = (double*)malloc(sizeof(double)*(tf->n));
	if ( xform->x == NULL || xform->u == NULL )
	{
		output_error("transform_add_filter(source='%s:%s',filter='%s',target='%s:%s'): memory allocation failure",
			object_name(target_obj,buffer1,sizeof(buffer1)),target_prop->name,filter, object_name(source_obj,buffer2,sizeof(buffer2)),source_prop->name);
		free(xform);
		return 0;
	}
	if ( state_name )
	{
		read_vector(state_name,xform->x,tf->n);
	}
	else
	{
		memset(xform->x,0,sizeof(double)*(tf->n));
	}
	if ( input_name )
	{
		read_vector(input_name,xform->u,tf->m);
	}
	else
	{
		memset(xform->u,0,sizeof(double)*(tf->n));
	}

	// build tranform
	xform->source = object_get_double_by_name(source_obj,source_prop->name);
	xform->source_type = get_source_type(source_prop);
	xform->target_obj = target_obj;
	xform->target_prop = target_prop;
	xform->function_type = XT_FILTER;
	xform->tf = tf;
	xform->t_last = TS_ZERO;
	xform->y = object_get_double_by_name(target_obj,target_prop->name);
	xform->t2 = (int64)(global_starttime/tf->timestep)*tf->timestep + tf->timeskew;
	xform->next = schedule_xformlist;
	schedule_xformlist = xform;

	IN_MYCONTEXT output_debug("added filter '%s' from source '%s:%s' to target '%s:%s'", filter,
 		object_name(target_obj,buffer1,sizeof(buffer1)),target_prop->name,object_name(source_obj,buffer2,sizeof(buffer2)),source_prop->name);
	return 1;

}

int transform_add_external(	OBJECT *target_obj,		/* pointer to the target object (lhs) */
							PROPERTY *target_prop,	/* pointer to the target property */
							const char *function,			/* function name to use */
							OBJECT *source_obj,		/* object containing source value (rhs) */
							PROPERTY *source_prop)		/* schedule object associated with target value, used if stype == XS_SCHEDULE */
{
	char buffer1[1024], buffer2[1024];
	TRANSFORM *xform = (TRANSFORM*)malloc(sizeof(TRANSFORM));
	if (xform==NULL)
		return 0;
	if ( (xform->function = module_get_transform_function(function))==NULL )
	{
		output_error("transform_add_external(source='%s:%s',function='%s',target='%s:%s'): function is not defined (probably a missing or invalid extern directive)", 
			object_name(target_obj,buffer1,sizeof(buffer1)),target_prop->name,function, object_name(source_obj,buffer2,sizeof(buffer2)),source_prop->name);
		free(xform);
		return 0;
	}

	xform->function_type = XT_EXTERNAL;

	/* apply source type */
	xform->source_type = get_source_type(source_prop);

	/* build lhs (target) */
	/* TODO extend to support multiple targets, e.g., complex */
	xform->nlhs = 1;
	xform->plhs = gldvar_create(xform->nlhs);
	gldvar_set(xform->plhs,0,object_get_addr(target_obj,target_prop->name),target_prop);
	xform->target_obj = target_obj;
	xform->target_prop = target_prop;

	/* build rhs (source) */
	/* TODO extend this to support more than one source */
	xform->nrhs = 1;
	xform->prhs = gldvar_create(xform->nrhs);
	gldvar_set(xform->prhs,0,object_get_addr(source_obj,source_prop->name),source_prop);

	xform->next = schedule_xformlist;
	schedule_xformlist = xform;
	IN_MYCONTEXT output_debug("added external transform %s:%s <- %s(%s:%s)", object_name(target_obj,buffer1,sizeof(buffer1)),target_prop->name,function, object_name(source_obj,buffer2,sizeof(buffer2)),source_prop->name);
	return 1;
}
int transform_add_linear(	TRANSFORMSOURCE stype,	/* specifies the type of source */
							double *source,		/* pointer to the source value */
							void *target,		/* pointer to the target value */
							double scale,		/* transform scalar */
							double bias,			/* transform offset */
							OBJECT *obj,			/* object containing target value */
							PROPERTY *prop,		/* property associated with target value */
							SCHEDULE *sched)		/* schedule object assoicated with target value, if stype == XS_SCHEDULE */
{
	char buffer[1024];
	TRANSFORM *xform = (TRANSFORM*)malloc(sizeof(TRANSFORM));
	if (xform==NULL)
		return 0;
	xform->source_type = stype;
	xform->source = source;
	xform->nrhs = 1;
	xform->nlhs = 1;
	xform->source_addr = source; /* this assumes the double is the first member of the structure */
	xform->source_schedule = sched;
	xform->target_obj = obj;
	xform->target_prop = prop;
	xform->target = (double*)target;
	xform->scale = scale;
	xform->bias = bias;
	xform->function_type = XT_LINEAR;
	xform->next = schedule_xformlist;
	schedule_xformlist = xform;
	IN_MYCONTEXT output_debug("added linear transform %s:%s <- scale=%.3g, bias=%.3g", object_name(obj,buffer,sizeof(buffer)), prop->name, scale, bias);
	return 1;
}

void cast_from_double(PROPERTYTYPE ptype, void *addr, double value)
{
	switch ( ptype ) {
	case PT_void: break;
	case PT_double: *(double*)addr = value; break;
	case PT_complex: ((complex*)addr)->Re() = value; ((complex*)addr)->Im() = 0; break;
	case PT_bool: *(int32*)addr = (value!=0); 
	case PT_int16: *(int16*)addr = (int16)value; break;
	case PT_int32: *(int32*)addr = (int32)value; break;
	case PT_int64: *(int64*)addr = (int64)value; break;
	case PT_enumeration: *(enumeration*)addr = (enumeration)value; break;
	case PT_set: *(set*)addr = (set)value; break;
	case PT_object: break;
	case PT_timestamp: *(int64*)addr = (int64)value; break;
	case PT_float: *(float*)addr = (float)value; break;
	case PT_loadshape: ((loadshape*)addr)->load = value; break;
	case PT_enduse: ((enduse*)addr)->total.Re() = value; ((enduse*)addr)->total.Im() = 0; break;
	case PT_random: ((randomvar*)addr)->value = value; break;
 	default: 
 		output_error("transform.c/cast_from_double(PROPERTYTYPE ptype=0x%x, void *addr=0x%p, double value=%lg): unsupported property type",ptype,addr,value);
 		break;
	}
}

TIMESTAMP apply_filter(TRANSFERFUNCTION *f,	///< transfer function
					   double *src,			///< next input value
					   double *u,			///< input vector
					   double *x,			///< state vector
					   double *y,			///< output vector
					   TIMESTAMP t1)		///< current time value
{
	unsigned int n = f->n;
	unsigned int m = f->m;
	double *a = f->a;
	double *b = f->b;

	char buffer[1024] = "";
	IN_MYCONTEXT 
	{
		dump_function(a,n,b,m,buffer,sizeof(buffer));
		output_debug("apply_transform(f={name='%s'; domain='%s'}): tf = %s",f->name,f->domain, buffer);
		dump_vector(a,n,buffer,sizeof(buffer));
		output_debug("apply_transform(f={name='%s'; domain='%s'}): a = %s",f->name,f->domain, buffer);
		dump_vector(b,n,buffer,sizeof(buffer));
		output_debug("apply_transform(f={name='%s'; domain='%s'}): b = %s",f->name,f->domain, buffer);
	}

	// update input
	memmove(u+1,u,sizeof(double)*(n-1));
	u[0] = *src;
	IN_MYCONTEXT
	{
		dump_vector(u,n,buffer,sizeof(buffer));
		output_debug("apply_transform(f={name='%s'; domain='%s'}): u = %s",f->name,f->domain, buffer);
	}

	// update state
	for ( unsigned int i = 0 ; i < n ; i++ )
	{
		x[0] += b[i]*u[i];
	}

	// update output
	y[0] = x[0] / a[0];
	IN_MYCONTEXT output_debug("apply_transform(f={name='%s'; domain='%s'}): y = %g",f->name,f->domain, *y);

	IN_MYCONTEXT
	{
		dump_vector(x,n,buffer,sizeof(buffer));
		output_debug("apply_transform(f={name='%s'; domain='%s'}): x = %s",f->name,f->domain, buffer);
	}

	return ((int64)(t1/f->timestep)+1)*f->timestep + f->timeskew;
}

void transform_reset(TRANSFORM *xform, TIMESTAMP t1)
{
	if ( xform->t_last == t1 )
	{
		return;
	}
	switch ( xform->function_type )
	{
	case XT_LINEAR:
		if ( xform->target != NULL )
		{
			xform->target[0] = 0.0;
		}
		break;
	case XT_EXTERNAL:
		if ( xform->plhs != NULL && xform->nlhs > 0 )
		{
			for ( int i = 0 ; i < xform->nlhs ; i++ )
			{
				*(double*)(xform->plhs[i].addr) = 0.0;
			}
		}
		break;
	case XT_FILTER:
		if ( xform->y != NULL )
		{
			char buffer[1024];
			TRANSFERFUNCTION *f = xform->tf;
			double *x = xform->x;
			double *a = f->a;
			int n = f->n;
			double *y = xform->y;

			// update state
			double x0 = 0.0;
			for ( int i = n-1 ; i > 0 ; i-- )
			{
				x[i] = x[i-1];	
				x0 -= a[i]*x[i];
			}
			x[0] = x0;
			IN_MYCONTEXT
			{
				dump_vector(x,n,buffer,sizeof(buffer));
				output_debug("transform_reset(f={name='%s'; domain='%s'},t1=%ld): x = %s",f->name,f->domain, t1, buffer);
			}

			// update output
			y[0] = x[0] / a[0];
			if ( ((f->flags)&FC_MINIMUM) == FC_MINIMUM && y[0] < f->minimum && f->minimum < f->maximum )
		 	{
		 		y[0] = f->minimum;
		 	}
		 	else if ( ((f->flags)&FC_MAXIMUM) == FC_MAXIMUM && y[0] > f->maximum && f->minimum < f->maximum  )
		 	{
		 		y[0] = f->maximum;
		 	}
		 	if ( ((f->flags)&FC_RESOLUTION) == FC_RESOLUTION && f->resolution > 0.0 )
		 	{
		 		y[0] = floor((*y - f->minimum)/f->resolution)*f->resolution + f->minimum;
		 	}	
			IN_MYCONTEXT output_debug("transform_reset(f={name='%s'; domain='%s'},t1=%ld): y = %.4g",f->name,f->domain, t1, y[0]);

		}
		break;
	default:
		output_error("transform_reset(): invalid function type %d",xform->function_type);
		break;
	}
	xform->t_last = t1;
}

/** apply the transform, source is optional and xform.source is used when source is NULL 
    @return timestamp for next update, TS_NEVER for none, TS_ZERO for error
**/
TIMESTAMP transform_apply(TIMESTAMP t1, TRANSFORM *xform, double *source)
{
	TIMESTAMP t2;
	switch (xform->function_type) {
	case XT_LINEAR:
		IN_MYCONTEXT output_debug("running linear transform for %s:%s", object_name(xform->target_obj), xform->target_prop->name);
		transform_reset(xform,t1);
		cast_from_double(xform->target_prop->ptype, xform->target, (source?(*source):(*(xform->source))) * xform->scale + xform->bias);
		t2 = TS_NEVER;
		break;
	case XT_EXTERNAL:
		IN_MYCONTEXT output_debug("running external transform for %s:%s", object_name(xform->target_obj), xform->target_prop->name);
		transform_reset(xform,t1);
		xform->retval = (*xform->function)(xform->nlhs, xform->plhs, xform->nrhs, xform->prhs);
		if ( xform->retval==-1 ) /* error */
			t2 = TS_ZERO;
		else if ( xform->retval==0 ) /* no timer */
			t2 = TS_NEVER;
		else
			t2 = t1 + xform->retval; /* timer given */
		break;
	case XT_FILTER:
		IN_MYCONTEXT output_debug("running filter transform for %s:%s", object_name(xform->target_obj), xform->target_prop->name);
		if ( xform->t2 <= t1 )
		{
			transform_reset(xform,t1);
			xform->t2 = apply_filter(xform->tf,xform->source,xform->u,xform->x,xform->y,t1);
		}
		t2 = xform->t2;
		break;
	default:
		output_error("transform_apply(): invalid function type %d", xform->function_type);
		t2 = TS_ZERO;
		break;
	}
	return t2;
}

clock_t transform_synctime = 0;
TIMESTAMP transform_syncall(TIMESTAMP t1, TRANSFORMSOURCE source)
{
	TRANSFORM *xform;
	clock_t start = (clock_t)exec_clock();
	TIMESTAMP t2 = TS_NEVER;
	TIMESTAMP tskew, t;

	/* process the schedule transformations */
	IN_MYCONTEXT output_debug("transform_syncall(t1=%lld, TRANSFORMSOURCE=0x%04llx): entering",t1,(int64)source);
	for (xform=schedule_xformlist; xform!=NULL; xform=xform->next)
	{	
		IN_MYCONTEXT output_debug("transform_syncall(t1=%lld, TRANSFORMSOURCE=0x%04llx): xform->source_type = %04llx, &source = %04llx",t1,(int64)source,(int64)xform->source_type,(int64)(xform->source_type&source));
		if ( xform->source_type == XS_UNKNOWN )
 			output_warning("transform_syncall(...): transform to property '%s' of object '%s' has an unknown source type, it will always be run", xform->target_prop->name, xform->target_obj->name?xform->target_obj->name:"(unnamed)");
 		if ( xform->source_type == XS_UNKNOWN || (xform->source_type&source)!=0 )
 		{
			if ( ( xform->source_type == XS_SCHEDULE ) 
			  && ( xform->target_obj->schedule_skew != 0 ) )
			{
				IN_MYCONTEXT output_debug("transform_syncall(t1=%lld, TRANSFORMSOURCE=0x%04llx): skew = %lld",t1,(int64)source,xform->target_obj->schedule_skew);
			    tskew = t1 - xform->target_obj->schedule_skew; // subtract so the +12 is 'twelve seconds later', not earlier
			    SCHEDULEINDEX index = schedule_index(xform->source_schedule,tskew);
			    int32 dtnext = schedule_dtnext(xform->source_schedule,index)*60;
			    double value = schedule_value(xform->source_schedule,index);
			    t = (dtnext == 0 ? TS_NEVER : t1 + dtnext - (tskew % 60));
			    if ( t < t2 ) t2 = t;
				if ( ( tskew <= xform->source_schedule->since ) 
						|| ( tskew >= xform->source_schedule->next_t ) )
				{
					t = transform_apply(t1,xform,&value);
					if ( t<t2 ) t2=t;
				} 
				else 
				{
					t = transform_apply(t1,xform,NULL);
					if ( t<t2 ) t2=t;
				}
			} 
			else 
			{
				t = transform_apply(t1,xform,NULL);
				if ( t<t2 ) t2=t;
			}
		}
	}
	transform_synctime += (clock_t)exec_clock() - start;
	return t2;
}

const char *get_source_name(void *addr)
{
	static char source_name[1024];
	OBJECT *obj;
	PROPERTY *prop;
	if ( ! object_locate_property(addr,&obj,&prop) )
		return NULL;
	if ( obj->name )
	{
		snprintf(source_name,sizeof(source_name)-1,"%s.%s",obj->name,prop->name);
	}
	else
	{
		snprintf(source_name,sizeof(source_name)-1,"%s:%d.%s",obj->oclass->name,obj->id,prop->name);
	}
	return source_name;
}

TRANSFORM *transform_has_target(void *addr)
{
	for ( TRANSFORM *xform = schedule_xformlist ; xform != NULL ; xform = xform->next )
	{
		if ( (void*)(xform->target) == addr )
			return xform;
	}
	return NULL;
}

TRANSFORM *transform_find(OBJECT *obj, PROPERTY *prop)
{
	for ( TRANSFORM *xform = transform_getnext(NULL) ; xform != NULL ; xform = transform_getnext(xform) )
	{
		if ( xform->target_obj == obj && xform->target_prop == prop )
		{
			return xform;
		}
	}
	return NULL;
}

int transform_to_string(char *buffer, int size, TRANSFORM *xform)
{
	int count = 0;
	double *source = xform->source;
	const char * source_name;
	switch ( xform->function_type ) {
	case XT_LINEAR:
		source_name = xform->source_schedule?xform->source_schedule->name:get_source_name(source);
		if ( source_name == NULL && get_source_name(source) )
			source_name = "(source not found)";
		snprintf(buffer,size,"%s*%g+%g",source_name,xform->scale,xform->bias);
		count = strlen(buffer);
		break;
	case XT_EXTERNAL:
		snprintf(buffer,size,"%s(",module_find_transform_function(xform->function));
		count = strlen(buffer);
		for ( int n = 0 ; n < xform->nrhs ; n++ )
		{
			if ( n > 0 )
			{
				snprintf(buffer+count,size-count-1,",");
				count = strlen(buffer);
			}
			snprintf(buffer+count,size-count-1,"%s",xform->prhs[n].prop->name);
			count = strlen(buffer);
		}
		snprintf(buffer+count,size-count-1,")");
		count = strlen(buffer);
		break;
	case XT_FILTER:
		source_name = get_source_name(source);
		if ( source_name == NULL && get_source_name(source) )
			source_name = "(source not found)";
		snprintf(buffer,size,"%s(%s)", xform->tf->name, source_name);
		count = strlen(buffer);
		break;
	default:
		throw_exception("transform_to_string(char *buffer=%p, int size=%d, TRANSFORM *xform=%p): xform->source_type = %d is invalid", buffer, size, xform, xform->source_type);
		break;
	}
	return count;

}
int transform_write(TRANSFORM *xform, FILE *fp)
{
	char buffer[1024];
	if ( transform_to_string(buffer,sizeof(buffer)-1,xform) > 0 )
	{
		return fprintf(fp,"%s",buffer);
	}
	else
	{
		throw_exception("transform_write(TRANSFORM *xform=%p, FILE *fp=%p): unable to write transform to buffer", xform, fp);
		return -1;
	}
}

int transform_saveall(FILE *fp)
{
	int count = 0;
	TRANSFERFUNCTION *tf;
	for ( tf = tflist ; tf != NULL ; tf = tf->next )
	{
		count += fprintf(fp,"filter %s(%s,%gs,%gs", tf->name, tf->domain, tf->timestep, tf->timeskew);
		if ( tf->resolution > 0 )
		{
			count += fprintf(fp,",resolution=%g",tf->resolution);
		}
		if ( tf->minimum < tf->maximum )
		{
			count += fprintf(fp,",minimum=%g,maximum=%g",tf->minimum,tf->maximum);
		}
		count += fprintf(fp,") = (");
		for ( int m = tf->m - 1 ; m >= 0 ; m-- )
		{
			if ( tf->b[m] != 1.0 || m == 0 )
			{
				count += fprintf(fp,"%+g",tf->b[m]);
			}
			if ( m > 0 )
			{
				count += fprintf(fp,"%s",tf->domain);
			}
			if ( m > 1 )
			{
				count += fprintf(fp,"^%d",m);
			}
		}
		count += fprintf(fp,") / (");
		for ( int n = tf->n - 1 ; n >= 0 ; n-- )
		{
			if ( tf->a[n] != 1.0 || n == 0 )
			{
				count += fprintf(fp,"%+g",tf->a[n]);
			}
			if ( n > 0 )
			{
				count += fprintf(fp,"%s",tf->domain);
			}
			if ( n > 1 )
			{
				count += fprintf(fp,"^%d",n);
			}
		}
		count += fprintf(fp,");");
	}
	return count;
}

TRANSFERFUNCTION *transfer_function_getfirst(void)
{
	return tflist;
}

int transfer_function_to_json(char *buffer, int size, TRANSFERFUNCTION *tf)
{
	int count = 0;
	
	snprintf(buffer+count,size-count,"\"%s\" : {",tf->name);
	count = strlen(buffer);

	snprintf(buffer+count,size-count,"\"domain\":\"%s\",",tf->domain);
	count = strlen(buffer);

	snprintf(buffer+count,size-count,"\"timestep\":\"%gs\",",tf->timestep);
	count = strlen(buffer);

	snprintf(buffer+count,size-count,"\"timeskew\":\"%gs\",",tf->timeskew);
	count = strlen(buffer);

	int flen = 0;
	char flags[256] = "";
	if ( tf->flags&FC_MINIMUM )
	{
		snprintf(flags+flen,sizeof(flags)-flen,"%sMINIMUM",flen>0?"|":"");
		flen = strlen(flags);

		snprintf(buffer+count,size-count,"\"minimum\":\"%g\",",tf->minimum);
		count = strlen(buffer);
	}
	if ( tf->flags&FC_MAXIMUM )
	{
		snprintf(flags+flen,sizeof(flags)-flen,"%sMAXIMUM",flen>0?"|":"");
		flen = strlen(flags);
	
		snprintf(buffer+count,size-count,"\"maximum\":\"%g\",",tf->maximum);
		count = strlen(buffer);
	}
	if ( tf->flags & FC_RESOLUTION )
	{
		snprintf(flags+flen,sizeof(flags)-flen,"%sRESOLUTION",flen>0?"|":"");
		flen = strlen(flags);

		snprintf(buffer+count,size-count,"\"resolution\":\"%g\",",tf->resolution);
		count = strlen(buffer);
	}
	snprintf(buffer+count,size-count,"\"numerator\":[");
	count = strlen(buffer);
	
	for ( unsigned int m = 0 ; m < tf->m ; m++ )
	{
		snprintf(buffer+count,size-count,"%g%s",tf->b[m],m<tf->m-1?",":"");
		count = strlen(buffer);
	}
	snprintf(buffer+count,size-count,"],");
	count = strlen(buffer);

	snprintf(buffer+count,size-count,"\"denominator\":[");
	count = strlen(buffer);

	for ( unsigned int n = 0 ; n < tf->n ; n++ )
	{
		count += snprintf(buffer+count,size-count,"%g%s",tf->a[n],n<tf->n-1?",":"");
	}
	snprintf(buffer+count,size-count,"]");
	count = strlen(buffer);

	snprintf(buffer+count,size-count,"}");
	count = strlen(buffer);
	
	return count;
}