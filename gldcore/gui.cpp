/*	gui.cpp
 *  Created by David P. Chassin on 10/26/10.
 *  Copyright 2010 Battelle Memorial Institute, All rights reserved.
 */

#include "gldcore.h"

SET_MYCONTEXT(DMC_GUI)

// TODO: delete this after reentrancy is completed
extern GldMain *my_instance;
DEPRECATED GUIENTITY *gui_create_entity()
{
	return my_instance->get_gui()->create_entity();
}
DEPRECATED void gui_set_html_stream(void *ref,GUISTREAMFN stream)
{
	return my_instance->get_gui()->set_html_stream(ref,stream);
}
DEPRECATED void gui_set_srcref(GUIENTITY *entity, char *filename, int linenum)
{
	return my_instance->get_gui()->set_srcref(entity,filename,linenum);
}
DEPRECATED void gui_set_type(GUIENTITY *entity, GUIENTITYTYPE type)
{
	return my_instance->get_gui()->set_type(entity,type);
}
DEPRECATED void gui_set_value(GUIENTITY *entity, char *value)
{
	return my_instance->get_gui()->set_value(entity,value);
}
DEPRECATED void gui_set_variablename(GUIENTITY *entity, char *globalname)
{
	return my_instance->get_gui()->set_variablename(entity,globalname);
}
DEPRECATED void gui_set_objectname(GUIENTITY *entity, char *objectname)
{
	return my_instance->get_gui()->set_objectname(entity,objectname);
}
DEPRECATED void gui_set_propertyname(GUIENTITY *entity, char *propertyname)
{
	return my_instance->get_gui()->set_propertyname(entity,propertyname);
}
DEPRECATED void gui_set_span(GUIENTITY *entity, int span)
{
	return my_instance->get_gui()->set_span(entity,span);
}
DEPRECATED void gui_set_unit(GUIENTITY *entity, char *unit)
{
	return my_instance->get_gui()->set_unit(entity,unit);
}
DEPRECATED void gui_set_next(GUIENTITY *entity, GUIENTITY *next)
{
	return my_instance->get_gui()->set_next(entity,next);
}
DEPRECATED void gui_set_parent(GUIENTITY *entity, GUIENTITY *parent)
{
	return my_instance->get_gui()->set_parent(entity,parent);
}
DEPRECATED void gui_set_source(GUIENTITY *entity, char *source)
{
	return my_instance->get_gui()->set_source(entity,source);
}
DEPRECATED void gui_set_options(GUIENTITY *entity, char *source)
{
	return my_instance->get_gui()->set_options(entity,source);
}
DEPRECATED void gui_set_wait(GUIENTITY *entity, char *wait)
{
	return my_instance->get_gui()->set_wait(entity,wait);
}
DEPRECATED STATUS gui_startup(int argc, const char *argv[])
{
	return my_instance->get_gui()->startup(argc,argv);
}
DEPRECATED int gui_post_action(char *action)
{
	return my_instance->get_gui()->post_action(action);
}
DEPRECATED GUIENTITY *gui_get_root(void)
{
	return my_instance->get_gui()->get_root();
}
DEPRECATED GUIENTITY *gui_get_last(void)
{
	return my_instance->get_gui()->get_last();
}
DEPRECATED GUIENTITYTYPE gui_get_type(GUIENTITY *entity)
{
	return my_instance->get_gui()->get_type(entity);
}
DEPRECATED GUIENTITY *gui_get_parent(GUIENTITY *entity)
{
	return my_instance->get_gui()->get_parent(entity);
}
DEPRECATED GUIENTITY *gui_get_next(GUIENTITY *entity)
{
	return my_instance->get_gui()->get_next(entity);
}
DEPRECATED const char *gui_get_name(GUIENTITY *entity)
{
	return my_instance->get_gui()->get_name(entity);
}
DEPRECATED OBJECT *gui_get_object(GUIENTITY *entity)
{
	return my_instance->get_gui()->get_object(entity);
}
DEPRECATED PROPERTY *gui_get_property(GUIENTITY *entity)
{
	return my_instance->get_gui()->get_property(entity);
}
DEPRECATED char *gui_get_value(GUIENTITY *entity)
{
	return my_instance->get_gui()->get_value(entity);
}
DEPRECATED void *gui_get_data(GUIENTITY *entity)
{
	return my_instance->get_gui()->get_data(entity);
}
DEPRECATED GLOBALVAR *gui_get_variable(GUIENTITY *entity)
{
	return my_instance->get_gui()->get_variable(entity);
}
DEPRECATED char *gui_get_environment(GUIENTITY *entity)
{
	return my_instance->get_gui()->get_environment(entity);
}
DEPRECATED int gui_get_span(GUIENTITY *entity)
{
	return my_instance->get_gui()->get_span(entity);
}
DEPRECATED UNIT *gui_get_unit(GUIENTITY *entity)
{
	return my_instance->get_gui()->get_unit(entity);
}
DEPRECATED int gui_is_grouping(GUIENTITY *entity)
{
	return my_instance->get_gui()->is_grouping(entity);
}
DEPRECATED int gui_is_labeling(GUIENTITY *entity)
{
	return my_instance->get_gui()->is_labeling(entity);
}
DEPRECATED int gui_is_input(GUIENTITY *entity)
{
	return my_instance->get_gui()->is_input(entity);
}
DEPRECATED int gui_is_output(GUIENTITY *entity)
{
	return my_instance->get_gui()->is_output(entity);
}
DEPRECATED int gui_is_action(GUIENTITY *entity)
{
	return my_instance->get_gui()->is_action(entity);
}
DEPRECATED int gui_is_header(GUIENTITY *entity)
{
	return my_instance->get_gui()->is_header(entity);
}
DEPRECATED void gui_html_start(void)
{
	return my_instance->get_gui()->html_start();
}
#ifdef X11
DEPRECATED void gui_X11_start(void)
{
	return my_instance->get_gui()->X11_start();
}
#endif
DEPRECATED void gui_wait_status(GUIACTIONSTATUS status)
{
	return my_instance->get_gui()->wait_status(status);
}
DEPRECATED int gui_html_output_page(const char *page)
{
	return my_instance->get_gui()->html_output_page(page);
}
DEPRECATED STATUS gui_html_output_all(void)
{
	return my_instance->get_gui()->html_output_all();
}
DEPRECATED size_t gui_glm_write_all(FILE *fp)
{
	return my_instance->get_gui()->glm_write_all(fp);
}
DEPRECATED int gui_wait(void)
{
	return my_instance->get_gui()->wait();
}

