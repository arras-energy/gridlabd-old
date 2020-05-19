/** Solver modeling
    Enable modeling of solution to improve performance
 **/

#if defined(SOLVER_ML)

#include "gridlabd.h"

#include <stdlib.h>
#include <unistd.h>
#include "solver_ml.h"

#define CONFIGLOG "solver_ml.log"
#define CONFIGNAME "solver_ml.conf"
#define CONFIGPATH "/usr/local/share/gridlabd/"

// default configuration settings
char1024 solver_ml_config = CONFIGPATH CONFIGNAME;
static double maximum_distance = 0; // 0 is never use solver model, 1e-9 is only when nearly identical
static const char *solver_model_logfile = CONFIGLOG;
static int solver_model_loglevel = -1; // -1=disable, 0 = minimal ... 9 = everything,
static size_t maximum_models = 100; // maximum number of models to track
static const char *model_busdump = NULL; // name of bus dumpfile
static const char *model_branchdump = NULL; // name of branch dumpfile
static const char *model_dump_handler = NULL; // name of python solver event handler
static const char *module_import_path = NULL; // path to use when importing modules
static const char *module_import_name = NULL; // module name to import (python only)

static PyObject *pModule = NULL;
static SOLVERMODELSTATUS solver_model_status = SMS_INIT;
static SOLVERMODEL *last = NULL, *first = NULL;
static size_t solver_model_count = 0;
static FILE *solver_model_logfh = NULL;
static struct timeb start;

double solver_model_get_maximum_distance(void)
{
	return maximum_distance;
}

SOLVERMODELSTATUS solver_model_config(const char *localconfig = NULL, 
									  const char *shareconfig = CONFIGPATH CONFIGNAME)
{
	SOLVERMODELSTATUS status = SMS_INIT;
	const char *configname = localconfig ? localconfig : (const char*)solver_ml_config;
	FILE *fp = fopen(configname,"r");
	if ( fp == NULL )
	{
		configname = shareconfig;
		fp = fopen(configname,"r");
	}
	if ( fp != NULL )
	{
		char line[1024];
		while ( fgets(line,sizeof(line),fp) != NULL )
		{
			char tag[1024],value[1024];
			if ( sscanf(line,"%s%s",tag,value) == 2 )
			{
				if ( tag[0] == '#' )
				{
					continue;
				}
				else if ( strcmp(tag,"maximum_metric") == 0 )
				{
					maximum_distance = atof(value);
				}
				else if ( strcmp(tag,"logfile") == 0 )
				{
					solver_model_logfile = strdup(value);
				}
				else if ( strcmp(tag,"loglevel") == 0 )
				{
					solver_model_loglevel = atoi(value);
				}
				else if ( strcmp(tag,"method") == 0 )
				{
					if ( strcmp(value,"none") == 0 )
					{
						status = SMS_DISABLED;
					}
					else if ( strcmp(value,"basic") == 0 )
					{
						status = SMS_READY;
					}
					else 
					{
						fprintf(stderr,"solver_model_config(configname='%s'): method '%s' is not known, using basic method\n",configname,value);
					}
				}
				else if ( strcmp(tag,"busdump") == 0 )
				{
					model_busdump = strdup(value);
				}
				else if ( strcmp(tag,"branchdump") == 0 )
				{
					model_branchdump = strdup(value);
				}
				else if ( strcmp(tag,"on_dump") == 0 )
				{
					model_dump_handler = strdup(value);
				}
				else if ( strcmp(tag,"import") == 0 )
				{
					module_import_name = strdup(value);
				}
				else if ( strcmp(tag,"import_path") == 0 )
				{
					module_import_path = strdup(value);
				}
				else
				{
					fprintf(stderr,"solver_model_config(configname='%s'): tag '%s' is not valid\n",configname,tag);
				}
			}
		}
		fclose(fp);
	}
	return status;
}

