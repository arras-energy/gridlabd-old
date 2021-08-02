"""Copyright (C) 2021 Regents of the Leland Stanford Junior University
See https://www.gridlabd.us/ for license, acknowledgments, credits, manuals, documentation, and tutorials.

"""
import sys, os
import timeit
from tkinter import *
from tkinter import Menu, messagebox, filedialog, simpledialog, ttk
import gridlabd
import json
import webbrowser
import subprocess

title = gridlabd.__title__
version = gridlabd.__version__.split('-')[0]
build = gridlabd.version()["build"]
branch = gridlabd.version()["branch"] 
python = f"{sys.version_info.major}.{sys.version_info.minor}.{sys.version_info.micro} ({sys.version_info.releaselevel})"
system = f"{os.uname().sysname} {os.uname().release} ({os.uname().machine})"
library = gridlabd.__file__

class Application(Tk):

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
        self.viewtype = 'objects' # 'objects','classes','modules','globals','schedules','filters'
        self.viewstyle = 'name' # 'name'

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

    def layout(self):
        return

    def load_model(self):
        with open(self.filename,"r") as f: self.model = json.load(f)
        if not "application" in self.model.keys() or self.model["application"] != gridlabd.__name__:
            messagebox.showerror(self.filename,f"file does not contain a valid {gridlabd.__title__} model")
        else:
            self.set_title(self.filename)
            self.treeview.set_model()

    def save_model(self):
        with open(self.filename,"w") as f: json.dump(self.model,f,indent=4)

    def set_title(self,text=None):
        if not text:
            self.title(f"{gridlabd.__title__} {gridlabd.__version__}")
        else:
            self.title(f"{gridlabd.__title__} {gridlabd.__version__} - {os.path.basename(text)}")            

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

    def file_exit(self,event=None):
        self.quit()

    #
    # Model menu
    #
    def model_build(self,event=None):
        self.outputview.append_text(f"\nStarting {os.path.split(self.filename)[1]}...\n")
        tic = timeit.default_timer()
        command = ["gridlabd",self.filename]
        if self.template:
            command.append(self.template)
        command.extend(["-D","show_progress=FALSE"])
        result = subprocess.run(command,capture_output=True)
        toc = timeit.default_timer()
        self.outputview.append_text(result.stderr)
        self.outputview.append_text(result.stdout)
        if result.returncode:
            self.outputview.append_text(f"\nSimulation error code {result.returncode}!\n")
        else:
            self.outputview.append_text(f"\nSimulation done\n")
        self.outputview.append_text(f"Elapsed time: {toc-tic:.2g} seconds\n")

    def model_template(self,event=None):
        template = filedialog.askopenfilename(
            initialfile = self.template,
            defaultextension = ".glm",
            initialdir = "/usr/local/share/gridlabd/template",
            )
        if template:
            self.template = template
            self.outputview.append_text(f"Template {self.template} added")

    def model_library(self,event=None):
        library = filedialog.askopenfilename(
            initialfile = self.library,
            defaultextension = ".glm",
            initialdir = "/usr/local/share/gridlabd/library",
            )
        if library:
            self.library = library
            self.outputview.append_text(f"Library {self.library} added")

    def model_weather(self,event=None):
        weather = filedialog.askopenfilename(
            initialfile = self.weather,
            defaultextension = ".tmy3",
            initialdir = "/usr/local/share/gridlabd/weather",
            )
        if weather:
            self.weather = weather
            self.outputview.append_text(f"Weather {self.weather} added")

    def about(self):
        messagebox.showinfo(title, f"Version: {version}-{build}\nSource: {branch}\nLibrary: {library}\n\nSystem: {system}\nPython: {python}")

    def license(self):
        title = gridlabd.__title__
        license = gridlabd.license()
        msg = Tk()
        msg.title(f"{title} License")
        txt = Label(msg,text=license, anchor=W, justify=LEFT)
        txt.grid(row=0, column=0)
        msg.mainloop()

    def documentation(self):
        webbrowser.open(f"https://docs.gridlabd.us/",new=2)

def nothing(self):
    return

