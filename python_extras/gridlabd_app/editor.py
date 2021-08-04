"""Copyright (C) 2021 Regents of the Leland Stanford Junior University
See https://www.gridlabd.us/ for license, acknowledgments, credits, manuals, documentation, and tutorials.

"""

import sys, os
import timeit
import json
import subprocess
import webbrowser

def stdout(*msg,file=sys.stdout):
    print(*msg,file=file)

def stderr(*msg,file=sys.stderr):
    print(*msg,file=file)

result = subprocess.run("/usr/local/bin/gridlabd --version=install".split(),capture_output=True)
if not result:
    stderr("ERROR: GridLAB-D is not installed on this system")
    quit(-1)
install_path = result.stdout.decode("utf-8").strip()

try:
    import gridlabd
except:
    stderr("ERROR: GridLAB-D is not installed for this python environment")
    quit(-1)

title = gridlabd.__title__
version = gridlabd.__version__.split('-')[0]
build = gridlabd.version()["build"]
branch = gridlabd.version()["branch"] 
python = f"{sys.version_info.major}.{sys.version_info.minor}.{sys.version_info.micro} ({sys.version_info.releaselevel})"
system = f"{os.uname().sysname} {os.uname().release} ({os.uname().machine})"
library = gridlabd.__file__

preferences = {
    "Show welcome dialog" : {
        "value" : False, # TODO: this should be True 
        "description" : "Show welcome dialog",
        },
    "Save output filename" : {
        "value" : "output.txt",
        "description" : "File name to save output on exit",
        },
    "Save output" : {
        "value" : True,
        "description" : "Enable saving output on exit",
        },
    "Reopen last file" : {
        "value" : True,
        "description" : "Enable reopening last file on initial open",
        },
    "Show unused classes" : {
        "value" : False,
        "description" : "Enable display of unused classes in elements list",
        },
    "Show classes" : {
        "value" : True,
        "description" : "Enable display of classes in elements list",
        }
    }
try:
    from tkinter import *
    from tkinter import Menu, messagebox, filedialog, simpledialog, ttk
except Exception as err:
    if system == 'Darwin':
        stderr(f"ERROR: {err}. Did you remember to run 'brew install python-tk'?",file=sys.stderr)
    else:
        stderr(f"ERROR: {err}. Did you remember to install tkinter support?",file=sys.stderr)
    quit(-1)

if sys.platform == "darwin":
    try:
        from Foundation import NSBundle
    except:
        import pip
        pip.main(["install","pyobjc"])
        from Foundation import NSBundle
    bundle = NSBundle.mainBundle()
    if bundle:
        info = bundle.localizedInfoDictionary() or bundle.infoDictionary()
        # stderr(info,file=sys.stderr)
        if info and info['CFBundleName'] == 'Python':
            info['CFBundleName'] = "HiPAS GridLAB-D"
            info['CFBundleShortVersionString'] = f"{version}"
            info['CFBundleVersion'] = f"{build} {branch}"
            info['NSHumanReadableCopyright'] = gridlabd.copyright().split("\n\n")[1]

