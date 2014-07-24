# ###############################################
# Check for a valid version of CMake.
CMAKE_MINIMUM_REQUIRED( VERSION 2.8.11 )


# ###############################################
# Start the project.

PROJECT( CMakeEnvironment )

LIST(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)

set (CMAKE_EXPORT_COMPILE_COMMANDS ON)

IF (${CMAKE_SYSTEM_NAME} MATCHES "SunOS")
    SET (SOLARIS true)
ELSEIF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET (LINUX true)
ENDIF()

#SET (PREBUILT_LIBS_FOLDER ${CMAKE_HOME_DIRECTORY}/3rdParty/prebuilt_libs)
#LINK_DIRECTORIES (${PREBUILT_LIBS_FOLDER})
# ###############################################
# Enable C++11 on Darwin
IF( APPLE )
    EXECUTE_PROCESS(COMMAND llvm-config --cxxflags OUTPUT_VARIABLE LLVM_CXXFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
    EXECUTE_PROCESS(COMMAND llvm-config --cppflags OUTPUT_VARIABLE LLVM_CPPFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
    EXECUTE_PROCESS(COMMAND llvm-config --ldflags OUTPUT_VARIABLE LLVM_LDFLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)

    SET( MY_CXX_FLAGS "-std=c++11 -stdlib=libc++" )
    #SET( MY_EXE_LINKER_FLAGS "-Wl,-rpath,${PREBUILT_LIBS_FOLDER} -stdlib=libc++" )
    SET( MY_EXE_LINKER_FLAGS "-stdlib=libc++" )
    SET( MY_SHARED_LINKER_FLAGS "-stdlib=libc++" )
    SET( MY_MODULE_LINKER_FLAGS "-stdlib=libc++" )

    IF (CMAKE_GENERATOR STREQUAL Xcode)
        MESSAGE ("Setting Specialized Xcode attributes")
        SET (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LANGUAGE_STANDARD "c++11")
        SET (CMAKE_XCODE_ATTRIBUTE_CLANG_CXX_LIBRARY "libc++")
    ENDIF()
ELSEIF( LINUX )
    SET( MY_CXX_FLAGS "-std=c++0x -pthread" )
ELSEIF ( SOLARIS )
    ADD_DEFINITIONS (-D_GLIBCXX_USE_NANOSLEEP -D_GLIBCXX_USE_SCHED_YIELD)
    SET( MY_EXE_LINKER_FLAGS "-m64 -lsocket -lnsl -lrt")
    SET( MY_SHARED_LINKER_FLAGS "-m64")
    SET( MY_CXX_FLAGS "-m64 -static-libstdc++ -static-libgcc -std=c++0x -DGTEST_LANG_CXX11=0" )
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
    INCLUDE(gmock)
ENDIF()


INCLUDE(CMakeDependentOption)
OPTION(INTEL_SSE "Enable SSE on Intel CPU ON" ON)
CMAKE_DEPENDENT_OPTION(INTEL_SSE_2 "Enable SSE2 instructions" OFF "INTEL_SSE" OFF)


# ###############################################
# Perform single-shot actions at the very first configuration only (see http://johnlamp.net/cmake-tutorial-3-gui-tool.html):
# We want CMAKE_xxx_FLAGS to be cached, but either
# 1) we set it _without_ FORCE, and then it won't be cached because CMAKE_xxx_FLAGS is a built-in variable, and hence it's present
#    in the cache even before the first time our project is configured
# 2) we set it _with_ FORCE, but then every time we configure, we'll keep appending
# More generally, many of the CMake variables exist in the cache before CMakeLists.txt is processed so that the only
# solution is to detect when this is the VERY FIRST CONFIGURATION and only in that case FORCEly set the cache variable:

IF (NOT CONFIGURED_ONCE)
    SET (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${MY_CXX_FLAGS}" CACHE STRING "Flags used by the compiler during all build types" FORCE)
    SET (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${MY_EXE_LINKER_FLAGS}" CACHE STRING "Flags used by the linker" FORCE)
    SET (CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${MY_SHARED_LINKER_FLAGS}" CACHE STRING "Flags used by the linker during the creation of DLLs" FORCE)
    SET (CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${MY_MODULE_LINKER_FLAGS}" CACHE STRING "Flags used by the linker during the creation of modules" FORCE)
ENDIF()

# NOTE: No need to FORCE this, as CONFIGURED_ONCE is not present in the cache
SET (CONFIGURED_ONCE TRUE CACHE INTERNAL "A flag showing that CMake has configured at least once")
#
#

# ###############################################
# configure RPATH handling so that it is correctly set both
# for build and install time
# (see http://www.cmake.org/Wiki/CMake_RPATH_handling and http://www.kitware.com/blog/home/post/510)

# enable @rpath in the install name for any shared library being built
SET(CMAKE_MACOSX_RPATH TRUE)

# use, i.e. don't skip the full RPATH for the build tree, this allows running
# executables from the build tree without having to mess with DYLD_LIBRARY_PATH etc..
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

#Make sure everything is able to access generatd config files...
INCLUDE_DIRECTORIES (${CMAKE_BINARY_DIR}/config)

# ###############################################
# Add the external libraries, internal libraries,
# applications and tools to the project.
ADD_SUBDIRECTORY( 3rdParty )
ADD_SUBDIRECTORY( src/Libraries )
ADD_SUBDIRECTORY( src/Applications )
ADD_SUBDIRECTORY( tools )



#########################################
#Communicate with the codebase
SET (THIS_IS_ON ON)
SET (THIS_IS_OFF OFF)
SET (TEST1 CameFromTest1)
SET (TEST2 CameFromTest2)
CONFIGURE_FILE(config/config.h.in config/config_generated.h)


########################################
#Install commands


#INSTALL (FILES config/commands.json config/log.ini config/iface2commands.json DESTINATION bin/config COMPONENT Runtime)
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
