#include <catch2/catch_test_macros.hpp>

#include "utils/FixedThreadPool.hpp"

using namespace GLaDOS;

Vector<int> testSuite;
void task1(int a, int b) {
  for (int i = 0; i < 100; i++) {
    int sum = a + b + i;
    testSuite.push_back(sum);
  }
};

TEST_CASE("FixedThreadPool unit tests", "[FixedThreadPool]") {
  SECTION("ThreadPool submit bool task with future test") {
    FixedThreadPool pool;
    std::vector<std::future<bool>> futures;
    std::size_t poolSize = pool.getThreadPoolSize();
    for (int i = 0; i < poolSize; i++) {
      futures.push_back(std::move(pool.execute(task1, 1, 2)));
    }
    for (auto& f : futures) {
      f.wait();
    }
    for (auto& f : futures) {
      REQUIRE(f.get() == true);
    }
    REQUIRE(testSuite.size() == 100 * poolSize);
  }
}