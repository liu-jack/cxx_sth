#include "gtest/gtest.h"

#include "loot/FixFakeRandom/FixFakeRandomBitSet.h"

TEST( FixFakeRandomCalculator, IsHitFixCorrect)
{
    EXPECT_TRUE( sFixFakeRD.IsHitFixCorrect(9,4,8));
    EXPECT_TRUE( sFixFakeRD.IsHitFixCorrect(9,4,6));
    EXPECT_TRUE( sFixFakeRD.IsHitFixCorrect(9,4,4));
    EXPECT_TRUE( sFixFakeRD.IsHitFixCorrect(9,4,2));
    EXPECT_FALSE( sFixFakeRD.IsHitFixCorrect(9,4,0));

    EXPECT_TRUE( sFixFakeRD.IsHitFixCorrect(9,5,8));
    EXPECT_TRUE( sFixFakeRD.IsHitFixCorrect(9,5,6));
    EXPECT_TRUE( sFixFakeRD.IsHitFixCorrect(9,5,4));
    EXPECT_TRUE( sFixFakeRD.IsHitFixCorrect(9,5,2));
    EXPECT_TRUE( sFixFakeRD.IsHitFixCorrect(9,5,0));

    EXPECT_FALSE( sFixFakeRD.IsHitFixCorrect(9,6,8));

    EXPECT_TRUE( sFixFakeRD.IsHitFixCorrect(9,3,8));
    EXPECT_TRUE( sFixFakeRD.IsHitFixCorrect(9,3,5));
    EXPECT_TRUE( sFixFakeRD.IsHitFixCorrect(9,3,2));
    EXPECT_FALSE( sFixFakeRD.IsHitFixCorrect(9,3,1));
    EXPECT_FALSE( sFixFakeRD.IsHitFixCorrect(9,3,0));

    EXPECT_TRUE( sFixFakeRD.IsHitFixCorrect(8,2,7));
    EXPECT_TRUE( sFixFakeRD.IsHitFixCorrect(8,2,3));

    EXPECT_TRUE( sFixFakeRD.IsHitFixCorrect(8,4,7));
    EXPECT_TRUE( sFixFakeRD.IsHitFixCorrect(8,4,5));
    EXPECT_TRUE( sFixFakeRD.IsHitFixCorrect(8,4,3));
    EXPECT_TRUE( sFixFakeRD.IsHitFixCorrect(8,4,1));

    EXPECT_TRUE( sFixFakeRD.IsHitFixCorrect(8,3,7));
    EXPECT_TRUE( sFixFakeRD.IsHitFixCorrect(8,3,5));
    EXPECT_TRUE( sFixFakeRD.IsHitFixCorrect(8,3,3));

}

TEST( FixFakeRandomCalculator, TryGetRemaintoHit)
{
    U32 remain = 0;
    sFixFakeRD.TryGetRemaintoHit(9,4,0, remain);
    EXPECT_EQ( remain , 3 );
}