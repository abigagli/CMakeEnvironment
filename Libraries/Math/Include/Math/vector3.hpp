#pragma once

#include "Math/math.hpp"


namespace Math
{
    class Vector3f
    {
    public:
        Vector3f()                                  {}
        Vector3f( float x, float y, float z );

        float           X() const                   {return mX;}
        float           Y() const                   {return mY;}
        float           Z() const                   {return mZ;}

    private:
        float           mX;
        float           mY;
        float           mZ;
    };
}