void solver_model_log(unsigned int level, const char *format, ...)
{
	if ( (int)level <= solver_model_loglevel && solver_model_logfh != NULL )
	{
		va_list ptr;
		va_start(ptr,format);
		vfprintf(solver_model_logfh,format,ptr);
		fprintf(solver_model_logfh,"\n");
		va_end(ptr);
	}
}

void solver_model_start_timer(void)
{
	ftime(&start);
}

void solver_model_stop_timer(SOLVERMODEL *model, bool apply=false)
{
	struct timeb stop;
	ftime(&stop);
	double timeval = (stop.time-start.time) + (stop.millitm-start.millitm)/1000.0;
	if ( apply == false )
	{
		model->runtime = timeval;
		solver_model_log(1,"solver runtime: %.3f s",timeval);
	}
	else
	{
		solver_model_log(1,"model runtime: %.3f s (%.1f s saved)",timeval,model->runtime-timeval);
	}
}

int solver_model_init(void)
{
	solver_model_start_timer();
	static int solver_model_state = SMS_INIT;
	switch ( solver_model_status )
	{
		case SMS_INIT:
			solver_model_log(1,"solver_model_init(): solver initialization started\n");
			solver_model_status = solver_model_config();
			if ( module_import_name )
			{
				if ( strncmp(module_import_name,"python:",7) == 0 )
				{
					pModule = python_import(module_import_name+7,module_import_path);
					if ( pModule == NULL )
					{
						solver_model_log(0,"solver_model_init(): unable to import python module '%s'\n", module_import_name+7);
						return 0;
					}
				}
				else
				{
					solver_model_log(0,"solver_model_init(): unable to import module '%s'\n", module_import_name);
					return 0;
				}
			}
			if ( solver_model_status == SMS_READY )
			{
				if ( solver_model_logfh != NULL )
				{
					fclose(solver_model_logfh);
				}
				solver_model_logfh = NULL;
				if ( solver_model_loglevel > 0 )
				{
					solver_model_logfh = fopen(solver_model_logfile,"w");
					if ( solver_model_logfh )
					{
						solver_model_log(1,"solver_model_init(): solver log %s opened ok\n",solver_model_logfile);
					}
					else
					{
						solver_model_log(0,"solver_model_init(): solver log %s opened failed (errno=%d, strerror='%s')\n",solver_model_logfile,errno,strerror(errno));
					}
				}
				else
				{
					unlink(solver_model_logfile);
				}
				return 1;
			}
			else
			{
				solver_model_log(0,"solver_model_init(): solver configuration failed\n");
				return 0;
			}
		case SMS_READY:
			if ( solver_model_state != SMS_READY )
			{
				solver_model_log(1,"solver_model_init(): solver ready\n");
				solver_model_state = SMS_READY;
			}
			return 1;
		case SMS_DISABLED:
			if ( solver_model_state != SMS_DISABLED )
			{
				solver_model_log(1,"solver_model_init(): solver initialization disabled\n");
				solver_model_state = SMS_DISABLED;
			}
			return 0;
		case SMS_FAILED:
			if ( solver_model_state != SMS_FAILED )
			{
				solver_model_log(0,"solver_model_init(): solver initialization failed\n");
				solver_model_state = SMS_FAILED;
			}
			return 0;
		default:
			solver_model_log(0,"solver_model_init(): solver state unknown (solver_model_status=%d)\n", solver_model_status);
			return 0;
	}
}