class MenuBar(Menu):

    def __init__(self, main):
        Menu.__init__(self, main)

        def nothing(self):
            return

        self.file_menu = Menu(self, tearoff=False)
        self.file_menu.add_command(label="New", underline=0, command=main.file_new, accelerator="Command-N")  
        main.bind("<Meta_L><n>",main.file_new)
        self.file_menu.add_command(label="Open", command=main.file_open, accelerator="Command-O")
        main.bind("<Meta_L><o>",main.file_open)
        self.file_menu.add_command(label="Import", command=main.file_import, accelerator="Shift-Command-O")
        main.bind("<Meta_L><O>",main.file_import)
        self.file_menu.add_separator()
        self.file_menu.add_command(label="Close", command=main.file_new, accelerator="Command-W")    
        main.bind("<Meta_L><w>",main.file_close)
        self.file_menu.add_command(label="Save", command=main.file_save, accelerator="Command-S")  
        main.bind("<Meta_L><s>",main.file_save)
        self.file_menu.add_command(label="Save as", command=main.file_saveas, accelerator="Command-A")    
        main.bind("<Meta_L><a>",main.file_saveas)
        self.file_menu.add_command(label="Export", command=main.file_new, accelerator="Shift-Command-S")    
        main.bind("<Meta_L><S>",main.file_new)
        if sys.platform != "darwin":
            self.file_menu.add_separator()
            self.file_menu.add_command(label="Exit",command=main.file_exit, accelerator="Command-Q")
            main.bind("<Meta_L><q>",main.file_exit)
        self.add_cascade(label="File", menu=self.file_menu)
        
        self.edit_menu = Menu(self, tearoff=False)  
        self.edit_menu.add_command(label="Undo", accelerator="Command-Z", command=main.undo)  
        main.bind("<Meta_L><z>",main.redo)
        self.edit_menu.add_command(label="Redo", accelerator="Command-Y", command=main.redo)  
        main.bind("<Meta_L><y>",main.redo)
        self.edit_menu.add_separator()     
        self.edit_menu.add_command(label="Cut", accelerator="Command-X", command=main.cut)  
        main.bind("<Meta_L><x>",main.cut)
        self.edit_menu.add_command(label="Copy", accelerator="Command-C", command=main.copy)  
        main.bind("<Meta_L><c>",main.copy)
        self.edit_menu.add_command(label="Paste", accelerator="Command-V", command=main.paste)
        main.bind("<Meta_L><v>",main.paste)
        self.edit_menu.add_command(label="Paste special", accelerator="Shift-Command-V", command=main.paste_special)
        main.bind("<Meta_L><V>",main.copy)
        if sys.platform != "darwin":
            self.edit_menu.add_separator()     
            self.edit_menu.add_command(label="Preferences", accelerator="Command-,", command=main.preferences)
            main.bind("<Meta_L><,>",main.preferences)
        self.add_cascade(label="Edit", menu=self.edit_menu) 

        self.model_menu = Menu(self, tearoff=False)

        self.model_menu.add_command(label="Build", command=main.model_build, accelerator="Command-B",)
        main.bind("<Meta_L><b>",main.model_build)
        self.model_menu.add_separator()     
        
        self.model_menu.add_command(label="Clock", command=main.model_clock, accelerator="Command-K")
        main.bind("<Meta_L><k>",main.model_clock)

        self.model_menu.add_command(label="Library", command=main.model_library, accelerator="Command-L",)
        main.bind("<Meta_L><l>",main.model_library)
        
        self.model_menu.add_command(label="Template", command=main.model_template, accelerator="Command-T",)
        main.bind("<Meta_L><t>",main.model_template)
        self.add_cascade(label="Model", menu=self.model_menu) 

        self.model_weather = Menu(self,tearoff=False)
        self.model_weather.add_command(label="Manager...", command=main.model_weather_manager)
        self.model_weather.add_command(label="Choose...", command=main.model_weather_choose,)
        self.model_menu.add_cascade(label="Weather", menu=self.model_weather)

        self.view_menu = Menu(self,tearoff=False)
        self.add_cascade(label="View", menu=self.view_menu)

        self.view_menu_elements = Menu(self,tearoff=False)
        # main.view_module = BooleanVar()
        # self.view_menu_elements.add_checkbutton(label = "Module", onvalue = True, offvalue = False, variable = main.view_module, command = main.on_view_elements)
        main.view_class = BooleanVar()
        main.view_class.set(preferences["Show classes"]["value"])
        self.view_menu_elements.add_checkbutton(label="Class", onvalue=True, offvalue=False, variable=main.view_class, command = main.on_view_elements)
        # main.view_group = BooleanVar()
        # self.view_menu_elements.add_checkbutton(label="Group", onvalue=True, offvalue=False, variable=main.view_group, command = main.on_view_elements)
        self.view_menu.add_cascade(label="Elements",menu=self.view_menu_elements)

        self.help_menu = Menu(self, tearoff=False)
        if sys.platform != "darwin":
            self.help_menu.add_command(label="About", command=main.help_about)
        self.help_menu.add_command(label="License", command=main.help_license)
        self.help_menu.add_command(label="Documentation", command=main.help_documentation)
        self.add_cascade(label="Help", menu=self.help_menu)  

        main.config(menu=self)

        main.bind_all("<Key>",self.key_event)

        self.main = main

    def key_event(self,event):
        # stderr("Key event: ",event)
        return

