/*	File: gui.h 
 	Copyright (C) 2008, Battelle Memorial Institute
 */

#ifndef _GUI_H
#define _GUI_H

#if ! defined _GLDCORE_H && ! defined _GRIDLABD_H
#error "this header may only be included from gldcore.h or gridlabd.h"
#endif

// Section: Underlying data types 

/*	Enum: e_guientitytype
	GUI_UNKNOWN = 0,

	GUI_ROW - a row group
	GUI_TAB - a tab group (includes tabs at top)
	GUI_PAGE - a page group (includes navigation |< < > >| buttons at top)
	GUI_GROUP - a group of entities with a labeled border around it
	GUI_SPAN - a group of entities that are not in columns

	GUI_TITLE - the title of the page, tab, or block
	GUI_STATUS - the status message of the page
	GUI_TEXT - a plain text entity 

	GUI_INPUT - an input textbox
	GUI_CHECK - a check box (set)
	GUI_RADIO - a radio button (enumeration)
	GUI_SELECT - a select drop down (enumeration)

	GUI_BROWSE - a text browsing entity
	GUI_TABLE - a tabulate presentation
	GUI_GRAPH - a graphing presentation

	GUI_ACTION - an action button
*/
enum e_guientitytype 
{
	GUI_UNKNOWN = 0,

	/* DO NOT CHANGE THE ORDER OF THE GROUPS!!! */
	GUI_ROW, 
	GUI_TAB, 
	GUI_PAGE, 
	GUI_GROUP, 
	GUI_SPAN, 
	_GUI_GROUPING_END, 

	GUI_TITLE, 
	GUI_STATUS, 
	GUI_TEXT, 
	_GUI_LABELING_END, 

	GUI_INPUT, 
	GUI_CHECK, 
	GUI_RADIO, 
	GUI_SELECT, 
	_GUI_INPUT_END, 

	GUI_BROWSE, 
	GUI_TABLE, 
	GUI_GRAPH, 
	_GUI_OUTPUT_END, 

	GUI_ACTION, 
	_GUI_ACTION_END, 

} ;

/* Enum: e_guidactionstatus
	GUIACT_NONE - no action pending
	GUIACT_WAITING - waiting for action to complete
	GUIACT_PENDING - action pending 
	GUIACT_HALT - halt action
 */
enum e_guidactionstatus 
{
	GUIACT_NONE 	= 0, 
	GUIACT_WAITING	= 1, 
	GUIACT_PENDING	= 2,
	GUIACT_HALT		= 3,
};

/* Structure: s_guientity
	type - gui entity type (see GE_*)
	srcref - reference to source file location
	value - value (if provided)
	globalname - ref to variable
	objectname - ref object
	propertyname - ref property
	action - action value
	span - col span
	size - size spec
	height - height spec
	width - width spec
	action_status  - 
	wait_for - 
	source - source file for data (output only)
	options - options for output
	gnuplot - gnuplot script
	hold - gui will remain on at the end of the simulation
	next - next entity
	parent - parent entity
	var - global variable context
	env - environment context
	obj - object context
	prop - property context
	data - data reference
	unit - units to use
 */
struct s_guientity 
{
	enum e_guientitytype type;
	char srcref[1024];
	char value[1024];
	char globalname[64];
	char objectname[64];
	char propertyname[64];
	char action[64];
	int span;
	int size;
	int height;
	int width;
	enum e_guidactionstatus action_status;
	char wait_for[64];
	char source[1024];
	char options[1024];
	char gnuplot[4096];
	int hold;
	struct s_guientity *next;
	struct s_guientity *parent;
	/* internal variables */
	GLOBALVAR *var;
	char *env;
	OBJECT *obj;
	PROPERTY *prop;
	void *data;
	UNIT *unit;
};

// Section: Type Definitions

// Typedef: GUIENTITYTYPE
// See <e_guientitytype>.
typedef enum e_guientitytype GUIENTITYTYPE;

// Typedef: GUIACTIONSTATUS
// See <e_guidactionstatus>
typedef enum e_guidactionstatus GUIACTIONSTATUS;

// Typedef: GUIENTITY
// See <s_guientity>
typedef struct s_guientity GUIENTITY;

// Typedef: GUISTREAMFN
typedef  int (*GUISTREAMFN)(void*,const char*,...);

