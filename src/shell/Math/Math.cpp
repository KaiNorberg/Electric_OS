#include "Math.h"

namespace Math
{
    double Sqrt(double X)
    {
        float i = 0;
        float X1, X2;
        while(i * i <= X)
        {
            i += 0.1f;
        }
        X1=i;

        for (int j = 0; j < 10; j++)
        {
            X2 = X;
            X2 /= X1;
            X2 += X1;
            X2 /= 2;
            X1 = X2;
        }

        return X2;
    }
}
