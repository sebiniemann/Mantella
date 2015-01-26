#if defined(MANTELLA_USE_PARALLEL)
// Catch
#include <catch.hpp>

// C++ Standard Library
#include <sstream>
#include <iostream>

// Mantella
#include <mantella>

TEST_CASE("cereal: save(Archive, arma::Mat<double>)", "") {
  SECTION("Serialises matrices elements in a JSON format.") {
    arma::Mat<double> matrix = arma::randn<arma::Mat<double>>(5, 5);

    std::ostringstream output; {
      cereal::JSONOutputArchive archive(output);
      archive(cereal::make_nvp("matrix", matrix));
    };

    std::cout << output.str() << std::endl;
  }

TEST_CASE("cereal: save(Archive, arma::Mat<unsigned int>)", "") {
  SECTION("Serialises matrices elements in a JSON format.") {
    arma::Mat<double> matrix = arma::randn<arma::Mat<double>>(5, 5);

    std::ostringstream output; {
      cereal::JSONOutputArchive archive(output);
      archive(cereal::make_nvp("matrix", matrix));
    };

    std::cout << output.str() << std::endl;
  }
}

TEST_CASE("cereal: load(Archive, arma::Mat<double>)", "") {
  SECTION("Deserialises matrices from a JSON format.") {

  }
}

TEST_CASE("cereal: load(Archive, arma::Mat<unsigned int>)", "") {
  SECTION("Deserialises matrices from a JSON format.") {

  }
}

TEST_CASE("cereal: save(Archive, arma::Col<double>)", "") {
  SECTION("Serialises vectors in a JSON format.") {
    arma::Col<double> columnVector = arma::randn<arma::Col<double>>(5, 5);

    std::ostringstream output; {
      cereal::JSONOutputArchive archive(output);
      archive(cereal::make_nvp("columnVector", columnVector));
    };

    std::cout << output.str() << std::endl;
  }

TEST_CASE("cereal: save(Archive, arma::Col<unsiged int>)", "") {
  SECTION("Serialises vectors in a JSON format.") {
    arma::Col<unsigned int> columnVector = arma::randn<arma::Col<unsigned int>>(5, 5);

    std::ostringstream output; {
      cereal::JSONOutputArchive archive(output);
      archive(cereal::make_nvp("columnVector", columnVector));
    };

    std::cout << output.str() << std::endl;
  }
}

TEST_CASE("cereal: load(Archive, arma::Col<double>)", "") {
  SECTION("Deserialises vectors from a JSON format.") {

  }
}

TEST_CASE("cereal: load(Archive, arma::Col<unsigned int>)", "") {
  SECTION("Deserialises vectors from a JSON format.") {

  }
}

TEST_CASE("cereal: save(Archive, arma::Row<double>)", "") {
  SECTION("Serialises vectors in a JSON format.") {
    arma::Row<double> rowVector = arma::randn<arma::Row<double>>(5, 5);

    std::ostringstream output; {
      cereal::JSONOutputArchive archive(output);
      archive(cereal::make_nvp("rowVector", rowVector));
    };

    std::cout << output.str() << std::endl;
  }

TEST_CASE("cereal: save(Archive, arma::Row<unsigned int>)", "") {
  SECTION("Serialises vectors in a JSON format.") {
    arma::Row<double> rowVector = arma::randn<arma::Row<double>>(5, 5);

    std::ostringstream output; {
      cereal::JSONOutputArchive archive(output);
      archive(cereal::make_nvp("rowVector", rowVector));
    };

    std::cout << output.str() << std::endl;
  }
}

TEST_CASE("cereal: load(Archive, arma::Row<double>)", "") {
  SECTION("Deserialises vectors from a JSON format.") {

  }
}

TEST_CASE("cereal: load(Archive, arma::Row<unsigned int>)", "") {
  SECTION("Deserialises vectors from a JSON format.") {

  }
}
#endif
