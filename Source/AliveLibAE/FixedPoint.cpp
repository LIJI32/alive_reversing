#include "stdafx.h"
#include "FixedPoint.hpp"
#include "Function.hpp"
#include <gtest/gtest.h>

EXPORT s32 CC Math_SquareRoot_Shifted_496E20(u32 value, s16 iterations)
{
    u32 value_shifted = value;
    s32 ret = 0;
    s32 counter = (iterations / 2) + 15;
    u32 tmp = 0;
    for (s32 i = 0; i <= counter; i++)
    {
        ret *= 2;
        tmp = (value_shifted >> 30) | 4 * tmp; // Hm.. what?
        const u32 v6 = (2 * ret) + 1;
        value_shifted *= 4;
        if (tmp >= v6)
        {
            tmp -= v6;
            ret++;
        }
    }
    return ret;
}

EXPORT s32 CC Math_SquareRoot_Int_496E70(s32 value)
{
    return Math_SquareRoot_Shifted_496E20(value, 0); // 15 iterations
}

EXPORT FP CC Math_SquareRoot_FP_496E90(FP value)
{
    return FP_FromRaw(Math_SquareRoot_Shifted_496E20(value.fpValue, 16)); // 23 iterations (16/2+15)
}

void FixedPoint_ForceLink()
{
    FP fp = FP_FromInteger(0);
    fp += FP_FromInteger(5);

    s32 f = FP_GetExponent(fp);
    ASSERT_EQ(f, 5);

    FixedPoint one = FP_FromDouble(1.0);
    ASSERT_EQ(FP_GetExponent(one), 1);
    ASSERT_EQ(one.fpValue, 0x10000);

    FixedPoint point5 = FP_FromDouble(0.5);
    ASSERT_EQ(FP_GetExponent(point5), 0);
    ASSERT_EQ(point5.fpValue, 0x8000);

    FixedPoint t1 = FP_NoFractional(FP_FromDouble(1.2));
    FixedPoint t2 = FP_NoFractional(FP_FromDouble(1.9));

    ASSERT_EQ(t1.fpValue, 0x10000);
    ASSERT_EQ(t2.fpValue, 0x10000);

    FixedPoint neg = {};
    neg.fpValue = 20;
    ASSERT_EQ(-20, (-neg).fpValue);

    ASSERT_EQ(1, Math_SquareRoot_Int_496E70(1));
    ASSERT_EQ(1, Math_SquareRoot_Int_496E70(2)); // Actually 1.414
    ASSERT_EQ(4, Math_SquareRoot_Int_496E70(16));
    ASSERT_EQ(10, Math_SquareRoot_Int_496E70(100));

    ASSERT_EQ(FP_FromRaw(1 * 256), Math_SquareRoot_FP_496E90(FP_FromRaw(1)));
    ASSERT_EQ(FP_FromRaw(362), Math_SquareRoot_FP_496E90(FP_FromRaw(2))); // 362/256 = 1.414 these are actually 24:8 fixed point, or just 8 bits in this instance ?
    ASSERT_EQ(FP_FromRaw(4 * 256), Math_SquareRoot_FP_496E90(FP_FromRaw(16)));
    ASSERT_EQ(FP_FromRaw(10 * 256), Math_SquareRoot_FP_496E90(FP_FromRaw(100)));
}