class Editor(Tk):

    def __init__(self):
        Tk.__init__(self)

        self.configure(background='lightgray')

        self.style = ttk.Style(self)
        self.style.configure('gridlabd.Treeview',
            padding = 3,
            selectmode = 'browse',
            )
        self.style.configure('gridlabd.Text',
            padding = 3,
            )

        self.filename = None
        self.model = None
        self.template = None
        self.weather = None
        self.library = None
        # self.viewtype = 'objects' # 'objects','classes','modules','globals','schedules','filters'
        # self.viewstyle = 'name' # 'name'

        self.output_height = 300
        self.sidebar_width = 200
        self.modelview_layout = dict(x=0, y=0, width=self.sidebar_width, height=self.winfo_screenheight()-self.output_height)
        self.dataview_layout = dict(x=self.sidebar_width, y=0, width=int(self.winfo_screenwidth()/2-self.sidebar_width), height=self.winfo_screenheight()-self.output_height)
        self.outputview_layout = dict(x=0, y=int(self.winfo_screenheight()-self.output_height), width=int(self.winfo_screenwidth()/2), height=self.output_height)

        self.set_title()
        self.geometry(f'{int(self.winfo_screenwidth()/2)}x{self.winfo_screenheight()}')

        self.menubar = MenuBar(self)
        self.config(menu=self.menubar)

        self.treeview = ModelTree(self)
        self.treeview.place(x=self.modelview_layout['x'],y=self.modelview_layout['y'])

        self.dataview = DataView(self)
        self.dataview.place(x=self.dataview_layout['x'],y=self.dataview_layout['y'])

        self.outputview = OutputView(self)
        self.outputview.place(x=self.outputview_layout['x'],y=self.outputview_layout['y'])

        if len(sys.argv) > 1:
            self.filename = sys.argv[1]
            self.load_model()
            self.update()

        if sys.platform == "darwin":
            self.createcommand('::tk::mac::ShowPreferences',self.preferences)
            self.createcommand('::tk::mac::standardAboutPanel',self.help_about)
            self.createcommand('::tk::mac::Quit',self.file_exit)
            self.bind("<Meta_L><,>",self.preferences)

    def output(self,msg,end='\n'):
        self.outputview.append_text(msg+end)

    def warning(self,msg,end='\n'):
        self.outputview.append_text("WARNING: +"+msg+end)

    def error(self,msg,end='\n'):
        self.outputview.append_text("ERROR: "+msg+end)

    def preferences(self):
        PreferencesDialog(self,preferences)

    def load_model(self):
        if self.filename.endswith(".glm"):
            self.output(f"Compiling {self.filename}...\n")
            result = subprocess.run(["gridlabd","-C",self.filename,"-o",self.filename.replace('.glm','.json')],capture_output=True)
            self.output(result.stderr.decode("utf-8"))
            self.output(result.stdout.decode("utf-8"))
            if result.returncode != 0:
                return
            else:
                self.filename = self.filename.replace('.glm','.json')
        with open(self.filename,"r") as f: 
            try:
                self.model = json.load(f)
            except Exception as errmsg:
                messagebox.showerror(self.filename,f"file does not contain a valid JSON data")
                return
        if not self.model \
                or not "application" in self.model.keys() \
                or self.model["application"] != gridlabd.__name__:
            messagebox.showerror(self.filename,f"file does not contain a valid {gridlabd.__title__} model")
            return
        else:
            self.set_title(self.filename)
            self.treeview.set_model()

    def save_model(self):
        with open(self.filename,"w") as f: json.dump(self.model,f,indent=4)

    def set_title(self,text=None):
        title = gridlabd.__title__
        if text:
            title = gridlabd.__title__ + " -- " + os.path.basename(text)
        self.title(title)

    def on_view_elements(self,event=None):
        self.treeview.set_model()

    #
    # File menu
    #
    def file_new(self,event=None):
        self.filename = None
        self.model = None
        self.load_model()

    def file_open(self,event=None):
        if self.filename:
            initialfile = os.path.basename(self.filename)
            defaultextension = os.path.splitext(self.filename)[1]
            initialdir = os.path.dirname(self.filename)
        else:
            initialfile = "untitled.json"
            defaultextension = ".json"
            initialdir = os.getcwd()
        self.filename = filedialog.askopenfilename(
            initialfile = initialfile,
            defaultextension = defaultextension,
            initialdir = initialdir,
            )
        self.load_model()

    def file_import(self,event=None):
        inputname = filedialog.askopenfilename()
        if not inputname:
            return
        import_dialog = ImportDialog(self,inputname=inputname)
        if import_dialog.result == True:
            self.filename = import_dialog.outputname
            inputext = os.path.splitext(inputname)[1][1:]
            outputext = os.path.splitext(import_dialog.outputname)[1][1:]
            if import_dialog.inputtype: 
                inputtype = f"{inputext}-{import_dialog.inputtype}"
            else: 
                inputtype = inputext
            if import_dialog.outputtype:
                outputtype = f"{outputext}-{import_dialog.outputtype}"
            else:
                outputtype = outputext
            command =  f"gridlabd convert -i {inputname} -o {import_dialog.outputname} -f {inputtype} -t {outputtype}"
            self.output(f"Running {command}...\n")
            result = subprocess.run(command.split(),capture_output=True)
            if result:
                self.output(result.stdout)
                self.output(result.stderr)
            else:
                self.error("command failed")
            if result and result.returncode == 0:
                self.load_model()
            else:
                messagebox.showerror(result.stderr)

    def file_close(self,event=None):
        return

    def file_save(self,event=None):
        self.save_model()

    def file_saveas(self,event=None):
        if self.filename:
            initialfile = os.path.basename(self.filename)
            defaultextension = os.path.splitext(self.filename)[1]
            initialdir = os.path.dirname(self.filename)
        else:
            initialfile = "untitled.json"
            defaultextension = ".json"
            initialdir = os.getcwd()
        self.filename = filedialog.asksaveasfilename(
            initialfile = initialfile,
            defaultextension = defaultextension,
            initialdir = initialdir,
            )
        self.save_model()
        self.update()

    def file_export(self,event=None):
        return

    def file_exit(self,event=None):

        # save outputview to output save file
        if preferences["Save output"]["value"]:
            with open(preferences["Save output filename"]["value"],"w") as f:
                f.write(self.outputview.get(1.0,"end-1c"))
        self.destroy()

    #
    # Edit menu
    #

    def undo(self,event=None):
        return

    def redo(self,event=None):
        return

    def cut(self,event=None):
        return

    def copy(self,event=None):
        focus = self.focus_get()
        item = focus.get_selected()
        # item[0] is self.filename
        text = self.model
        for ref in item["context"]:
            text = text[ref]
        self.clipboard_clear()
        self.clipboard_append(json.dumps(text,indent=4))

    def paste(self,event=None):
        return

    def paste_special(self,event=None):
        return

    #
    # Model menu
    #
    def model_build(self,event=None):
        self.output(f"\nStarting {os.path.split(self.filename)[1]}...\n")
        tic = timeit.default_timer()
        command = ["gridlabd",self.filename]
        if self.template:
            command.append(self.template)
        command.extend(["-D","show_progress=FALSE"])
        result = subprocess.run(command,capture_output=True)
        toc = timeit.default_timer()
        self.output(result.stderr)
        self.output(result.stdout)
        if result.returncode:
            self.error(f"\nreturn code {result.returncode}!\n")
        else:
            self.output(f"\nSimulation done\n")
        self.output(f"Elapsed time: {toc-tic:.2g} seconds\n")

    def model_clock(self,event=None):
        msg = Tk()
        msg.title("Clock")
        table = ttk.Treeview(msg,columns=['value'],show='tree')
        table.insert('',END,text='Time zone',values=[self.model['globals']['timezone_locale']['value']])
        table.insert('',END,text='Start time',values=[self.model['globals']['starttime']['value']])
        table.insert('',END,text='Stop time',values=[self.model['globals']['stoptime']['value']])
        table.column('#0',width=100)
        table.column('value',width=500,stretch=YES)
        table.grid(row=0, column=0)

    def model_template(self,event=None):
        template = filedialog.askopenfilename(
            initialfile = self.template,
            defaultextension = ".glm",
            initialdir = "/usr/local/share/gridlabd/template",
            )
        if template:
            self.template = template
            self.output(f"Template {self.template} added")

    def model_library(self,event=None):
        library = filedialog.askopenfilename(
            initialfile = self.library,
            defaultextension = ".glm",
            initialdir = "/usr/local/share/gridlabd/library",
            )
        if library:
            self.library = library
            self.output(f"Library {self.library} added")

    def model_weather_manager(self,event=None):
        os.system("/usr/local/bin/python3 /usr/local/share/gridlabd/weather.py &")

    def model_weather_choose(self,event=None):
        weather = filedialog.askopenfilename(
            initialfile = self.weather,
            defaultextension = ".tmy3",
            initialdir = "/usr/local/share/gridlabd/weather",
            )
        if weather:
            self.weather = weather
            self.output(f"Weather {self.weather} added")

    #
    # Help
    #
    def help_about(self):
        messagebox.showinfo(title, f"Version: {version}-{build}\nSource: {branch}\nLibrary: {library}\n\nSystem: {system}\nPython: {python}")

    def help_license(self):
        title = gridlabd.__title__
        license = gridlabd.license()
        msg = Tk()
        msg.title(f"{title} License")
        txt = Label(msg,text=license, anchor=W, justify=LEFT)
        txt.grid(row=0, column=0)
        msg.mainloop()

    def help_documentation(self):
        webbrowser.open(f"https://docs.gridlabd.us/",new=2)

