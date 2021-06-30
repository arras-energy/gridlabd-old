# gldcore/solver/makefile

LIB_TARGETS += $(LIB)glsolvers.so

GLDCORE_SOLVERS_SOURCES += $(wildcard $(SOURCE)gldcore/solvers/*.cpp)
GLDCORE_SOLVERS_HEADERS += 
GLDCORE_SOLVERS_OBJECTS += $(foreach SRC,$(notdir $(basename $(GLDCORE_SOLVER_SOURCES))),$(GLDCORE_SOLVER_BUILD)$(SRC).o)

GLDCORE_SOLVERS_CFLAGS +=

GLDCORE_SOLVERS_ARFLAGS += 

GLDCORE_SOLVERS_SOFLAGS += 


$(GLDCORE_SOLVER_BUILD)%.o: $(GLDCORE_SOLVER_SOURCES)%.cpp 
	$(INFO) "  CPP $@"
	$(ECHO)gcc $(GLDCORE_SOLVER_CFLAGS) -I$(<D) -c $< -o $@

$(LIB)glsolvers.la: $(GLDCORE_SOLVER_OBJECTS)
	$(INFO) "  AR  $@"
	$(ECHO)gcc $(GLDCORE_SOLVER_ARFLAGS) $^ -o $@ 

$(LIB)glsolvers.so: $(LIB)glsolvers.la
	$(INFO) "  SO  $@"
	$(ECHO)libtool $(GLDCORE_SOLVERS_SOFLAGS) $^ -o $@ 
