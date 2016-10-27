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
      expansion_weight(T1(2.0)),
      contraction_weight(T1(0.5)),
      shrinking_weight(T1(0.5)) {
  this->optimisation_function = [this](const T2<T1, N>& problem, const std::vector<std::array<T1, N>>& initial_parameters) {
    assert(initial_parameters.size() == N + 1);
    assert(reflection_weight > T1(0.0));
    assert(expansion_weight > T1(0.0));
    assert(contraction_weight > T1(0.0));
    assert(shrinking_weight > T1(0.0));
    
    auto&& start_time  = std::chrono::steady_clock::now();
    optimise_result<T1, N> result;
    
    result.best_parameter = initial_parameters.at(0);
    result.best_objective_value = problem.objective_function(initial_parameters.at(0));
    ++result.number_of_evaluations;
    result.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start_time);
        
    if (result.best_objective_value <= this->acceptable_objective_value) {
      return result;
    } else if (result.number_of_evaluations >= this->maximal_number_of_evaluations) {
      return result;
    } else if (result.duration >= this->maximal_duration) {
      return result;
    }

    std::array<std::pair<std::array<T1, N>, T1>, N> simplex;
    for (std::size_t n = 1; n < initial_parameters.size(); ++n) {
      const auto& parameter = initial_parameters.at(n);
      const auto objective_value = problem.objective_function(parameter);
      ++result.number_of_evaluations;
      result.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start_time);
      
      simplex.at(n - 1) = {parameter, objective_value};
      
      if (objective_value < result.best_objective_value) {
        result.best_parameter = parameter;
        result.best_objective_value = objective_value;
        
        if (result.best_objective_value <= this->acceptable_objective_value) {
          return result;
        }
      }
      
      if (result.number_of_evaluations >= this->maximal_number_of_evaluations) {
        return result;
      } else if (result.duration >= this->maximal_duration) {
        return result;
      }
    }
    
    std::sort(
      simplex.begin(), simplex.end(),
      [](const auto& simplex, const auto& other_simplex){
        return std::get<1>(simplex) < std::get<1>(other_simplex);
      });
    
    std::array<T1, N> centroid = result.best_parameter;
    std::for_each(
      simplex.cbegin(), std::prev(simplex.cend()),
      [this, &centroid](const auto& point) {
        for (std::size_t n = 0; n < this->active_dimensions.size(); ++n) {
          centroid.at(n) += std::get<0>(point).at(n) / static_cast<T1>(N);
        }
      });
    
    while (result.duration < this->maximal_duration && result.number_of_evaluations < this->maximal_number_of_evaluations && result.best_objective_value > this->acceptable_objective_value) {
      std::array<T1, N> reflected_point;
      std::transform(
        centroid.cbegin(), std::next(centroid.cbegin(), this->active_dimensions.size()),
        result.best_parameter.cbegin(),
        reflected_point.begin(),
        [this](const auto centroid, const auto best_parameter) {
          return centroid + reflection_weight * (centroid - best_parameter);
        });
      
      auto objective_value = problem.objective_function(reflected_point);
      ++result.number_of_evaluations;
      result.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start_time);
      
      if (objective_value < result.best_objective_value) {
        for (std::size_t n = 0; n < N; ++n) {
          centroid.at(n) += (reflected_point.at(n) - result.best_parameter.at(n)) / static_cast<T1>(N);
        }
        
        result.best_parameter = reflected_point;
        result.best_objective_value = objective_value;
        
        if (result.best_objective_value <= this->acceptable_objective_value) {
          return result;
        } else if (result.number_of_evaluations >= this->maximal_number_of_evaluations) {
          return result;
        } else if (result.duration >= this->maximal_duration) {
          return result;
        }
        
        std::array<T1, N> expanded_point;
        std::transform(
          centroid.cbegin(), std::next(centroid.cbegin(), this->active_dimensions.size()),
          reflected_point.cbegin(),
          expanded_point.begin(),
          [this](const auto centroid, const auto reflected_point) {
            return centroid + expansion_weight * (reflected_point - centroid);
          });
          
        objective_value = problem.objective_function(expanded_point);
        ++result.number_of_evaluations;
        result.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start_time);
        
        if (objective_value < result.best_objective_value) {
          for (std::size_t n = 0; n < N; ++n) {
            centroid.at(n) += (expanded_point.at(n) - result.best_parameter.at(n)) / static_cast<T1>(N);
          }
        
          result.best_parameter = expanded_point;
          result.best_objective_value = objective_value;
        }
      
        continue;
      }
      
      if (result.number_of_evaluations >= this->maximal_number_of_evaluations) {
        return result;
      } else if (result.duration >= this->maximal_duration) {
        return result;
      }
      
      if (objective_value < std::get<1>(std::get<N-1>(simplex))) {
        auto position = std::find_if(
          simplex.begin(), std::prev(simplex.end()),
          [objective_value](const auto& point) {
            return objective_value < std::get<1>(point);
          });
          
        for (std::size_t n = 0; n < N; ++n) {
          centroid.at(n) += (reflected_point.at(n) - std::get<0>(*position).at(n)) / static_cast<T1>(N);
        }
          
        std::copy_backward(position, std::prev(simplex.end()), simplex.end());
        *position = {reflected_point, objective_value};
      } else {
        std::array<T1, N> contracted_point;
        std::transform(
          centroid.cbegin(), std::next(centroid.cbegin(), this->active_dimensions.size()),
          std::get<0>(std::get<N-1>(simplex)).cbegin(),
          contracted_point.begin(),
          [this](const auto centroid, const auto worst_parameter) {
            return centroid + contraction_weight * (worst_parameter - centroid);
          });
        
        auto objective_value = problem.objective_function(contracted_point);
        ++result.number_of_evaluations;
        result.duration = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - start_time);
        
        if (objective_value < result.best_objective_value) {
          for (std::size_t n = 0; n < N; ++n) {
            centroid.at(n) += (contracted_point.at(n) - result.best_parameter.at(n)) / static_cast<T1>(N);
          }
        
          result.best_parameter = contracted_point;
          result.best_objective_value = objective_value;
        } else if (objective_value < std::get<1>(std::get<N-1>(simplex))) {
          auto position = std::find_if(
            simplex.begin(), std::prev(simplex.end()),
            [objective_value](const auto& point) {
              return objective_value < std::get<1>(point);
            });
            
          for (std::size_t n = 0; n < N; ++n) {
            centroid.at(n) += (contracted_point.at(n) - std::get<0>(*position).at(n)) / static_cast<T1>(N);
          }
            
          std::copy_backward(position, std::prev(simplex.end()), simplex.end());
          *position = {contracted_point, objective_value};
        } else {
          for (auto& point : simplex) {
            std::transform(
              result.best_parameter.cbegin(), std::next(result.best_parameter.cbegin(), this->active_dimensions.size()),
              std::get<0>(point).cbegin(),
              std::get<0>(point).begin(),
              [this](const auto best_parameter, const auto point) {
                return best_parameter + shrinking_weight * (point - best_parameter);
              });
              
            point = {std::get<0>(point), problem.objective_function(std::get<0>(point))};
            ++result.number_of_evaluations;
      
            if (result.number_of_evaluations >= this->maximal_number_of_evaluations) {
              return result;
            } else if (result.duration >= this->maximal_duration) {
              return result;
            }
          }
          
          std::sort(
            simplex.begin(), simplex.end(),
            [](const auto& simplex, const auto& other_simplex){
              return std::get<1>(simplex) < std::get<1>(other_simplex);
            });
          
          std::array<T1, N> centroid = result.best_parameter;
          std::for_each(
            simplex.cbegin(), std::prev(simplex.cend()),
            [this, &centroid](const auto& point) {
              for (std::size_t n = 0; n < this->active_dimensions.size(); ++n) {
                centroid.at(n) += std::get<0>(point).at(n) / static_cast<T1>(N);
              }
            });
        }
      }
    }
    
    return result;
  };
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("nelder_mead_method", "[nelder_mead_method]") {
  constexpr std::size_t number_of_dimensions = 3;
  const mant::nelder_mead_method<double, number_of_dimensions, mant::problem> optimiser; 
  
  SECTION("Default configuration") {
    CHECK(optimiser.reflection_weight == Approx(1.0));
    CHECK(optimiser.expansion_weight == Approx(2.0));
    CHECK(optimiser.contraction_weight == Approx(0.5));
    CHECK(optimiser.shrinking_weight == Approx(0.5));
  }
  
  SECTION("Benchmarking") {
    const std::array<std::unique_ptr<mant::problem<double, number_of_dimensions>>, 5> problems = {
      std::unique_ptr<mant::problem<double, number_of_dimensions>>(new mant::ackley_function<double, number_of_dimensions>),
      std::unique_ptr<mant::problem<double, number_of_dimensions>>(new mant::rastrigin_function<double, number_of_dimensions>),
      std::unique_ptr<mant::problem<double, number_of_dimensions>>(new mant::rosenbrock_function<double, number_of_dimensions>),
      std::unique_ptr<mant::problem<double, number_of_dimensions>>(new mant::sphere_function<double, number_of_dimensions>),
      std::unique_ptr<mant::problem<double, number_of_dimensions>>(new mant::sum_of_different_powers_function<double, number_of_dimensions>)
    };
    
    std::vector<std::array<double, number_of_dimensions>> initial_parameters(number_of_dimensions + 1);
    for (auto& parameter : initial_parameters) {
      std::generate(
        parameter.begin(), std::next(parameter.begin(), optimiser.active_dimensions.size()),
        std::bind(
          std::uniform_real_distribution<double>(0.0, 1.0),
          std::ref(random_number_generator())));
    }
    
    std::array<mant::optimise_result<double, number_of_dimensions>, problems.size()> results;
    std::transform(
      problems.cbegin(), problems.cend(),
      results.begin(),
      [&optimiser, &initial_parameters](auto&& problem) {
        return optimiser.optimisation_function(*problem, initial_parameters);
      }
    );
    
    std::cout << "Nelder-Mead method" << std::endl;
    for (auto&& result : results) {
      std::cout << "best_parameter: [ ";
      for (auto&& element : result.best_parameter) {
        std::cout << element << " ";
      }
      std::cout << "], best_objective_value: " << result.best_objective_value
                << ", number_of_evaluations: " << result.number_of_evaluations
                << ", duration: " << result.duration.count() << "ns" << std::endl;
    }
  }
  
  SECTION("Boundary handling") {
    mant::sphere_function<double, number_of_dimensions> problem;
    problem.lower_bounds.fill(0.5);
    
    std::vector<std::array<double, number_of_dimensions>> initial_parameters(number_of_dimensions + 1);
    for (auto& parameter : initial_parameters) {
      std::generate(
        parameter.begin(), std::next(parameter.begin(), optimiser.active_dimensions.size()),
        std::bind(
          std::uniform_real_distribution<double>(0.0, 1.0),
          std::ref(random_number_generator())));
    }
    
    const auto&& result = optimiser.optimisation_function(problem, initial_parameters);
    CHECK(std::all_of(
      result.best_parameter.cbegin(), std::next(result.best_parameter.cbegin(), optimiser.active_dimensions.size()),
      [](const auto elem){ 
        return elem >= 0.5;
      }
    ) == true);
  }
}
#endif