class ModelTree(ttk.Treeview):

    def __init__(self,main):
        ttk.Treeview.__init__(self,main,style='gridlabd.Treeview',
            height = main.modelview_layout['height'],
            )
        self.main = main
        self.bind("<ButtonRelease-1>",self.on_select)
        self.set_model()

    def clear_model(self):
        self.heading('#0',text='Element')
        for item in self.get_children():
            self.delete(item)

    def set_model(self):
        """styles in ['name','rank','class','parent']"""
        self.clear_model()
        if self.main.model:
            self.show_objects_by_name()
        self.main.update()

    def show_objects_by_name(self):
        self.heading('#0',text='Objects by name')
        basename = os.path.basename(self.main.model['globals']['modelname']['value'])
        root = self.insert('',END,text=basename)
        if self.main.view_class.get():
            classes = {}
            for name in sorted(self.main.model['classes'].keys(),key=str.lower):
                classes[name] = self.insert(root,END,text=name)
        for name in sorted(self.main.model['objects'].keys(),key=str.lower):
            if self.main.view_class.get():
                oclass = self.main.model['objects'][name]['class']
                self.insert(classes[oclass],END,text=name)
            else:
                self.insert(root,END,text=name)
        if self.main.view_class.get() and not preferences["Show unused classes"]["value"]:
            for name in self.get_children(root):
                if not self.get_children(name):
                    self.delete(name)

    def on_select(self,event):
        item = self.get_selected()
        if item:
            name = item['context'][-1]
            basename = os.path.basename(self.main.model['globals']['modelname']['value'])
            if name == basename:
                self.main.dataview.show_globals()
            elif name in self.main.model['objects'].keys():
                self.main.dataview.show_object(name)

    def get_selected(self):
        tag = self.selection()[0]
        if tag:
            name = self.item(tag,'text')
            for spec in self.main.model.keys():
                if type(self.main.model[spec]) is dict and name in self.main.model[spec].keys():
                    return {
                        "file" : self.main.filename,
                        "context" : [spec,name],
                        "value" : self.main.model[spec][name],
                        }
            return {"file":name,"context":['globals'],"values":self.main.model['globals']}

