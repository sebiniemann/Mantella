#pragma once

/**

*/
template <typename T, std::size_t number_of_dimensions>
struct hill_climbing : optimisation_algorithm<T, number_of_dimensions> {
  T minimal_stepsize;
  T maximal_stepsize;
  
  constexpr hill_climbing() noexcept;
};

//
// Implementation
//

template <typename T, std::size_t number_of_dimensions>
constexpr hill_climbing<T, number_of_dimensions>::hill_climbing() noexcept 
    : optimisation_algorithm<T, number_of_dimensions>() {
  this->next_parameters_functions = {{
    [this](
        auto state) {
      assert(minimal_stepsize <= maximal_stepsize);
       
      std::generate(
        state.parameters.begin(),
        state.parameters.end(),
        [this, &state]() {
          return random_neighbour(
            state.best_found_parameter,
            minimal_stepsize,
            maximal_stepsize,
            this->active_dimensions.size());
        }
      );
      
      return state;
    },
    "Draws all parameters randomly and uniformly from a hyper-sphere (with hollow centre), centred at *best_found_paramter*, with outer radius *maximal_stepsize* and inner radius *minimal_stepsize*."
  }};
  
  this->minimal_stepsize = T(0.0);
  this->maximal_stepsize = T(0.1);
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("hill_climbing_state", "[hill_climbing][hill_climbing_state]") {
  // typename mant::hill_climbing<value_type, number_of_dimensions>::state_type hill_climbing_state;
  
  // SECTION("Default state") {
    // CHECK(hill_climbing_state.objective_values.empty() == true);
    // CHECK(hill_climbing_state.best_found_objective_value == std::numeric_limits<value_type>);
    // CHECK(hill_climbing_state.objective_values.empty() == true);
  // }
// }

// TEST_CASE("hill_climbing", "[hill_climbing]") {
  // constexpr std::size_t number_of_dimensions = 2;
  // typedef double value_type;
  
  // mant::hill_climbing<value_type, number_of_dimensions> hill_climbing;
  // typename mant::hill_climbing<value_type, number_of_dimensions>::state_type hill_climbing_state;
  
  // SECTION("Default parameters") {
    // CHECK(hill_climbing.minimal_stepsize == Approx(0.0));
    // CHECK(hill_climbing.maximal_stepsize == Approx(0.1));
  // }
  
  // SECTION("Functions") {
    // CHECK(hill_climbing.initialising_functions.size() == 0);
    
    // CHECK(hill_climbing.boundaries_handling_functions.size() == 1);
    // CHECK(hill_climbing.next_parameters_functions.at(0).second == "Draws all parameters randomly and uniformly from a hyper-sphere (with hollow centre), centred at *best_found_paramter*, with outer radius *maximal_stepsize* and inner radius *minimal_stepsize*.");
    
    // CHECK(hill_climbing.is_stagnating_functions.size() == 1);
    // CHECK(hill_climbing.next_parameters_functions.at(0).second == "Draws all parameters randomly and uniformly from a hyper-sphere (with hollow centre), centred at *best_found_paramter*, with outer radius *maximal_stepsize* and inner radius *minimal_stepsize*.");
    
    // CHECK(hill_climbing.next_parameters_functions.size() == 1);
    // CHECK(hill_climbing.next_parameters_functions.at(0).second == "Draws all parameters randomly and uniformly from a hyper-sphere (with hollow centre), centred at *best_found_paramter*, with outer radius *maximal_stepsize* and inner radius *minimal_stepsize*.");
    
    // CHECK(hill_climbing.restarting_functions.size() == 1);
    // CHECK(hill_climbing.next_parameters_functions.at(0).second == "Draws all parameters randomly and uniformly from a hyper-sphere (with hollow centre), centred at *best_found_paramter*, with outer radius *maximal_stepsize* and inner radius *minimal_stepsize*.");
  // }
  
  
  
  
  
    // std::vector<std::array<T, number_of_dimensions>> parameters;
  // std::vector<T> objective_values;
  
  // std::array<T, number_of_dimensions> best_found_parameter;
  // T best_found_objective_value;
  // std::size_t used_number_of_iterations;
  // std::size_t stagnating_number_of_iterations;
  
  
  // constexpr std::size_t number_of_repetitions = 10000;
  
  // mant::hill_climbing<double, 2> hill_climbing;
  
  // hill_climbing.minimal_stepsize = 1.0;
  // hill_climbing.maximal_stepsize = 3.0;
  
  // hill_climbing_state.best_found_parameter = {1.0, -2.0};
  
  // std::array<double, number_of_repetitions> lengths;
  // for (std::size_t n = 0; n < number_of_repetitions; ++n) {
    // auto&& neighbours = (hill_climbing.next_parameters_functions.at(0).first)(hill_climbing_state);
    // CHECK()
    
    // // Translate *neighbour* to be starting from the origin.
    // neighbour = {std::get<0>(neighbour) - std::get<0>(hill_climbing_state.best_found_parameter), std::get<1>(neighbour) - std::get<1>(hill_climbing_state.best_found_parameter)};
    
    // // Normalises all lengths and angles to be within [0, 1].
    // lengths.at(n) = (cblas_dnrm2(neighbour.size(), neighbour.data(), 1) - hill_climbing.minimal_stepsize) / (hill_climbing.maximal_stepsize - hill_climbing.minimal_stepsize);
    // assert(0.0 <= lengths.at(n) && lengths.at(n) <= 1.0);
  // }
  
  // // Performs an entropy-based uniformity test.
  // constexpr std::size_t number_of_bins = 10;
  // std::array<double, number_of_bins> histogram;
  // histogram.fill(0.0);
  // for (const auto length : lengths) {
    // // Ensures that the index will never be out-of-range (esp. in case *length* is 1).
    // histogram.at(std::min<std::size_t>(length * number_of_bins, number_of_bins - 1)) += 1.0 / static_cast<double>(number_of_repetitions);
  // }
  // CHECK(std::accumulate(histogram.cbegin(), histogram.cend(), 0.0, [](const double entropy, const double element) {return entropy + -std::log(element) * element;}) > std::log(number_of_bins) * 0.9);
}
#endif
