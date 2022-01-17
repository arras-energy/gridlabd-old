#!/usr/local/bin/python3

import os, sys, json, atexit, subprocess
import readline
import textwrap
import tempfile

shared = os.getenv("GLD_ETC")
if not shared:
    shared = "/usr/local/share/gridlabd/"
elif not shared[-1] == '/':
    shared += "/"

shell = os.getenv("SHELL")
if not shell:
    shell = "/bin/bash"

editor = os.getenv("EDITOR")
if not editor:
    editor = "open"

prompt = "tutorial>"

builtin = {
    "continue" : "Continue with the tutorial",
    "help" : "Get help on a tutorial command",
    "quit" : "Quit the tutorial",
    "shell" : "Run a full command shell within the tutorial",
}

class tutorial_config:
    
    current = 0
    filename = shared+"tutorial.json"

    def __init__(self,filename=None):

        self.load(filename)
        atexit.register(self.save)
    
    def __str__(self):

        return str(self.data())

    def load(self,filename=None):

        if filename:
            self.filename = filename
        if os.path.exists(self.filename):
            with open(self.filename,"r") as f:
                data = json.load(f)
                self.update(data)

    def save(self,filename=None):

        if filename:
            self.filename = filename
        try:
            with open(self.filename,"w") as f:
                json.dump(self.data(),f,indent=4)
        except Exception as err:
            print("ERROR [tutorial]:",err)
    
    def data(self):

        return dict(
            current = self.current,
            )

    def update(self,data):

        self.current = data["current"]

def show_code(source):

    for item in source:
        if item[0] == '!':
            print(f"> {os.path.basename(shell)}% {item[1:]}")
            # os.system(item[1:])
            # print("")
            result = subprocess.run(item[1:].split(),stdout=subprocess.PIPE,stderr=subprocess.STDOUT)
            print("\n".join(["> "+line.lstrip() for line in result.stdout.decode().strip().split("\n")]))
            print("")
        elif item[0] == '#':
            while True:
                try:
                    command = input(f"{prompt} ")
                except KeyboardInterrupt:
                    print("Use `quit` to exit tutorial")
                    continue
                except EOFError:
                    print("Use `quit` to exit tutorial")
                    continue
                except Exception as err:
                    print(f"ERROR: {err}")
                    return False
                if not command:
                    continue
                valid = item[1:].split(",")
                if "edit" in valid:
                    valid.extend(["vi","nano"])
                args = command.split()
                if args[0] == "edit":
                    for file in args[1:]:
                        if not os.path.exists(file):
                            print("Creating",file)
                            os.system(f"touch {file}")
                        os.system(f"'{editor}' {file} &")
                elif command == "continue":
                    return True
                elif args[0] == "help":
                    valid.extend(builtin.keys())
                    if len(args) == 2:
                        show_help(args[1],valid)
                    elif len(args) == 1:
                        print("Valid tutorial commands in this context are:")
                        for cmd in sorted(valid):
                            print(f"    {cmd}")
                    else:
                        print("Syntax: help [COMMAND]")
                elif command == "quit":
                    return False
                elif command == "shell":
                    os.system(shell)
                elif args[0] in valid:
                    os.system(command)
                else:
                    print("ERROR: invalid command")
    # raise Exception("internal error")
    return True

def show_markdown(source):

    text = [[]]
    for line in source:
        if line.startswith("#"):
            line = line.strip("# \t\n")
            text.append(f"{line}\n{'-'*len(line)}")
            text.append([])
        elif len(line) > 0:
            if line == '\n':
                if not text[-1]:
                    text.append([])
            else:
                text[-1].append(line.strip(" \t\n"))
        else:
            text.append([])
    for par in text:
        if not par:
            print("")
        elif type(par) is str:
            print(par.strip())
        else:
            print("\n".join(textwrap.wrap("".join(par),80)).strip("\n"))
            print("")

def show_raw(source):

    print("")
    print("".join(["| "+line.lstrip() for line in source]))

def show_help(source,commands):

    if source in builtin.keys():
        print(builtin[source])
    elif source in commands:
        os.system(f"man {source} 2>/dev/null || ({source} --help | less)")
    else:
        print(f"{source} is not a valid command in this context")

def run_tutorial(number = 0):

    with open(shared+f"tutorial_{number}.ipynb","r") as f:
        data = json.load(f)
        for cell in data["cells"]:
            celltype = cell["cell_type"]
            source = cell["source"]
            if celltype == "code":
                if not show_code(source):
                    break
            elif celltype == "markdown":
                show_markdown(source)
            elif celltype == "raw":
                show_raw(source)

    print(f"\nEnd of tutorial {number}")

def show_menu():

    config = tutorial_config()
    olddir = os.getcwd()
    while True:
        try:
            with open(shared+"tutorial_list.json","r") as f:
                print("\nAvailable tutorials:\n")
                dirlist = json.load(f)
        except Exception as err:
            print(f"ERROR: no tutorials found in {shared} ({err})")
            dirlist = []
        for n, file in enumerate(dirlist):
            print(f"  {n:2d}. {file}")
        try:
            command = input(f"\nChoose one: [{config.current}] ")
            if not command:
                command = config.current
            if command in ["q","quit"]:
                break
            elif int(command) >= 0:
                workdir = tempfile.TemporaryDirectory()
                os.chdir(workdir.name)
                try:
                    run_tutorial(int(command))
                except Exception as err:
                    raise
                finally:
                    os.chdir(olddir)
        except KeyboardInterrupt:
            print("Enter 'q' to quit")
        except EOFError:
            print("Enter 'q' to quit")
        except Exception as err:
            raise

def main():

    show_menu()

if __name__ == '__main__':

    main()
