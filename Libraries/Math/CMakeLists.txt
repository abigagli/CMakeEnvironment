PROJECT(MATH)

SET( BUILD_INCLUDE_DIRS
    ${MATH_SOURCE_DIR}/Include
)


SET( INCLUDE_FILES
    Include/Math/Math.hpp
    Include/Math/Vector3.hpp
)

SET( SOURCE_FILES
    Source/Vector3.cpp
)


ADD_LIBRARY( Math
    STATIC
    ${INCLUDE_FILES}
    ${SOURCE_FILES}
)

TARGET_INCLUDE_DIRECTORIES (Math PUBLIC ${BUILD_INCLUDE_DIRS})
TARGET_COMPILE_DEFINITIONS (Math PRIVATE BUILDING_MATH INTERFACE USING_MATH)

IF (BUILD_UNIT_TESTS)

    ADD_EXECUTABLE(
        _TestMath
        Tests/main.cpp
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
