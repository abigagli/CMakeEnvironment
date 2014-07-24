SET (PRIVATE_INCLUDE_DIRS Source)

SET (PUBLIC_INCLUDE_DIRS Include)

FILE ( GLOB_RECURSE
       INCLUDE_FILES
       Include/**
       Source/detail/*.h
       Source/detail/*.hpp
      )


####################### CUSTOMIZE LIBRARY BUILD ########################
SET (TARGET_NAME Math)

#SET(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE (Boost 1.46 REQUIRED)

SET ( PRIVATE_INCLUDE_DIRS
    ${PRIVATE_INCLUDE_DIRS}
    ${Boost_INCLUDE_DIR}
    )


SET ( SOURCE_FILES
    Source/vector3.cpp
    )

########################################################################


ADD_LIBRARY( ${TARGET_NAME}
    STATIC
    ${INCLUDE_FILES}
    ${SOURCE_FILES}
    )

#PRIVATE and PUBLIC items will populate the INCLUDE_DIRECTORIES property of <target>.
#PUBLIC and INTERFACE items will populate the INTERFACE_INCLUDE_DIRECTORIES property of <target>.
TARGET_INCLUDE_DIRECTORIES (${TARGET_NAME} PUBLIC ${PUBLIC_INCLUDE_DIRS} PRIVATE ${PRIVATE_INCLUDE_DIRS})
TARGET_COMPILE_DEFINITIONS (${TARGET_NAME} PRIVATE BUILDING_${TARGET_NAME} INTERFACE USING_${TARGET_NAME})


# ADD_GMOCK_TEST(<library_to_test> <sources of test>...)
#
#  Adds a Google Mock based test executable, _Test_<library_to_test>, built from <sources of test> and
#  adds the test so that CTest will run it. Both the executable and the test
#  will be named _Test_<library_to_test>.
#
FUNCTION(ADD_GMOCK_TEST lib_under_test)
    SET (TEST_TARGET_NAME "_Test_${lib_under_test}")

    ADD_EXECUTABLE(${TEST_TARGET_NAME} ${ARGN})

    #Allow tests to access private headers of its dependents
    TARGET_INCLUDE_DIRECTORIES (${TEST_TARGET_NAME} PRIVATE $<TARGET_PROPERTY:${lib_under_test},INCLUDE_DIRECTORIES>)

    TARGET_LINK_LIBRARIES(${TEST_TARGET_NAME} ${lib_under_test} gmock)

    ADD_TEST (NAME
        ${TEST_TARGET_NAME}
        COMMAND
        ${TEST_TARGET_NAME})

    SET_PROPERTY (TARGET ${TEST_TARGET_NAME} PROPERTY FOLDER Tests)
ENDFUNCTION()

IF (BUILD_UNIT_TESTS)
    ADD_GMOCK_TEST(${TARGET_NAME} Tests/main.cpp)
ENDIF()
