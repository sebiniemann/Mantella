#include <mantella>
#include <iostream>

int main() {
  for (const auto& twoSetsPartition : mant::getTwoSetsPartitions(6)) {
    std::cout << "{ ";
    for (const auto value : twoSetsPartition.first) {
      std::cout << value << " ";
    }
    std::cout << "}, { ";
    for (const auto value : twoSetsPartition.second) {
      std::cout << value << " ";
    }
    std::cout << "}" << std::endl;
  }
  
  return 0;
}