#ifdef __cplusplus
extern "C" {
#endif

// Section: GUI Functions 

// Function: gui_create_entity
DEPRECATED GUIENTITY *gui_create_entity();

// Function: gui_set_html_stream
DEPRECATED void gui_set_html_stream(void *ref,GUISTREAMFN stream);

// Function: gui_set_srcref
DEPRECATED void gui_set_srcref(GUIENTITY *entity, char *filename, int linenum);

// Function: gui_set_type
DEPRECATED void gui_set_type(GUIENTITY *entity, GUIENTITYTYPE type);

// Function: gui_set_value
DEPRECATED void gui_set_value(GUIENTITY *entity, char *value);

// Function: gui_set_variablename
DEPRECATED void gui_set_variablename(GUIENTITY *entity, char *globalname);

// Function: gui_set_objectname
DEPRECATED void gui_set_objectname(GUIENTITY *entity, char *objectname);

// Function: gui_set_propertyname
DEPRECATED void gui_set_propertyname(GUIENTITY *entity, char *propertyname);

// Function: gui_set_span
DEPRECATED void gui_set_span(GUIENTITY *entity, int span);

// Function: gui_set_unit
DEPRECATED void gui_set_unit(GUIENTITY *entity, char *unit);

// Function: gui_set_next
DEPRECATED void gui_set_next(GUIENTITY *entity, GUIENTITY *next);

// Function: gui_set_parent
DEPRECATED void gui_set_parent(GUIENTITY *entity, GUIENTITY *parent);

// Function: gui_set_source
DEPRECATED void gui_set_source(GUIENTITY *entity, char *source);

// Function: gui_set_options
DEPRECATED void gui_set_options(GUIENTITY *entity, char *source);

// Function: gui_set_wait
DEPRECATED void gui_set_wait(GUIENTITY *entity, char *wait);

// Function: gui_startup
DEPRECATED STATUS gui_startup(int argc, const char *argv[]);

// Function: gui_post_action
DEPRECATED int gui_post_action(char *action);

// Function: gui_get_root
DEPRECATED GUIENTITY *gui_get_root(void);

// Function: gui_get_last
DEPRECATED GUIENTITY *gui_get_last(void);

// Function: gui_get_type
DEPRECATED GUIENTITYTYPE gui_get_type(GUIENTITY *entity);

// Function: gui_get_parent
DEPRECATED GUIENTITY *gui_get_parent(GUIENTITY *entity);

// Function: gui_get_next
DEPRECATED GUIENTITY *gui_get_next(GUIENTITY *entity);

// Function: gui_get_name
DEPRECATED const char *gui_get_name(GUIENTITY *entity);

// Function: gui_get_object
DEPRECATED OBJECT *gui_get_object(GUIENTITY *entity);

// Function: gui_get_property
DEPRECATED PROPERTY *gui_get_property(GUIENTITY *entity);

// Function: gui_get_value
DEPRECATED char *gui_get_value(GUIENTITY *entity);

// Function: gui_get_data
DEPRECATED void *gui_get_data(GUIENTITY *entity);

// Function: gui_get_variable
DEPRECATED GLOBALVAR *gui_get_variable(GUIENTITY *entity);

// Function: gui_get_environment
DEPRECATED char *gui_get_environment(GUIENTITY *entity);

// Function: gui_get_span
DEPRECATED int gui_get_span(GUIENTITY *entity);

// Function: gui_get_unit
DEPRECATED UNIT *gui_get_unit(GUIENTITY *entity);

// Function: gui_is_grouping
DEPRECATED int gui_is_grouping(GUIENTITY *entity);

// Function: gui_is_labeling
DEPRECATED int gui_is_labeling(GUIENTITY *entity);

// Function: gui_is_input
DEPRECATED int gui_is_input(GUIENTITY *entity);

// Function: gui_is_output
DEPRECATED int gui_is_output(GUIENTITY *entity);

// Function: gui_is_action
DEPRECATED int gui_is_action(GUIENTITY *entity);

// Function: gui_is_header
DEPRECATED int gui_is_header(GUIENTITY *entity);

// Function: gui_html_start
DEPRECATED void gui_html_start(void);

// Function: gui_X11_start
DEPRECATED void gui_X11_start(void);

// Function: gui_wait_status
DEPRECATED void gui_wait_status(GUIACTIONSTATUS status);

// Function: gui_html_output_page
DEPRECATED int gui_html_output_page(const char *page);

// Function: gui_html_output_all
DEPRECATED STATUS gui_html_output_all(void);

// Function: gui_glm_write_all
DEPRECATED size_t gui_glm_write_all(FILE *fp);

// Function: gui_wait
DEPRECATED int gui_wait(void);

#ifdef __cplusplus
}

// Define: MAX_TABLES
#define MAX_TABLES 16

/* Class: GldGui

	Graphical User Interface support class
 */
class GldGui
{
private:
	GldMain *main;
	GUIENTITY *root;
	GUIENTITY *last;
	void *fp;
	GUIACTIONSTATUS status;
	const char *TABLEOPTIONS;
	GUISTREAMFN html_output;
	int table;
	int row[MAX_TABLES];
	int col[MAX_TABLES];
	int span[MAX_TABLES];
public:

	// Constructor: GldGui
	GldGui(GldMain *ptr);

	// Destructor: GldGui
	~GldGui(void);

	// Method: create_entity
	GUIENTITY *create_entity();

	// Method: set_html_stream
	void set_html_stream(void *ref,GUISTREAMFN stream);

	// Method: set_srcref
	void set_srcref(GUIENTITY *entity, char *filename, int linenum);

