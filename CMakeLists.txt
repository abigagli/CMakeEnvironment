# ###############################################
# Check for a valid version of CMake.
CMAKE_MINIMUM_REQUIRED( VERSION 2.8.11 )



# ###############################################
# Start the project.

PROJECT( CMakeEnvironment )

IF (${CMAKE_SYSTEM_NAME} MATCHES "SunOS")
    SET (SOLARIS true)
ELSEIF (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    SET (LINUX true)
ENDIF()

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
    SET( CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x -DGTEST_LANG_CXX11=0" )
ELSE()
    MESSAGE ("Unsupported platform")
ENDIF( APPLE )

MESSAGE ("Compiler ID: ${CMAKE_CXX_COMPILER_ID}")
MESSAGE ("System Name: ${CMAKE_SYSTEM_NAME}")
MESSAGE ("System Version: ${CMAKE_SYSTEM_VERSION}")
OPTION (BUILD_UNIT_TESTS "Build unit tests" ON)

IF (BUILD_UNIT_TESTS)
    ENABLE_TESTING()
ENDIF()

INCLUDE(CMakeDependentOption)
OPTION(INTEL_SSE "Enable SSE on Intel CPU ON" ON)
CMAKE_DEPENDENT_OPTION(INTEL_SSE_2 "Enable SSE2 instructions" OFF "INTEL_SSE" OFF)


# ###############################################
# Add the external libraries, internal libraries,
# applications and tools to the project.
#INCLUDE( External/CMakeLists.txt )
#INCLUDE( Libraries/CMakeLists.txt )
ADD_SUBDIRECTORY( External )
ADD_SUBDIRECTORY( Libraries )
ADD_SUBDIRECTORY( Applications )
ADD_SUBDIRECTORY( Tools )



#########################################
#Communicate with the codebase
SET (THIS_IS_ON ON)
SET (THIS_IS_OFF OFF)
SET (TEST1 CameFromTest1)
SET (TEST2 CameFromTest2)
CONFIGURE_FILE(Config/config.h.in Config/config.h)
