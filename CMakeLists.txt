PROJECT(pthread_sample)
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(CMAKE_CONFIGURATION_TYPES "Debug;Release" CACHE STRING "Configs" FORCE)

IF(DEFINED CMAKE_BUILD_TYPE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${CMAKE_CONFIGURATION_TYPES} )
ENDIF()

IF(NOT CMAKE_BUILD_TYPE )
   SET( CMAKE_BUILD_TYPE "Debug" )
ENDIF()

#############################################################################################
# Create a folder and mount via sshfs the raspberry partition in order to copy binary to rpi with
# make install. If you don't want this feature, comment or delete lines: 16-22
#IF(NOT EXISTS remote-folder AND EXISTS ${CMAKE_TOOLCHAIN_FILE})
#	MESSAGE("Creating remote-folder for mounting raspberry pi partition with sshfs")
#	execute_process(COMMAND mkdir ${PROJECT_SOURCE_DIR}/remote-folder
#                    COMMAND sudo sshfs pi@192.168.1.187:/home/pi/pi_sshfs ${PROJECT_SOURCE_DIR}/remote-folder
#					WORKING_DIRECTORY ${PROJECT_SOURCE_DIR})
#ENDIF()
#SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/remote-folder)
#############################################################################################

OPTION(WARNINGS_ARE_ERRORS	"Treat warnings as errors"										ON)
OPTION(WARNINGS_ANSI_ISO    "Issue all the mandatory diagnostics Listed in C standard"		ON)
OPTION(ENABLE_PROFILING     "Enable profiling in Valgrind (Add flags: -g -fno_inline)"		OFF)

#INCLUDE(cmake_stuff/compileroptions.cmake REQUIRED)

SET(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR})
ADD_SUBDIRECTORY(src)

MESSAGE( STATUS "C++ flags :       ${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE}")
MESSAGE( STATUS "WARNINGS_ANSI_ISO =            ${WARNINGS_ANSI_ISO}" )
MESSAGE( STATUS "WARNINGS_ARE_ERRORS =          ${WARNINGS_ARE_ERRORS}" )
MESSAGE( STATUS "ENABLE_PROFILING =             ${ENABLE_PROFILING}" )
MESSAGE( STATUS "CMAKE_INSTALL_PREFIX =         ${CMAKE_INSTALL_PREFIX}" )
MESSAGE( STATUS "CMAKE_TOOLCHAIN_FILE =         ${CMAKE_TOOLCHAIN_FILE}" )
MESSAGE( STATUS "CMAKE_C_COMPILER   =         ${CMAKE_C_COMPILER}" )
MESSAGE( STATUS "CMAKE_CXX_COMPILER =         ${CMAKE_CXX_COMPILER}" )
MESSAGE( STATUS "CMAKE_AR =         ${CMAKE_AR}" )
MESSAGE( STATUS "CMAKE_LINKER  =         ${CMAKE_LINKER}" )
MESSAGE( STATUS "CMAKE_NM      =         ${CMAKE_NM}" )
MESSAGE( STATUS "CMAKE_OBJCOPY =         ${CMAKE_OBJCOPY}" )
MESSAGE( STATUS "CMAKE_OBJDUMP =         ${CMAKE_OBJDUMP}" )
MESSAGE( STATUS "CMAKE_STRIP   =         ${CMAKE_STRIP}" )
MESSAGE( STATUS "CMAKE_RANLIB  =         ${CMAKE_RANLIB}" )
