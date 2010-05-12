#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/optionsbase.o \
	${OBJECTDIR}/parserbase.o \
	${OBJECTDIR}/optionsparser.o \
	${OBJECTDIR}/streamlogger.o \
	${OBJECTDIR}/ToString.o

# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/libstatsem_string.a

dist/Debug/GNU-Linux-x86/libstatsem_string.a: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${RM} dist/Debug/GNU-Linux-x86/libstatsem_string.a
	${AR} rv ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/libstatsem_string.a ${OBJECTFILES} 
	$(RANLIB) dist/Debug/GNU-Linux-x86/libstatsem_string.a

${OBJECTDIR}/optionsbase.o: nbproject/Makefile-${CND_CONF}.mk optionsbase.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/optionsbase.o optionsbase.cpp

${OBJECTDIR}/parserbase.o: nbproject/Makefile-${CND_CONF}.mk parserbase.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/parserbase.o parserbase.cpp

${OBJECTDIR}/optionsparser.o: nbproject/Makefile-${CND_CONF}.mk optionsparser.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/optionsparser.o optionsparser.cpp

${OBJECTDIR}/streamlogger.o: nbproject/Makefile-${CND_CONF}.mk streamlogger.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/streamlogger.o streamlogger.cpp

${OBJECTDIR}/ToString.o: nbproject/Makefile-${CND_CONF}.mk ToString.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -Wall -MMD -MP -MF $@.d -o ${OBJECTDIR}/ToString.o ToString.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/libstatsem_string.a

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
