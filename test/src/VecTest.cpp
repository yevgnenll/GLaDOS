#include <catch2/catch_test_macros.hpp>

#include "math/Vec.hpp"

using namespace GLaDOS;

TEST_CASE("Vec unit tests", "[Vector]") {
    SECTION("Vec constructor test") {
        Vec<real, 2> v1;
        REQUIRE(v1 == Vec<real, 2>{0.f, 0.f});
        v1.xy = Vec<real, 2>{1.f, 2.f};
        REQUIRE(v1 == Vec<real, 2>{1.f, 2.f});
        REQUIRE(v1.xx == Vec<real, 2>{1.f, 1.f});
        REQUIRE(v1.yy == Vec<real, 2>{2.f, 2.f});

        Vec<real, 3> v2;
        REQUIRE(v2 == Vec<real, 3>{0.f, 0.f, 0.f});
        v2.xyz = Vec<real, 3>{1.f, 2.f, 3.f};
        REQUIRE(v2 == Vec<real, 3>{1.f, 2.f, 3.f});
        REQUIRE(v2.s == 1.f);
        REQUIRE(v2.t == 2.f);
        REQUIRE(v2.p == 3.f);
        REQUIRE(v2.v[0] == 1.f);
        REQUIRE(v2.v[1] == 2.f);
        REQUIRE(v2.v[2] == 3.f);
        REQUIRE(v2.xz == Vec<real, 2>{1.f, 3.f});
        REQUIRE(v2.x == 1.f);
        v2.yx = Vec<real, 2>{1.f, 2.f};
        REQUIRE(v2.xy == Vec<real, 2>{2.f, 1.f});
        REQUIRE(v2.xyz == Vec<real, 3>{2.f, 1.f, 3.f});
        REQUIRE(v2.xxx == Vec<real, 3>{2.f, 2.f, 2.f});
        REQUIRE(v2.yyy == Vec<real, 3>{1.f, 1.f, 1.f});
        REQUIRE(v2.zzz == Vec<real, 3>{3.f, 3.f, 3.f});

        Vec<real, 4> v3;
        REQUIRE(v3 == Vec<real, 4>{0.f, 0.f, 0.f, 0.f});
        v3.s = 1.f;
        v3.t = 2.f;
        v3.p = 3.f;
        v3.q = 4.f;
        REQUIRE(v3 == Vec<real, 4>{1.f, 2.f, 3.f, 4.f});
    }

    SECTION("Vec copy test") {
        Vec<real, 2> v1;
        Vec<real, 2> v2{1.f, 2.f};
        v1 = v2;
        REQUIRE(v1 == Vec<real, 2>{1.f, 2.f});

        Vec<real, 2> v3 = Vec<real, 2>{1.f, 2.f};
        REQUIRE(v3 == Vec<real, 2>{1.f, 2.f});

        Vec<real, 3> v4;
        Vec<real, 3> v5{1.f, 2.f, 3.f};
        v4 = v5;
        REQUIRE(v4 == Vec<real, 3>{1.f, 2.f, 3.f});

        Vec<real, 3> v6 = Vec<real, 3>{1.f, 2.f, 3.f};
        REQUIRE(v6 == Vec<real, 3>{1.f, 2.f, 3.f});

        Vec<real, 4> v7;
        Vec<real, 4> v8{1.f, 2.f, 3.f, 4.f};
        v7 = v8;
        REQUIRE(v7 == Vec<real, 4>{1.f, 2.f, 3.f, 4.f});

        Vec<real, 4> v9 = Vec<real, 4>{1.f, 2.f, 3.f, 4.f};
        REQUIRE(v9 == Vec<real, 4>{1.f, 2.f, 3.f, 4.f});
    }

    SECTION("Vec subscript operator test") {
        Vec<real, 2> v1{1.f, 2.f};
        REQUIRE(v1[0] == 1.f);
        REQUIRE(v1[1] == 2.f);

        Vec<real, 3> v2{1.f, 2.f, 3.f};
        REQUIRE(v2[0] == 1.f);
        REQUIRE(v2[1] == 2.f);
        REQUIRE(v2[2] == 3.f);

        Vec<real, 4> v3{1.f, 2.f, 3.f, 4.f};
        REQUIRE(v3[0] == 1.f);
        REQUIRE(v3[1] == 2.f);
        REQUIRE(v3[2] == 3.f);
        REQUIRE(v3[3] == 4.f);
    }

    SECTION("Vec arithmetic test") {
        Vec<real, 2> v1{1.f, 2.f};
        Vec<real, 2> v2{2.f, 1.f};
        REQUIRE((v1 + v2) == Vec<real, 2>{3.f, 3.f});
        REQUIRE((v1 - v2) == Vec<real, 2>{-1.f, 1.f});
        REQUIRE(v1 * 3.f == Vec<real, 2>{3.f, 6.f});

        Vec<real, 3> v3{1.f, 2.f, 1.f};
        Vec<real, 3> v4{2.f, 1.f, 1.f};
        REQUIRE((v3 + v4) == Vec<real, 3>{3.f, 3.f, 2.f});
        REQUIRE((v3 - v4) == Vec<real, 3>{-1.f, 1.f, 0.f});
        REQUIRE(v3 * 3.f == Vec<real, 3>{3.f, 6.f, 3.f});

        Vec<real, 4> v5{1.f, 2.f, 1.f, 2.f};
        Vec<real, 4> v6{2.f, 1.f, 1.f, 1.f};
        REQUIRE((v5 + v6) == Vec<real, 4>{3.f, 3.f, 2.f, 3.f});
        REQUIRE((v5 - v6) == Vec<real, 4>{-1.f, 1.f, 0.f, 1.f});
        REQUIRE(v5 * 3.f == Vec<real, 4>{3.f, 6.f, 3.f, 6.f});

        Vec<real, 10> v7{1.f, 2.f, 3.f, 4.f, 5.f, 6.f, 7.f, 8.f, 9.f, 10.f};
        Vec<real, 10> v8{10.f, 9.f, 8.f, 7.f, 6.f, 5.f, 4.f, 3.f, 2.f, 1.f};
        REQUIRE((v7 + v8) == Vec<real, 10>{11.f});
        REQUIRE((v7 - v8) == Vec<real, 10>{-9.f, -7.f, -5.f, -3.f, -1.f, 1.f, 3.f, 5.f, 7.f, 9.f});
        REQUIRE(v7 * 3.f == Vec<real, 10>{3.f, 6.f, 9.f, 12.f, 15.f, 18.f, 21.f, 24.f, 27.f, 30.f});
    }

    SECTION("Vec negate test") {
        Vec<real, 2> v1{1.f, 2.f};
        REQUIRE(Vec<real, 2>{-1.f, -2.f} == Vec<real, 2>::negate(v1));
        REQUIRE(Vec<real, 2>{-1.f, -2.f} == v1.makeNegate());

        Vec<real, 3> v2{1.f, 2.f, 3.f};
        REQUIRE(Vec<real, 3>{-1.f, -2.f, -3.f} == Vec<real, 3>::negate(v2));
        REQUIRE(Vec<real, 3>{-1.f, -2.f, -3.f} == v2.makeNegate());

        Vec<real, 4> v3{1.f, 2.f, 3.f, 4.f};
        REQUIRE(Vec<real, 4>{-1.f, -2.f, -3.f, -4.f} == Vec<real, 4>::negate(v3));
        REQUIRE(Vec<real, 4>{-1.f, -2.f, -3.f, -4.f} == v3.makeNegate());

        Vec<real, 5> v4{1.f, 2.f, 3.f, 4.f, 5.f};
        REQUIRE(Vec<real, 5>{-1.f, -2.f, -3.f, -4.f, -5.f} == Vec<real, 5>::negate(v4));
        REQUIRE(Vec<real, 5>{-1.f, -2.f, -3.f, -4.f, -5.f} == v4.makeNegate());
    }

    SECTION("Vec3 cross product test") {
        Vec<real, 3> v1{5.f, 6.f, 7.f};
        Vec<real, 3> v2{8.f, 9.f, 10.f};
        REQUIRE(Vec<real, 3>::cross(v1, v2) == Vec<real, 3>{-3, 6, -3});
    }

    SECTION("Vec dot product test") {
        Vec<real, 2> v1{1.f, 2.f};
        Vec<real, 2> v2{4.f, 5.f};
        REQUIRE(Vec<real, 2>::dot(v1, v2) == 14.f);

        Vec<real, 3> v3{1.f, 2.f, 3.f};
        Vec<real, 3> v4{4.f, 5.f, 6.f};
        REQUIRE(Vec<real, 3>::dot(v3, v4) == 32.f);

        Vec<real, 4> v5{5.f, 6.f, 7.f, 8.f};
        Vec<real, 4> v6{9.f, 10.f, 11.f, 12.f};
        REQUIRE(Vec<real, 4>::dot(v5, v6) == 278.f);

        Vec<real, 5> v7{5.f, 6.f, 7.f, 8.f, 13.f};
        Vec<real, 5> v8{9.f, 10.f, 11.f, 12.f, 14.f};
        REQUIRE(Vec<real, 5>::dot(v7, v8) == 460.f);
    }

    SECTION("Vec swizzle test") {
        Vec<real, 2> v1{1.f, 2.f};
        REQUIRE(v1.xy == Vec<real, 2>{1.f, 2.f});
        REQUIRE(v1.yx == Vec<real, 2>{2.f, 1.f});
        REQUIRE(v1.xx == Vec<real, 2>{1.f, 1.f});
        REQUIRE(v1.yy == Vec<real, 2>{2.f, 2.f});

        Vec<real, 3> v2{1.f, 2.f, 3.f};
        REQUIRE(v2.xx == Vec<real, 2>{1.f, 1.f});
        REQUIRE(v2.xy == Vec<real, 2>{1.f, 2.f});
        REQUIRE(v2.xz == Vec<real, 2>{1.f, 3.f});
        REQUIRE(v2.yx == Vec<real, 2>{2.f, 1.f});
        REQUIRE(v2.yy == Vec<real, 2>{2.f, 2.f});
        REQUIRE(v2.yz == Vec<real, 2>{2.f, 3.f});
        REQUIRE(v2.zx == Vec<real, 2>{3.f, 1.f});
        REQUIRE(v2.zy == Vec<real, 2>{3.f, 2.f});
        REQUIRE(v2.zz == Vec<real, 2>{3.f, 3.f});
        REQUIRE(v2.xxx == Vec<real, 3>{1.f, 1.f, 1.f});
        REQUIRE(v2.xxy == Vec<real, 3>{1.f, 1.f, 2.f});
        REQUIRE(v2.xxz == Vec<real, 3>{1.f, 1.f, 3.f});
        REQUIRE(v2.xyx == Vec<real, 3>{1.f, 2.f, 1.f});
        REQUIRE(v2.xyy == Vec<real, 3>{1.f, 2.f, 2.f});
        REQUIRE(v2.xyz == Vec<real, 3>{1.f, 2.f, 3.f});
        REQUIRE(v2.xzx == Vec<real, 3>{1.f, 3.f, 1.f});
        REQUIRE(v2.xzy == Vec<real, 3>{1.f, 3.f, 2.f});
        REQUIRE(v2.xzz == Vec<real, 3>{1.f, 3.f, 3.f});
        REQUIRE(v2.yxx == Vec<real, 3>{2.f, 1.f, 1.f});
        REQUIRE(v2.yxy == Vec<real, 3>{2.f, 1.f, 2.f});
        REQUIRE(v2.yxz == Vec<real, 3>{2.f, 1.f, 3.f});
        REQUIRE(v2.yyx == Vec<real, 3>{2.f, 2.f, 1.f});
        REQUIRE(v2.yyy == Vec<real, 3>{2.f, 2.f, 2.f});
        REQUIRE(v2.yyz == Vec<real, 3>{2.f, 2.f, 3.f});
        REQUIRE(v2.yzx == Vec<real, 3>{2.f, 3.f, 1.f});
        REQUIRE(v2.yzy == Vec<real, 3>{2.f, 3.f, 2.f});
        REQUIRE(v2.yzz == Vec<real, 3>{2.f, 3.f, 3.f});
        REQUIRE(v2.zxx == Vec<real, 3>{3.f, 1.f, 1.f});
        REQUIRE(v2.zxy == Vec<real, 3>{3.f, 1.f, 2.f});
        REQUIRE(v2.zxz == Vec<real, 3>{3.f, 1.f, 3.f});
        REQUIRE(v2.zyx == Vec<real, 3>{3.f, 2.f, 1.f});
        REQUIRE(v2.zyy == Vec<real, 3>{3.f, 2.f, 2.f});
        REQUIRE(v2.zyz == Vec<real, 3>{3.f, 2.f, 3.f});
        REQUIRE(v2.zzx == Vec<real, 3>{3.f, 3.f, 1.f});
        REQUIRE(v2.zzy == Vec<real, 3>{3.f, 3.f, 2.f});
        REQUIRE(v2.zzz == Vec<real, 3>{3.f, 3.f, 3.f});
    }

    SECTION("Vec constant specialization test") {
        REQUIRE(Vec<real, 2>{0, 1} == Vec<real, 2>::up);
        REQUIRE(Vec<real, 2>{0, -1} == Vec<real, 2>::down);
        REQUIRE(Vec<real, 2>{-1, 0} == Vec<real, 2>::left);
        REQUIRE(Vec<real, 2>{1, 0} == Vec<real, 2>::right);
        REQUIRE(Vec<real, 2>{1, 1} == Vec<real, 2>::one);
        REQUIRE(Vec<real, 2>{0, 0} == Vec<real, 2>::zero);

        REQUIRE(Vec<real, 3>{0, 1, 0} == Vec<real, 3>::up);
        REQUIRE(Vec<real, 3>{0, -1, 0} == Vec<real, 3>::down);
        REQUIRE(Vec<real, 3>{-1, 0, 0} == Vec<real, 3>::left);
        REQUIRE(Vec<real, 3>{1, 0, 0} == Vec<real, 3>::right);
        REQUIRE(Vec<real, 3>{0, 0, -1} == Vec<real, 3>::forward);
        REQUIRE(Vec<real, 3>{0, 0, 1} == Vec<real, 3>::backward);
        REQUIRE(Vec<real, 3>{1, 1, 1} == Vec<real, 3>::one);
        REQUIRE(Vec<real, 3>{0, 0, 0} == Vec<real, 3>::zero);

        REQUIRE(Vec<real, 4>{0, 1, 0, 0} == Vec<real, 4>::up);
        REQUIRE(Vec<real, 4>{0, -1, 0, 0} == Vec<real, 4>::down);
        REQUIRE(Vec<real, 4>{-1, 0, 0, 0} == Vec<real, 4>::left);
        REQUIRE(Vec<real, 4>{1, 0, 0, 0} == Vec<real, 4>::right);
        REQUIRE(Vec<real, 4>{0, 0, -1, 0} == Vec<real, 4>::forward);
        REQUIRE(Vec<real, 4>{0, 0, 1, 0} == Vec<real, 4>::backward);
        REQUIRE(Vec<real, 4>{1, 1, 1, 1} == Vec<real, 4>::one);
        REQUIRE(Vec<real, 4>{0, 0, 0, 0} == Vec<real, 4>::zero);
    }

    SECTION("Vec length and distance test") {
        Vec<real, 2> v1{1.f, 2.f};
        REQUIRE(Math::equal(v1.length(), Math::sqrt(5)));
        REQUIRE(Math::equal(v1.squaredLength(), real(5)));

        Vec<real, 3> v2{1.f, 2.f, 3.f};
        REQUIRE(Math::equal(v2.length(), Math::sqrt(14)));
        REQUIRE(Math::equal(v2.squaredLength(), real(14)));

        Vec<real, 4> v3{1.f, 2.f, 3.f, 4.f};
        REQUIRE(Math::equal(v3.length(), Math::sqrt(30)));
        REQUIRE(Math::equal(v3.squaredLength(), real(30)));

        Vec<real, 5> v4{1.f, 2.f, 3.f, 4.f, 5.f};
        REQUIRE(Math::equal(v4.length(), Math::sqrt(55)));
        REQUIRE(Math::equal(v4.squaredLength(), real(55)));

        Vec<real, 2> v5{7, 4};
        Vec<real, 2> v6{17, 6};
        REQUIRE(Math::equal(v5.distance(v6), Math::sqrt(104)));
        REQUIRE(Math::equal(v5.distanceSquare(v6), real(104)));

        Vec<real, 3> v7{7, 4, 3};
        Vec<real, 3> v8{17, 6, 2};
        REQUIRE(Math::equal(v7.distance(v8), Math::sqrt(105)));
        REQUIRE(Math::equal(v7.distanceSquare(v8), real(105)));

        Vec<real, 4> v9{7, 4, 3, 2};
        Vec<real, 4> v10{17, 6, 2, 1};
        REQUIRE(Math::equal(v9.distance(v10), Math::sqrt(106)));
        REQUIRE(Math::equal(v9.distanceSquare(v10), real(106)));

        Vec<real, 5> v11{7.f, 4.f, 3.f, 2.f, 99.f};
        Vec<real, 5> v12{17.f, 6.f, 2.f, 1.f, 97.f};
        REQUIRE(Math::equal(v11.distance(v12), Math::sqrt(110)));
        REQUIRE(Math::equal(v11.distanceSquare(v12), real(110)));
    }

    SECTION("Vec angle between two normalized vector test") {
    }

    SECTION("Vec3 angle between two vector test") {
    }

    SECTION("Vec2 angle between two normalized vector test") {
    }

    SECTION("Vec2 angle between two vector test") {
    }

    SECTION("Vec2 slerp test") {
    }

    SECTION("Vec2 slerp2 test") {
    }

    SECTION("Vec2 slerp3 test") {
    }
}