#define MAXDIST 9.999999e99
#define ALPHA 0.5 // parameter to tune how much more important bus metric is than branch metric
double solver_get_metric(SOLVERMODEL *model, // use NULL to get absolute metric for problem id
						 unsigned int &bus_count,
						 BUSDATA *&bus,
						 unsigned int &branch_count,
						 BRANCHDATA *&branch)
{
	if ( model && ( model->bus_count != bus_count || model->branch_count != branch_count) )
	{
		return MAXDIST;
	}
	double bus_dist = 0, branch_dist = 0;
	size_t n;
	unsigned char phases[3] = {0x01,0x02,0x04};
	for ( n = 0 ; n < bus_count ; n++ )
	{
		size_t phase;
		for ( phase = 0 ; phase < 3 ; phase++ )
		{
			if ( (bus[n].phases&phases[phase]) == 0 )
				continue;

			// load contribution to power distance
			complex P = complex(bus[n].PL[phase]);
			if ( model )
			{
				P -= complex(model->bus[n].PL[phase],model->bus[n].QL[phase]);
			}
			double dist = P.Mag();
			if ( dist > 0 && model )
				solver_model_log(2,"bus %d (%s) phase %c distance %g", n, bus[n].name, 'A'+phase, dist);
			bus_dist += dist; 
			
			// TODO: generation contribution to power distance
		}
	}
	for ( n = 0 ; n < branch_count ; n ++ )
	{
		size_t phase;
		for ( phase = 0; phase < 3 ; phase++ )
		{
			// line loss contribution to power distance
			int in = branch[n].from;
			int out = branch[n].to;

			if ( (bus[in].phases&phases[phase]) == 0 
				|| (bus[out].phases&phases[phase]) == 0 )
				continue;

			complex Vin = bus[in].V[phase];
			complex Vout = bus[out].V[phase];
			complex Yin = branch[n].Yfrom[phase];
			complex Yout = branch[n].Yto[phase];
			complex Pin = Vin*~(Vin*Yin);
			complex Pout = Vout*~(Vout*Yout);
			complex Pdiff = (Pin - Pout);
			if ( model )
			{
				Vin = model->bus[in].V[phase];
				Vout = model->bus[out].V[phase];
				Yin = model->branch[n].Yfrom[phase];
				Yout = model->branch[n].Yto[phase];
				Pin = Vin*~(Vin*Yin);
				Pout = Vout*~(Vout*Yout);
				Pdiff -= (Pin - Pout);
			}
			double dist = Pdiff.Mag();
			if ( dist > 0 && model )
				solver_model_log(2,"branch %d (%s) phase %c distance %g", n, branch[n].name, 'A'+phase, dist);
			branch_dist += dist;
		}
		// TODO: calculate self-impedance distance
	}
	double metric = ALPHA*sqrt(bus_dist)/bus_count + (1.0-ALPHA)*sqrt(branch_dist)/branch_count;
	if ( model )
	{
		solver_model_log(1,"model %llx (%dx%d) distance is %g",(model?model->id:0),bus_count,branch_count,metric);
	}
	else
	{
		solver_model_log(1,"new model (%dx%d) distance is %g",bus_count,branch_count,metric);
	}
	return metric;
}

PROBLEMID solver_problem_id(unsigned int bus_count,
							BUSDATA *bus, 
							unsigned int branch_count, 
							BRANCHDATA *branch)
{
	double metric = solver_get_metric(NULL,bus_count,bus,branch_count,branch);
	return (PROBLEMID)(metric*1000.0);
}


void *memdup(const void *mem, size_t bytes)
{
	if ( mem == NULL || bytes == 0 )
	{
		return NULL;
	}
	void *ptr = (void*) new char[bytes];
	return ptr ? memcpy(ptr,mem,bytes) : NULL;
}

void solver_model_del(SOLVERMODEL *model)
{
	solver_model_log(1,"deleted model %x (%dx%d)", model->id, model->bus_count, model->branch_count);
	if ( first == model )
	{
		first = model->prev;
	}
	if ( last == model )
	{
		last = model->next;
	}
	if ( model->prev != NULL )
	{
		model->prev->next = model->next;
	}
	if ( model->next != NULL )
	{
		model->next->prev = model->prev;
	}
	delete [] model->bus;
	delete [] model->branch;
	delete [] model->mesh_imped_values;
	if ( model->bad_computations != NULL ) 
	{
		delete [] model->bad_computations;
	}
	delete model;
	solver_model_count--;
}