#ifdef _DEBUG
#define TABLEOPTIONS " border=0 CELLPADDING=0 CELLSPACING=0"
#else
#define TABLEOPTIONS 
#endif

int default_stream(void *ref,const char *format,...)
{
	int len;
	va_list ptr;

	FILE *fp = (FILE*)ref;
	if (fp==NULL) fp=stdout;
	va_start(ptr,format);
	len = vfprintf(fp,format,ptr);
	va_end(ptr);

	return len;
}

GldGui::GldGui(GldMain *ptr) 
: 	main(ptr) 
{
	root = NULL;
	last = NULL;
	status = GUIACT_NONE;
	fp = NULL;
	html_output = default_stream;
}

GldGui::~GldGui(void)
{
	// TODO: delete gui entities
	// TODO: close the output stream
}

void GldGui::set_html_stream(void *ref, GUISTREAMFN stream)
{
	html_output = stream;
	fp = ref;
}

GUIENTITY *GldGui::get_root(void)
{
	return root;
}

GUIENTITY *GldGui::get_last(void)
{
	return last;
}

GUIENTITY *GldGui::create_entity()
{
	/* resuse last entity if it remains unused */
	GUIENTITY *entity = last;
	if (entity==NULL || entity->type!=GUI_UNKNOWN)
		entity = (GUIENTITY*)malloc(sizeof(GUIENTITY));
	if (entity==NULL)
		return NULL;
	memset(entity,0,sizeof(GUIENTITY));
	if (root==NULL) root = entity;
	if (last!=NULL) last->next = entity;
	last = entity;
	return entity;
}

GUIENTITYTYPE GldGui::get_type(GUIENTITY *entity)
{
	return entity->type;
}

static const char *get_typename(GUIENTITY *entity)
{
	switch (entity->type) {
#define CASE(X) case X: return #X;
	CASE(GUI_UNKNOWN)
	CASE(GUI_ROW)
	CASE(GUI_TAB)
	CASE(GUI_PAGE)
	CASE(GUI_GROUP)
	CASE(GUI_SPAN)
	CASE(_GUI_GROUPING_END)
	CASE(GUI_TITLE)
	CASE(GUI_STATUS)
	CASE(GUI_TEXT)
	CASE(GUI_INPUT)
	CASE(GUI_CHECK)
	CASE(GUI_RADIO)
	CASE(GUI_SELECT)
	CASE(GUI_ACTION)
	default: return "<INVALID>";
	}
}

int GldGui::is_header(GUIENTITY *entity)
{
	if (entity->type==GUI_TITLE && !(entity->parent!=NULL && entity->parent->type==GUI_GROUP)) return 1;
	return 0;
}

int GldGui::is_grouping(GUIENTITY *entity)
{
	return (entity->type<_GUI_GROUPING_END);
}

int GldGui::is_labeling(GUIENTITY *entity)
{
	return (entity->type<_GUI_LABELING_END && entity->type>_GUI_GROUPING_END);
}
int GldGui::is_input(GUIENTITY *entity)
{
	return (entity->type<_GUI_INPUT_END && entity->type>_GUI_LABELING_END);
}
int GldGui::is_output(GUIENTITY *entity)
{
	return (entity->type<_GUI_OUTPUT_END && entity->type>_GUI_INPUT_END);
}
int GldGui::is_action(GUIENTITY *entity)
{
	return (entity->type<_GUI_ACTION_END && entity->type>_GUI_OUTPUT_END);
}

