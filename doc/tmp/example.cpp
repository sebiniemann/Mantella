
        
          #include <mantella0>
          #include <iostream> // Used for std::cout
          #include <iterator> // Used for std::ostream_iterator
          
          int main() {
            mant::problem<double, 3> my_problem;
            // Only the first and third dimension are active.
            const std::array<unsigned, 2> active_dimensions = {0, 2};
            my_problem.objective_function = [&active_dimensions](const auto& parameter) {
              double objective_value = 0.0;
              for (unsigned n = 0; n < active_dimensions.size(); ++n) {
                // Make sure this computation is actually a heavy-weight. Otherwise, the additional memory lookups and 
                // code complexity might not be worth it.
                // How can you ensure this? Benchmarking, benchmarking and benchmarking ;)
                objective_value += std::pow(parameter.at(active_dimensions.at(n)), 2.0);
              }
              return objective_value;
            };

            // Optimises your problem. Selects and tunes the optimiser automatically at each invocation.
            const auto&& result = mant::optimise(my_problem);

            // Prints out the best parameter and its objective value
            std::copy(result.parameter.cbegin(), result.parameter.cend(), std::ostream_iterator<double>(std::cout, " "));
            std::cout << "-> " << result.objective_value << std::endl;
            
            return 0;
          }
  