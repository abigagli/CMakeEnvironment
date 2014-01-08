INCLUDE(ExternalProject)

SET(MYSQLPP_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS}")

IF (APPLE)
    SET(WITH_MYSQL_INCLUDE "--with-mysql-include=$ENV{HOME}/include/mysql")
    SET(WITH_MYSQL_LIB "--with-mysql-lib=$ENV{HOME}/lib") 
    #NOTE: mysqlpp configure step performs a link-check against the found libmysqlclient and the test
    #is done using /usr/bin/cc, i.e. a C compiler. Now, despite being a C-API, libmysqlclient has come c++
    #code inside it, which makes it necessary to explicitly link with libc++ and libc++abi when using a C
    #compiler to build anything against it. Don't know why, but this doesn't happen when _not_ using libc++, 
    SET (MYSQLPP_LINKER_FLAGS "${MYSQLPP_LINKER_FLAGS} -lc++ -lc++abi")
ELSEIF (SOLARIS)
    SET(WITH_MYSQL_LIB "--with-mysql-lib=/usr/mysql/lib/amd64/mysql")
ENDIF()

ExternalProject_Add(
    MySQL++
    #DEPENDS MySQLClient
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}
    URL http://tangentsoft.net/mysql++/releases/mysql++-3.0.9.tar.gz
    BUILD_COMMAND make
    BUILD_IN_SOURCE ON
    INSTALL_DIR INSTALL
    CONFIGURE_COMMAND <SOURCE_DIR>/configure --prefix=<INSTALL_DIR> --enable-thread-check ${WITH_MYSQL_INCLUDE} ${WITH_MYSQL_LIB} CXX=${CMAKE_CXX_COMPILER} CC=${CMAKE_C_COMPILER} CXXFLAGS=${CMAKE_CXX_FLAGS} LDFLAGS=${MYSQLPP_LINKER_FLAGS}
    PATCH_COMMAND patch -p0 -f -N -i ${CMAKE_CURRENT_SOURCE_DIR}/mysqlpp.patch
    #LOG_CONFIGURE ON
    #LOG_BUILD ON
    )
    
ExternalProject_Get_Property(MySQL++ INSTALL_DIR)

#Need to create these at configuration time, because the ExternalProject_Add will only create them at build time
#and so the very first time cmake runs and configures the build tree, it would complain at any use of any variable that refers to these
#not-yet existing directories
FILE (MAKE_DIRECTORY "${INSTALL_DIR}/lib" "${INSTALL_DIR}/include")
SET (MYSQLPP_LIBRARY_DIR "${INSTALL_DIR}/lib")
SET (MYSQLPP_INCLUDE_DIR "${INSTALL_DIR}/include")

FIND_LIBRARY (MYSQLPP_LIBRARY NAMES mysqlpp HINTS ${MYSQLPP_LIBRARY_DIR} NO_SYSTEM_ENVIRONMENT_PATH NO_DEFAULT_PATH)

#Create an imported target to "represent" mysqlpp inside this project
ADD_LIBRARY (MYSQLPP SHARED IMPORTED GLOBAL)
SET_TARGET_PROPERTIES (MYSQLPP PROPERTIES IMPORTED_LOCATION ${MYSQLPP_LIBRARY} INTERFACE_INCLUDE_DIRECTORIES ${MYSQLPP_INCLUDE_DIR})

INSTALL (DIRECTORY ${MYSQLPP_LIBRARY_DIR} DESTINATION . COMPONENT Runtime USE_SOURCE_PERMISSIONS)
