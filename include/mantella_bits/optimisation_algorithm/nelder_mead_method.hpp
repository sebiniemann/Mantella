#pragma once

/**

*/
template <
  typename T,
  std::size_t number_of_dimensions>
struct nelder_mead_method_state : optimisation_algorithm_state<T, number_of_dimensions> {
  enum class phase_type {
    CENTROID,
    REFLECTION,
    EXPANSION,
    CONTRACTION
  };
  
  std::array<std::pair<std::array<T, number_of_dimensions>, T>, number_of_dimensions + 1> simplex;
  
  std::array<T, number_of_dimensions> centroid;
  std::array<T, number_of_dimensions> reflected_point;
  
  phase_type phase;
  
  constexpr nelder_mead_method_state() noexcept;
};

/**

*/
template <
  typename T1,
  std::size_t number_of_dimensions,
  template <class, std::size_t> class T2 = nelder_mead_method_state>
struct nelder_mead_method : optimisation_algorithm<T1, number_of_dimensions, T2> {
  using typename optimisation_algorithm<T1, number_of_dimensions, T2>::state_type;
  using typename optimisation_algorithm<T1, number_of_dimensions, T2>::value_type;
  
  T1 reflection_weight;
  T1 expansion_weight;
  T1 contraction_weight;
  T1 shrinking_weight;

  constexpr nelder_mead_method() noexcept;
};

//
// Implementation
//

template <
  typename T,
  std::size_t number_of_dimensions>
constexpr nelder_mead_method_state<T, number_of_dimensions>::nelder_mead_method_state() noexcept
    : optimisation_algorithm_state<T, number_of_dimensions>::optimisation_algorithm_state() {
  static_assert(std::is_floating_point<T>::value, "");
  static_assert(number_of_dimensions > 0, "");
  
  phase = state_type::phase_type::CENTROID;
}

template <
  typename T1,
  std::size_t number_of_dimensions,
  template <class, std::size_t> class T2>
constexpr nelder_mead_method<T1, number_of_dimensions, T2>::nelder_mead_method() noexcept 
    : optimisation_algorithm<T1, number_of_dimensions, T2>() {
  static_assert(std::is_floating_point<T1>::value, "");
  static_assert(number_of_dimensions > 0, "");
  static_assert(std::is_base_of<nelder_mead_method_state<T1, number_of_dimensions>, T2<T1, number_of_dimensions>>::value, "");
  
  // this->next_parameters_functions = {{
    // [this](auto& state) {
      // // Ensure that there are enough sample points for the simplex
    // },
    // ""
  // }};
  
  /*
  1. Sort (x_i, f(x_i)) pairs, such that f(x_1) < ... < f(x_n+1)
  
  
  
  
  
  
  
  
  
  
  */
  
  
  this->next_parameters_functions = {{
    [this](auto& state) {
      if (state.used_number_of_iterations > 1) {
        assert(state.parameters.size() == 1);
      } else {
        assert(state.parameters.size() == number_of_dimensions + 1);
        
        // Fills the simplex
        std::transform(
          state.parameters.cbegin(), state.parameters.cend(),
          state.objective_values.cbegin(),
          state.simplex.begin(),
          [](const auto parameter, const auto objective_value) {
            return std::make_pair(parameter, objective_value);
          }
        );
        
        // Sorts the simplex
        std::sort(
          state.simplex.begin(), state.simplex.end(),
          [](const auto& simplex, const auto& other_simplex){
            return std::get<1>(simplex) < std::get<1>(other_simplex);
          });
      }
      
      switch (state.phase) {
        case state_type::phase_type::CENTROID: {
          // Calculates the simplex's centroid, excluding the worst sample.
          state.centroid.fill(T1(0.0));
          std::for_each(
            state.simplex.cbegin(), std::prev(state.simplex.cend()),
            [this, &state](const auto& sample) {
              for (std::size_t n = 0; n < this->active_dimensions.size(); ++n) {
                state.centroid.at(n) += std::get<0>(sample).at(n) / number_of_dimensions;
              }
            });
            
          // Calculates the reflected point.
          std::transform(
            state.centroid.cbegin(), std::next(state.centroid.cbegin(), this->active_dimensions.size()),
            std::get<0>(std::get<number_of_dimensions>(state.simplex)).cbegin(),
            state.reflected_point.begin(),
            [this](const auto centroid_element, const auto sample_element) {
              return centroid_element + reflection_weight * (centroid_element - sample_element);
            });
            
          state.phase = state_type::phase_type::REFLECTION;
        } break;
        case state_type::phase_type::REFLECTION: {
          if (state.objective_value.at(0) < std::get<1>(std::get<0>(state.simplex))) {
            
            state.phase = state_type::phase_type::EXPANSION;
          } else if (state.objective_value.at(0) < std::get<1>(std::get<number_of_dimensions - 1>(state.simplex))) { 
            auto position = std::find_if(
              std::next(state.simplex.cbegin()), std::prev(state.simplex.cend()),
              [&state](const auto& sample) {
                return state.objective_value.at(0) < std::get<1>(sample);
              });
            std::copy_backward(position, std::prev(simplex.cend()), simplex.cend());
            *position = std::make_pair(state.parameters.at(0), state.objective_value.at(0));
          } else {
            
            state.phase = state_type::phase_type::CONTRACTION;
          }
        } break;
        case state_type::phase_type::EXPANSION: {
          
        } break;
        case state_type::phase_type::CONTRACTION: {
          state.phase = state_type::phase_type::REFLECTION;
        }
      }
    },
    "downhill simplex method"
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