/* SET OPERATIONS */
void GldGui::set_srcref(GUIENTITY *entity, char *filename, int linenum)
{
	sprintf(entity->srcref,"%s(%d)", filename, linenum);
}
void GldGui::set_type(GUIENTITY *entity, GUIENTITYTYPE type)
{
	entity->type = type;
}
void GldGui::set_value(GUIENTITY *entity, char *value)
{
	strncpy(entity->value,value,sizeof(entity->value));
}
void GldGui::set_variablename(GUIENTITY *entity, char *globalname)
{
	strncpy(entity->globalname,globalname,sizeof(entity->globalname));
}
void GldGui::set_objectname(GUIENTITY *entity, char *objectname)
{
	entity->data = NULL;
	strncpy(entity->objectname,objectname,sizeof(entity->objectname));
}
void GldGui::set_propertyname(GUIENTITY *entity, char *propertyname)
{
	entity->data = NULL;
	strncpy(entity->propertyname,propertyname,sizeof(entity->propertyname));
}
void GldGui::set_span(GUIENTITY *entity, int span)
{
	entity->span = span;
}
void GldGui::set_unit(GUIENTITY *entity, char *unit)
{
	entity->unit = unit_find(unit);
}
void GldGui::set_next(GUIENTITY *entity, GUIENTITY *next)
{
	entity->next = next;
}
void GldGui::set_parent(GUIENTITY *entity, GUIENTITY *parent)
{
	entity->parent = parent;
}
void GldGui::set_source(GUIENTITY *entity, char *source)
{
	strncpy(entity->source,source,sizeof(entity->source));
}
void GldGui::set_options(GUIENTITY *entity, char *options)
{
	strncpy(entity->options,options,sizeof(entity->options));
}
void GldGui::set_wait(GUIENTITY *entity, char *wait)
{
	strncpy(entity->wait_for,wait,sizeof(entity->wait_for));
}

/* GET OPERATIONS */
char *GldGui::get_dump(GUIENTITY *entity)
{
	static char buffer[1024];
	sprintf(buffer,"{type=%s,srcref='%s',value='%s',globalname='%s',object='%s',property='%s',action='%s',span=%d}",
		get_typename(entity), entity->srcref, entity->value, entity->globalname, entity->objectname, entity->propertyname, entity->action, entity->span);
	return buffer;
}
GUIENTITY *GldGui::get_parent(GUIENTITY *entity)
{
	return entity->parent;
}
GUIENTITY *GldGui::get_next(GUIENTITY *entity)
{
	return entity->next;
}
char *GldGui::get_value(GUIENTITY *entity)
{
	OBJECT *obj = get_object(entity);
	char buffer[64];
	if (obj)
	{
		if (strcmp(entity->propertyname,"name")==0)
			strcpy(entity->value,object_name(obj, buffer, 63));
		else if (strcmp(entity->propertyname,"class")==0)
			strcpy(entity->value,obj->oclass->name);
		else if (strcmp(entity->propertyname,"parent")==0)
			strcpy(entity->value,obj->parent?object_name(obj->parent, buffer, 63):"");
		else if (strcmp(entity->propertyname,"rank")==0)
			sprintf(entity->value,"%d",obj->rank);
		else if (strcmp(entity->propertyname,"clock")==0)
			convert_from_timestamp(obj->clock,entity->value,sizeof(entity->value));
		else if (strcmp(entity->propertyname,"valid_to")==0)
			convert_from_timestamp(obj->valid_to,entity->value,sizeof(entity->value));
		else if (strcmp(entity->propertyname,"in_svc")==0)
			convert_from_timestamp(obj->in_svc,entity->value,sizeof(entity->value));
		else if (strcmp(entity->propertyname,"out_svc")==0)
			convert_from_timestamp(obj->out_svc,entity->value,sizeof(entity->value));
		else if (strcmp(entity->propertyname,"latitude")==0)
			convert_from_latitude(obj->latitude,entity->value,sizeof(entity->value));
		else if (strcmp(entity->propertyname,"longitude")==0)
			convert_from_longitude(obj->longitude,entity->value,sizeof(entity->value));
		else if (!object_get_value_by_name(obj,entity->propertyname,entity->value,sizeof(entity->value)))
			output_error_raw("%s: ERROR: %s refers to a non-existent property '%s'", entity->srcref, get_typename(entity), entity->propertyname);
	}
	else if (get_variable(entity))
	{
		entity->var = global_find(entity->globalname);
		global_getvar(entity->globalname,entity->value,sizeof(entity->value));
	}
	else if (get_environment(entity))
		strcpy(entity->value,entity->env);
	return entity->value;
}
OBJECT *GldGui::get_object(GUIENTITY *entity)
{
	if (!entity->obj) entity->obj = object_find_name(entity->objectname);
	return entity->obj;
}
PROPERTY *GldGui::get_property(GUIENTITY *entity)
{
	if (entity->prop==NULL)
	{
		if (get_object(entity))
			entity->prop = class_find_property(entity->obj->oclass,entity->propertyname);
		else if (get_variable(entity))
			entity->prop = entity->var->prop;
	}
	return entity->prop;
}
const char *GldGui::get_name(GUIENTITY *entity)
{
	if (get_object(entity))
	{
		static char buffer[1024];
		sprintf(buffer,"%s.%s", entity->objectname, entity->propertyname); 
		return buffer;
	}
	else if (get_variable(entity))
		return entity->var->prop->name;
	else
		return "";
}
void *GldGui::get_data(GUIENTITY *entity)
{
	if (entity->data==NULL)
	{
		if (get_object(entity)) 
			entity->data = object_get_addr(entity->obj,entity->propertyname);
		else if (get_variable(entity))
			entity->data = entity->var->prop->addr; 
		else
			entity->data=NULL;
	}
	return entity->data;
}
GLOBALVAR *GldGui::get_variable(GUIENTITY *entity)
{
	if (entity->var==NULL) entity->var = global_find(entity->globalname);
	return entity->var;
}
char *GldGui::get_environment(GUIENTITY *entity)
{
	if (entity->env==NULL) entity->env = getenv(entity->globalname);
	return entity->env;
}
int GldGui::get_span(GUIENTITY *entity)
{
	return entity->span;
}
UNIT *GldGui::get_unit(GUIENTITY *entity)
{
	return entity->unit;
}

