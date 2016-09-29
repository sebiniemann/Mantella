#pragma once

/**

*/
template <typename T, std::size_t number_of_dimensions>
std::pair<std::vector<std::array<T, number_of_dimensions>>, std::vector<T>> elitists(
    const std::vector<std::array<T, number_of_dimensions>>& parameters,
    const std::vector<T>& objective_values,
    const std::size_t number_of_elitists_to_select);

//
// Implementation
//

template <typename T, std::size_t number_of_dimensions>
std::pair<std::vector<std::array<T, number_of_dimensions>>, std::vector<T>> elitists(
    const std::vector<std::array<T, number_of_dimensions>>& parameters,
    const std::vector<T>& objective_values,
    const std::size_t number_of_elitists_to_select) {
  assert(number_of_dimensions >= number_of_elitists_to_select && "elitists: The number of parameters must be greater than or equal to the number of samples to select.");
  assert(parameters.size() == objective_values.size() && "elitists: The number of parameters must be equal to the number of objective values.");

  if (number_of_elitists_to_select == 0) {
    return {};
  }

  std::vector<std::size_t> indicies(objective_values.size());
  std::iota(indicies.begin(), indicies.end(), 0);
  std::partial_sort(indicies.begin(), indicies.begin() + number_of_elitists_to_select, indicies.end(), [&objective_values](const auto i, const auto j) {return objective_values[i] < objective_values[j];});
  
  std::pair<std::vector<std::array<T, number_of_dimensions>>, std::vector<T>> elitists;
  elitists.first.reserve(number_of_elitists_to_select);
  elitists.second.reserve(number_of_elitists_to_select);
  
  for (std::size_t n = 0; n < number_of_elitists_to_select; ++n) {
    elitists.first.push_back(parameters[indicies[n]]);
    elitists.second.push_back(objective_values[indicies[n]]);
  }
  
  return elitists;
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("elitists", "[samples_selection][elitists]") {
  std::vector<std::array<double, 2>> best_parameters;
  std::vector<double> best_objective_values;
  std::tie(best_parameters, best_objective_values) = mant::elitists<double, 2>({{0.0, -3.0}, {2.0, -3.0}}, {-1.0, 4.0}, 0);
  CHECK(best_parameters.empty() == true);
  CHECK(best_objective_values.empty() == true);
  
  std::tie(best_parameters, best_objective_values) = mant::elitists<double, 2>({{2.0, -3.0}, {0.0, -3.0}, {2.0, -1.0}}, {3.0, -1.0, -4.0}, 2);
  CHECK((best_parameters == std::vector<std::array<double, 2>>({{2.0, -1.0}, {0.0, -3.0}})));
  CHECK((best_objective_values == std::vector<double>({-4.0, -1.0})));
}
#endif