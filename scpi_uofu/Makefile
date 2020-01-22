
# Populate the source files required for this test.
C_SRC_FILES            = scpi.c

CPP_SRC_FILES          =  



# The content below this point probably does not need to be modified.


BASEDIR              = $(CURDIR)
SRCDIR               = $(BASEDIR)/src
INCLUDEDIR           = $(BASEDIR)/inc
TESTDIR	             = $(BASEDIR)
CATCHDIR             = $(BASEDIR)/catch

BINDIR               = bin
BUILDDIR             = build

PROJDIR              = $(notdir $(CURDIR))
PROJECT              = test_$(PROJDIR)
TARGET               = $(BINDIR)/$(PROJECT)

prefix               = 
INCLUDES             = -I$(INCLUDEDIR) -I$(TESTDIR)

# File definitions 

CATCH_MAIN           = catch_main
SOURCES              = $(CPP_SRC_FILES) $(PROJECT).cpp $(CATCH_MAIN).cpp

CSOURCES			 =  $(C_SRC_FILES) 
						
CPPOBJECTS           = $(SOURCES:%.cpp=$(BUILDDIR)/%.o)
COBJECTS             = $(CSOURCES:%.c=$(BUILDDIR)/%.o)

OBJECTS				 = $(CPPOBJECTS) \
					   $(COBJECTS) 

# Tools
RM                   = rm -r -f
CC                   = g++
LINKER               = g++
COVERAGE             = gcov

# Compiler Flags
WFLAGS               = -std=c++11 -Wall -Wno-switch $(INCLUDES)
CFLAGS               = -fprofile-arcs -ftest-coverage
OPT_FLAGS            = 
LDFLAGS              = -lgcov --coverage

# Primary build rule for basic target
all:	$(TARGET)
		@echo "Running $(TARGET) test suite..."
		@$(TARGET)


debug:
		$(MAKE) $(MAKEFILE) DEBUGFLAG="-g -D DEBUG"
		@echo "$(TARGET) Make Debug Complete"

# build all of the unit-tests
test:	$(TARGET)
		@echo "Running $(TARGET) test suite..."
		@$(TARGET)
		@$(COVERAGE) $(SOURCES)


clean:
		@-$(RM) $(BINDIR)
		@-$(RM) $(BUILDDIR)
		@echo "SCPI - $(PROJECT) Clean Complete"


.PHONY: release


release:


FORCE:



# Linking rules for target
$(TARGET): $(OBJECTS)
		@mkdir -p $(BINDIR)
		@$(LINKER) -o $(TARGET) $(OBJECTS) $(LDFLAGS)
		@echo "$(TARGET) Build Complete"

# Rule to build objects
$(BUILDDIR)/$(CATCH_MAIN).o : $(CATCHDIR)/$(CATCH_MAIN).cpp
		$(CC) -c $(WFLAGS) $(CFLAGS) $(OPT_FLAGS) $(DEBUGFLAG) $< -o $(@)

# Rule to build objects
$(BUILDDIR)/$(PROJECT).o : $(CURDIR)/$(PROJECT).cpp
		$(CC) -c $(WFLAGS) $(CFLAGS) $(OPT_FLAGS) $(DEBUGFLAG) $< -o $(@)

# Rule to build objects
$(BUILDDIR)/%.o : $(SRCDIR)/%.cpp
		@mkdir -p $(dir $(@))
		@echo "compiling: $(@)"
		$(CC) -c $(WFLAGS) $(CFLAGS) $(OPT_FLAGS) $(DEBUGFLAG) $< -o $(@)
		@echo "processed: $(@)"

# Rule to build objects
$(BUILDDIR)/%.o : $(SRCDIR)/%.c
		@mkdir -p $(dir $(@))
		@echo "compiling: $(@)"
		$(CC) -c $(WFLAGS) $(CFLAGS) $(OPT_FLAGS) $(DEBUGFLAG) $< -o $(@)
		@echo "processed: $(@)"

