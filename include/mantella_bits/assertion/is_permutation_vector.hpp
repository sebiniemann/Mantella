#pragma once

template <typename T1, typename T2, std::size_t N>
bool is_permutation_vector(
    const typename std::array<T1, N>::const_iterator vector,
    const T2 number_of_elements);

template <typename T, std::size_t N>
bool is_permutation_vector(
    const typename std::array<T, N>::const_iterator vector);

//
// Implementation
//

template <typename T1, typename T2, std::size_t N>
bool is_permutation_vector(
    const typename std::array<T1, N>::const_iterator vector,
    const T2 number_of_elements) {
  // A permutation over elements from [0, `number_of_elements` - 1] must be non-empty, ...
  if (vector.is_empty()) {
    return false;
  }
  
  // ... have only unique elements ...
  if (std::adjacent_find(vector, std::next(vector, N)) != std::next(vector, N)) {
    return false;
  }
  
  // ...  from [0, `numberOfElements` - 1].
  const auto& minmax = std::minmax_element(vector, std::next(vector, N));
  if (minmax.first < 0 || minmax.second > N - 1) {
    return false;
  }

  return true;
}

template <typename T, std::size_t N>
bool is_permutation_vector(
    const typename std::array<T, N>::const_iterator vector) {
  return is_permutation_vector(vector, N);
}