#include "Math/vector3.hpp"
#include "config_generated.h"

#define MYSQLPP_MYSQL_HEADERS_BURIED
#include <mysql++/mysql++.h>
#include <mysql++/ssqls.h>
#include <boost/serialization/strong_typedef.hpp>
#include <boost/algorithm/string.hpp>

int main(int argc, char *argv[])
{
    float x;
    Math::Vector3f {x, 11, 13.1};

    return 0;
}
