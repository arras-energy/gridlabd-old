// module/loads/building.cpp
// Copyright (C) 2022 Regents of the Leland Stanford Junior University

#include "powerflow.h"

EXPORT_CREATE(building);
EXPORT_INIT(building);
EXPORT_SYNC(building);
EXPORT_METHOD(building,airtemperature_history);
EXPORT_METHOD(building,masstemperature_history);
EXPORT_METHOD(building,building_response);
EXPORT_METHOD(building,input);
EXPORT_METHOD(building,temperature_input);
EXPORT_METHOD(building,temperature_response);
EXPORT_METHOD(building,occupancy_input);
EXPORT_METHOD(building,occupancy_response);
EXPORT_METHOD(building,solar_input);
EXPORT_METHOD(building,solar_response);
EXPORT_METHOD(building,loads_input);
EXPORT_METHOD(building,loads_response);
EXPORT_METHOD(building,ventilation_input);
EXPORT_METHOD(building,ventilation_response);
EXPORT_METHOD(building,systemmode_input);
EXPORT_METHOD(building,systemmode_response);

CLASS *building::oclass = NULL;
building *building::defaults = NULL;

building::building(MODULE *module)
: load(module)
{
	if (oclass==NULL)
	{
		// register to receive notice for first top down. bottom up, and second top down synchronizations
		oclass = gld_class::create(module,"building",sizeof(building),PC_PRETOPDOWN|PC_BOTTOMUP|PC_POSTTOPDOWN|PC_UNSAFE_OVERRIDE_OMIT|PC_AUTOLOCK);
		if (oclass==NULL)
			throw "unable to register class building";
		else
			oclass->trl = TRL_PROVEN;

		defaults = this;
		if (gl_publish_variable(oclass,

			PT_INHERIT, "load",

			PT_double, "loads_powerfactor", get_loads_powerfactor_offset(),
				PT_DEFAULT, "0.97",
				PT_DESCRIPTION, "power factor to use when modeling effect in heat gain from internal loads",

			PT_double, "system_powerfactor", get_system_powerfactor_offset(),
				PT_DEFAULT, "0.95",
				PT_DESCRIPTION, "power factor to use when modeling HVAC system power demand",

			PT_double, "timestep[s]", get_timestep_offset(),
				PT_DEFAULT, "60s",
				PT_DESCRIPTION, "timestep to use when modeling building response to inputs",

			PT_double, "output_timestep[s]", get_output_timestep_offset(),
				PT_DESCRIPTION, "timestep to use when outputting response to inputs",

			PT_method, "airtemperature_history", method_building_airtemperature_history,
				PT_DESCRIPTION, "air temperature history",
			
			PT_method, "masstemperature_history", method_building_masstemperature_history,
				PT_DESCRIPTION, "mass temperature history",
			
			PT_method, "building_response", method_building_building_response,
				PT_DESCRIPTION, "building response parameters (second-order transfer function denominator terms)",
			
			PT_method, "input", method_building_input,
				PT_DESCRIPTION, "transfer function input specification input (source and numerator coefficients)",

			PT_method, "temperature_input", method_building_temperature_input,
				PT_DESCRIPTION, "source object and property for temperature input",

			PT_method, "temperature_response", method_building_temperature_response,
				PT_DESCRIPTION, "temperature response parameters (second-order transfer function numerator terms)",
			
			PT_method, "occupancy_input", method_building_occupancy_input,
				PT_DESCRIPTION, "source object and property for occupancy input",

			PT_method, "occupancy_response", method_building_occupancy_response,
				PT_DESCRIPTION, "occupancy response parameters (second-order transfer function numerator terms)",
			
			PT_method, "solar_input", method_building_solar_input,
				PT_DESCRIPTION, "source object and property for solar input",

			PT_method, "solar_response", method_building_solar_response,
				PT_DESCRIPTION, "solar response parameters (second-order transfer function numerator terms)",
			
			PT_method, "loads_input", method_building_loads_input,
				PT_DESCRIPTION, "source object and property for loads input",

			PT_method, "loads_response", method_building_loads_response,
				PT_DESCRIPTION, "loads response parameters (second-order transfer function numerator terms)",
			
			PT_method, "ventilation_input", method_building_ventilation_input,
				PT_DESCRIPTION, "source object and property for ventilation input",

			PT_method, "ventilation_response", method_building_ventilation_response,
				PT_DESCRIPTION, "ventilation response parameters (second-order transfer function numerator terms)",
			
			PT_method, "systemmode_input", method_building_systemmode_input,
				PT_DESCRIPTION, "source object and property for systemmode input",

			PT_method, "systemmode_response", method_building_systemmode_response,
				PT_DESCRIPTION, "systemmode response parameters (second-order transfer function numerator terms)",

			PT_double, "air_temperature", PADDR(x0),
			PT_double, "mass_temperature", PADDR(x1),
			
			NULL)<1){
				char msg[256];
				snprintf(msg,sizeof(msg)-1, "unable to publish properties in %s",__FILE__);
				throw msg;
		}
	}
}

