#include "GLaDOS.h"

using namespace GLaDOS;

int main(int argc, char** argv) {
  std::atexit(&dumpMemory);
  return 0;
}