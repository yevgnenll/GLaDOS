#include <catch2/catch_all.hpp>

#include "memory/FixedSizeMemoryPool.hpp"
#include "memory/ThreadLocalAllocator.hpp"
#include <thread>

using namespace GLaDOS;

TEST_CASE("FixedSizeMemoryPool unit test", "[FixedSizeMemoryPool]") {
  struct Person {
    Person(int age, std::string name, std::string haircolor) : mAge{age}, mName{std::move(name)}, mHairColor{std::move(haircolor)} {}
    ~Person() {}

    int mAge;
    std::string mName;
    std::string mHairColor;
  };

  SECTION("default constructor test") {
    FixedSizeMemoryPool<Person> personPool;
    Person* p1 = personPool.allocate(18, "Peter", "brown");
    Person* p2 = personPool.allocate(20, "Alice", "blond");

    REQUIRE(p1->mAge == 18);
    REQUIRE(p1->mName == "Peter");
    REQUIRE(p1->mHairColor == "brown");
    REQUIRE(p2->mAge == 20);
    REQUIRE(p2->mName == "Alice");
    REQUIRE(p2->mHairColor == "blond");

    personPool.deallocate(p1);
    personPool.deallocate(p2);
  }

  SECTION("thread local allocator test") {
    ThreadLocalAllocator<Person> tlsPool;
    std::function<void(int)> fn = [&tlsPool](int count) {
      for (int i = 0; i < count; i++) {
        Person* p1 = tlsPool.allocate(18, "Peter", "brown");
        Person* p2 = tlsPool.allocate(20, "Alice", "blond");

        REQUIRE(p1->mAge == 18);
        REQUIRE(p1->mName == "Peter");
        REQUIRE(p1->mHairColor == "brown");
        REQUIRE(p2->mAge == 20);
        REQUIRE(p2->mName == "Alice");
        REQUIRE(p2->mHairColor == "blond");

        tlsPool.deallocate(p1);
        tlsPool.deallocate(p2);
      }
    };

    std::thread t1(fn, 2);
    std::thread t2(fn, 2);

    t1.join();
    t2.join();
  }
}