class MenuBar(Menu):

    def __init__(self, main):
        Menu.__init__(self, main)

        self.file = Menu(self, tearoff=False)
        self.file.add_command(label="New", underline=0, command=main.file_new, accelerator="Command-N")  
        main.bind("<Meta_L><n>",main.file_new)
        self.file.add_command(label="Open", command=main.file_open, accelerator="Command-O")
        main.bind("<Meta_L><o>",main.file_open)
        self.file.add_separator()
        self.file.add_command(label="Save", command=main.file_save, accelerator="Command-S")  
        main.bind("<Meta_L><s>",main.file_save)
        self.file.add_command(label="Save as", command=main.file_saveas, accelerator="Command-A")    
        main.bind("<Meta_L><a>",main.file_saveas)
        self.file.add_command(label="Close", command=main.file_new, accelerator="Command-W")    
        main.bind("<Meta_L><w>",main.file_new)
        self.file.add_separator()
        self.file.add_command(label="Quit", underline=1, command=main.file_exit, accelerator="Command-Q")
        self.add_cascade(label="File", menu=self.file)
        
        self.edit = Menu(self, tearoff=False)  
        self.edit.add_command(label="Undo", accelerator="Command-Z")  
        self.edit.add_command(label="Redo", accelerator="Command-Y")  
        self.edit.add_separator()     
        self.edit.add_command(label="Cut", accelerator="Command-X")  
        self.edit.add_command(label="Copy", accelerator="Command-C")  
        self.edit.add_command(label="Paste", accelerator="Command-V")  
        self.add_cascade(label="Edit", menu=self.edit) 

        self.model = Menu(self, tearoff=False)
        self.model.add_command(label="Build", command=main.model_build, accelerator="Command-B",)
        main.bind("<Meta_L><b>",main.model_build)
        self.edit.add_separator()     
        self.model.add_command(label="Library", command=main.model_library, accelerator="Command-L",)
        main.bind("<Meta_L><l>",main.model_library)
        self.model.add_command(label="Template", command=main.model_template, accelerator="Command-T",)
        main.bind("<Meta_L><t>",main.model_template)
        self.model.add_command(label="Weather", command=main.model_weather, accelerator="Command-E",)
        main.bind("<Meta_L><e>",main.model_weather)
        self.add_cascade(label="Model", menu=self.model) 

        help = Menu(self, tearoff=False)  
        help.add_command(label="About", command=main.about)
        help.add_command(label="License", command=main.license)
        help.add_command(label="Documentation", command=main.documentation)
        self.add_cascade(label="Help", menu=help)  

        main.config(menu=self)

        main.bind_all("<Key>",self.key_event)

        self.main = main

    def key_event(self,event):
        print("Key event: ",event)

class ModelTree(ttk.Treeview):

    def __init__(self,main):
        ttk.Treeview.__init__(self,main,style='gridlabd.Treeview',
            height = main.modelview_layout['height'],
            )
        self.main = main
        self.bind("<ButtonRelease-1>",self.on_select)
        self.set_model()

    def clear_model(self):
        self.heading('#0',text='(none)')
        for item in self.get_children():
            self.delete(item)

    def set_model(self):
        """styles in ['name','rank','class','parent']"""
        self.clear_model()
        if self.main.model and self.main.viewtype == 'objects' \
                and 'objects' in self.main.model.keys():
            if self.main.viewstyle == 'name':
                self.show_objects_by_name()
        self.main.update()

    def show_objects_by_name(self):
        self.heading('#0',text='Objects by name')
        basename = os.path.basename(self.main.model['globals']['modelname']['value'])
        item = self.insert('',END,text=basename)
        for name in sorted(self.main.model['objects'].keys(),key=str.lower):
            self.insert(item,END,text=name)

    def on_select(self,event):
        tag = self.selection()[0]
        name = self.item(tag,'text')
        basename = os.path.basename(self.main.model['globals']['modelname']['value'])
        if name == basename:
            self.main.dataview.show_globals()
        else:
            self.main.dataview.show_object(name)

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

if __name__ == "__main__":
    root = Application()
    root.mainloop()
