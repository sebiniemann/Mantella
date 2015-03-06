namespace mant {
  inline std::vector<std::pair<arma::Col<unsigned int>, arma::Col<unsigned int>>> getTwoSetsPartitions(
      const unsigned int& numberOfElements) noexcept;

  //
  // Implementation
  //

  inline std::vector<std::pair<arma::Col<unsigned int>, arma::Col<unsigned int>>> getTwoSetsPartitions(
      const unsigned int& numberOfElements) noexcept {
    arma::Col<unsigned int> elements = arma::linspace<arma::Col<unsigned int>>(0, numberOfElements - 1, numberOfElements);

    std::vector<std::pair<arma::Col<unsigned int>, arma::Col<unsigned int>>> partitions;

    for(std::size_t n = 1; n <= std::floor(numberOfElements / 2.0); ++n) {
      arma::Col<unsigned int> firstSet = elements.head(n);
      arma::Col<unsigned int> secondSet = elements.tail(elements.n_elem - n);

      partitions.push_back({firstSet, secondSet});

      arma::Col<unsigned int> counter(n, arma::fill::zeros);
      int index = counter.n_elem - 1;

      int lastIndex = -1;
      if(firstSet.n_elem == secondSet.size()) {
        lastIndex = 0;
      }

      while (index > lastIndex) {
        std::iter_swap(firstSet.begin() + index, secondSet.begin() + counter(index));
        partitions.push_back({firstSet, secondSet});

        if(counter(index) < secondSet.n_elem - 1) {
          counter.tail(counter.n_elem - index).fill(counter(index) + 1);
          index = counter.n_elem - 1;
        } else {
          --index;
        }
      }
    }

    return partitions;
  }
}