SOLVERMODEL *solver_model_new(unsigned int bus_count,
							  BUSDATA *bus, 
							  unsigned int branch_count, 
							  BRANCHDATA *branch, 
							  NR_SOLVER_STRUCT *powerflow_values, 
							  NRSOLVERMODE powerflow_type , 
							  NR_MESHFAULT_IMPEDANCE *mesh_imped_values, 
							  bool *bad_computations,
							  int64 iterations)
{
	SOLVERMODEL *model = new SOLVERMODEL;
	if ( ! model )
	{
		solver_model_log(0,"memory allocation failed");
		return NULL;
	}
	model->id = solver_problem_id(bus_count,bus,branch_count,branch);
	model->bus_count = bus_count;
	model->bus = (BUSDATA*)memdup((void*)bus,sizeof(BUSDATA)*bus_count);
	// TODO: check to see if any contents also should be duplicated (e.g., links)
	model->branch_count = branch_count;
	model->branch = (BRANCHDATA*)memdup((void*)branch,sizeof(BRANCHDATA)*branch_count);
	// TODO: check to see if any contents also should be duplicated (e.g., Y values)
	model->powerflow_values = (NR_SOLVER_STRUCT*)memdup((void*)powerflow_values,sizeof(NR_SOLVER_STRUCT));
	model->powerflow_type = powerflow_type;
	model->mesh_imped_values = (NR_MESHFAULT_IMPEDANCE*)memdup((void*)mesh_imped_values,sizeof(NR_MESHFAULT_IMPEDANCE));
	model->bad_computations = ( bad_computations ? NULL : (bool*)memdup((void*)bad_computations,sizeof(bool)) );
	model->iterations = iterations;
	model->next = last;
	model->prev = NULL;
	solver_model_count++;
	if ( first == NULL )
	{
		first = model;
	}
	else if ( solver_model_count > maximum_models )
	{
		solver_model_del(first);
	}
	if ( last != NULL )
	{
		last->prev = model;
	}
	last = model;
	solver_model_log(1,"adding model %x (%dx%d)", model->id, bus_count, branch_count);
	solver_model_stop_timer(model);
	return last;
}

// iterators
SOLVERMODEL *solver_model_getfirst() 
{
	return last; // LIFO
}
SOLVERMODEL *solver_model_getnext(SOLVERMODEL *model)
{
	return model->next;
}
bool solver_model_islast(SOLVERMODEL *model)
{
	return model ? model->next == NULL : true;
}

