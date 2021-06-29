# gldcore/makefile

BUILD_FOLDERS += $(BUILD)gldcore 

INCLUDE_DIRS += $(BUILD)gldcore

GLDCORE_SOURCES = $(wildcard $(SOURCE)gldcore/*.cpp)
GLDCORE_HEADERS = $(subst $(SOURCE)gldcore/gridlabd.h,,$(subst $(SOURCE)gldcore/gldcore.h,,$(wildcard $(SOURCE)gldcore/*.h)))
GLDCORE_OBJECTS = $(foreach SRC,$(notdir $(basename $(GLDCORE_SOURCES))),$(BUILD)gldcore/$(SRC).o)

BIN_TARGETS += $(BIN)gridlabd

ETC_TARGETS += $(ETC)tzinfo.txt
ETC_TARGETS += $(ETC)unitfile.txt

$(BUILD)gldcore/%.o: $(SOURCE)gldcore/%.cpp $(SOURCE)gldcore/gldcore.h $(SOURCE)gldcore/gridlabd.h
	@echo "  C++ $@"
	$(ECHO)gcc $(CFLAGS) $(foreach DIR,$(INCLUDE_DIRS),-I$(DIR)) -I$(<D) $(BUILDINFO) $(PYTHONCFLAGS) -DMAIN_PYTHON -fPIC -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS)

$(BIN)gridlabd: $(SOURCE)gldcore/gridlabd $(BIN)gridlabd.bin
	@echo "  REP $@"
	$(ECHO)$(SOURCE)utilities/replace PREFIX=$(PREFIX) PYTHON_EXEC=$(PYTHON_EXEC) -i $< -o $@
	$(ECHO)chmod +x $@

$(BIN)gridlabd.bin: $(GLDCORE_OBJECTS)
	@echo "  LD  $@"
	$(ECHO)gcc $(LDFLAGS) $^ $(PYTHONLDFLAGS) -lcurl -lcurses -lc++ -lpython$(PYTHON_MAJOR).$(PYTHON_MINOR) $(LDFLAGS) -o $@ 

$(ETC)%.txt: $(SOURCE)gldcore/%.txt
	@echo "  CP  $@"
	$(ECHO)cp $< $@

$(SOURCE)gldcore/gridlabd.h: $(GLDCORE_HEADERS)

$(SOURCE)gldcore/gldcore.h: $(GLDCORE_HEADERS)
