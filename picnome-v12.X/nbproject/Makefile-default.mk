#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
include Makefile

# Environment
MKDIR=mkdir -p
RM=rm -f 
CP=cp 
# Macros
CND_CONF=default

ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/picnome-v12.X.${IMAGE_TYPE}.elf
else
IMAGE_TYPE=production
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/picnome-v12.X.${IMAGE_TYPE}.elf
endif
# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}
# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/PIC24\ HID\ Bootloader\ Remapping.o ${OBJECTDIR}/_ext/1472/usb_descriptors.o ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o ${OBJECTDIR}/_ext/1472/picnome.o ${OBJECTDIR}/_ext/1360907413/usb_device.o


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

OS_ORIGINAL="Darwin"
OS_CURRENT="$(shell uname -s)"
############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
MP_CC=/Applications/microchip/mplabc30/v3.24/bin/pic30-gcc
# MP_BC is not defined
MP_AS=/Applications/microchip/mplabc30/v3.24/bin/pic30-as
MP_LD=/Applications/microchip/mplabc30/v3.24/bin/pic30-ld
MP_AR=/Applications/microchip/mplabc30/v3.24/bin/pic30-ar
# MP_BC is not defined
MP_CC_DIR=/Applications/microchip/mplabc30/v3.24/bin
# MP_BC_DIR is not defined
MP_AS_DIR=/Applications/microchip/mplabc30/v3.24/bin
MP_LD_DIR=/Applications/microchip/mplabc30/v3.24/bin
MP_AR_DIR=/Applications/microchip/mplabc30/v3.24/bin
# MP_BC_DIR is not defined
.build-conf: ${BUILD_SUBPROJECTS}
ifneq ($(OS_CURRENT),$(OS_ORIGINAL))
	@echo "***** WARNING: This make file contains OS dependent code. The OS this makefile is being run is different from the OS it was created in."
endif
	${MAKE}  -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/picnome-v12.X.${IMAGE_TYPE}.elf

# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
.PHONY: ${OBJECTDIR}/_ext/1472/PIC24\ HID\ Bootloader\ Remapping.o
${OBJECTDIR}/_ext/1472/PIC24\ HID\ Bootloader\ Remapping.o: ../PIC24\ HID\ Bootloader\ Remapping.s  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/1472 
	${MP_CC}  -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -c -mcpu=24FJ64GB002  -o ${OBJECTDIR}/_ext/1472/PIC24\ HID\ Bootloader\ Remapping.o ../PIC24\ HID\ Bootloader\ Remapping.s  -Wa,--defsym=__MPLAB_BUILD=1,--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,-g,-I".."
else
.PHONY: ${OBJECTDIR}/_ext/1472/PIC24\ HID\ Bootloader\ Remapping.o
${OBJECTDIR}/_ext/1472/PIC24\ HID\ Bootloader\ Remapping.o: ../PIC24\ HID\ Bootloader\ Remapping.s  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/1472 
	${MP_CC}  -omf=elf -c -mcpu=24FJ64GB002  -o ${OBJECTDIR}/_ext/1472/PIC24\ HID\ Bootloader\ Remapping.o ../PIC24\ HID\ Bootloader\ Remapping.s  -Wa,--defsym=__MPLAB_BUILD=1,-I".."
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/usb_descriptors.o: ../usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/1472 
	${RM} ${OBJECTDIR}/_ext/1472/usb_descriptors.o.d 
	${MP_CC} -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=24FJ64GB002 -Wall -I".." -I"../USB" -I"../Include/USB" -I"../Include" -MMD -MF ${OBJECTDIR}/_ext/1472/usb_descriptors.o.d -o ${OBJECTDIR}/_ext/1472/usb_descriptors.o ../usb_descriptors.c  
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	@sed -e 's/\"//g' -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/1472/usb_descriptors.o.d > ${OBJECTDIR}/_ext/1472/usb_descriptors.o.tmp
	${RM} ${OBJECTDIR}/_ext/1472/usb_descriptors.o.d 
	${CP} ${OBJECTDIR}/_ext/1472/usb_descriptors.o.tmp ${OBJECTDIR}/_ext/1472/usb_descriptors.o.d 
	${RM} ${OBJECTDIR}/_ext/1472/usb_descriptors.o.tmp}