int building::create(void) 
{
	return load::create(); /* return 1 on success, 0 on failure */
}

void building::check_poles(double *a,const char *name)
{
	double zr = a[1]*a[1] - 4*a[0]*a[2];
	if ( zr < 0 ) // complex poles
	{
		double x = -0.5*a[1]/a[0];
		double y = 0.5*sqrt(-zr)/a[0];
		if ( sqrt(x*x + y*y) >= 1.0 )
		{
			warning("building %s response is not stable (poles are %f+/-%fj)",name,x,y);
		}
		p0 = complex(x,y);
		p1 = complex(x,-y);
	}
	else // real poles
	{
		double z0 = 0.5/a[0]*(-a0[1]-sqrt(zr));
		double z1 = 0.5/a[0]*(-a0[1]+sqrt(zr));
		if ( z0 >= 1.0 || z1 >= 1.0 )
		{
			warning("building %s response is not stable (poles are %f+,%f)",name,z0,z1);
		}
		p0 = complex(z0,0);
		p1 = complex(z1,0);
	}
}

int building::init(OBJECT *parent)
{
	if ( timestep <= 0 )
	{
		exception("timestep must be positive");
	}
	if ( output_timestep == 0.0 )
	{
		output_timestep = timestep;
	}
	if ( fabs(loads_powerfactor) > 1.0 )
	{
		exception("loads_powerfactor must be equal to or between -1 and +1");
	}
	if ( fabs(system_powerfactor) > 1.0 )
	{
		exception("system_powerfactor must be equal to or between -1 and +1");
	}
	if ( pT == NULL && pN == NULL && pS == NULL && pL == NULL && pV == NULL && pM == NULL )
	{
		warning("all inputs are null");
	}
	if ( pT != NULL && bT[0] == 0.0 && bT[1] == 0.0 && bT[2] == 0.0 )
	{
		warning("temperature input response is null");
	}
	if ( pN != NULL && bN[0] == 0.0 && bN[1] == 0.0 && bN[2] == 0.0 )
	{
		warning("occupancy input response is null");
	}
	if ( pS != NULL && bS[0] == 0.0 && bS[1] == 0.0 && bS[2] == 0.0 )
	{
		warning("solar input response is null");
	}
	if ( pL != NULL && bL[0] == 0.0 && bL[1] == 0.0 && bL[2] == 0.0 )
	{
		warning("loads input response is null");
	}
	if ( pV != NULL && bV[0] == 0.0 && bV[1] == 0.0 && bV[2] == 0.0 )
	{
		warning("ventilation input response is null");
	}
	if ( pM != NULL && bM[0] == 0.0 && bM[1] == 0.0 && bM[2] == 0.0 )
	{
		warning("system input response is null");
	}
	if ( a0[0] == 0.0 && a0[1] == 0.0 && a0[2] == 0.0 )
	{
		warning("building air response is null");
	}
	else
	{
		check_poles(a0,"air");
	}
	if ( a1[0] == 0.0 && a1[1] == 0.0 && a1[2] == 0.0 )
	{
		warning("building mass response is null");
	}
	else
	{
		check_poles(a1,"mass");
	}
	return load::init(parent);
}

TIMESTAMP building::precommit(TIMESTAMP t1, TIMESTAMP t2)
{
	if ( t2 % (int)timestep == 0 )
	{
		update_input();
		update_state((t2-t1)/timestep);
		update_output();
	}
	return TS_NEVER;
}

TIMESTAMP building::presync(TIMESTAMP t0)
{
	TIMESTAMP t2 = load::presync(t0);
	TIMESTAMP t1 = (TIMESTAMP)((TIMESTAMP)(t0/output_timestep)+1)*output_timestep;
	return t1 < t2 ? -t1 : t2;
}

TIMESTAMP building::sync(TIMESTAMP t0)
{
	return load::sync(t0);
}

int building::airtemperature_history(char *buffer, size_t len)
{
	if ( buffer != NULL && len == 0 ) // read values in buffer
	{
		return sscanf(buffer,"%lf,%lf,%lf",&x0[0],&x0[1],&x0[2]);
	}
	else // save values to buffer (or return buffer size if buffer is null)
	{
		return snprintf(buffer,len,"%lf,%lf,%lf",x0[0],x0[1],x0[2]);
	}
}

