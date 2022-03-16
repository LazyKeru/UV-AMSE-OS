# Variables
# Directory flow
INC_DIR := ./header/
SRC_DIR := ./src/
BIN_DIR := ./bin/
BUILD_DIR := ./build/

# Location of cpp files and object list
## CPP location
DCMOTOR_LIST := $(wildcard ${SRC_DIR}DCmotor/*.c)
## Object location
OBJ_DCMOTOR_LIST := $(addprefix $(BUILD_DIR), $(notdir $(DCMOTOR_LIST:.c=.o)))

# Looking at the OS to define or RM function 
ifdef OS
   RM = del /Q *.o *.exe
else
   ifeq ($(shell uname), Linux)
      RM = rm -f *.o *.exe 
   endif
endif

# Variables for tools
CXXFLAGS:= -Wall
LDFLAGS:= -Wall
LIBRARY:= -lrt -lm
INCLUDES:= -I${INC_DIR}
CXX := gcc
LD := gcc

DCmotor: $(OBJ_DCMOTOR_LIST)
	${LD} $(LDFLAGS) $(OBJ_DCMOTOR_LIST) -o $(BIN_DIR)$@ $(LIBRARY)

# Compile the c files
${BUILD_DIR}%.o: ${SRC_DIR}*/%.c
	$(CXX) $(CXXFLAGS) -c $< -o $@


SUBDIRS:= ${BUILD_DIR} ${BIN_DIR}
SUBDIRSCLEAN:=$(addsuffix clean,$(SUBDIRS))

clean: $(SUBDIRSCLEAN)

clean_curdir:
	$(RM)

%clean: %
	$(MAKE) -C $< -f ../Makefile clean_curdir