class DataView(ttk.Treeview):

    def __init__(self,main):
        columns = ['Property','Value','Description']
        widths = {'Property':100,'Value':200,'Description':main.dataview_layout['width']-306}
        ttk.Treeview.__init__(self,main,style='gridlabd.Treeview',
            height = main.dataview_layout['height'],
            columns = columns,
            )
        self['show'] = 'headings'
        for item in columns:
            self.heading(item,text=item)
            self.column(item,width=widths[item])
        self.bind("<Double-1>",self.on_doubleclick)
        self.target = None
        self.main = main

    def clear_model(self):
        self.heading('#0',text='(none)')
        for item in self.get_children():
            self.delete(item)

    def show_globals(self):
        self.clear_model()
        self.object = None
        for name,data in self.main.model['globals'].items():
            if 'description' in data.keys():
                description = data['description']
            else:
                description = ''
            self.insert('',END,
                text=f"globals/{name}",
                values=[name,data['value'],description])
        self.main.update()

    def show_object(self,name):
        self.clear_model()
        self.object = name
        obj = self.main.model['objects'][name]
        for prop,value in obj.items():
            oclass = self.main.model['classes'][obj['class']]
            if prop in oclass.keys() and 'description' in oclass[prop]:
                description = oclass[prop]['description']
            else:
                description = ""
            self.insert('',END,prop,
                text=f"objects/{name}/{prop}",
                values=[prop,value,description])
        self.main.update()

    def on_doubleclick(self,event):
        item = self.selection()[0]
        info = self.item(item)['text'].split('/')
        if not info:
            return
        elif info[0] == 'globals':
            varname = info[1]
            var = self.main.model['globals'][varname]
            if var['access'] != 'PUBLIC':
                messagebox.showerror(f"Global set error",f"Global {varname} cannot be changed")
                return
            value = var['value']
            edit = simpledialog.askstring(title=f"Model global",prompt=f"Enter new value for global '{varname}'",initialvalue=value)
            var['value'] = edit
            self.set(item,"#2",edit)
        elif info[0] == 'objects':
            objname = info[1]
            propname = info[2]
            if propname in ['id','class']:
                messagebox.showerror(f"Property set error",f"Property {propname} cannot be changed")
                return
            obj = self.main.model['objects'][objname]
            value = obj[propname]
            edit = simpledialog.askstring(title=f"Object {objname}",prompt=f"Enter new value for property '{propname}'",initialvalue=value)
            obj[propname] = edit
            self.set(name,"#2",edit)

    def get_selected(self):
        tag = self.selection()[0]
        if tag:
            return {
                "file" : self.main.filename,
                "context" : self.item(tag,'text').split('/'),
                "value" : self.item(tag,'value')[1],
                }

