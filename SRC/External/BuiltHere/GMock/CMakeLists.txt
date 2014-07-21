PROJECT(GMOCK)

SET( GOOGLEMOCK_ROOT $ENV{GMOCKROOT} CACHE PATH "The Path to the GoogleMock test framework")

# New version relying directly on Gmock's CMake configuration
ADD_SUBDIRECTORY(${GOOGLEMOCK_ROOT} ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
TARGET_INCLUDE_DIRECTORIES (gmock INTERFACE ${GOOGLEMOCK_ROOT}/include ${GOOGLEMOCK_ROOT}/gtest/include)

# Obsoleted version
#SET (BUILD_INCLUDE_DIRS 
#    ${GOOGLEMOCK_ROOT}
#    ${GOOGLEMOCK_ROOT}/include 
#    ${GOOGLEMOCK_ROOT}/gtest 
#    ${GOOGLEMOCK_ROOT}/gtest/include )
#
#
#SET( SOURCE_FILES
#    ${GOOGLEMOCK_ROOT}/src/gmock-all.cc
#    ${GOOGLEMOCK_ROOT}/gtest/src/gtest-all.cc
#)
#
#
## Add as a static library target.
#ADD_LIBRARY( GMock
#    STATIC
#    ${SOURCE_FILES}
#)

#TARGET_INCLUDE_DIRECTORIES (GMock PRIVATE ${BUILD_INCLUDE_DIRS} )
#TARGET_INCLUDE_DIRECTORIES (gmock INTERFACE ${GOOGLEMOCK_ROOT}/include ${GOOGLEMOCK_ROOT}/gtest/include)