else 
	@sed -e 's/\"//g' ${OBJECTDIR}/_ext/1472/usb_descriptors.o.d > ${OBJECTDIR}/_ext/1472/usb_descriptors.o.tmp
	${RM} ${OBJECTDIR}/_ext/1472/usb_descriptors.o.d 
	${CP} ${OBJECTDIR}/_ext/1472/usb_descriptors.o.tmp ${OBJECTDIR}/_ext/1472/usb_descriptors.o.d 
	${RM} ${OBJECTDIR}/_ext/1472/usb_descriptors.o.tmp}
endif
${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o: ../USB/usb_function_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/1360907413 
	${RM} ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.d 
	${MP_CC} -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=24FJ64GB002 -Wall -I".." -I"../USB" -I"../Include/USB" -I"../Include" -MMD -MF ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.d -o ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o ../USB/usb_function_cdc.c  
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	@sed -e 's/\"//g' -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.d > ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.tmp
	${RM} ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.d 
	${CP} ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.tmp ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.d 
	${RM} ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.tmp}
else 
	@sed -e 's/\"//g' ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.d > ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.tmp
	${RM} ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.d 
	${CP} ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.tmp ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.d 
	${RM} ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.tmp}
endif
${OBJECTDIR}/_ext/1472/picnome.o: ../picnome.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/1472 
	${RM} ${OBJECTDIR}/_ext/1472/picnome.o.d 
	${MP_CC} -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=24FJ64GB002 -Wall -I".." -I"../USB" -I"../Include/USB" -I"../Include" -MMD -MF ${OBJECTDIR}/_ext/1472/picnome.o.d -o ${OBJECTDIR}/_ext/1472/picnome.o ../picnome.c  
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	@sed -e 's/\"//g' -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/1472/picnome.o.d > ${OBJECTDIR}/_ext/1472/picnome.o.tmp
	${RM} ${OBJECTDIR}/_ext/1472/picnome.o.d 
	${CP} ${OBJECTDIR}/_ext/1472/picnome.o.tmp ${OBJECTDIR}/_ext/1472/picnome.o.d 
	${RM} ${OBJECTDIR}/_ext/1472/picnome.o.tmp}
else 
	@sed -e 's/\"//g' ${OBJECTDIR}/_ext/1472/picnome.o.d > ${OBJECTDIR}/_ext/1472/picnome.o.tmp
	${RM} ${OBJECTDIR}/_ext/1472/picnome.o.d 
	${CP} ${OBJECTDIR}/_ext/1472/picnome.o.tmp ${OBJECTDIR}/_ext/1472/picnome.o.d 
	${RM} ${OBJECTDIR}/_ext/1472/picnome.o.tmp}
endif
${OBJECTDIR}/_ext/1360907413/usb_device.o: ../USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/1360907413 
	${RM} ${OBJECTDIR}/_ext/1360907413/usb_device.o.d 
	${MP_CC} -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -omf=elf -x c -c -mcpu=24FJ64GB002 -Wall -I".." -I"../USB" -I"../Include/USB" -I"../Include" -MMD -MF ${OBJECTDIR}/_ext/1360907413/usb_device.o.d -o ${OBJECTDIR}/_ext/1360907413/usb_device.o ../USB/usb_device.c  
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	@sed -e 's/\"//g' -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/1360907413/usb_device.o.d > ${OBJECTDIR}/_ext/1360907413/usb_device.o.tmp
	${RM} ${OBJECTDIR}/_ext/1360907413/usb_device.o.d 
	${CP} ${OBJECTDIR}/_ext/1360907413/usb_device.o.tmp ${OBJECTDIR}/_ext/1360907413/usb_device.o.d 
	${RM} ${OBJECTDIR}/_ext/1360907413/usb_device.o.tmp}
