#include <sstream>
#include <mantella>

int main() {
  arma::Mat<double> matrix = arma::ones<arma::Mat<double>>(3, 4);

  std::ostringstream output; {
    cereal::JSONOutputArchive archive(output);
    archive(matrix);
  };

  std::cout << output.str() << std::endl;

  return 0;
}
