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
CND_CONF=Release
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/url-parse.o

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
LDLIBSOPTIONS=-Lgoogle-url/googleurl/src /home/hotdox/Projects/Whalebot/whalebot/statsem_string/dist/Release/GNU-Linux-x86/libstatsem_string.a -lgoogleurl

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Release.mk dist/Release/GNU-Linux-x86/url-parsing-benchmark

dist/Release/GNU-Linux-x86/url-parsing-benchmark: /home/hotdox/Projects/Whalebot/whalebot/statsem_string/dist/Release/GNU-Linux-x86/libstatsem_string.a

dist/Release/GNU-Linux-x86/url-parsing-benchmark: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-Linux-x86
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/url-parsing-benchmark ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/url-parse.o: nbproject/Makefile-${CND_CONF}.mk url-parse.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -I../whalebot/webspider -Igoogle-url -Igoogle-url/googleurl -MMD -MP -MF $@.d -o ${OBJECTDIR}/url-parse.o url-parse.cpp

# Subprojects
.build-subprojects:
	cd /home/hotdox/Projects/Whalebot/whalebot/statsem_string && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/GNU-Linux-x86/url-parsing-benchmark

# Subprojects
.clean-subprojects:
	cd /home/hotdox/Projects/Whalebot/whalebot/statsem_string && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
