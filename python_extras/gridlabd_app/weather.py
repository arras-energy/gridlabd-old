"""Copyright (C) 2021 Regents of the Leland Stanford Junior University
See https://www.gridlabd.us/ for license, acknowledgments, credits, manuals, documentation, and tutorials.

"""
import sys, os
import timeit
from tkinter import *
from tkinter import Menu, messagebox, filedialog, simpledialog, ttk
import subprocess

def command(text):
    subcommand = ["gridlabd","weather"]
    subcommand.extend(text)
    result = subprocess.run(subcommand,capture_output=True)
    if result.returncode == 0:
        return result.stdout.decode('utf-8').split('\n')
    else:
        messagebox.showerror(f"Weather error",result.stderr)
        return []

dialog_width = 420
dialog_height = 400

class Weather(Tk):

    def __init__(self):
        Tk.__init__(self)

        self.configure()

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
        self.indexview.pack(padx=5, pady=10, side=LEFT, fill=Y)

        self.listview = ListView(self)
        self.listview.pack(padx=5, pady=10, side=LEFT, fill=Y)


class IndexView(ttk.Treeview):

    def __init__(self,main):
        ttk.Treeview.__init__(self)
        self.main = main
        self.bind("<Double-1>",self.on_select)
        self.index = {}

        self.heading('#0',text="Remote files")
        usa = self.insert('',END,text="US")
        files = command(["index"])
        states = []
        for file in files:
            state = file[0:2]
            if not state in states:
                states.append(state)
        items = {}
        for state in states:
            items[state] = self.insert(usa,END,text=state)
        for file in files:
            state = file[0:2]
            tag = self.insert(items[state],END,text=file[3:].replace("_"," ").split(".")[0])
            self.index[tag] = file

    def on_select(self,event):
        tag = self.selection()[0]
        file = self.index[tag]
        if file:
            command(["get",file])
            self.main.update()
        else:
            print(self.selection)

class ListView(ttk.Treeview):

    def __init__(self,main):
        ttk.Treeview.__init__(self)
        self.main = main

        self.heading('#0',text="Local files")
        for item in command(["list"]):
            self.insert('',END,text=item)


if __name__ == "__main__":
    root = Weather()
    root.mainloop()