	// Method: set_type
	void set_type(GUIENTITY *entity, GUIENTITYTYPE type);

	// Method: set_value
	void set_value(GUIENTITY *entity, char *value);

	// Method: set_variablename
	void set_variablename(GUIENTITY *entity, char *globalname);

	// Method: set_objectname
	void set_objectname(GUIENTITY *entity, char *objectname);

	// Method: set_propertyname
	void set_propertyname(GUIENTITY *entity, char *propertyname);

	// Method: set_span
	void set_span(GUIENTITY *entity, int span);

	// Method: set_unit
	void set_unit(GUIENTITY *entity, char *unit);

	// Method: set_next
	void set_next(GUIENTITY *entity, GUIENTITY *next);

	// Method: set_parent
	void set_parent(GUIENTITY *entity, GUIENTITY *parent);

	// Method: set_source
	void set_source(GUIENTITY *entity, char *source);

	// Method: set_options
	void set_options(GUIENTITY *entity, char *source);

	// Method: set_wait
	void set_wait(GUIENTITY *entity, char *wait);

	// Method: startup
	STATUS startup(int argc, const char *argv[]);

	// Method: post_action
	int post_action(char *action);

	// Method: get_root
	GUIENTITY *get_root(void);

	// Method: get_last
	GUIENTITY *get_last(void);

	// Method: get_type
	GUIENTITYTYPE get_type(GUIENTITY *entity);

	// Method: get_parent
	GUIENTITY *get_parent(GUIENTITY *entity);

	// Method: get_next
	GUIENTITY *get_next(GUIENTITY *entity);

	// Method: get_name
	const char *get_name(GUIENTITY *entity);

	// Method: get_object
	OBJECT *get_object(GUIENTITY *entity);

	// Method: get_property
	PROPERTY *get_property(GUIENTITY *entity);

	// Method: get_value
	char *get_value(GUIENTITY *entity);

	// Method: get_data
	void *get_data(GUIENTITY *entity);

	// Method: get_variable
	GLOBALVAR *get_variable(GUIENTITY *entity);

	// Method: get_environment
	char *get_environment(GUIENTITY *entity);

	// Method: get_span
	int get_span(GUIENTITY *entity);

	// Method: get_unit
	UNIT *get_unit(GUIENTITY *entity);

	// Method: is_grouping
	int is_grouping(GUIENTITY *entity);

	// Method: is_labeling
	int is_labeling(GUIENTITY *entity);

	// Method: is_input
	int is_input(GUIENTITY *entity);

	// Method: is_output
	int is_output(GUIENTITY *entity);

	// Method: is_action
	int is_action(GUIENTITY *entity);

	// Method: is_header
	int is_header(GUIENTITY *entity);

	// Method: html_start
	void html_start(void);

	// Method: X11_start
	void X11_start(void);

	// Method: wait_status
	void wait_status(GUIACTIONSTATUS status);

	// Method: html_output_page
	int html_output_page(const char *page);

	// Method: html_output_all
	STATUS html_output_all(void);

	// Method: glm_write_all
	size_t glm_write_all(FILE *fp);

	// Method: wait
	int wait(void);

	// Method: get_dump
	char *get_dump(GUIENTITY *entity);

	// Method: cmd_entity
	int cmd_entity(int item, GUIENTITY *entity);

	// Method: cmd_prompt
	void cmd_prompt(GUIENTITY *parent);

	// Method: cmd_input_count
	int cmd_input_count(GUIENTITY *entity);

	// Method: cmd_menu
	void cmd_menu(GUIENTITY *parent);

	// Method: cmd_start
	void cmd_start(void);

	// Method: startspan
	void startspan();

	// Method: endspan
	void endspan();

	// Method: newtable
	void newtable(GUIENTITY *entity);

	// Method: newrow
	void newrow(GUIENTITY *entity);

	// Method: newcol
	void newcol(GUIENTITY *entity);

	// Method: endtable
	void endtable();

	// Method: output_html_textarea
	void output_html_textarea(GUIENTITY *entity);

	// Method: output_html_table
	void output_html_table(GUIENTITY *entity);

	// Method: output_html_graph
	void output_html_graph(GUIENTITY *entity);

	// Method: html_source_page
	int html_source_page(char *source);

	// Method: entity_html_content
	void entity_html_content(GUIENTITY *entity);

	// Method: entity_html_open
	void entity_html_open(GUIENTITY *entity);

	// Method: entity_html_close
	void entity_html_close(GUIENTITY *entity);

	// Method: html_output_children
	void html_output_children(GUIENTITY *entity);

	// Method: include_element
	void include_element(const char *tag, const char *options, const char *file);

	// Method: glm_typename
	const char *glm_typename(GUIENTITYTYPE type);

	// Method: include_element
	size_t glm_write(FILE *fp, GUIENTITY *entity, int indent);

#ifdef X11

	// Method: X11_start
	void gui_X11_start(void);

#endif

};
#endif
	
#endif

