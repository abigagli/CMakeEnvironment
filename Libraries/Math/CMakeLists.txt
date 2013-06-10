PROJECT(Math)

SET( BUILD_INCLUDE_DIRS
    ${Math_SOURCE_DIR}/Include
)

INCLUDE_DIRECTORIES( ${BUILD_INCLUDE_DIRS} )

SET( INCLUDE_FILES
    Include/Math/Math.hpp
    Include/Math/Vector3.hpp
)

SET( SOURCE_FILES
    Source/Vector3.cpp
)

ADD_DEFINITIONS( -DBUILDING_WORLD )

ADD_LIBRARY( Math
    STATIC
    ${INCLUDE_FILES}
    ${SOURCE_FILES}
)

# Export the include directory.
SET( MATH_INCLUDE_DIRS ${BUILD_INCLUDE_DIRS} PARENT_SCOPE )

IF (BUILD_UNIT_TESTS)
    INCLUDE_DIRECTORIES(${GMOCK_PUBLIC_INCLUDE_DIRS})

    ADD_EXECUTABLE(
        _TestMath
        Tests/Main.cpp
        )

    TARGET_LINK_LIBRARIES (
        _TestMath
        Math
        GMock
        )

    ADD_TEST (NAME
        _TestMath
        COMMAND
        _TestMath)

    SET_PROPERTY (TARGET _TestMath PROPERTY FOLDER Tests)
ENDIF()
