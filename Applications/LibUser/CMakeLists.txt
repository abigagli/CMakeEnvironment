PROJECT (LIBUSER)

SET ( SOURCE_FILES
    Source/main.cpp)

ADD_EXECUTABLE (LibUser ${SOURCE_FILES})

#Can't get any better solution that do this to gain visibility to imported target's includes...
INCLUDE_DIRECTORIES (${DOUBLER_SOURCE_DIR}/include)

TARGET_LINK_LIBRARIES (LibUser Math Doubler)
