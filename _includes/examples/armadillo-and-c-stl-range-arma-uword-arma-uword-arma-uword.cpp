#include <mantella>

int main() {
  std::cout << mant::range(0 ,10).t() << std::endl;
  std::cout << mant::range(5, -10, 2).t() << std::endl;
  
  return 0;
}
