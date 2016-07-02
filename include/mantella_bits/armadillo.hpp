#pragma once

// Armadillo
#include <armadillo>

// Mantella
#include "mantella_bits/randomNumberGenerator.hpp"

namespace mant {
  class Hash {
   public:
    arma::uword operator()(
        const arma::vec& key) const;
  };

  class IsEqual {
   public:
    bool operator()(
        const arma::vec& firstKey,
        const arma::vec& secondKey) const;
  };
  
  arma::mat randu(
      const arma::uword numberOfRows,)
      const arma::uword numberOfColumns)
    arma::mat randomMatrix(numberOfRows, numberOfColumns);
    auto distribution = std::bind(uniform_real_distribution<double>, Rng::getGenerator());
    vector<int> vec(10);
    generate(begin(vec), end(vec), gen);
    return randomMatrix;
  }
  
  //matrix, generic elems
  template<typename obj_type>
  static obj_type randu(const arma::uword numberOfRows, const arma::uword numberOfCols, const typename arma::arma_Mat_Col_Row_only<obj_type>::result* junk = 0) {
    arma_ignore(junk);

    if(arma::is_Col<obj_type>::value == true)
      {
      arma::arma_debug_check( (numberOfCols != 1), "randu(): incompatible size" );
      }
    else
    if(arma::is_Row<obj_type>::value == true)
      {
      arma::arma_debug_check( (numberOfRows != 1), "randu(): incompatible size" );
      }

    arma::Mat<typename obj_type::elem_type> matrix(numberOfRows,numberOfCols);
    for(arma::uword i = 0; i < numberOfRows; i++) {
      for(arma::uword j = 0; j < numberOfCols; j++) {
        matrix(i,j) = randu<typename obj_type::elem_type>();
      }
    }
    return matrix;
  }
  
    
  //mat
  const arma::mat randu(const arma::uword n_rows, const arma::uword n_cols) {
    return randu<arma::mat>(n_rows,n_cols);
  }

  //mat
  const arma::mat randu(const arma::SizeMat& s) {
    return randu<arma::mat>(s.n_rows, s.n_cols);
  }
  
  //generic mat
  template<typename obj_type>
  obj_type randu(const arma::SizeMat& s, const typename arma::arma_Mat_Col_Row_only<obj_type>::result* junk = 0) {
    arma_ignore(junk);

    return randu<obj_type>(s.n_rows, s.n_cols);
  }
  
  /////////////////////////RANDN
  //double
  double randn(){
    std::mt19937_64& generator = Rng::getGenerator(omp_get_thread_num());
    std::normal_distribution<double> distribution;
    return distribution(generator);
  }

  //generic scalar
  template<typename eT>
  typename arma::arma_scalar_only<eT>::result randn() {
    std::mt19937_64& generator = Rng::getGenerator(omp_get_thread_num());
    std::normal_distribution<double> distribution;
    return eT(distribution(generator));
  }
  
  //col or row, generic elems
  template<typename obj_type>
  static obj_type randn(const arma::uword numberOfElements, const arma::arma_empty_class junk1 = arma::arma_empty_class(), const typename arma::arma_Mat_Col_Row_only<obj_type>::result* junk2 = 0) {
    arma_ignore(junk1);
    arma_ignore(junk2);

    if(arma::is_Row<obj_type>::value == true) {
      arma::Row<typename obj_type::elem_type> vector(numberOfElements);
      for(arma::uword i = 0; i < vector.n_elem; i++) {
        vector(i) = randn<typename obj_type::elem_type>();
      }
      return vector;
    } else {
      arma::Col<typename obj_type::elem_type> vector(numberOfElements);
      for(arma::uword i = 0; i < vector.n_elem; i++) {
        vector(i) = randn<typename obj_type::elem_type>();
      }
      return vector;
    }
  }
  
  //matrix, generic elems
  template<typename obj_type>
  static obj_type randn(const arma::uword numberOfRows, const arma::uword numberOfCols, const typename arma::arma_Mat_Col_Row_only<obj_type>::result* junk = 0) {
    arma_ignore(junk);

    if(arma::is_Col<obj_type>::value == true)
      {
      arma::arma_debug_check( (numberOfCols != 1), "randn(): incompatible size" );
      }
    else
    if(arma::is_Row<obj_type>::value == true)
      {
      arma::arma_debug_check( (numberOfRows != 1), "randn(): incompatible size" );
      }

    arma::Mat<typename obj_type::elem_type> matrix(numberOfRows,numberOfCols);
    for(arma::uword i = 0; i < numberOfRows; i++) {
      for(arma::uword j = 0; j < numberOfCols; j++) {
        matrix(i,j) = randn<typename obj_type::elem_type>();
      }
    }
    return matrix;
  }
  
    
  //mat
  const arma::mat randn(const arma::uword n_rows, const arma::uword n_cols) {
    return randn<arma::mat>(n_rows,n_cols);
  }

  //mat
  const arma::mat randn(const arma::SizeMat& s) {
    return randn<arma::mat>(s.n_rows, s.n_cols);
  }
  
  //generic mat
  template<typename obj_type>
  obj_type randn(const arma::SizeMat& s, const typename arma::arma_Mat_Col_Row_only<obj_type>::result* junk = 0) {
    arma_ignore(junk);

    return randn<obj_type>(s.n_rows, s.n_cols);
  }
  
  /////////////////////////RANDI
  //double
  double randi(){
    std::mt19937_64& generator = Rng::getGenerator(omp_get_thread_num());
    std::uniform_int_distribution<int> distribution;
    return distribution(generator);
  }

  //generic scalar
  template<typename eT>
  typename arma::arma_scalar_only<eT>::result randi() {
    std::mt19937_64& generator = Rng::getGenerator(omp_get_thread_num());
    std::uniform_int_distribution<int> distribution;
    return eT(distribution(generator));
  }
  
  //col or row, generic elems
  template<typename obj_type>
  static obj_type randi(const arma::uword numberOfElements, const arma::arma_empty_class junk1 = arma::arma_empty_class(), const typename arma::arma_Mat_Col_Row_only<obj_type>::result* junk2 = 0) {
    arma_ignore(junk1);
    arma_ignore(junk2);

    if(arma::is_Row<obj_type>::value == true) {
      arma::Row<typename obj_type::elem_type> vector(numberOfElements);
      for(arma::uword i = 0; i < vector.n_elem; i++) {
        vector(i) = randi<typename obj_type::elem_type>();
      }
      return vector;
    } else {
      arma::Col<typename obj_type::elem_type> vector(numberOfElements);
      for(arma::uword i = 0; i < vector.n_elem; i++) {
        vector(i) = randi<typename obj_type::elem_type>();
      }
      return vector;
    }
  }
  
  //matrix, generic elems
  template<typename obj_type>
  static obj_type randi(const arma::uword numberOfRows, const arma::uword numberOfCols, const typename arma::arma_Mat_Col_Row_only<obj_type>::result* junk = 0) {
    arma_ignore(junk);

    if(arma::is_Col<obj_type>::value == true)
      {
      arma::arma_debug_check( (numberOfCols != 1), "randi(): incompatible size" );
      }
    else
    if(arma::is_Row<obj_type>::value == true)
      {
      arma::arma_debug_check( (numberOfRows != 1), "randi(): incompatible size" );
      }

    arma::Mat<typename obj_type::elem_type> matrix(numberOfRows,numberOfCols);
    for(arma::uword i = 0; i < numberOfRows; i++) {
      for(arma::uword j = 0; j < numberOfCols; j++) {
        matrix(i,j) = randi<typename obj_type::elem_type>();
      }
    }
    return matrix;
  }
  
    
  //mat
  const arma::mat randi(const arma::uword n_rows, const arma::uword n_cols) {
    return randi<arma::mat>(n_rows,n_cols);
  }

  //mat
  const arma::mat randi(const arma::SizeMat& s) {
    return randi<arma::mat>(s.n_rows, s.n_cols);
  }
  
  //generic mat
  template<typename obj_type>
  obj_type randi(const arma::SizeMat& s, const typename arma::arma_Mat_Col_Row_only<obj_type>::result* junk = 0) {
    arma_ignore(junk);

    return randi<obj_type>(s.n_rows, s.n_cols);
  }
}