int building::masstemperature_history(char *buffer, size_t len)
{
	if ( buffer != NULL && len == 0 ) // read values in buffer
	{
		return sscanf(buffer,"%lf,%lf,%lf",&x1[0],&x1[1],&x1[2]);
	}
	else // save values to buffer (or return buffer size if buffer is null)
	{
		return snprintf(buffer,len,"%lf,%lf,%lf",x1[0],x1[1],x1[2]);
	}
}

int building::building_response(char *buffer, size_t len)
{
	if ( buffer != NULL && len == 0 ) // read values in buffer
	{
		return sscanf(buffer,"%lf,%lf,%lf;%lf,%lf,%lf",&a0[0],&a0[1],&a0[2],&a1[0],&a1[1],&a1[2]);
	}
	else // save values to buffer (or return buffer size if buffer is null)
	{
		return snprintf(buffer,len,"%lf,%lf,%lf;%lf,%lf,%lf",a0[0],a0[1],a0[2],a1[0],a1[1],a1[2]);
	}
}

static int property_method(gld_property **prop, char *buffer, size_t len)
{
	char objectname[64];
	char propertyname[64];
	if ( buffer != NULL && len == 0 )
	{
		if ( sscanf(buffer,"%[^.].%s",objectname,propertyname) == 2 )
		{
			*prop = new gld_property(objectname,propertyname);
			if ( ! (*prop)->is_valid() )
			{
				return -1;
			}
			return 1;
		}
		else
		{
			return 0; // invalid input syntax
		}
	}
	else if ( (*prop)->is_valid() )
	{
		return (*prop)->to_string(buffer,len);
	}
	else
	{
		if ( buffer )
		{
			strcpy(buffer,"");
		}
		return 0;
	}	
}

int building::input(char *buffer, size_t len)
{
	char objectname[64];
	char propertyname[64];
	double b0,b1,b2;
	if ( buffer != NULL && len == 0 )
	{
		if ( sscanf(buffer,"%63[^.].%63[^.],%lf,%lf,%lf",objectname,propertyname,&b0,&b1,&b2) == 4)
		{
			INPUT *last = input_list;
			input_list = new INPUT;
			asprintf(&(input_list->source),"%s.%s",objectname,propertyname);
			input_list->b[0] = b0;
			input_list->b[1] = b1;
			input_list->b[2] = b2;
			input_list->next = last;
			input_list->prop = new gld_property(objectname,propertyname);
			if ( ! input_list->prop->is_valid() )
			{
				exception("input '%s' not found",input_list->source);
			}
		}
		return 4;
	}
	else if ( buffer == NULL )
	{
		int result = 0;
		for ( INPUT *item = input_list ; item != NULL ; item = item->next )
		{
			result += snprintf(NULL,0,"%s,%lf,%lf,%lf\n",item->source,item->b[0],item->b[1],item->b[2]);
		}
		return result;
	}
	else
	{
		int result = 0;
		for ( INPUT *item = input_list ; item != NULL ; item = item->next )
		{
			result += snprintf(buffer+result,len-result,"%s,%lf,%lf,%lf\n",item->source,item->b[0],item->b[1],item->b[2]);
		}
		return result;
	}
}

int building::temperature_input(char *buffer, size_t len)
{
	int result = property_method(&Tin,buffer,len);
	if ( result < 0 )
	{
		exception("temperature input '%s' not found",buffer);
		return 0;
	}
	else
	{
		pT = (double*)Tin->get_addr();
		return result;
	}
}

int building::temperature_response(char *buffer, size_t len)
{
	if ( buffer != NULL && len == 0 ) // read values in buffer
	{
		return sscanf(buffer,"%lf,%lf,%lf",&bT[0],&bT[1],&bT[2]);
	}
	else // save values to buffer (or return buffer size if buffer is null)
	{
		return snprintf(buffer,len,"%lf,%lf,%lf",bT[0],bT[1],bT[2]);
	}
}

int building::occupancy_input(char *buffer, size_t len)
{
	int result = property_method(&Nin,buffer,len);
	if ( result < 0 )
	{
		exception("occupancy input '%s' not found",buffer);
		return 0;
	}
	else
	{
		pN = (double*)Nin->get_addr();
		return result;
	}
}

int building::occupancy_response(char *buffer, size_t len)
{
	if ( buffer != NULL && len == 0 ) // read values in buffer
	{
		return sscanf(buffer,"%lf,%lf,%lf",&bN[0],&bN[1],&bN[2]);
	}
	else // save values to buffer (or return buffer size if buffer is null)
	{
		return snprintf(buffer,len,"%lf,%lf,%lf",bN[0],bN[1],bN[2]);
	}
}

int building::solar_input(char *buffer, size_t len)
{
	int result = property_method(&Sin,buffer,len);
	if ( result < 0 )
	{
		exception("solar input '%s' not found",buffer);
		return 0;
	}
	else
	{
		pS = (double*)Sin->get_addr();
		return result;
	}
}

