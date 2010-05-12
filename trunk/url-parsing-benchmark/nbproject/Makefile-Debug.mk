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
	${OBJECTDIR}/_ext/_DOTDOT/whalebot/webspider/LinkFactory.o \
	${OBJECTDIR}/log_message.o \
	${OBJECTDIR}/_ext/_DOTDOT/whalebot/webspider/link.o \
	${OBJECTDIR}/google-url/googleurl/base/string16.o \
	${OBJECTDIR}/_ext/_DOTDOT/whalebot/webspider/prefix.o \
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
LDLIBSOPTIONS=-Lgoogle-url/googleurl/src /home/hotdox/Projects/Whalebot/whalebot/statsem_string/dist/Debug/GNU-Linux-x86/libstatsem_string.a -lpthread -licuuc google-url/googleurl/src/libgoogleurl.a

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	${MAKE}  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/url-parsing-benchmark

dist/Debug/GNU-Linux-x86/url-parsing-benchmark: /home/hotdox/Projects/Whalebot/whalebot/statsem_string/dist/Debug/GNU-Linux-x86/libstatsem_string.a

dist/Debug/GNU-Linux-x86/url-parsing-benchmark: google-url/googleurl/src/libgoogleurl.a

dist/Debug/GNU-Linux-x86/url-parsing-benchmark: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/url-parsing-benchmark ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/_DOTDOT/whalebot/webspider/LinkFactory.o: nbproject/Makefile-${CND_CONF}.mk ../whalebot/webspider/LinkFactory.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/whalebot/webspider
	${RM} $@.d
	$(COMPILE.cc) -g -I../whalebot/webspider -Igoogle-url -Igoogle-url/googleurl -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/whalebot/webspider/LinkFactory.o ../whalebot/webspider/LinkFactory.cpp

${OBJECTDIR}/log_message.o: nbproject/Makefile-${CND_CONF}.mk log_message.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../whalebot/webspider -Igoogle-url -Igoogle-url/googleurl -MMD -MP -MF $@.d -o ${OBJECTDIR}/log_message.o log_message.cpp

${OBJECTDIR}/_ext/_DOTDOT/whalebot/webspider/link.o: nbproject/Makefile-${CND_CONF}.mk ../whalebot/webspider/link.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/whalebot/webspider
	${RM} $@.d
	$(COMPILE.cc) -g -I../whalebot/webspider -Igoogle-url -Igoogle-url/googleurl -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/whalebot/webspider/link.o ../whalebot/webspider/link.cpp

${OBJECTDIR}/google-url/googleurl/base/string16.o: nbproject/Makefile-${CND_CONF}.mk google-url/googleurl/base/string16.cc 
	${MKDIR} -p ${OBJECTDIR}/google-url/googleurl/base
	${RM} $@.d
	$(COMPILE.cc) -g -I../whalebot/webspider -Igoogle-url -Igoogle-url/googleurl -MMD -MP -MF $@.d -o ${OBJECTDIR}/google-url/googleurl/base/string16.o google-url/googleurl/base/string16.cc

${OBJECTDIR}/_ext/_DOTDOT/whalebot/webspider/prefix.o: nbproject/Makefile-${CND_CONF}.mk ../whalebot/webspider/prefix.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/_DOTDOT/whalebot/webspider
	${RM} $@.d
	$(COMPILE.cc) -g -I../whalebot/webspider -Igoogle-url -Igoogle-url/googleurl -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/_DOTDOT/whalebot/webspider/prefix.o ../whalebot/webspider/prefix.cpp

${OBJECTDIR}/url-parse.o: nbproject/Makefile-${CND_CONF}.mk url-parse.cpp 
	${MKDIR} -p ${OBJECTDIR}
	${RM} $@.d
	$(COMPILE.cc) -g -I../whalebot/webspider -Igoogle-url -Igoogle-url/googleurl -MMD -MP -MF $@.d -o ${OBJECTDIR}/url-parse.o url-parse.cpp

# Subprojects
.build-subprojects:
	cd /home/hotdox/Projects/Whalebot/whalebot/statsem_string && ${MAKE}  -f Makefile CONF=Debug

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/url-parsing-benchmark

# Subprojects
.clean-subprojects:
	cd /home/hotdox/Projects/Whalebot/whalebot/statsem_string && ${MAKE}  -f Makefile CONF=Debug clean

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
