#include <iostream>
#include <map>
#include <string>

#include "allocator.hpp"
#include "container.hpp"
#include "project.h"

constexpr int elementsCount = 10;
constexpr inline int factorial(int n) {
  if (n == 0 || n == 1) return 1;
  return n * factorial(n - 1);
}

int main(int argc, char const *argv[]) {
  struct ProjectInfo info = {};
  std::cout << info.nameString << "\t" << info.versionString << '\n';

  // 1.1

  std::map<int, int> defaultAllocatedMap;
  for (int i = 0; i < elementsCount; ++i) {
    defaultAllocatedMap[i] = factorial(i);
  }
  std::cout << "default map"
            << ": \n\t";
  for (auto &[key, value] : defaultAllocatedMap) {
    std::cout << key << " " << value << "\n\t";
  }
  std::cout << '\n';

  // 1.2

  using valueType = std::map<int, int>::value_type;
  using allocator = otus::Allocator<valueType, elementsCount>;
  std::map<int, int, std::less<>, allocator> poolMap;
  for (int i = 0; i < elementsCount; ++i) {
    poolMap[i] = factorial(i);
  }
  std::cout << "pool map"
            << ": \n\t";
  for (auto &&[id, val] : poolMap) {
    std::cout << id << " " << val << "\n\t";
  }
  std::cout << '\n';

  // 2.1

  otus::Vector<int> defaultAllocatedVector;
  for (int i = 0; i < elementsCount; ++i) {
    defaultAllocatedVector.push_back(i);
  }

  std::cout << "pool custom vector"
            << ": \n\t";
  for (auto &&value : defaultAllocatedVector) {
    std::cout << value << "\n\t";
  }
  std::cout << std::endl;

  // 2.2

  otus::Vector<int, otus::Allocator<int, elementsCount * 2>> poolVector;
  for (int i = 0; i < elementsCount; ++i) {
    poolVector.push_back(i);
  }

  std::cout << "pool custom vector"
            << ": \n\t";
  for (auto &&value : poolVector) {
    std::cout << value << "\n\t";
  }
  std::cout << std::endl;

  (void)argc;
  (void)argv;
  return 0;
}
