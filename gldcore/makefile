# gldcore/makefile

BIN_TARGETS += $(BIN)gridlabd

GLDCORE_SOURCES += $(wildcard $(GLDCORE_SOURCE)*.cpp)
GLDCORE_INCLUDE += $(GLDCORE_SOURCE)
GLDCORE_OBJECTS += $(foreach SRC,$(notdir $(basename $(GLDCORE_SOURCES))),$(GLDCORE_BUILD)$(SRC).o)

GLDCORE_CFLAGS += $(foreach DIR,$(GLDCORE_INCLUDE),-I$(DIR))
GLDCORE_CFLAGS += $(PYTHONCFLAGS) 
GLDCORE_CFLAGS += $(BUILDINFO) -DMAIN_PYTHON -fPIC 

GLDCORE_LDFLAGS += $(PYTHONLDFLAGS) 
GLDCORE_LDFLAGS += -lcurl -lcurses -lc++ -lpython$(PYTHON_MAJOR).$(PYTHON_MINOR)

GLDCORE_SCRIPT_PARAMS += PREFIX=$(PREFIX) 
GLDCORE_SCRIPT_PARAMS += PYTHON_EXEC=$(PYTHON_EXEC) 

CLEAN_TARGETS += $(BIN)gridlabd.bin

#
# Main executable
#

$(GLDCORE_BUILD)%.o: $(GLDCORE_DEPENDS)%.cpp
	$(INFO) "  C++ $@"
	$(ECHO)$(CXX) $(GLDCORE_CFLAGS) -I$(<D) -c $< -o $@ 

$(BIN)gridlabd: $(GLDCORE_SOURCE)gridlabd $(BIN)gridlabd.bin
	$(INFO) "  TXT $@"
	$(ECHO)$(SOURCE)utilities/replace $(GLDCORE_SCRIPT_PARAMS)-i $< -o $@
	$(ECHO)chmod +x $@

$(BIN)gridlabd.bin: $(GLDCORE_OBJECTS)
	$(INFO) "  LD  $@"
	$(ECHO)$(CXX) $(GLDCORE_LDFLAGS) $^ -o $@ 

$(ETC)%.txt: $(GLDCORE_SOURCE)%.txt
	$(INFO) "  TXT $@"
	$(ECHO)cp $< $@

#
# Shared files
#

ETC_TARGETS += $(ETC)tzinfo.txt
ETC_TARGETS += $(ETC)unitfile.txt
