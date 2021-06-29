BUILD_TARGETS += $(BUILD)bin/gridlabd
BUILD_TARGETS += $(BUILD)bin/gridlabd.bin

INCLUDE_DIRS += $(BUILD)gldcore

BUILD_FOLDERS += $(BUILD)gldcore 
GLDCORE_SOURCES = $(wildcard $(SOURCE)gldcore/*.cpp)
GLDCORE_HEADERS= $(wildcard $(SOURCE)gldcore/*.h)
GLDCORE_OBJECTS = $(foreach SRC,$(notdir $(basename $(GLDCORE_SOURCES))),$(BUILD)gldcore/$(SRC).o)

$(BUILD)%.o: $(SOURCE)%.cpp $(SOURCE)gridlabd.h
	@echo "  C++ $@"
	$(ECHO)gcc -Wall -O3 -g $(foreach DIR,$(INCLUDE_DIRS),-I$(DIR)) -I$(<D) $(BUILDINFO) $(PYTHONCFLAGS) -DMAIN_PYTHON -fPIC -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(CXXFLAGS)

$(BUILD)bin/gridlabd: $(SOURCE)gldcore/gridlabd
	@echo "  CP $@"
	$(ECHO)cp $< $@
	$(ECHO)chmod +x $@

$(SOURCE)gldcore/gridlabd.in: $(SOURCE)gldcore/gridlabd.m4sh
	@echo "  M4 $@"
	$(ECHO)autom4te -l m4sh $< > $@

$(BUILD)bin/gridlabd.bin: $(GLDCORE_OBJECTS)
	@echo "  LD $@"
	$(ECHO)gcc -W -O3 -g $^ $(PYTHONLDFLAGS) -lcurl -lcurses -lc++ -lpython3.9 $(LDFLAGS) -o $@ 

$(SOURCE)gridlabd.h: $(GLDCORE_HEADERS)