/** Post GUI action 
    @returns 0 if not accepted, 1 if accepted, -1 for server shutdown
 **/
int GldGui::post_action(char *action)
{
	GUIENTITY *entity;

	/* built in actions */
	if (strcmp(action,"quit")==0) return -1;
	if (strcmp(action,"continue")==0)
	{
		status = GUIACT_PENDING;
		return 1;
	}

	/* gui-defined actions */
	for (entity=root; entity!=NULL; entity=entity->next)
	{
		if (entity->type==GUI_ACTION && strcmp(entity->action,action)==0)
		{
			entity->action_status = GUIACT_PENDING;
			return 1;
		}
	}
	return 0;
}

/**************************************************************************/
/* COMMAND LINE OPERATIONS */
/**************************************************************************/
int GldGui::cmd_entity(int item, GUIENTITY *entity)
{
	switch (entity->type) {
	case GUI_TITLE: 
		fprintf(stdout, "%s ", get_value(entity));
		break;
	case GUI_STATUS: 
		break;
	case GUI_ROW: 
		if (item>0) fprintf(stdout,"\n%2d. ",item);
		return 1;
		break;
	case GUI_TAB:
		break;
	case GUI_PAGE: 
		break;
	case GUI_GROUP:
		break;
	case GUI_SPAN: 
		break;
	case GUI_TEXT: 
		fprintf(stdout, "%s ", get_value(entity));
		break;
	case GUI_INPUT:
		fprintf(stdout,"[%s] ",get_value(entity));
		break;
	case GUI_CHECK: 
		fprintf(stdout,"[%s] ",get_value(entity));
		break;
	case GUI_RADIO: 
		fprintf(stdout,"[%s] ",get_value(entity));
		break;
	case GUI_SELECT: 
		fprintf(stdout,"[%s] ",get_value(entity));
		break;
	case GUI_ACTION: 
		break;	
	default:
		break;
	}
	return 0;
}

void GldGui::cmd_prompt(GUIENTITY *parent)
{
	char buffer[1024];
	char *label = NULL;
	GUIENTITY *entity;
	for ( entity=root ; entity!=NULL ; entity=entity->next )
	{
		if ( entity->parent==parent )
		{
			if ( is_labeling(entity) )
				label = get_value(entity);
			else
				break;
		}
	}
Retry:
	fprintf(stdout,"\n%s> [%s] ",label, get_value(entity));
	fflush(stdout);
	if ( fgets(buffer,sizeof(buffer),stdin) == NULL )
		output_error("GldGui::cmd_prompt read failed");
	buffer[strlen(buffer)-1]='\0';
	if (strcmp(buffer,"")==0)
		return;
	if (entity->obj && !object_set_value_by_name(entity->obj,entity->propertyname,buffer))
	{
		fprintf(stdout,"Invalid input, try again.\n");
		goto Retry;
	}
	else if (entity->var && global_setvar(entity->globalname,buffer)==FAILED)
	{
		fprintf(stdout,"Invalid input, try again.\n");
		goto Retry;
	}
	else if (entity->env)
	{
#ifdef WIN32
		char env[1024];
		sprintf(env,"%s=%s",entity->env,buffer);
		putenv(env);
#else
		setenv(entity->env,buffer,1);
#endif
	}
}

/* returns count of input items below it */
int GldGui::cmd_input_count(GUIENTITY *entity)
{
	GUIENTITY *item;
	int count = 0;
	for ( item=root; item!=NULL ; item=item->next )
	{
		if (item->parent!=entity)
			continue;
		if (is_input(item))
			count++;
		else
			count += cmd_input_count(item);
	}
	return count;
}

void GldGui::cmd_menu(GUIENTITY *parent)
{
	char buffer[1024];
	GUIENTITY *entity;
	GUIENTITY *list[100];
	int count;

	while (1) {
		int item=0, ans=-1;

		for ( entity=root ; entity!=NULL ; entity=entity->next )
		{
			if ( entity->parent==parent && cmd_entity(item+1,entity))
				list[++item]=entity; 
			else if (item>0 && list[item]==entity->parent)
				cmd_entity(item+1,entity);
		}
		if (item==0)
			break;
		fprintf(stdout,"\n\n 0. %s\n",parent?"Return":"Done");
Retry:
		fprintf(stdout,"\nGLM> [%d] ",ans<item?ans+1:0);
		fflush(stdout);
		if ( fgets(buffer,sizeof(buffer),stdin) == NULL )
			output_error("GldGui::cmd_menu read failed");
		buffer[strlen(buffer)-1]='\0';
		ans = atoi(buffer);
		if (ans<0 || ans>item)
		{
			fprintf(stdout,"Input must be between 0 and %d\n", item);
			goto Retry;
		}
		else if ( ans==0 )
			break;
		count = cmd_input_count(list[ans]);
		if ( count==1 )
			cmd_prompt(list[ans]);
		else if ( count>0 )
			cmd_menu(list[ans]);
	}
}