void solver_dump(unsigned int &bus_count,
				 BUSDATA *&bus,
				 unsigned int &branch_count,
				 BRANCHDATA *&branch,
				 bool allow_generic_dumpfile)
{
	FILE *fh;
	const char *bustype[] = {"PQ","PV","SWING","SWING_PG"};
	const char *busflag[] = {"NONE","HASSOURCE","ISSOURCE","HASSOURCE|ISSOURCE"};
	char phases[256][9];
	for ( int c = 0 ; c < 256 ; c++ )
	{
		memset(phases[c],0,sizeof(phases[c]));
		if ( c&0x80 ) strcat(phases[c],"S"); // split
		if ( c&0x20 ) strcat(phases[c],"X"); // transformer
		if ( c&0x04 ) strcat(phases[c],"A"); // phase A
		if ( c&0x02 ) strcat(phases[c],"B"); // phase B
		if ( c&0x01 ) strcat(phases[c],"C"); // phase C
		if ( c&0x08 ) strcat(phases[c],"D"); // delta
		if ( c&0x10 ) strcat(phases[c],"Z"); // different children	
		if ( c&0x40 ) strcat(phases[c],"H"); // house connected node
	}
	//
	// Bus dump
	//
	if ( model_busdump == NULL && allow_generic_dumpfile )
		model_busdump = "busdump.csv";
	gl_verbose("dumping bus data to '%s'",model_busdump);
	fh = fopen(model_busdump,"w");
	if ( fh == NULL )
	{
		solver_model_log(1,"unable to open bus dumpfile '%s' for write", model_busdump);
		return;
	}
	fprintf(fh,"id,name,type,phases,origphases,busflags,"
		"vbase,mvabase,"
#define POLAR(X) #X "Am," #X "Aa," #X "Bm," #X "Ba," #X "Cm," #X "Ca,"
#define RECT(X)  #X "Ar," #X "Ai," #X "Br," #X "Bi," #X "Cr," #X "Ci,"
#define DELIM ""
#include "solver_ml_branchdump.h"
#undef POLAR
#undef RECT
#undef DELIM
		"PA,QA,PB,QB,PC,QC,"
		"YcAAr,YcAAi,YcABr,YcABi,YcACr,YcACi,YcBBr,YcBBi,YcBCr,YcBCu,YcCCr,YcCCi,"
		"YsAAr,YsAAi,YsABr,YsABi,YsACr,YsACi,YsBBr,YsBBi,YsBCr,YsBCi,YsCCr,YsCCi,"
		"YlAr,YlAi,YlBr,YlBi,YlCr,YlCi,"
		"JA0,JA1,JA2,"
		"JB0,JB1,JB2,"
		"JC0,JC1,JC2,"
		"JD0,JD1,JD2,"
		"EOL\n");
	for ( unsigned int m = 0 ; m < bus_count ; m++ )
	{
		BUSDATA *b = bus + m;
		char bustypeerr[32] = "ERROR", phaseserr[32] = "ERROR", origphaseserr[32] = "ERROR", busflagerr[32] = "ERROR";
		if ( bustype[b->type] == NULL ) snprintf(bustypeerr,sizeof(bustypeerr)-1,"ERROR:%d",b->type);
		if ( phases[b->phases] == NULL ) snprintf(phaseserr,sizeof(phaseserr)-1,"ERROR:%d",(int)b->phases);
		if ( phases[b->origphases] == NULL ) snprintf(origphaseserr,sizeof(origphaseserr)-1,"ERROR:%d",(int)b->origphases);
		if ( busflag[*(b->busflag)] == NULL ) snprintf(busflagerr,sizeof(busflagerr)-1,"ERROR:%d",(int)*(b->busflag));
		fprintf(fh,"%d,%s,%s,%s,%s,%s,"
			// Vbase and MVAbase
			"%g,%g,"
#define POLAR(X) "%g,%g,%g,%g,%g,%g," 
#define RECT(X) "%g,%g,%g,%g,%g,%g," 
#define DELIM ""
#include "solver_ml_branchdump.h"
#undef POLAR
#undef RECT
#undef DELIM
			// loads
			"%g,%g,%g,%g,%g,%g,"
			// Y matrices
			"%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,"
			"%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,%g,"
			"%g,%g,%g,%g,%g,%g,"
			// Jacobians
			"%g,%g,%g,"
			"%g,%g,%g,"
			"%g,%g,%g,"
			"%g,%g,%g,"
			"%s\n",
			m,b->name?b->name:"",bustype[b->type]?bustype[b->type]:bustypeerr,phases[b->phases]?phases[b->phases]:phaseserr,phases[b->origphases]?phases[b->origphases]:origphaseserr,*(b->busflag)<sizeof(busflag)/sizeof(busflag[0])?busflag[*(b->busflag)]:busflagerr,
			b->volt_base > 0 ? b->volt_base : QNAN, b->mva_base > 0 ? b->mva_base : QNAN,
#define POLAR(X) (b->X)==NULL?QNAN:(b->X)[0].Mag(),(b->X)==NULL?QNAN:(b->X)[0].Ang(),(b->X)==NULL?QNAN:(b->X)[1].Mag(),(b->X)==NULL?QNAN:(b->X)[1].Ang(),(b->X)==NULL?QNAN:(b->X)[2].Mag(),(b->X)==NULL?QNAN:(b->X)[2].Ang()
#define RECT(X) (b->X)==NULL?QNAN:(b->X)[0].Re(),(b->X)==NULL?QNAN:(b->X)[0].Im(),(b->X)==NULL?QNAN:(b->X)[1].Re(),(b->X)==NULL?QNAN:(b->X)[1].Im(),(b->X)==NULL?QNAN:(b->X)[2].Re(),(b->X)==NULL?QNAN:(b->X)[2].Im()
#define DELIM ,
#include "solver_ml_branchdump.h"
#undef POLAR
#undef RECT
#undef DELIM
#define RECT(X,N) (b->X!=NULL)?(b->X[N]).Re():QNAN,(b->X!=NULL)?(b->X[N]).Im():QNAN
			b->PL[0],b->QL[0],b->PL[1],b->QL[1],b->PL[2],b->QL[2],
			RECT(full_Y,0),RECT(full_Y,1),RECT(full_Y,2),RECT(full_Y,4),RECT(full_Y,5),RECT(full_Y,8),
			RECT(full_Y_all,0),RECT(full_Y_all,1),RECT(full_Y_all,2),RECT(full_Y_all,4),RECT(full_Y_all,5),RECT(full_Y_all,8),
			RECT(full_Y_load,0),RECT(full_Y_load,1),RECT(full_Y_load,2),
			b->Jacob_A[0],b->Jacob_A[1],b->Jacob_A[2],
			b->Jacob_B[0],b->Jacob_B[1],b->Jacob_B[2],
			b->Jacob_C[0],b->Jacob_C[1],b->Jacob_C[2],
			b->Jacob_D[0],b->Jacob_D[1],b->Jacob_D[2],
			"EOL");
	}
	fclose(fh);

	//
	// Branch dump
	//
	if ( model_branchdump == NULL && allow_generic_dumpfile )
		model_branchdump = "branchdump.csv";
	gl_verbose("dumping branch data to '%s'",model_branchdump);
	fh = fopen(model_branchdump,"w");
	if ( fh == NULL )
	{
		solver_model_log(1,"unable to open branch dumpfile '%s' for write", model_branchdump);
		return;
	}
	fprintf(fh,"id,name,type,phases,origphases,faultphases,from,to,fault_link_below,v_ratio,vratio,"
		"YfromAr,YfromAi,YfromBr,YfromBi,YfromCr,YfromCi,"
		"YtoAr,YtoAi,YtoBr,YtoBi,YtoCr,YtoCi,"
		"YSfromAr,YSfromAi,YSfromBr,YSfromBi,YSfromCr,YSfromCi,"
		"YStoAr,YStoAi,YStoBr,YStoBi,YStoCr,YStoCi,"
		"EOL\n");
	for ( unsigned int n = 0 ; n < branch_count ; n++ )
	{
		BRANCHDATA *b = branch+n;
		char phaseserr[32] = "ERROR", origphaseserr[32] = "ERROR", faultphaseserr[32] = "ERROR";
		const char *branchtypes[] = {"LINE","TRIPLEX","SWITCH","FUSE","TRANSFORMER","SECTIONALIZER","RECLOSER"};
		const char *branchtype = ( b->lnk_type >= 0 && b->lnk_type < sizeof(branchtypes)/sizeof(branchtypes[0]) ? branchtypes[b->lnk_type] : "ERROR" );
		if ( phases[b->phases] == NULL ) snprintf(phaseserr,sizeof(phaseserr)-1,"ERROR:%d",(int)b->phases);
		if ( phases[b->origphases] == NULL ) snprintf(origphaseserr,sizeof(origphaseserr)-1,"ERROR:%d",(int)b->origphases);
		if ( phases[b->faultphases] == NULL ) snprintf(faultphaseserr,sizeof(faultphaseserr)-1,"ERROR:%d",(int)b->faultphases);
		fprintf(fh,"%d,%s,%s,%s,%s,%s,%d,%d,%s,%g,%g,"
			"%g,%g,%g,%g,%g,%g,"
			"%g,%g,%g,%g,%g,%g,"
			"%g,%g,%g,%g,%g,%g,"
			"%g,%g,%g,%g,%g,%g,"
			"%s\n",
			n,b->name?b->name:"",branchtype,phases[b->phases]?phases[b->phases]:phaseserr,phases[b->origphases]?phases[b->origphases]:origphaseserr,phases[b->faultphases]?phases[b->faultphases]:faultphaseserr,b->from,b->to,b->fault_link_below?"FALSE":"TRUE",b->v_ratio,b->v_ratio,
			b->Yfrom[0].Re(),b->Yfrom[0].Im(),b->Yfrom[1].Re(),b->Yfrom[1].Im(),b->Yfrom[2].Re(),b->Yfrom[2].Im(),
			b->Yto[0].Re(),b->Yto[0].Im(),b->Yto[1].Re(),b->Yto[1].Im(),b->Yto[2].Re(),b->Yto[2].Im(),
			b->YSfrom[0].Re(),b->YSfrom[0].Im(),b->YSfrom[1].Re(),b->YSfrom[1].Im(),b->YSfrom[2].Re(),b->YSfrom[2].Im(),
			b->YSto[0].Re(),b->YSto[0].Im(),b->YSto[1].Re(),b->YSto[1].Im(),b->YSto[2].Re(),b->YSto[2].Im(),
			"EOL");
	}
	fclose(fh);
	if ( model_dump_handler )
	{
		if ( strncmp(model_dump_handler,"python:",7) == 0 )
		{
			if ( ! python_call(pModule,model_dump_handler+7,NULL) )
			{
				solver_model_log(1,"model_dump_handler failed, rc = FALSE");
			}
		}
		else
		{
			int rc = system(model_dump_handler);
			if ( rc != 0 )
			{
				solver_model_log(1,"model_dump_handler failed, rc = %d", rc);
			}
		}
	}
}

