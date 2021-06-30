# gldcore/solver/makefile

BUILD_FOLDERS += $(BUILD)gldcore/solvers 

INCLUDE_DIRS += $(BUILD)gldcore/solvers

GLDCORE_SOLVERS_SOURCES += $(wildcard $(SOURCE)gldcore/solvers/*.cpp)
GLDCORE_SOLVERS_HEADERS += 
GLDCORE_SOLVERS_OBJECTS += $(foreach SRC,$(notdir $(basename $(GLDCORE_SOURCES))),$(BUILD)gldcore/$(SRC).o)

GLDCORE_SOLVERS_CFLAGS +=

GLDCORE_SOLVERS_ARFLAGS += 

GLDCORE_SOLVERS_SOFLAGS += 

#
# Main executable
#

LIB_TARGETS += $(LIB)glsolvers.so

$(BUILD)gldcore/solvers/%.o: $(SOURCE)gldcore/solver/%.cpp 
	$(INFO) "  CPP $@"
	$(ECHO)gcc $(GLDCORE_SOLVER_CFLAGS) -I$(<D) -c $< -o $@

$(LIB)glsolvers.la: $(GLDCORE_SOLVER_OBJECTS)
	$(INFO) "  AR  $@"
	$(ECHO)gcc $(GLDCORE_SOLVER_ARFLAGS) $^ -o $@ 

$(LIB)glsolvers.so: $(LIB)glsolvers.la
	$(INFO) "  SO  $@"
	$(ECHO)libtool $(GLDCORE_SOLVERS_SOFLAGS) $^ -o $@ 
