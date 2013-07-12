PROJECT(MATH)

SET ( PRIVATE_INCLUDE_DIRS
    Include/Private
    $ENV{BOOSTROOT}
    )


SET ( PUBLIC_INCLUDE_DIRS
    Include/Public
    )


SET ( SOURCE_FILES
    Source/vector3.cpp
    )


ADD_LIBRARY( Math
    STATIC
    ${INCLUDE_FILES}
    ${SOURCE_FILES}
)

TARGET_INCLUDE_DIRECTORIES (Math PUBLIC ${PUBLIC_INCLUDE_DIRS} PRIVATE ${PRIVATE_INCLUDE_DIRS})
TARGET_COMPILE_DEFINITIONS (Math PRIVATE BUILDING_MATH INTERFACE USING_MATH)

IF (BUILD_UNIT_TESTS)

    ADD_EXECUTABLE(
        _TestMath
        Tests/main.cpp
        )

    #Allow tests to access private headers of its dependents
    TARGET_INCLUDE_DIRECTORIES (_TestMath PRIVATE $<TARGET_PROPERTY:Math,INCLUDE_DIRECTORIES>)

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
