# gldcore/makefile

BIN_TARGETS += $(BIN)gridlabd
INCLUDE_DIRS += $(BUILD)gldcore

GLDCORE_SOURCES += $(wildcard $(SOURCE)gldcore/*.cpp)
GLDCORE_HEADERS += $(subst $(SOURCE)gldcore/gridlabd.h,,$(subst $(SOURCE)gldcore/gldcore.h,,$(wildcard $(SOURCE)gldcore/*.h)))
GLDCORE_OBJECTS += $(foreach SRC,$(notdir $(basename $(GLDCORE_SOURCES))),$(BUILD)gldcore/$(SRC).o)

GLDCORE_CFLAGS += $(foreach DIR,$(INCLUDE_DIRS),-I$(DIR))
GLDCORE_CFLAGS += $(PYTHONCFLAGS) 
GLDCORE_CFLAGS += $(BUILDINFO) -DMAIN_PYTHON -fPIC 

GLDCORE_LDFLAGS += $(PYTHONLDFLAGS) 
GLDCORE_LDFLAGS += -lcurl -lcurses -lc++ -lpython$(PYTHON_MAJOR).$(PYTHON_MINOR)

GLDCORE_SCRIPT_PARAMS += PREFIX=$(PREFIX) 
GLDCORE_SCRIPT_PARAMS += PYTHON_EXEC=$(PYTHON_EXEC) 

#
# Main executable
#

$(BUILD)gldcore/%.o: $(SOURCE)gldcore/%.cpp $(SOURCE)gldcore/gldcore.h $(SOURCE)gldcore/gridlabd.h
	$(INFO) "  CPP $@"
	$(ECHO)gcc $(GLDCORE_CFLAGS) -I$(<D) -c $< -o $@ 

$(BIN)gridlabd: $(SOURCE)gldcore/gridlabd $(BIN)gridlabd.bin
	$(INFO) "  TXT $@"
	$(ECHO)$(SOURCE)utilities/replace $(GLDCORE_SCRIPT_PARAMS)-i $< -o $@
	$(ECHO)chmod +x $@

$(BIN)gridlabd.bin: $(GLDCORE_OBJECTS)
	$(INFO) "  LD  $@"
	$(ECHO)gcc $(GLDCORE_LDFLAGS) $^ -o $@ 

$(ETC)%.txt: $(SOURCE)gldcore/%.txt
	$(INFO) "  TXT $@"
	$(ECHO)cp $< $@

$(SOURCE)gldcore/gridlabd.h: $(GLDCORE_HEADERS)

$(SOURCE)gldcore/gldcore.h: $(GLDCORE_HEADERS)

#
# Shared files
#

ETC_TARGETS += $(ETC)tzinfo.txt
ETC_TARGETS += $(ETC)unitfile.txt