else 
	@sed -e 's/\"//g' ${OBJECTDIR}/_ext/1360907413/usb_device.o.d > ${OBJECTDIR}/_ext/1360907413/usb_device.o.tmp
	${RM} ${OBJECTDIR}/_ext/1360907413/usb_device.o.d 
	${CP} ${OBJECTDIR}/_ext/1360907413/usb_device.o.tmp ${OBJECTDIR}/_ext/1360907413/usb_device.o.d 
	${RM} ${OBJECTDIR}/_ext/1360907413/usb_device.o.tmp}
endif
else
${OBJECTDIR}/_ext/1472/usb_descriptors.o: ../usb_descriptors.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/1472 
	${RM} ${OBJECTDIR}/_ext/1472/usb_descriptors.o.d 
	${MP_CC}  -omf=elf -x c -c -mcpu=24FJ64GB002 -Wall -I".." -I"../USB" -I"../Include/USB" -I"../Include" -MMD -MF ${OBJECTDIR}/_ext/1472/usb_descriptors.o.d -o ${OBJECTDIR}/_ext/1472/usb_descriptors.o ../usb_descriptors.c  
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	@sed -e 's/\"//g' -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/1472/usb_descriptors.o.d > ${OBJECTDIR}/_ext/1472/usb_descriptors.o.tmp
	${RM} ${OBJECTDIR}/_ext/1472/usb_descriptors.o.d 
	${CP} ${OBJECTDIR}/_ext/1472/usb_descriptors.o.tmp ${OBJECTDIR}/_ext/1472/usb_descriptors.o.d 
	${RM} ${OBJECTDIR}/_ext/1472/usb_descriptors.o.tmp}
else 
	@sed -e 's/\"//g' ${OBJECTDIR}/_ext/1472/usb_descriptors.o.d > ${OBJECTDIR}/_ext/1472/usb_descriptors.o.tmp
	${RM} ${OBJECTDIR}/_ext/1472/usb_descriptors.o.d 
	${CP} ${OBJECTDIR}/_ext/1472/usb_descriptors.o.tmp ${OBJECTDIR}/_ext/1472/usb_descriptors.o.d 
	${RM} ${OBJECTDIR}/_ext/1472/usb_descriptors.o.tmp}
endif
${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o: ../USB/usb_function_cdc.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/1360907413 
	${RM} ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.d 
	${MP_CC}  -omf=elf -x c -c -mcpu=24FJ64GB002 -Wall -I".." -I"../USB" -I"../Include/USB" -I"../Include" -MMD -MF ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.d -o ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o ../USB/usb_function_cdc.c  
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	@sed -e 's/\"//g' -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.d > ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.tmp
	${RM} ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.d 
	${CP} ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.tmp ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.d 
	${RM} ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.tmp}
else 
	@sed -e 's/\"//g' ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.d > ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.tmp
	${RM} ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.d 
	${CP} ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.tmp ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.d 
	${RM} ${OBJECTDIR}/_ext/1360907413/usb_function_cdc.o.tmp}
endif
${OBJECTDIR}/_ext/1472/picnome.o: ../picnome.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/1472 
	${RM} ${OBJECTDIR}/_ext/1472/picnome.o.d 
	${MP_CC}  -omf=elf -x c -c -mcpu=24FJ64GB002 -Wall -I".." -I"../USB" -I"../Include/USB" -I"../Include" -MMD -MF ${OBJECTDIR}/_ext/1472/picnome.o.d -o ${OBJECTDIR}/_ext/1472/picnome.o ../picnome.c  
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	@sed -e 's/\"//g' -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/1472/picnome.o.d > ${OBJECTDIR}/_ext/1472/picnome.o.tmp
	${RM} ${OBJECTDIR}/_ext/1472/picnome.o.d 
	${CP} ${OBJECTDIR}/_ext/1472/picnome.o.tmp ${OBJECTDIR}/_ext/1472/picnome.o.d 
	${RM} ${OBJECTDIR}/_ext/1472/picnome.o.tmp}
