#pragma once

/**

*/
template <
  typename T,
  std::size_t number_of_dimensions>
struct nelder_mead_method_state : optimisation_algorithm_state<T, number_of_dimensions> {
  std::array<std::pair<std::array<T, number_of_dimensions>, T>, number_of_dimensions + 1> simplex;
  std::array<T, number_of_dimensions> centroid;
  bool update_whole_simplex;
  
  nelder_mead_method_state() noexcept;
};

/**

*/
template <
  typename T1,
  std::size_t number_of_dimensions,
  template <class, std::size_t> class T2 = nelder_mead_method_state>
struct nelder_mead_method : optimisation_algorithm<T1, number_of_dimensions, T2> {
  T1 reflection_weight;
  T1 expansion_weight;
  T1 contraction_weight;
  T1 shrinking_weight;

  nelder_mead_method() noexcept;
};

//
// Implementation
//

template <
  typename T,
  std::size_t number_of_dimensions>
nelder_mead_method_state<T, number_of_dimensions>::nelder_mead_method_state() noexcept
    : optimisation_algorithm_state<T, number_of_dimensions>::optimisation_algorithm_state() {
  static_assert(std::is_floating_point<T>::value, "");
  static_assert(number_of_dimensions > 0, "");
}

template <
  typename T1,
  std::size_t number_of_dimensions,
  template <class, std::size_t> class T2>
nelder_mead_method<T1, number_of_dimensions, T2>::nelder_mead_method() noexcept 
    : optimisation_algorithm<T1, number_of_dimensions, T2>() {
  static_assert(std::is_floating_point<T1>::value, "");
  static_assert(number_of_dimensions > 0, "");
  static_assert(std::is_base_of<nelder_mead_method_state<T1, number_of_dimensions>, T2<T1, number_of_dimensions>>::value, "");
  
  this->next_parameters_functions = {{
    [this](auto& state) {
      if (state.update_whole_simplex) {
        // Fills the simplex with the previous parameters and their corresponding objective values.
        std::transform(
          state.parameters.cbegin(), state.parameters.cend(),
          state.objective_values.cbegin(),
          state.simplex.begin(),
          [](const auto parameter, const auto objective_value) {
            return std::make_pair(parameter, objective_value);
          }
        );
      }
      
      if (state.update_whole_simplex) {
        // Sorts the simplex points in an ascending order, based on their objective value.
        std::sort(
          state.simplex.begin(), state.simplex.end(),
          [](const auto& simplex, const auto& other_simplex){
            return std::get<1>(simplex) < std::get<1>(other_simplex);
          });
      }
      
      if (state.update_whole_simplex) {
        // Calculates the centroid of all simplex point, excepts the worst one.
        state.centroid.fill(T1(0.0));
        std::for_each(
          state.simplex.cbegin(), std::prev(state.simplex.cend()),
          [this, &state](const auto& point) {
            for (std::size_t n = 0; n < this->active_dimensions.size(); ++n) {
              state.centroid.at(n) += std::get<0>(point).at(n) / number_of_dimensions;
            }
          });
      }
      
      } else {
        // Selects the best previously evaluated parameter.
        const auto best_found_objective_value = std::min_element(
          state.objective_values.cbegin(), state.objective_values.cend(),
          [](const auto& objective_value, const auto& other_objective_value) {
            return objective_value < other_objective_value;
          });
        
        if (*best_found_objective_value < std::get<1>(std::get<number_of_dimensions>(simplex))) {
          // Update the simplex and centroid
          auto position = std::find_if(
            state.simplex.cbegin(), std::prev(state.simplex.cend()),
            [best_found_objective_value](const auto& point) {
              return *best_found_objective_value < std::get<1>(point);
            });
            
          std::copy_backward(position, std::prev(state.simplex.cend()), state.simplex.end());
          *position = std::make_pair(state.parameters.at(0), state.objective_values.at(0));
        
          // *Nots:* The centroid is based on all points except the worst one, so we will only update it the parameter
          // is at least better than the second worst one.
          if () {
            
          }
        } else {
          // Shrinks the simplex.
          std::for_each(
            state.simplex.cbegin(), std::prev(state.simplex.cend()),
            [this, &state](const auto& point) {
              std::transform(
                state.centroid.cbegin(), std::next(state.centroid.cbegin(), this->active_dimensions.size()),
                std::get<0>(std::get<number_of_dimensions>(state.simplex)).cbegin(),
                state.parameters.at(0).begin(),
                [this](const auto centroid_element, const auto sample_element) {
                  return centroid_element + reflection_weight * (centroid_element - sample_element);
                });
            });
        }
      }
        
      state.parameters.resize(3);
      // Computes the reflected point.
      std::transform(
        state.centroid.cbegin(), std::next(state.centroid.cbegin(), this->active_dimensions.size()),
        std::get<0>(std::get<number_of_dimensions>(state.simplex)).cbegin(),
        state.parameters.at(0).begin(),
        [this](const auto centroid_element, const auto sample_element) {
          return centroid_element + reflection_weight * (centroid_element - sample_element);
        });
      // Computes the expanded point.
      std::transform(
        state.centroid.cbegin(), std::next(state.centroid.cbegin(), this->active_dimensions.size()),
        std::get<0>(std::get<number_of_dimensions>(state.simplex)).cbegin(),
        state.parameters.at(0).begin(),
        [this, &state](const auto centroid_element, const auto sample_element) {
          return centroid_element + reflection_weight * (state.parameters - centroid_element - sample_element);
        });
      // Computes the contracted point.
      std::transform(
        state.centroid.cbegin(), std::next(state.centroid.cbegin(), this->active_dimensions.size()),
        std::get<0>(std::get<number_of_dimensions>(state.simplex)).cbegin(),
        state.parameters.at(0).begin(),
        [this](const auto centroid_element, const auto sample_element) {
          return centroid_element + reflection_weight * (centroid_element - sample_element);
        });
    },
    "Nelder-Mead method next parameters"
  }};
  
  this->reflection_weight = T1(1.0);
  this->expansion_weight = T1(1.0);
  this->contraction_weight = T1(0.5);
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("nelder_mead_method", "[nelder_mead_method]") {
  typedef double value_type;
  constexpr std::size_t number_of_dimensions = 3;
  mant::nelder_mead_method<value_type, number_of_dimensions> nelder_mead_method;
  mant::nelder_mead_method_state<value_type, number_of_dimensions> nelder_mead_method_state;
  
  SECTION("Default values") {
    CHECK(nelder_mead_method.reflection_weight == 1.0);
    CHECK(nelder_mead_method.expansion_weight == 1.0);
    CHECK(nelder_mead_method.contraction_weight == 0.5);
  }
}
#endif