// solver_find_model(SOLVERMODEL): identifies a satisfactory model and return the distance metric
bool solver_model_find(SOLVERMODEL *&model,
						 unsigned int &bus_count,
						 BUSDATA *&bus,
						 unsigned int &branch_count,
						 BRANCHDATA *&branch)
{
	double dist = MAXDIST;
	SOLVERMODEL *m;
	if ( model_dump_handler )
	{
		solver_dump(bus_count,bus,branch_count,branch);
	}
	for ( m = solver_model_getfirst() ; m != NULL ; m = solver_model_getnext(m) )
	{
		double n = solver_get_metric(m,bus_count,bus,branch_count,branch);
		solver_model_log(1,"check model %x (%dx%d) old distance %g", m->id, bus_count, branch_count, dist);
		if ( n < dist )
		{
			model = m;
			dist = n;
		}
	}
	if ( model != NULL )
	{
		solver_model_log(1,"found model %x (%dx%d) new distance %g", model->id, bus_count, branch_count, dist);
		return dist < maximum_distance; 
	}
	else
	{
		return false;
	}
}

int64 solver_model_apply(SOLVERMODEL *model,
						NR_SOLVER_STRUCT *&powerflow_values,
						NRSOLVERMODE &powerflow_type,
						NR_MESHFAULT_IMPEDANCE *&mesh_imped_values,
						bool *&bad_computations,
						int64 iterations)
{
	powerflow_values = model->powerflow_values;
	powerflow_type = model->powerflow_type;
	mesh_imped_values = model->mesh_imped_values;
	bad_computations = model->bad_computations;
	solver_model_stop_timer(model,true);
	return model->iterations; 
}

#endif