class OutputView(Text):

    def __init__(self,main):
        Text.__init__(self,main,
            height = main.outputview_layout['height'],
            width = main.outputview_layout['width'],
            )
        scroll = Scrollbar(self)
        self.configure(yscrollcommand=scroll.set)
        self.append_text(f"{title} {version}-{build} ({branch}) {system}\n{__doc__}")

    def append_text(self,text):
        self.insert(END,text)
        self.update()

class PreferencesDialog(simpledialog.Dialog):

    def __init__(self,parent,preferences):
        Toplevel.__init__(self, parent)
        self.parent = parent
        self.transient(parent)
        self.title("Preferences")

        tree = ttk.Treeview(self,columns=["value","description"])
        tree.heading("#0",text="Setting")
        tree.column("#0",width=150)
        tree.heading("value",text="Value")
        tree.column("value",width=200)
        tree.heading("description",text="Description")
        tree.column("description",width=450)
        tree.grid(row=0,column=1)
        for name in sorted(preferences.keys()):
            value = preferences[name]
            item = tree.insert('',END,text=name,values=[str(value["value"]),value["description"]])

        Button(self, text="Save", width=10, command=self.on_save, default=DISABLED).grid(row=3,column=1,padx=10,pady=10)
        self.grab_set()
        self.wait_window(self)

    def on_save(self):
        global preferences
        self.destroy()

