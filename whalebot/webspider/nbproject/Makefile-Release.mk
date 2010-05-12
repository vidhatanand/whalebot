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
	${OBJECTDIR}/filters.o \
	${OBJECTDIR}/webspider_options.o \
	${OBJECTDIR}/LinkFactory.o \
	${OBJECTDIR}/collector_link.o \
	${OBJECTDIR}/FilenameHandler.o \
	${OBJECTDIR}/OneFetcher.o \
	${OBJECTDIR}/prefix.o \
	${OBJECTDIR}/LinkBuffer.o \
	${OBJECTDIR}/link.o \
	${OBJECTDIR}/HeaderParser.o \
	${OBJECTDIR}/webspider.o

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
LDLIBSOPTIONS=-L/home/hotdox/Projects/Whalebot/whalebot/statsem_filesystem/dist/Release/GNU-Linux-x86 -L/home/hotdox/Projects/Whalebot/whalebot/statsem_string/dist/Release/GNU-Linux-x86 -lboost_system -lhtmlcxx -lboost_filesystem -lboost_program_options -lboost_thread /home/hotdox/Projects/Whalebot/whalebot/statsem_string/dist/Release/GNU-Linux-x86/libstatsem_string.a /home/hotdox/Projects/Whalebot/whalebot/statsem_filesystem/dist/Release/GNU-Linux-x86/libstatsem_filesystem.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Release.mk dist/Release/GNU-Linux-x86/webspider

dist/Release/GNU-Linux-x86/webspider: /home/hotdox/Projects/Whalebot/whalebot/statsem_string/dist/Release/GNU-Linux-x86/libstatsem_string.a

dist/Release/GNU-Linux-x86/webspider: /home/hotdox/Projects/Whalebot/whalebot/statsem_filesystem/dist/Release/GNU-Linux-x86/libstatsem_filesystem.a

dist/Release/GNU-Linux-x86/webspider: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-Linux-x86
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/webspider ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/filters.o: nbproject/Makefile-${CND_CONF}.mk filters.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -I../include -I/usr/local/include -I/usr/include/boost-1_39 -MMD -MP -MF $@.d -o ${OBJECTDIR}/filters.o filters.cpp

${OBJECTDIR}/webspider_options.o: nbproject/Makefile-${CND_CONF}.mk webspider_options.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -I../include -I/usr/local/include -I/usr/include/boost-1_39 -MMD -MP -MF $@.d -o ${OBJECTDIR}/webspider_options.o webspider_options.cpp

${OBJECTDIR}/LinkFactory.o: nbproject/Makefile-${CND_CONF}.mk LinkFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -I../include -I/usr/local/include -I/usr/include/boost-1_39 -MMD -MP -MF $@.d -o ${OBJECTDIR}/LinkFactory.o LinkFactory.cpp

${OBJECTDIR}/collector_link.o: nbproject/Makefile-${CND_CONF}.mk collector_link.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -I../include -I/usr/local/include -I/usr/include/boost-1_39 -MMD -MP -MF $@.d -o ${OBJECTDIR}/collector_link.o collector_link.cpp

${OBJECTDIR}/FilenameHandler.o: nbproject/Makefile-${CND_CONF}.mk FilenameHandler.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -I../include -I/usr/local/include -I/usr/include/boost-1_39 -MMD -MP -MF $@.d -o ${OBJECTDIR}/FilenameHandler.o FilenameHandler.cpp

${OBJECTDIR}/OneFetcher.o: nbproject/Makefile-${CND_CONF}.mk OneFetcher.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -I../include -I/usr/local/include -I/usr/include/boost-1_39 -MMD -MP -MF $@.d -o ${OBJECTDIR}/OneFetcher.o OneFetcher.cpp

${OBJECTDIR}/prefix.o: nbproject/Makefile-${CND_CONF}.mk prefix.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -I../include -I/usr/local/include -I/usr/include/boost-1_39 -MMD -MP -MF $@.d -o ${OBJECTDIR}/prefix.o prefix.cpp

${OBJECTDIR}/LinkBuffer.o: nbproject/Makefile-${CND_CONF}.mk LinkBuffer.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -I../include -I/usr/local/include -I/usr/include/boost-1_39 -MMD -MP -MF $@.d -o ${OBJECTDIR}/LinkBuffer.o LinkBuffer.cpp

${OBJECTDIR}/link.o: nbproject/Makefile-${CND_CONF}.mk link.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -I../include -I/usr/local/include -I/usr/include/boost-1_39 -MMD -MP -MF $@.d -o ${OBJECTDIR}/link.o link.cpp

${OBJECTDIR}/HeaderParser.o: nbproject/Makefile-${CND_CONF}.mk HeaderParser.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -I../include -I/usr/local/include -I/usr/include/boost-1_39 -MMD -MP -MF $@.d -o ${OBJECTDIR}/HeaderParser.o HeaderParser.cpp

${OBJECTDIR}/webspider.o: nbproject/Makefile-${CND_CONF}.mk webspider.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -O2 -Wall -I../include -I/usr/local/include -I/usr/include/boost-1_39 -MMD -MP -MF $@.d -o ${OBJECTDIR}/webspider.o webspider.cpp

# Subprojects
.build-subprojects:
	cd /home/hotdox/Projects/Whalebot/whalebot/statsem_string && ${MAKE}  -f Makefile CONF=Release
	cd /home/hotdox/Projects/Whalebot/whalebot/statsem_filesystem && ${MAKE}  -f Makefile CONF=Release
	cd /home/hotdox/Projects/Whalebot/whalebot/statsem_string && ${MAKE}  -f Makefile CONF=Release
	cd /home/hotdox/Projects/Whalebot/whalebot/statsem_filesystem && ${MAKE}  -f Makefile CONF=Release

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/GNU-Linux-x86/webspider

# Subprojects
.clean-subprojects:
	cd /home/hotdox/Projects/Whalebot/whalebot/statsem_string && ${MAKE}  -f Makefile CONF=Release clean
	cd /home/hotdox/Projects/Whalebot/whalebot/statsem_filesystem && ${MAKE}  -f Makefile CONF=Release clean
	cd /home/hotdox/Projects/Whalebot/whalebot/statsem_string && ${MAKE}  -f Makefile CONF=Release clean
	cd /home/hotdox/Projects/Whalebot/whalebot/statsem_filesystem && ${MAKE}  -f Makefile CONF=Release clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