void GldGui::cmd_start(void)
{
	cmd_menu(NULL);
} 

#ifdef X11
/**************************************************************************/
/* X11 OPERATIONS */
/**************************************************************************/
void GUI_X11_start(void)
{
}
#endif

/**************************************************************************/
/* HTML OPERATIONS */
/**************************************************************************/
void GldGui::html_start(void)
{
	// TODO start server
}

void GldGui::startspan()
{
	if (table>=0) span[table]++;
}
void GldGui::endspan()
{
	if (table>=0) span[table]--;
}
void GldGui::newtable(GUIENTITY *entity)
{
	if (table<MAX_TABLES)
	{
		table++;
		row[table]=0;
		col[table]=0;
	}
	else
		output_error_raw("%s: ERROR: table nesting exceeded limit of %d", entity->srcref, MAX_TABLES);
}
void GldGui::newrow(GUIENTITY *entity)
{
	if (table<0) newtable(entity);
	if (row[table]==0) html_output(fp,"\t<table" TABLEOPTIONS "> <!-- table %d %s -->\n",table,get_typename(entity));
	if (col[table]>0) html_output(fp,"\t</td> <!-- table %d col %d -->\n", table, col[table]);
	col[table]=0;
	if (row[table]>0) html_output(fp,"\t</tr> <!--  table %d row %d -->\n", table, row[table]);
	row[table]++;
	html_output(fp,"\t<tr> <!-- row %d -->\n", row[table]);
}
void GldGui::newcol(GUIENTITY *entity)
{
	if (span[table]>0) return;
	if (table<0 || row[table]==0) newrow(entity);
	if (col[table]>0) html_output(fp,"\t</td> <!-- table %d col %d -->\n",  table, col[table]);
	col[table]++;
	if (entity->type==GUI_SPAN)
	{
		html_output(fp,"\t<td colspan=\"%d\"> <!-- table %d col %d -->\n", entity->size, table, col[table]);
		if (entity->size==0) output_warning("%s: not all browsers accept span size 0 (meaning to end), span size may not work as expected", entity->srcref);
	}
	else
		html_output(fp,"\t<td> <!-- table %d col %d -->\n", table, col[table]);
}
void GldGui::endtable()
{
	if (table<0) return;
	if (col[table]>0) html_output(fp,"\t</td> <!-- table %d col %d -->\n", table, col[table]);
	if (row[table]>0) html_output(fp,"\t</tr> <!-- table %d row %d -->\n", table, row[table]);
	html_output(fp,"\t</table> <!-- table %d -->\n", table--);
}

void GldGui::output_html_textarea(GUIENTITY *entity)
{
	FILE *src = fopen(entity->source,"r");
	char buffer[65536];
	size_t len;
	char rows[32]="";
	char cols[32]="";
	if (entity->height>0) sprintf(rows," rows=\"%d\"",entity->height);
	if (entity->width>0) sprintf(cols," cols=\"%d\"",entity->width);
	html_output(fp,"<textarea class=\"browse\"%s%s >\n",rows,cols);
	if (src==NULL)
	{
		html_output(fp,"***'%s' is not found: %s***",entity->source,strerror(errno));
		goto Done;
	}
	len = fread(buffer,1,sizeof(buffer),src);
	if ( len<0 )
		html_output(fp,"***'%s' read failed: %s***",entity->source,strerror(errno));
	else if (len<sizeof(buffer))
	{
		buffer[len]='\0';
		html_output(fp,"%s",buffer);
	}
	if ( len>=sizeof(buffer) )
		html_output(fp,"\n***file truncated***");
Done:
	html_output(fp,"</textarea>\n");
}

void GldGui::output_html_table(GUIENTITY *entity)
{
	FILE *src = fopen(entity->source,"r");
	char line[65536];
	int row=0;
	char header[1024];
	html_output(fp,"<table class=\"data\" %s>\n",entity->options);
	if (src==NULL)
	{
		html_output(fp,"***'%s' is not found: %s***",entity->source,strerror(errno));
		goto Done;
	}
	while ( fgets(line,sizeof(line),src)!=NULL )
	{
		char *eol = strchr(line,'\n');
		if (eol) *eol='\0';
		if ( line[0]=='#' )
		{
			if ( row==0 )
				strncpy(header,line+1,sizeof(header));
		}
		else 
		{
			char *p = NULL;
			if ( row++==0 )
			{
				html_output(fp,"<tr>");
				while ( (p=strtok(p?NULL:header,","))!=NULL )
					html_output(fp,"<th>%s</th>", p);
				html_output(fp,"</tr>\n");
			}
			if ( entity->height==0 || row<=entity->height )
			{
				html_output(fp,"<tr>");
				while ( (p=strtok(p?NULL:line,","))!=NULL )
					html_output(fp,"<td>%s</td>", p);
				html_output(fp,"</tr>\n");
			}
		}
	}
	if (ferror(src))
		html_output(fp,"<tr><td>ERROR: %s</td></tr>\n",strerror(errno));
Done:
	html_output(fp,"</table>\n");
}

