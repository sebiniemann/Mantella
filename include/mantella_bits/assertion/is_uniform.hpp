#pragma once

template <typename T1, typename T2, std::size_t N>
bool is_uniform(
    const typename std::array<T1, N>::const_iterator data,
    const T1 lower_bound,
    const T1 upper_bound,
    const T2 number_of_bins,
    const std::function<bool(const T1 first, const T2 second)> is_equal);

template <typename T1, typename T2, std::size_t N>
bool is_uniform(
    const typename std::array<T1, N>::const_iterator data,
    const T1 lower_bound,
    const T1 upper_bound,
    const T2 number_of_bins);

template <typename T, std::size_t N>
bool is_uniform(
    const typename std::array<T, N>::const_iterator data,
    const T lower_bound,
    const T upper_bound);

template <typename T, std::size_t N>
bool is_uniform(
    const typename std::array<T, N>::const_iterator data);
    
//
// Implementation
//

template <typename T1, typename T2, std::size_t N>
bool is_uniform(
    const typename std::array<T1, N>::const_iterator data,
    const T1 lower_bound,
    const T1 upper_bound,
    const T2 number_of_bins) {
  static_assert(std::is_floating_point<T1>::value, "The data, lower and upper bounds must be floating points.");
  static_assert(std::is_integral<T2>::value, "The number of bins must be an integer.");
  static_assert(N > 0, "The number of data elements must be at greater than 0.");
  assert(lower_bound <= upper_bound);
  assert(number_of_bins > 0);
  
  // Checks whether all elements are within the lower and upper bound and returns false if any element is out of bound.
  const auto& minmax = std::minmax_element(data, std::next(data, N));
  if (minmax.first < lower_bound || upper_bound < minmax.second) {
    return false;
  }
  
  // Sing elemental data 
  if (data.size() == 1) {
    return true;
  }
  
  std::vector<T1> histogram(number_of_bins, 0);
  for (const auto : data) {
    ++histogram[std::floor(static_cast<T1>(number_of_bins) * (data - lower_bound) / (upper_bound - lower_bound)];
  }
  
  return (std::accumulate(histogram.cbegin(), histogram.cend(), 0, [](const T1 sum, const T1 element) { return sum + std::log(element / static_cast<T1>(N)) * (element / static_cast<T1>(N)); }) > std::log(N) * 0.9);
}

template <typename T1, typename T2, std::size_t N>
bool is_uniform(
    const typename std::array<T1, N>::const_iterator data,
    const T1 lower_bound,
    const T1 upper_bound,
    const T2 number_of_bins) {
  return is_uniform(data, lower_bound, upper_bound, number_of_bins, [](const T1 first, const T1 second) {first});
}

template <typename T1, typename T2, typename T3, std::size_t N>
bool is_uniform(
    const typename std::array<T1, N>::const_iterator data,
    const T2 lower_bound,
    const T3 upper_bound) {
  return is_uniform(data, lower_bound, upper_bound, 10);
}

template <typename T, std::size_t N>
bool is_uniform(
    const typename std::array<T, N>::const_iterator data) {
  const auto& minmax = std::minmax_element(data, std::next(data, N));
  return is_uniform(data, minmax.first, minmax.second);
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS) 
TEST_CASE("is_uniform", "[is_uniform]") {
  CHECK(is_uniform({}) == true);
  CHECK(is_uniform({}) == false);
#endif