# ###############################################
# Check for a valid version of CMake.
CMAKE_MINIMUM_REQUIRED( VERSION 2.8.11 )


# ###############################################
# Start the project.

PROJECT( CMakeEnvironment )

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

IF (${CMAKE_SYSTEM_NAME} MATCHES "SunOS")
    SET (SOLARIS true)
ELSEIF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET (LINUX true)
ENDIF()

SET (CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/INSTALL")

#SET (PREBUILT_LIBS_FOLDER ${CMAKE_HOME_DIRECTORY}/External/prebuilt_libs)
#LINK_DIRECTORIES (${PREBUILT_LIBS_FOLDER})
# ###############################################
# Enable C++11 on Darwin
IF( APPLE )
    EXECUTE_PROCESS(COMMAND llvm-config --cxxflags OUTPUT_VARIABLE LLVM_CXXFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
    EXECUTE_PROCESS(COMMAND llvm-config --cppflags OUTPUT_VARIABLE LLVM_CPPFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
    EXECUTE_PROCESS(COMMAND llvm-config --ldflags OUTPUT_VARIABLE LLVM_LDFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)

	SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -stdlib=libc++" )
    #SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -Wl,-rpath,${PREBUILT_LIBS_FOLDER} -stdlib=libc++" )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++" )
	SET( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -stdlib=libc++" )
	SET( CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} -stdlib=libc++" )

    IF (CMAKE_GENERATOR STREQUAL Xcode)
        MESSAGE ("Setting Specialized Xcode attributes")
        SET (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
        SET (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    ENDIF()
ELSEIF( LINUX )
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -pthread" )
ELSEIF ( SOLARIS )
    ADD_DEFINITIONS (-D_GLIBCXX_USE_NANOSLEEP -D_GLIBCXX_USE_SCHED_YIELD)
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -m64 -lsocket -lnsl -lrt")
    SET( CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m64")
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64 -static-libstdc++ -static-libgcc -std=c++0x -DGTEST_LANG_CXX11=0" )
ELSE()
    MESSAGE ("Unsupported platform")
ENDIF( APPLE )

MESSAGE ("Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
MESSAGE ("System Name: ${CMAKE_SYSTEM_NAME}")
MESSAGE ("System Version: ${CMAKE_SYSTEM_VERSION}")
MESSAGE ("Install Prefix: ${CMAKE_INSTALL_PREFIX}")

OPTION (BUILD_UNIT_TESTS "Build unit tests" ON)

IF (BUILD_UNIT_TESTS)
    ENABLE_TESTING()
ENDIF()

INCLUDE(CMakeDependentOption)
OPTION(INTEL_SSE "Enable SSE on Intel CPU ON" ON)
CMAKE_DEPENDENT_OPTION(INTEL_SSE_2 "Enable SSE2 instructions" OFF "INTEL_SSE" OFF)


# ###############################################
# configure RPATH handling so that it is correctly set both
# for build and install time
# (see http://www.cmake.org/Wiki/CMake_RPATH_handling and http://www.kitware.com/blog/home/post/510)

# enable @rpath in the install name for any shared library being built
SET(CMAKE_MACOSX_RPATH TRUE)

# use, i.e. don't skip the full RPATH for the build tree
SET(CMAKE_SKIP_BUILD_RPATH  FALSE)
#
# when building, don't use the install RPATH already
# (but later on when installing)
SET(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) 
#
# add the automatically determined parts of the RPATH
# which point to directories outside the build tree to the install RPATH
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
#
#
# the RPATH to be used when installing, but only if it's not a system directory
LIST(FIND CMAKE_PLATFORM_IMPLICIT_LINK_DIRECTORIES "${CMAKE_INSTALL_PREFIX}/lib" isSystemDir)
IF("${isSystemDir}" STREQUAL "-1")
    IF (APPLE)
        SET(CMAKE_INSTALL_RPATH "@executable_path/../lib")
    ELSE(APPLE)
        SET(CMAKE_INSTALL_RPATH "\$ORIGIN/../lib")
    ENDIF()
ENDIF()

# ###############################################
# Add the external libraries, internal libraries,
# applications and tools to the project.
ADD_SUBDIRECTORY( SRC/External )
ADD_SUBDIRECTORY( SRC/Libraries )
ADD_SUBDIRECTORY( SRC/Applications )
ADD_SUBDIRECTORY( TOOLS )



#########################################
#Communicate with the codebase
SET (THIS_IS_ON ON)
SET (THIS_IS_OFF OFF)
SET (TEST1 CameFromTest1)
SET (TEST2 CameFromTest2)
CONFIGURE_FILE(Config/config.h.in Config/config_generated.h)


########################################
#Install commands


#INSTALL (FILES Config/commands.json Config/log.ini Config/iface2commands.json DESTINATION bin/config COMPONENT Runtime)
#INSTALL (DIRECTORY scripts DESTINATION bin USE_SOURCE_PERMISSIONS COMPONENT Runtime)
#
#
#SET(CPACK_GENERATOR "TGZ")
#SET(CPACK_PACKAGE_NAME "BB")
#SET(CPACK_PACKAGE_VERSION_MAJOR "1")
#SET(CPACK_PACKAGE_VERSION_MINOR "0")
#SET(CPACK_PACKAGE_VERSION_PATCH "0")
#
#SET(CPACK_TGZ_COMPONENT_INSTALL ON)
#SET(CPACK_COMPONENTS_ALL Runtime)
#SET(CPACK_INCLUDE_TOPLEVEL_DIRECTORY ON)
##SET(CPACK_PACKAGING_INSTALL_PREFIX "BB")
#
#INCLUDE (CPack)
