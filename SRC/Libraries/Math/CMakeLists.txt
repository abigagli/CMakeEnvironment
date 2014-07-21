PROJECT(MATH)

#SET(Boost_USE_STATIC_LIBS ON)
FIND_PACKAGE (Boost 1.46 REQUIRED)

SET ( PRIVATE_INCLUDE_DIRS
    Source
    ${Boost_INCLUDE_DIR}
    )

SET ( PUBLIC_INCLUDE_DIRS
    Include
    )

FILE ( GLOB_RECURSE
       INCLUDE_FILES
       Include/**
       Source/detail/*.h
       Source/detail/*.hpp
      )

SET ( SOURCE_FILES
    Source/vector3.cpp
    )


ADD_LIBRARY( Math
    STATIC
    ${INCLUDE_FILES}
    ${SOURCE_FILES}
)

#PRIVATE and PUBLIC items will populate the INCLUDE_DIRECTORIES property of <target>.  
#PUBLIC and INTERFACE items will populate the INTERFACE_INCLUDE_DIRECTORIES property of <target>.
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
        gmock
        )

    ADD_TEST (NAME
        _TestMath
        COMMAND
        _TestMath)

    SET_PROPERTY (TARGET _TestMath PROPERTY FOLDER Tests)
ENDIF()
