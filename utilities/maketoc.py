import os, sys

if len(sys.argv) == 1:
	print(f"Syntax: {os.path.splitext(os.path.basename(sys.argv[0]))[0]} DOCPATH",file=sys.stderr)
	exit(1)

print("#","Table of Contents","\n")

PATH = sys.argv[1].replace("docs","")

# process chapters
CHAPTERS = [ x for x in os.listdir(sys.argv[1]) if len(x.split(' - ')) > 1 ]
CHAPTERS.sort()
for CHAPTER in CHAPTERS:

	print("##",CHAPTER,"\n")
	chapter = CHAPTER.split()[0]

	# process sections
	SECTIONS = [ x for x in os.listdir(sys.argv[1]+"/"+CHAPTER) if len(x.split(" - ")) > 1]
	SECTIONS.sort()
	for SECTION in SECTIONS:
		spec = SECTION.split(" - ")
		section = spec[0]
		name = spec[1]
		print("  ",f"{chapter}.[[{PATH}{CHAPTER}/{section} - {name.replace('.md','~')}]]","\n")