else 
	@sed -e 's/\"//g' ${OBJECTDIR}/_ext/1472/picnome.o.d > ${OBJECTDIR}/_ext/1472/picnome.o.tmp
	${RM} ${OBJECTDIR}/_ext/1472/picnome.o.d 
	${CP} ${OBJECTDIR}/_ext/1472/picnome.o.tmp ${OBJECTDIR}/_ext/1472/picnome.o.d 
	${RM} ${OBJECTDIR}/_ext/1472/picnome.o.tmp}
endif
${OBJECTDIR}/_ext/1360907413/usb_device.o: ../USB/usb_device.c  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} ${OBJECTDIR}/_ext/1360907413 
	${RM} ${OBJECTDIR}/_ext/1360907413/usb_device.o.d 
	${MP_CC}  -omf=elf -x c -c -mcpu=24FJ64GB002 -Wall -I".." -I"../USB" -I"../Include/USB" -I"../Include" -MMD -MF ${OBJECTDIR}/_ext/1360907413/usb_device.o.d -o ${OBJECTDIR}/_ext/1360907413/usb_device.o ../USB/usb_device.c  
ifneq (,$(findstring MINGW32,$(OS_CURRENT))) 
	@sed -e 's/\"//g' -e 's/\\$$/__EOL__/g' -e 's/\\ /__ESCAPED_SPACES__/g' -e 's/\\/\//g' -e 's/__ESCAPED_SPACES__/\\ /g' -e 's/__EOL__$$/\\/g' ${OBJECTDIR}/_ext/1360907413/usb_device.o.d > ${OBJECTDIR}/_ext/1360907413/usb_device.o.tmp
	${RM} ${OBJECTDIR}/_ext/1360907413/usb_device.o.d 
	${CP} ${OBJECTDIR}/_ext/1360907413/usb_device.o.tmp ${OBJECTDIR}/_ext/1360907413/usb_device.o.d 
	${RM} ${OBJECTDIR}/_ext/1360907413/usb_device.o.tmp}
else 
	@sed -e 's/\"//g' ${OBJECTDIR}/_ext/1360907413/usb_device.o.d > ${OBJECTDIR}/_ext/1360907413/usb_device.o.tmp
	${RM} ${OBJECTDIR}/_ext/1360907413/usb_device.o.d 
	${CP} ${OBJECTDIR}/_ext/1360907413/usb_device.o.tmp ${OBJECTDIR}/_ext/1360907413/usb_device.o.d 
	${RM} ${OBJECTDIR}/_ext/1360907413/usb_device.o.tmp}
endif
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/picnome-v12.X.${IMAGE_TYPE}.elf: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC}  -omf=elf  -mcpu=24FJ64GB002  -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -o dist/${CND_CONF}/${IMAGE_TYPE}/picnome-v12.X.${IMAGE_TYPE}.elf ${OBJECTFILES}       -Wl,--defsym=__MPLAB_BUILD=1,-L"../C:/Program Files (x86)/Microchip/MPLAB C30/lib/PIC24F",-L"..",-Map="$(BINDIR_)$(TARGETBASE).map",--report-mem,--report-mem,--script=../boot_hid_p24FJ64GB002.gld,--defsym=__MPLAB_DEBUG=1,--defsym=__ICD2RAM=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1
else
dist/${CND_CONF}/${IMAGE_TYPE}/picnome-v12.X.${IMAGE_TYPE}.elf: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk
	${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC}  -omf=elf  -mcpu=24FJ64GB002  -o dist/${CND_CONF}/${IMAGE_TYPE}/picnome-v12.X.${IMAGE_TYPE}.elf ${OBJECTFILES}       -Wl,--defsym=__MPLAB_BUILD=1,-L"../C:/Program Files (x86)/Microchip/MPLAB C30/lib/PIC24F",-L"..",-Map="$(BINDIR_)$(TARGETBASE).map",--report-mem,--report-mem,--script=../boot_hid_p24FJ64GB002.gld
	${MP_CC_DIR}/pic30-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/picnome-v12.X.${IMAGE_TYPE}.elf -omf=elf
endif


# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf:
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
