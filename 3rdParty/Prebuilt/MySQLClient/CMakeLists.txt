PROJECT(MYSQLCLIENT)

IF (APPLE)
    SET (MYSQLCLIENT_LIBRARY_DIR "$ENV{HOME}/lib")
    SET (MYSQLCLIENT_INCLUDE_DIR "$ENV{HOME}/include")
ELSEIF (SOLARIS)
    SET (MYSQLCLIENT_LIBRARY_DIR "/usr/mysql/lib/amd64/mysql")
    SET (MYSQLCLIENT_INCLUDE_DIR "/usr/mysql/include")
ENDIF()

FIND_LIBRARY (MYSQLCLIENT_LIBRARY NAMES mysqlclient_r HINTS "${MYSQLCLIENT_LIBRARY_DIR}")

ADD_LIBRARY(MYSQLCLIENT SHARED IMPORTED GLOBAL)

SET_TARGET_PROPERTIES (MYSQLCLIENT PROPERTIES IMPORTED_LOCATION "${MYSQLCLIENT_LIBRARY}" INTERFACE_INCLUDE_DIRECTORIES "${MYSQLCLIENT_INCLUDE_DIR}")
