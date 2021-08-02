"""Copyright (C) 2021 Regents of the Leland Stanford Junior University
See https://www.gridlabd.us/ for license, acknowledgments, credits, manuals, documentation, and tutorials.

"""
import sys, os
import timeit
from tkinter import *
from tkinter import Menu, messagebox, filedialog, simpledialog, ttk
import subprocess

dialog_width = 600
dialog_height = 400

class Weather(Tk):

    def __init__(self):
        Tk.__init__(self)

        self.title("GridLAB-D Weather")
        self.configure()
        self.focus()

        self.style = ttk.Style(self)
        self.style.configure('gridlabd.Treeview',
            padding = 3,
            selectmode = 'browse',
            )
        self.style.configure('gridlabd.Text',
            padding = 3,
            )

        self.geometry(f'{dialog_width}x{dialog_height}')

        self.indexview = IndexView(self)
        self.indexview.column('#0',width=290)
        self.indexview.pack(padx=5, pady=10, side=LEFT, fill=Y)

        self.listview = ListView(self)
        self.listview.column('#0',width=290)
        self.listview.pack(padx=5, pady=10, side=LEFT, fill=Y)

        self.message = Label(self,text="Loading data...")
        self.message.pack(padx=5,pady=5,side=BOTTOM, fill=X)

    def command(self,text):
        subcommand = ["gridlabd","weather"]
        subcommand.extend(text)
        self.config(cursor="wait")
        self.update()
        result = subprocess.run(subcommand,capture_output=True)
        self.config(cursor="")
        self.update()
        if result.returncode == 0:
            return result.stdout.decode('utf-8').split('\n')
        else:
            messagebox.showerror(f"Weather error",result.stderr)
            return []

class IndexView(ttk.Treeview):

    def __init__(self,main):
        ttk.Treeview.__init__(self)
        self.main = main
        self.bind("<Double-1>",self.download_file)
        self.bind("<Button-2>",self.show_popup)
        self.index = {}

        self.heading('#0',text="Remote archive")
        usa = self.insert('',END,text="US")
        files = self.main.command(["index"])
        states = []
        for file in files:
            state = file[0:2]
            if not state in states:
                states.append(state)
        items = {}
        for state in states:
            tag = items[state] = self.insert(usa,END,text=state)
            self.index[tag] = f"^{state}"
        for file in files:
            state = file[0:2]
            tag = self.insert(items[state],END,text=file[3:].replace("_"," ").split(".")[0])
            self.index[tag] = file

    def show_popup(self,event):
        iid = self.identify_row(event.y)
        if iid:
            self.selection_set(iid)
            popup = Menu(self,tearoff=0);
            popup.add_command(label="Download",command=self.download_file)
            popup.add_command(label="Remote settings...",command=self.edit_remote)
            try:
                popup.tk_popup(event.x_root,event.y_root,0)
            finally:
                popup.grab_release()

    def download_file(self,event=None):
        tag = self.selection()[0]
        if tag in self.index.keys():
            file = self.index[tag]
            self.main.command(["get",file])
            self.main.update()
            self.main.listview.reload()

    def edit_remote(self,event=None):
        msg = Tk()
        msg.title("Remote settings")
        info = self.main.command(["config","show"])
        table = ttk.Treeview(msg,columns=['value'],show='tree')
        for item in info:
            spec = item.split("=")
            if len(spec) > 1:
                table.insert('',END,text=spec[0],values=[spec[1].replace('"','')])
        table.column('#0',width=100)
        table.column('value',width=500,stretch=YES)
        table.grid(row=0, column=0)

class ListView(ttk.Treeview):

    def __init__(self,main):
        ttk.Treeview.__init__(self)
        self.main = main
        self.reload()
        self.bind("<Double-1>",self.show_info)
        self.bind("<Button-2>",self.show_popup)

    def show_popup(self,event):
        iid = self.identify_row(event.y)
        if iid:
            self.selection_set(iid)
            popup = Menu(self,tearoff=0);
            popup.add_command(label="Show info",command=self.show_info)
            popup.add_command(label="Copy name",command=self.copy_name)
            popup.add_separator()
            popup.add_command(label="Delete",command=self.delete_item)
            popup.add_command(label="Delete all",command=self.delete_all)
            try:
                popup.tk_popup(event.x_root,event.y_root,0)
            finally:
                popup.grab_release()

    def copy_name(self):
        tag = self.selection()[0]
        file = self.item(tag,'text')
        if file:
            self.clipboard_clear()
            self.clipboard_append(file)
            self.update()

    def delete_item(self):
        tag = self.selection()[0]
        file = self.item(tag,'text')
        if file:
            self.main.command(["delete",file])
            self.reload()

    def delete_all(self):
        ans = messagebox.askokcancel("Delete all?","Are you sure you want to delete all weather data?")
        print("ans =",ans)
        if ans == True:
            self.main.command(["delete"])
            self.reload()

    def reload(self):
        for item in self.get_children():
            self.delete(item)
        self.heading('#0',text="Local files")
        for item in self.main.command(["list"]):
            self.insert('',END,text=item)

    def show_info(self,event=None):
        tag = self.selection()[0]
        file = self.item(tag,'text')
        if file:
            info = self.main.command(["info",file])
            names = info[0].split(",")
            values = info[1].split(",")
            msg = Tk()
            msg.title(file)
            table = ttk.Treeview(msg,columns=['value'],show='tree')
            for spec in list(zip(names,values)):
                table.insert('',END,text=spec[0],values=[spec[1].replace('"','')])
            table.column('#0',width=100)
            table.column('value',width=500)
            table.grid(row=0, column=0)

if __name__ == "__main__":
    root = Weather()
    root.mainloop()
    quit(0)