int building::solar_response(char *buffer, size_t len)
{
	if ( buffer != NULL && len == 0 ) // read values in buffer
	{
		return sscanf(buffer,"%lf,%lf,%lf",&bS[0],&bS[1],&bS[2]);
	}
	else // save values to buffer (or return buffer size if buffer is null)
	{
		return snprintf(buffer,len,"%lf,%lf,%lf",bS[0],bS[1],bS[2]);
	}
}

int building::loads_input(char *buffer, size_t len)
{
	int result = property_method(&Lin,buffer,len);
	if ( result < 0 )
	{
		exception("loads input '%s' not found",buffer);
		return 0;
	}
	else
	{
		pL = (double*)Lin->get_addr();
		return result;
	}
}

int building::loads_response(char *buffer, size_t len)
{
	if ( buffer != NULL && len == 0 ) // read values in buffer
	{
		return sscanf(buffer,"%lf,%lf,%lf",&bL[0],&bL[1],&bL[2]);
	}
	else // save values to buffer (or return buffer size if buffer is null)
	{
		return snprintf(buffer,len,"%lf,%lf,%lf",bL[0],bL[1],bL[2]);
	}
}

int building::ventilation_input(char *buffer, size_t len)
{
	int result = property_method(&Vin,buffer,len);
	if ( result < 0 )
	{
		exception("ventilation input '%s' not found",buffer);
		return 0;
	}
	else
	{
		pV = (double*)Vin->get_addr();
		return result;
	}
}

int building::ventilation_response(char *buffer, size_t len)
{
	if ( buffer != NULL && len == 0 ) // read values in buffer
	{
		return sscanf(buffer,"%lf,%lf,%lf",&bV[0],&bV[1],&bV[2]);
	}
	else // save values to buffer (or return buffer size if buffer is null)
	{
		return snprintf(buffer,len,"%lf,%lf,%lf",bV[0],bV[1],bV[2]);
	}
}

int building::systemmode_input(char *buffer, size_t len)
{
	int result = property_method(&Min,buffer,len);
	if ( result < 0 )
	{
		exception("system control input '%s' not found",buffer);
		return 0;
	}
	else
	{
		pM = (double*)Min->get_addr();
		return result;
	}
}

int building::systemmode_response(char *buffer, size_t len)
{
	if ( buffer != NULL && len == 0 ) // read values in buffer
	{
		return sscanf(buffer,"%lf,%lf,%lf",&bM[0],&bM[1],&bM[2]);
	}
	else // save values to buffer (or return buffer size if buffer is null)
	{
		return snprintf(buffer,len,"%lf,%lf,%lf",bM[0],bM[1],bM[2]);
	}
}

void building::update_state(unsigned int n)
{
	// update timesteps
	// TODO: use poles to do this faster when doing more than 3 timesteps
	while ( n-- > 0 )
	{
		// initialize update
		x0[2] = x0[1]; 
		x0[1] = x0[0]; 
		x0[0] = -a0[1]*x0[1] - a0[2]*x0[2];
		x1[2] = x1[1]; 
		x1[1] = x1[0]; 
		x1[0] = -a1[1]*x1[1] - a1[2]*x1[2];

		// air temperature update
		if ( pT != NULL )
		{
			x0[0] += bT[0]*T[0] + bT[1]*T[1] + bT[2]*T[2];
		}
		if ( pN != NULL )
		{
			x0[0] += bN[0]*N[0] + bN[1]*N[1] + bN[2]*N[2];
		}
		if ( pV != NULL )
		{
			x0[0] += bV[0]*V[0] + bV[1]*V[1] + bV[2]*V[2];
		}
		if ( pM != NULL )
		{
			x0[0] += bM[0]*M[0] + bM[1]*M[1] + bM[2]*M[2];
		}

		// mass temperature update
		if ( pS != NULL )
		{
			x1[0] += bS[0]*S[0] + bS[1]*S[1] + bS[2]*S[2];
		}

		// finalize update
		x0[0] /= a0[0];
		x1[0] /= a1[0];
	}
}

void building::update_input(void)
{
	// update history
	if ( pT != NULL )
	{
		T[2] = T[1]; T[1] = T[0]; T[0] = pT[0];
	}
	if ( pN != NULL )
	{
		N[2] = N[1]; N[1] = N[0]; N[0] = pN[0];
	}
	if ( pV != NULL )
	{
		V[2] = V[1]; V[1] = V[0]; V[0] = pV[0];
	}
	if ( pM != NULL )
	{
		M[2] = M[1]; M[1] = M[0]; M[0] = pM[0];
	}
	if ( pS != NULL )
	{
		S[2] = S[1]; S[1] = S[0]; S[0] = pS[0];
	}
}

void building::update_output(void)
{
	return;
}