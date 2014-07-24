SET( GOOGLEMOCK_ROOT $ENV{GMOCKROOT} CACHE PATH "The Path to the GoogleMock test framework")

# New version relying directly on Gmock's CMake configuration
ADD_SUBDIRECTORY(${GOOGLEMOCK_ROOT} ${CMAKE_BINARY_DIR}/GMOCK)
TARGET_INCLUDE_DIRECTORIES (gmock INTERFACE ${GOOGLEMOCK_ROOT}/include ${GOOGLEMOCK_ROOT}/gtest/include)

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