void GldGui::output_html_graph(GUIENTITY *entity)
{
	char script[1024];
	char command[1024];
	char image[1024];
	char height[32]="";
	char width[32]="";
	FILE *plot=NULL;

	/* setup gnuplot command */
	sprintf(script,"%s.plt",entity->source);
#ifdef WIN32
	sprintf(command,"start wgnuplot %s",script);
#else
	sprintf(command,"gnuplot %s",script);
#endif
	sprintf(image,"%s.png",entity->source);
	if (entity->width>0) sprintf(width," width=\"%d\"", entity->width);
	if (entity->height>0) sprintf(height, " height=\"%d\"", entity->height);

	/* generate script */
	plot = fopen(script,"w");
	if ( plot==NULL )
	{
		html_output(fp,"<span class=\"error\">Unable to run gnuplot</span>\n");
		goto Done;
	}
	if (entity->gnuplot[0]=='\0')
	{
		if ( entity->width>0 && entity->height>0 )
			fprintf(plot,"set terminal png size %d,%d %s\n", entity->width, entity->height, entity->options);
		else
			fprintf(plot,"set terminal png %s\n", entity->options);
		fprintf(plot,"set output '%s'\n",image);
		fprintf(plot,"set key off\n");
		fprintf(plot,"set datafile separator \",\"\n");
		fprintf(plot,"set xdata time\n");
		fprintf(plot,"set timefmt \"%%Y-%%m-%%d %%H:%%M:%%S\"\n");
		fprintf(plot,"set format x \"%%H:%%M\"\n");
		fprintf(plot,"set xlabel \"Time\"\n");
		if (entity->unit)
			fprintf(plot,"set ylabel \"%s\"\n",entity->unit->name);
		fprintf(plot,"plot '%s' using 1:2\n",entity->source);
	}
	else
		fprintf(plot,"%s",entity->gnuplot);
	fclose(plot);
	plot=NULL;

	/* run gnuplot */
	if (system(command)==0)
		html_output(fp,"<img src=\"/output/%s\" alt=\"%s\"%s%s/>\n",image,entity->source,height,width);
	else
	{
		html_output(fp,"<span class=\"error\">Unable to run gnuplot</span>\n");
		goto Done;
	}
Done:
	if ( plot!=NULL )
		fclose(plot);
	return;
}

