#include "Math/vector3.hpp"
#include "detail/math_internal.h"

#include "gmock/gmock.h"

TEST (Math, Vector3f)
{
    Math::Vector3f  test0( 0.0f, 0.0f, 0.0f );

    EXPECT_EQ( 0.0f, test0.X() );
    EXPECT_EQ( 0.0f, test0.Y() );
    EXPECT_EQ( 0.0f, test0.Z() );

    Math::Vector3f  test1x( 1.0f, 0.0f, 0.0f );

    EXPECT_EQ( 1.0f, test1x.X() );
    EXPECT_EQ( 0.0f, test1x.Y() );
    EXPECT_EQ( 0.0f, test1x.Z() );

    Math::Vector3f  test1y( 0.0f, 1.0f, 0.0f );

    EXPECT_EQ( 0.0f, test1y.X() );
    EXPECT_EQ( 1.0f, test1y.Y() );
    EXPECT_EQ( 0.0f, test1y.Z() );

    Math::Vector3f  test1z( 0.0f, 0.0f, 1.0f );

    EXPECT_EQ( 0.0f, test1z.X() );
    EXPECT_EQ( 0.0f, test1z.Y() );
    EXPECT_EQ( 1.0f, test1z.Z() );
}
