#include <catch2/catch.hpp>

#include "math/Point.hpp"
#include "math/Size.hpp"
#include "math/Rect.hpp"

using namespace GLaDOS;

TEST_CASE("Rect unit test", "[Rect]") {
    SECTION("Rect construct & copy") {
        Rect<real> r;
        REQUIRE(r.x == 0.f);
        REQUIRE(r.y == 0.f);
        REQUIRE(r.w == 1.f);
        REQUIRE(r.h == 1.f);

        Rect<real> r1 = {0, 1, 2, 3};
        REQUIRE(r1.x == 0.f);
        REQUIRE(r1.y == 1.f);
        REQUIRE(r1.w == 2.f);
        REQUIRE(r1.h == 3.f);

        Rect<real> r2 = r1;
        REQUIRE(r2.x == 0.f);
        REQUIRE(r2.y == 1.f);
        REQUIRE(r2.w == 2.f);
        REQUIRE(r2.h == 3.f);

        Rect<real> r3;
        r3 = r1;
        REQUIRE(r3.x == 0.f);
        REQUIRE(r3.y == 1.f);
        REQUIRE(r3.w == 2.f);
        REQUIRE(r3.h == 3.f);
    }

    SECTION("Rect operator") {
        Rect<real> r1 = {1, 2, 3, 4};
        Rect<real> r2 = {1, 2, 3, 4};
        REQUIRE(r1 == r2);

        r2.x = 2;
        REQUIRE(r1 != r2);

        Rect<real> r3 = r1 + r2;
        REQUIRE(r3.x == 3);
        REQUIRE(r3.y == 4);
        REQUIRE(r3.w == 6);
        REQUIRE(r3.h == 8);

        Rect<real> r4;
        r4 += r3;
        REQUIRE(r4.x == 3);
        REQUIRE(r4.y == 4);
        REQUIRE(r4.w == 7);
        REQUIRE(r4.h == 9);

        Rect<real> r5;
        Rect<real> r6 = r5 - r1;
        REQUIRE(r6.x == -1);
        REQUIRE(r6.y == -2);
        REQUIRE(r6.w == -2);
        REQUIRE(r6.h == -3);

        Rect<real> r7;
        r7 -= r1;
        REQUIRE(r7.x == -1);
        REQUIRE(r7.y == -2);
        REQUIRE(r7.w == -2);
        REQUIRE(r7.h == -3);

        Rect<real> r8;
        Rect<real> r9 = r8 * r1;
        REQUIRE(r9.x == 0);
        REQUIRE(r9.y == 0);
        REQUIRE(r9.w == 3);
        REQUIRE(r9.h == 4);

        Rect<real> r10;
        r10 *= r1;
        REQUIRE(r10.x == 0);
        REQUIRE(r10.y == 0);
        REQUIRE(r10.w == 3);
        REQUIRE(r10.h == 4);

        Rect<real> r11;
        Rect<real> r12 = r11 / r1;
        REQUIRE(r12.x == 0);
        REQUIRE(r12.y == 0);
        REQUIRE(r12.w == 1.f/3.f);
        REQUIRE(r12.h == 1.f/4.f);

        Rect<real> r13;
        r13 /= r1;
        REQUIRE(r13.x == 0);
        REQUIRE(r13.y == 0);
        REQUIRE(r13.w == 1.f/3.f);
        REQUIRE(r13.h == 1.f/4.f);
    }

    SECTION("Rect to size") {
        Rect<real> r1 = {1, 2, 3, 4};
        Size<real> s1 = r1.toSize();
        REQUIRE(s1.w == 3);
        REQUIRE(s1.h == 4);
    }

    SECTION("Rect merge") {
        // static merge test
        Rect<real> r1 = {1, 1, 3, 2};
        Rect<real> r2 = {3, 2, 3, 2};
        Rect<real> merge = Rect<real>::merge(r1, r2);
        REQUIRE(merge.x == 1);
        REQUIRE(merge.y == 1);
        REQUIRE(merge.w == 5);
        REQUIRE(merge.h == 3);

        // mutable make merge test
        Rect<real> r3;
        r3.makeMerge(r2);
        REQUIRE(r3.x == 0);
        REQUIRE(r3.y == 0);
        REQUIRE(r3.w == 6);
        REQUIRE(r3.h == 4);

        // minus w,h rect merge test
        Rect<real> r4 = {4, 3, -3, -2};
        Rect<real> merge2 = Rect<real>::merge(r4, r2);
        REQUIRE(merge2.x == 1);
        REQUIRE(merge2.y == 1);
        REQUIRE(merge2.w == 5);
        REQUIRE(merge2.h == 3);

        Rect<real> r5 = {6,4,-3,-2};
        Rect<real> merge3 = Rect<real>::merge(r1, r5);
        REQUIRE(merge3.x == 1);
        REQUIRE(merge3.y == 1);
        REQUIRE(merge3.w == 5);
        REQUIRE(merge3.h == 3);

        Rect<real> merge4 = Rect<real>::merge(r4, r5);
        REQUIRE(merge4.x == 1);
        REQUIRE(merge4.y == 1);
        REQUIRE(merge4.w == 5);
        REQUIRE(merge4.h == 3);
    }

    SECTION("Rect merge2") {
        Rect<real> r1 = {3,2,3,2};
        Rect<real> r2 = {5,1,4,2};
        Rect<real> merge = Rect<real>::merge(r1, r2);
        REQUIRE(merge.x == 3);
        REQUIRE(merge.y == 1);
        REQUIRE(merge.w == 6);
        REQUIRE(merge.h == 3);

        Rect<real> r3 = {6,4,-3,-2};
        Rect<real> r4 = {9,3,-4,-2};
        Rect<real> merge2 = Rect<real>::merge(r3, r4);
        REQUIRE(merge2.x == 3);
        REQUIRE(merge2.y == 1);
        REQUIRE(merge2.w == 6);
        REQUIRE(merge2.h == 3);
    }

    SECTION("Rect contains") {
        // rect contains point
        Rect<real> r1 = {3,2,3,2};
        Point<real> p1 = {4,3};
        Point<real> p2 = {6,4};
        Point<real> p3 = {3,2};
        Point<real> p4 = {2,3};
        Point<real> p5 = {6,5};

        REQUIRE(r1.contains(p1));
        REQUIRE(r1.contains(p2));
        REQUIRE(r1.contains(p3));
        REQUIRE(r1.contains(p4) == false);
        REQUIRE(r1.contains(p5) == false);

        // minus w, h rect contains point
        Rect<real> rr0 = {6,4, -3, -2};
        REQUIRE(rr0.contains(p1));
        REQUIRE(rr0.contains(p2));
        REQUIRE(rr0.contains(p3));
        REQUIRE(rr0.contains(p4) == false);
        REQUIRE(rr0.contains(p5) == false);

        // rect contains rect
        Rect<real> r2 = {1,1,3,2};
        Rect<real> r3 = {1,1,1,3};
        Rect<real> r4 = {1,1,1,1};
        Rect<real> r5 = {1,1,3,1};

        REQUIRE(r1.overlaps(r2));
        REQUIRE(r1.overlaps(r3) == false);
        REQUIRE(r1.overlaps(r4) == false);
        REQUIRE(r1.overlaps(r5));

        // minus w, h rect contains rect
        Rect<real> rr1 = {6,4, -3, -2};
        REQUIRE(rr1.overlaps(r2));
        REQUIRE(rr1.overlaps(r3) == false);
        REQUIRE(rr1.overlaps(r4) == false);
        REQUIRE(rr1.overlaps(r5));

        Rect<real> rr2 = {8,6, -3, -2};
        REQUIRE(rr1.overlaps(rr2));
    }

    SECTION("Rect contains2") {
        Rect<real> r1 = {3,2,3,2};
        Rect<real> r2 = {5,1,4,2};
        Point<real> p1 = {6,4};

        REQUIRE(r1.overlaps(r2));
        REQUIRE(r1.contains(p1));
    }

    SECTION("Rect intersection") {
        Rect<real> r1 = {3,2,3,2};
        Rect<real> r2 = {1,1,3,2};

        Rect<real> result = {3,2,1,1};
        REQUIRE(Rect<real>::intersection(r1, r2) == result);

        // minus w, h Rect
        Rect<real> r3 = {6,4,-3,-2};
        REQUIRE(Rect<real>::intersection(r3, r2) == result);

        // minus w, h Rect
        Rect<real> r4 = {4,3,-3,-2};
        REQUIRE(Rect<real>::intersection(r1, r4) == result);

        // both minus w, h Rect
        REQUIRE(Rect<real>::intersection(r3, r4) == result);

        // no intersection
        Rect<real> r5 = {1,1,2,2};
        Rect<real> r6 = {4,4,2,2};
        REQUIRE(Rect<real>::intersection(r5, r6) == Rect<real>::zero);
    }

    SECTION("Rect from Two points") {
        Rect<real> r = Rect<real>::fromPointSize(Point<real>{1, 1}, Size<real>{2, 3});
        REQUIRE(r.x == 1);
        REQUIRE(r.y == 1);
        REQUIRE(r.w == 2);
        REQUIRE(r.h == 3);
    }

    SECTION("Rect area") {
        Rect<real> r1 = {1,1,3,2};
        REQUIRE(r1.area() == 6);
    }
}