int GldGui::html_source_page(char *source)
{
	char buffer[65536];
	FILE *src = fopen(source,"rt");
	size_t len;
	if (src==NULL) return 0;
	while ((len=fread(buffer,1,sizeof(buffer)-1,src))>0)
	{
		buffer[len]='\0';
		html_output(fp,"%s",buffer);
	}
	fclose(src);
	return 1;
}
void GldGui::entity_html_content(GUIENTITY *entity)
{
	const char *ptype = get_property(entity) ? class_get_property_typename(entity->prop->ptype) : "";
	switch (entity->type) {

	case GUI_PAGE:
		if ( !html_source_page(entity->source) )
			html_output(fp,"ERROR: page '%s' not found: %s",entity->source,strerror(errno));
		break;

	// labeling 
	case GUI_TITLE: 
		if (entity->parent==NULL)
			html_output(fp,"<title>%s</title>\n",get_value(entity));
		else if (entity->parent->type==GUI_GROUP)
			html_output(fp,"<legend>%s</legend>\n",get_value(entity));
		else 
			html_output(fp,"<h%d>%s</h%d>\n",table+1,get_value(entity),table+1);
		break;
	case GUI_STATUS: 
		html_output(fp,"<script lang=\"jscript\"> window.status=\"%s\";</script>\n", get_value(entity));
		break;
	case GUI_TEXT: 
		if (!entity->parent || get_type(entity->parent)!=GUI_SPAN) newcol(entity);
		html_output(fp,"<span class=\"text\">%s</span>\n",get_value(entity));
		break;

	// inputting
	case GUI_INPUT:
		if (!entity->parent || get_type(entity->parent)!=GUI_SPAN) newcol(entity);
		html_output(fp,"<input class=\"%s\" type=\"text\" name=\"%s\" value=\"%s\" onchange=\"update_%s(this)\"/>\n", ptype, get_name(entity),get_value(entity), ptype);
		break;
	case GUI_CHECK: 
		{	
			PROPERTY *prop = get_property(entity);
			KEYWORD *key = NULL;
			if (!entity->parent || get_type(entity->parent)!=GUI_SPAN) newcol(entity);
			for (key=prop->keywords; key!=NULL; key=key->next)
			{
				uint64 value = *(uint64*)get_data(entity);
				const char *checked = (value==key->value)?"checked":"";
				char label[64], *p;
				strcpy(label,key->name);
				for (p=label; *p!='\0'; p++) if (*p=='_') *p=' '; else if (p>label && *p>='A' && *p<='Z') *p+='a'-'A';
				if (key->value!=0)
					html_output(fp,"<nobr><input class=\"%s\"type=\"checkbox\" name=\"%s\" value=\"%" FMT_INT64 "d\" %s onchange=\"update_%s(this)\"/>%s</nobr>\n",
						ptype, get_name(entity), key->value, checked, ptype, label);
			}
		}
		break;
	case GUI_RADIO: 
		{
			PROPERTY *prop = get_property(entity);
			KEYWORD *key = NULL;
			if (!entity->parent || get_type(entity->parent)!=GUI_SPAN) newcol(entity);
			for (key=prop->keywords; key!=NULL; key=key->next)
			{
				uint64 value = *(uint64*)get_data(entity);
				const char *checked = (value==key->value)?"checked":"";
				char label[64], *p;
				strcpy(label,key->name);
				for (p=label; *p!='\0'; p++) if (*p=='_') *p=' '; else if (p>label && *p>='A' && *p<='Z') *p+='a'-'A';
				html_output(fp,"<nobr><input class=\"%s\" type=\"radio\" name=\"%s\" value=\"%" FMT_INT64 "d\" %s onchange=\"update_%s(this)\" />%s</nobr>\n",
					ptype, get_name(entity), key->value, checked, ptype, label);
			}
		}
		break;
	case GUI_SELECT: 
		{
			PROPERTY *prop = get_property(entity);
			KEYWORD *key = NULL;
			const char *multiple = (prop->ptype==PT_set?"multiple":"");
			char size[64] = "";
			if (entity->size>0) sprintf(size,"size=\"%d\"",entity->size);
			if (!entity->parent || get_type(entity->parent)!=GUI_SPAN) newcol(entity);
			html_output(fp,"<select class=\"%s\" name=\"%s\" %s %s onchange=\"update_%s(this)\">\n", ptype, get_name(entity),multiple,size,ptype);
			for (key=prop->keywords; key!=NULL; key=key->next)
			{
				uint64 value = *(uint64*)get_data(entity);
				const char *checked = (value==key->value)?"selected":"";
				char label[64], *p;
				strcpy(label,key->name);
				for (p=label; *p!='\0'; p++) if (*p=='_') *p=' '; else if (p>label && *p>='A' && *p<='Z') *p+='a'-'A';
				html_output(fp,"<option value=\"%" FMT_INT64 "d\" %s>%s</option>\n",
					key->value, checked, label);
			}
			html_output(fp,"</select>\n");
		}
		break;

	// outputting
	case GUI_BROWSE:
		if (!entity->parent || get_type(entity->parent)!=GUI_SPAN) newcol(entity);
		output_html_textarea(entity);
		break;
	case GUI_TABLE:
		if (!entity->parent || get_type(entity->parent)!=GUI_SPAN) newcol(entity);
		output_html_table(entity);
		break;
	case GUI_GRAPH:
		if (!entity->parent || get_type(entity->parent)!=GUI_SPAN) newcol(entity);
		output_html_graph(entity);
		break;

	// action
	case GUI_ACTION: 
		if (!entity->parent || get_type(entity->parent)!=GUI_SPAN) newcol(entity);
		html_output(fp,"<input class=\"action\" type=\"submit\" name=\"action\" value=\"%s\" onclick=\"click(this)\" />\n",entity->action);
		break;
	default: break;
	}
}
void GldGui::entity_html_open(GUIENTITY *entity)
{
	switch (entity->type) {
	case GUI_TAB: 
		// TODO
		break;
	case GUI_GROUP:
		newcol(entity);
		html_output(fp,"<fieldset>\n");
		newtable(entity);
		break;
	case GUI_SPAN: 
		newcol(entity);
		startspan();
		break;
	default: 
		break;
	}
	entity_html_content(entity);
}
void GldGui::entity_html_close(GUIENTITY *entity)
{
	switch (entity->type) {
	case GUI_ROW: 
		newrow(entity);
		break;
	case GUI_TAB: 
		// TODO
		break;
	case GUI_GROUP:
		endtable();
		html_output(fp,"</fieldset>\n");
		break;
	case GUI_SPAN: 
		endspan();
		break;
	default: 
		break;
	}
}

void GldGui::html_output_children(GUIENTITY *entity)
{
	GUIENTITY *child;
	if (entity!=NULL) entity_html_open(entity);
	for (child=get_root(); child!=NULL; child=child->next)
	{
		if (!is_header(child) && child->parent==entity)
			html_output_children(child);
	}
	if (entity!=NULL) entity_html_close(entity);
}

void GldGui::include_element(const char *tag, const char *options, const char *file)
{
	char path[1024];
	if (!find_file(file,NULL,R_OK,path,sizeof(path)))
		output_error("unable to find '%s'", file);
	else
	{
		FILE *fin = fopen(path,"r");
		if (!fin)
			output_error("unable to open '%s'", path);
		else
		{
			char buffer[65536];
			size_t len = fread(buffer,1,sizeof(buffer),fin);
			if (len>=0)
			{
				buffer[len]='\0';
				html_output(fp,"<%s%s%s>\n%s\n</%s>\n",tag,options?" ":"",options?options:"",buffer,tag);
			}
			else
				output_error("unable to read '%s'", path);
			fclose(fin);
		}
	}
}

int GldGui::html_output_page(const char *page)
{
	GUIENTITY *entity;
	int len = 0;

	// output specific page
	if (page!=NULL)
	{
		for (entity=get_root(); entity!=NULL; entity=entity->next)
			if (strcmp(page,entity->value)==0)
				return html_source_page(entity->source);
	}

	// header entities
	len += html_output(fp,"<html>\n<head>\n");
	for (entity=get_root(); entity!=NULL; entity=entity->next)
	{
		if (is_header(entity))
			entity_html_content(entity);
	}
	len += html_output(fp,"</head>\n");

	include_element("script","lang=\"jscript\"","gridlabd.js");
	include_element("style",NULL,"gridlabd.css");

	// body entities
	len += html_output(fp,"<body>\n");
	html_output_children(NULL);
	endtable();
	len += html_output(fp,"</body>\n</html>\n");

	return len;
}

