# Variables
# Directory flow
INC_DIR := ./header/
SRC_DIR := ./src/
BIN_DIR := ./bin/
BUILD_DIR := ./build/

# Location of cpp files and object list
## CPP location
DCMOTOR_LIST := $(wildcard ${SRC_DIR}DCmotor/*.c)
KINEMATICS_LIST := $(wildcard ${SRC_DIR}Kinematics/*.c)
REGPID_LIST := $(wildcard ${SRC_DIR}RegPID/*.c)
SETU_LIST := $(wildcard ${SRC_DIR}SetU/*.c)
SETTV_LIST := $(wildcard ${SRC_DIR}SetTv/*.c)
## Object location
OBJ_DCMOTOR_LIST := $(addprefix $(BUILD_DIR), $(notdir $(DCMOTOR_LIST:.c=.o)))
OBJ_KINEMATICS_LIST := $(addprefix $(BUILD_DIR), $(notdir $(KINEMATICS_LIST:.c=.o)))
OBJ_REGPID_LIST := $(addprefix $(BUILD_DIR), $(notdir $(REGPID_LIST:.c=.o)))
OBJ_SETU_LIST := $(addprefix $(BUILD_DIR), $(notdir $(SETU_LIST:.c=.o)))
OBJ_SETTV_LIST := $(addprefix $(BUILD_DIR), $(notdir $(SETTV_LIST:.c=.o)))

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


all: DCmotor.exe Kinematics.exe RegPID.exe SetU.exe SetTv.exe

DCmotor.exe: $(OBJ_DCMOTOR_LIST)
	${LD} $(LDFLAGS) $(OBJ_DCMOTOR_LIST) -o $(BIN_DIR)$@ $(LIBRARY)

Kinematics.exe: $(OBJ_KINEMATICS_LIST)
	${LD} $(LDFLAGS) $(OBJ_KINEMATICS_LIST) -o $(BIN_DIR)$@ $(LIBRARY)

RegPID.exe: $(OBJ_REGPID_LIST)
	${LD} $(LDFLAGS) $(OBJ_REGPID_LIST) -o $(BIN_DIR)$@ $(LIBRARY)

SetU.exe: $(OBJ_SETU_LIST)
	${LD} $(LDFLAGS) $(OBJ_SETU_LIST) -o $(BIN_DIR)$@ $(LIBRARY)

SetTv.exe: $(OBJ_SETTV_LIST)
	${LD} $(LDFLAGS) $(OBJ_SETTV_LIST) -o $(BIN_DIR)$@ $(LIBRARY)

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