class ImportDialog(simpledialog.Dialog):

    def __init__(self,parent,inputname,outputname=None):
        Toplevel.__init__(self, parent)
        self.parent = parent
        self.transient(parent)
        self.title("Import options")

        self.inputname = inputname
        inputext = os.path.splitext(inputname)[1]
        if outputname:
            outputext = os.path.splitext(outputname)[1]
            self.outputname = outputname
        else:
            outputext = ".glm"
            outputname = inputname.replace(inputext,outputext)
        self.outputname = outputname
        self.inputtype = None
        self.outputtype = None
        self.result = None

        config = subprocess.run(f"/usr/local/bin/python3 {install_path}/share/gridlabd/{inputext[1:]}2{outputext[1:]}.py --config".split(),capture_output=True)
        Label(self,text=f"Input {self.inputname.split('/')[-1]} type:").grid(row=0,column=0)
        Label(self,text=f"Output {self.outputname.split('/')[-1]} type:").grid(row=1,column=0)
        if not config or config.returncode != 0:
            raise Exception("unable to get converter configuration options")
        else:
            options = json.loads(config.stdout.decode('utf-8'))
        try:
            from_options = options["from"]
            self.inputtype = StringVar()
            self.inputtype.set(from_options[0])
            item = OptionMenu(self,self.inputtype,*from_options)
            item.grid(row=0,column=1)
        except Exception as err:
            Label(self,text="None").grid(row=0,column=1)
            stderr("ERROR:",err,file=sys.stderr)
            self.inputtype = None
        try:
            to_options = options["type"]
            self.outputtype = StringVar()
            self.outputtype.set(to_options[0])
            item = OptionMenu(self,self.outputtype,*to_options)
            item.grid(row=1,column=1)
        except Exception as err:
            Label(self,text="None").grid(row=1,column=1)
            stderr("ERROR:",err,file=sys.stderr)
            self.outputtype = None

        Label(self,text=f"Convert {inputname.split('/')[-1]} to {outputname.split('/')[-1]}?").grid(row=2)
        Button(self, text="Cancel", width=10, command=self.on_cancel).grid(row=3,column=0,padx=10,pady=10)
        Button(self, text="OK", width=10, command=self.on_ok, default=ACTIVE).grid(row=3,column=1,padx=10,pady=10)

        self.bind("&lt;Return>", self.on_ok)
        self.bind("&lt;Escape>", self.on_cancel)        
        self.grab_set()
        self.wait_window(self)

    def on_ok(self,event=None):
        self.result = True
        self.inputtype = str(self.inputtype.get())
        self.outputtype = str(self.outputtype.get())
        self.destroy()

    def on_cancel(self,event=None):
        self.result = False
        self.inputtype = None
        self.outputtype = None
        self.destroy()
    

class ExportDialog(simpledialog.Dialog):

    def __init__(self,parent,inputname,outputname=None):
        Toplevel.__init__(self, parent)
        self.parent = parent
        self.transient(parent)
        self.title("Import options")
        self.grab_set()
        self.wait_window(self)

if __name__ == "__main__":
    root = Editor()
    if preferences["Show welcome dialog"]["value"]:
        messagebox.showinfo("Welcome",
            f"""{title}\n{version}-{build} ({branch}) {system}\n{__doc__}
            """)
    root.mainloop()