STATUS GldGui::html_output_all(void)
{
	GUIENTITY *entity;

	// header entities
	html_output(fp,"<html>\n<head>\n");
	for (entity=get_root(); entity!=NULL; entity=entity->next)
	{
		if (is_header(entity))
			entity_html_content(entity);
	}
	html_output(fp,"</head>\n");

	include_element("script","lang=\"jscript\"","gridlabd.js");
	include_element("style",NULL,"gridlabd.css");

	// body entities
	html_output(fp,"<body>\n");
	html_output_children(NULL);
	endtable();
	html_output(fp,"</body>\n</html>\n");
	return SUCCESS;
}

/**************************************************************************/
/* GLM OPERATIONS */
/**************************************************************************/
const char *GldGui::glm_typename(GUIENTITYTYPE type)
{
	const char *typemap[] = {
		NULL, 
		"row", "tab", "page", "group", "span", NULL,
		"title", "status", "text", NULL,
		"input", "check", "radio", "select", "action", NULL,
		"browse", "table", "graph", NULL,
		NULL,
	};
	if (type>=0 || type<sizeof(typemap)/sizeof(typemap[0]))
		return typemap[type];
	else
		return NULL;
}
size_t GldGui::glm_write(FILE *fp, GUIENTITY *entity, int indent)
{
	size_t count=0;
	GUIENTITY *parent = entity;
	const char *type = glm_typename(parent->type);
	char tabs[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";
	if ( indent < 0 ) 
	{
		tabs[0]='\0'; 
	}
	else if ( (size_t)indent < sizeof(tabs) ) 
	{
		tabs[indent]='\0';
	}
	if (type==NULL)
	{
		return FAILED;
	}
	
	if (entity->type==GUI_ACTION)
	{
		count += fprintf(fp,"%saction %s;\n",tabs,entity->action);
	}
	else
	{
		count += fprintf(fp,"%s%s {\n",tabs,type);

		if (get_object(entity))
			count += fprintf(fp,"%s\tlink %s:%s;\n", tabs,entity->objectname,entity->propertyname);
		else if (get_variable(entity))
			count += fprintf(fp,"%s\tglobal %s;\n", tabs,entity->globalname);
		else if (entity->value[0]!='\0')
			count += fprintf(fp,"%s\tvalue \"%s\";\n", tabs,entity->value);
		if (entity->unit)
			count += fprintf(fp,"%s\tunit \"%s\";\n", tabs,entity->unit->name);
		if (entity->size>0)
			count += fprintf(fp,"%s\tsize %d;\n", tabs,entity->size);
		if (entity->action[0]!='\0')
			count += fprintf(fp,"%s\taction %s;\n", tabs,entity->action);

		if (is_grouping(entity))
			for ( entity=root ; entity!=NULL ; entity=entity->next )
				if (entity->parent==parent)
					count+=glm_write(fp,entity,indent+1);

		count += fprintf(fp,"%s}\n",tabs);
	}
	return count;
}
size_t GldGui::glm_write_all(FILE *fp)
{
	size_t count=0;
	GUIENTITY *entity;
	if (root==NULL)
		return 0;
	count += fprintf(fp,"gui {\n");
	for ( entity=get_root() ; entity!=NULL ; entity=entity->next )
	{
		if (entity->parent==NULL)
			count += glm_write(fp,entity,1);
	}
	count += fprintf(fp,"}\n");
	return count;
}

/**************************************************************************/
/* LOAD OPERATIONS */
/**************************************************************************/
STATUS GldGui::startup(int argc, const char *argv[])
{
	static int started = 0;
	char cmd[1024];
	if (started)
		return SUCCESS;
#ifdef WIN32
	sprintf(cmd,"start %s http://localhost:%d/gui/", global_browser, global_server_portnum);
#else
	sprintf(cmd,"%s http://localhost:%d/gui/ & ps -p $! >/dev/null", global_browser, global_server_portnum);
#endif
	if (system(cmd)!=0)
	{
		output_error("unable to start interface");
		return FAILED;
	}
	else
	{
		IN_MYCONTEXT output_verbose("starting interface");
	}
	started = 1;
	return SUCCESS;
}
/** wait for GUI action
    @return 0 to halt system, 1 to continue
 **/
int GldGui::wait(void)
{
	if (server_startup(0,NULL)==FAILED)
	{
		output_error("gui is unable to start server");
		return 0;
	}
	if (startup(0,NULL)==FAILED)
	{
		output_error("gui is unable to start client");
		return 0;
	}

	/* if not wait action is expected */
	if (strcmp(root->wait_for,"")==0)

		/* immediate return success */
		return 1;

	/* begin waiting for action */
	status = GUIACT_WAITING;
	while (status==GUIACT_WAITING)
		exec_sleep(250000);
	if (status==GUIACT_HALT)
		return 0;
	status = GUIACT_NONE;
	return 1;
}

void GldGui::wait_status(GUIACTIONSTATUS s)
{
	status = s;
}
