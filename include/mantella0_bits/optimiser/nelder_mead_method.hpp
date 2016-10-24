/**

*/
template <typename T1, std::size_t N, template <class, std::size_t> class T2>
struct nelder_mead_method : optimiser<T1, N, T2> {
  T1 reflection_weight;
  T1 expansion_weight;
  T1 contraction_weight;
  T1 shrinking_weight;

  nelder_mead_method() noexcept;
};

//
// Implementation
//

template <typename T1, std::size_t N, template <class, std::size_t> class T2>
nelder_mead_method<T1, N, T2>::nelder_mead_method() noexcept 
    : optimiser<T1, N, T2>(),
      reflection_weight(T1(1.0)),
      expansion_weight(T1(1.0)),
      contraction_weight(T1(0.5)),
      shrinking_weight(T1(0.5)) {
  this->optimisation_function = [this](const T2<T1, N>& problem, std::vector<std::array<T1, N>> initial_parameters) {
    assert(initial_parameters.size() == N + 1);
    assert(reflection_weight > T1(0.0));
    assert(expansion_weight > T1(0.0));
    assert(contraction_weight > T1(0.0));
    assert(shrinking_weight > T1(0.0));
    
    optimise_result<T1, N> result;
    
    std::array<std::pair<std::array<T1, N>, T1>, N + 1> simplex;
    for (const auto& parameter : initial_parameters) {
      const auto objective_value = problem.objective_function(parameter);
      
      simplex.at() = {parameter, objective_value};
      
      if (objective_value < result.best_objective_value) {
        result.best_parameter = parameter;
        result.best_objective_value = objective_value;
      }
    }
    
    std::sort(
      simplex.begin(), simplex.end(),
      [](const auto& simplex, const auto& other_simplex){
        return std::get<1>(simplex) < std::get<1>(other_simplex);
      });
    
    std::array<T1, N> centroid;
    centroid.fill(T1(0.0));
    std::for_each(
      simplex.cbegin(), std::prev(simplex.cend()),
      [&centroid](const auto& point) {
        for (std::size_t n = 0; n < this->active_dimensions.size(); ++n) {
          centroid.at(n) += std::get<0>(point).at(n) / static_cast<T1>(N);
        }
      });
    
    for (; result.number_of_evaluations < this->maximal_number_of_evaluations && result.best_objective_value > this->acceptable_objective_value; ++result.number_of_evaluations) {
      

    
    if (state.update_whole_simplex) {
      // Calculates the centroid of all simplex point, excepts the worst one.
      
    }
    
    } else {
      // Selects the best previously evaluated parameter.
      const auto best_found_objective_value = std::min_element(
        state.objective_values.cbegin(), state.objective_values.cend(),
        [](const auto& objective_value, const auto& other_objective_value) {
          return objective_value < other_objective_value;
        });
      
      if (*best_found_objective_value < std::get<1>(std::get<N>(simplex))) {
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
              std::get<0>(std::get<N>(state.simplex)).cbegin(),
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
      std::get<0>(std::get<N>(state.simplex)).cbegin(),
      state.parameters.at(0).begin(),
      [this](const auto centroid_element, const auto sample_element) {
        return centroid_element + reflection_weight * (centroid_element - sample_element);
      });
    // Computes the expanded point.
    std::transform(
      state.centroid.cbegin(), std::next(state.centroid.cbegin(), this->active_dimensions.size()),
      std::get<0>(std::get<N>(state.simplex)).cbegin(),
      state.parameters.at(0).begin(),
      [this, &state](const auto centroid_element, const auto sample_element) {
        return centroid_element + reflection_weight * (state.parameters - centroid_element - sample_element);
      });
    // Computes the contracted point.
    std::transform(
      state.centroid.cbegin(), std::next(state.centroid.cbegin(), this->active_dimensions.size()),
      std::get<0>(std::get<N>(state.simplex)).cbegin(),
      state.parameters.at(0).begin(),
      [this](const auto centroid_element, const auto sample_element) {
        return centroid_element + reflection_weight * (centroid_element - sample_element);
      });
  };
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("nelder_mead_method", "[nelder_mead_method]") {
  const mant::nelder_mead_method<double, 3, mant::problem> optimiser;
  
  CHECK(optimiser.reflection_weight == Approx(1.0));
  CHECK(optimiser.expansion_weight == Approx(1.0));
  CHECK(optimiser.contraction_weight == Approx(0.5));
  CHECK(optimiser.shrinking_weight == Approx(0.5));
  
  const std::array<std::unique_ptr<mant::problem<double, 3>>, 5> problems = {
    std::unique_ptr<mant::problem<double, 3>>(new mant::ackley_function<double, 3>),
    std::unique_ptr<mant::problem<double, 3>>(new mant::rastrigin_function<double, 3>),
    std::unique_ptr<mant::problem<double, 3>>(new mant::rosenbrock_function<double, 3>),
    std::unique_ptr<mant::problem<double, 3>>(new mant::sphere_function<double, 3>),
    std::unique_ptr<mant::problem<double, 3>>(new mant::sum_of_different_powers_function<double, 3>)
  };
  
  std::array<mant::optimise_result<double, 3>, problems.size()> results;
  std::transform(
    problems.cbegin(), problems.cend(),
    results.begin(),
    [&optimiser](auto&& problem) {
      return optimiser.optimisation_function(*problem, {{5.0, 5.0, 5.0}});
    }
  );
  
  std::cout << "Nelder-Mead method" << std::endl;
  for (auto&& result : results) {
    std::cout << "best_parameter: [ ";
    for (auto&& element : result.best_parameter) {
      std::cout << element << " ";
    }
    std::cout << "], best_objective_value: " << result.best_objective_value
              << ", number_of_evaluations: " << result.number_of_evaluations << std::endl;
  }
}
#endif