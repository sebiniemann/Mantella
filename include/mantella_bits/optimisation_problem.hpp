#pragma once

/**
Optimisation Problem
====================

.. cpp:class:: template <T, N> optimisation_problem

  .. versionadded:: 1.x

  The :cpp:class:`optimisation_problem` struct is the base of every `optimisation problem <https://en.wikipedia.org/wiki/Optimization_problem>`_ and stores

  * a set of objective functions (:cpp:member:`objective_functions`),
  * the acceptable lower (:cpp:member:`lower_bounds`) and upper bounds (:cpp:member:`upper_bounds`)

  The template parameter ``T`` defines the type used by all internal containers. The parameter ``N`` stands for the number of dimensions.

  **This struct itself has no member functions and only aggregates data**, to describe an optimisation problem. It is then used in :cpp:func:`evaluate` to compute the sum over all objective functions for a single parameter and in :cpp:func:`optimise` to find a parameter - within the defined bounds - that minimises the result of :cpp:func:`evaluate`.

  While `hard-constraints <https://en.wikipedia.org/wiki/Constrained_optimization>`_ that are not in the shape of an interval (like :math:`\text{minimise } f(x), \text{such that } x^2 < 1`) are not supported by Mantella, `soft-constraints/penalty functions <https://en.wikipedia.org/wiki/Constrained_optimization>`_ can be used by adding them to :cpp:class:`objective_functions`. In general, we also advice to prefer soft-constraints to hard-constraints, as soft-constraints give hints about how far we are off from the acceptable parameter space, aiding solvers in finding constraint satisfying parameters.
  
  .. code-block:: c++

    #include <mantella0>
    
    int main() {
      constexpr std::size_t number_of_dimensions = 2;
      mant::optimisation_problem<double, number_of_dimensions> optimisation_problem;
      optimisation_problem.objective_functions = {{
        [](
            const auto begin_parameter,
            const auto end_parameter) {
          return std::accumulate(begin_parameter, end_parameter);
        },
        "My awesome objective function, the sum over all elements *eye roll*"
      }, {
        [](
            const auto begin_parameter,
            const auto end_parameter) {
          static_assert(number_of_dimensions > 0, "The number of dimensions must be greater than 0 ... what were you even expecting?!");
          
          return *begin_parameter < 0.0 ? -*begin_parameter : 0.0;
        },
        "My soft-constraint/penalty function, giving every parameter starting with a negative number a worse/higher value, relative to its magnitude."
      }};
      
      std::cout << "The objective value of (-2.0, 3.0) is: " << mant::evaluate(optimisation_problem, {-2.0, 3.0}) << std::endl;

      static_assert(number_of_dimensions > 0, "The number of dimensions must still be greater than 0 ... please stop doing this to me ...");
      
      // Lets avoid parameters starting with a negative number completely and remove the (then obsolete) penalty function as well.
      optimisation_problem.lower_bound[0] = 0.0;
      optimisation_problem.objective_functions.pop_back();
      
      std::cout << "The best found objective value (using  random search) was: " << mant::optimise(optimisation_problem, mant::random_search).best_found_objective_value << std::endl;
      
      return 0;
    }

  In addition to the aforementioned member variables, it also stores variables to

  * permute (:cpp:member:`parameter_permutation`),
  * scale (:cpp:member:`parameter_scaling`),
  * translate (:cpp:member:`parameter_translation`) and 
  * rotate (:cpp:member:`parameter_rotation`) 

  the parameter, as well as attributes to

  * scale (:cpp:member:`objective_value_scaling`) and 
  * translate (:cpp:member:`objective_value_translation`)

  the objective value.

  These variables are used in :cpp:func:`evaluate` and affect the parameter prior to be evaluated the objective value after summation. They are set to their neutral element as default (therefore leaving the parameter and objective value unchanged) and come in handy to test for parameter / objective value biases in optimisation algorithms.
  
  .. note::

    If you want to extend this struct, make sure to call its constructor :cpp:func:`optimisation_problem()`, ensuring that all attributes are set to their default value. Otherwise you should at least initialise the variables handling the parameter and objective value adjustments, to avoid funny results when calling :cpp:func:`evaluate`.
   
  .. cpp:type:: std::function<T(begin_parameter, end_parameter)> objective_function_t
     
    Defines the type of an objective function, being an actual ``std::function`` instance that requires a parameter given by two iterators and returns a single objective value.
     
    **Parameters**
     
      * **begin_parameter** (``std::array<T, N>::const_iterator``) - An iterator referring to the first element within the parameter.
      * **end_parameter** (``std::array<T, N>::const_iterator``) - An iterator referring to the *past-the-end* element within the parameter.

    **Returns**
     
      ``T`` - The computed objective value.

  .. cpp:member:: std::vector<std::pair<objective_function_t, std::string>> objective_functions

    .. versionadded:: 1.x
   
    Stores a vector of pairs of objective functions (:cpp:type:`objective_function_t`), together with a string describing each objective function.

    The string - despite being a human readable representation - can also be used to recognise an objective function later one, especially when the position within the vector is not known.

    The attribute is empty as default.

  .. cpp:member:: std::array<T, N> lower_bounds

    .. versionadded:: 1.x

    Stores the lower bounds. 
    
    The attribute is set to ``(-10.0, .., -10.0)`` as default.

  .. cpp:member:: std::array<T, N> upper_bounds

    .. versionadded:: 1.x

    Stores the upper bounds. 
    
    The attribute is set to ``(10.0, .., 10.0)`` as default.

  .. cpp:member:: std::array<std::size_t, N> parameter_permutation

    .. versionadded:: 1.x

    Stores the parameter permutation. 
    
    The attribute is set to ``(0, 1, ..., N-1)`` as default.

  .. cpp:member:: std::array<T, N> parameter_scaling

    .. versionadded:: 1.x

    Stores the parameter scaling.
    
    The attribute is set to ``(1.0, ..., 1.0)`` as default.

  .. cpp:member:: std::array<T, N> parameter_translation

    .. versionadded:: 1.x

    Stores the parameter translation.
    
    The attribute is set to ``(0.0, ..., 0.0)`` as default.

  .. cpp:member:: std::array<T, N*N> parameter_rotation

    .. versionadded:: 1.x

    Stores the parameter rotation.
    
    The attribute contains a flatten identity matrix (concatenating its columns) as default.

  .. cpp:member:: T objective_value_scaling

    .. versionadded:: 1.x

    Stores the objective value scaling.
    
    The attribute it set to ``1.0`` as default.

  .. cpp:member:: T objective_value_translation

    .. versionadded:: 1.x
    
    The attribute it set to ``0.0`` as default.

  .. cpp:function:: constexpr optimisation_problem() noexcept

    .. versionadded:: 1.x

    The only constructor defined, sets all attributes to their aforementioned default value.
*/
template <typename T, std::size_t N>
struct optimisation_problem {
  typedef std::function<T(const typename std::array<T, N>::const_iterator begin_parameter, const typename std::array<T, N>::const_iterator end_parameter)> objective_function_t;

  std::vector<std::pair<objective_function_t, std::string>> objective_functions;
  std::array<T, N> lower_bounds;
  std::array<T, N> upper_bounds;
  std::array<std::size_t, N> parameter_permutation;
  std::array<T, N> parameter_scaling;
  std::array<T, N> parameter_translation;
  std::array<T, N*N> parameter_rotation;
  T objective_value_scaling;
  T objective_value_translation;
  
  constexpr optimisation_problem() noexcept;
};

//
// Implementation
//

template <typename T, std::size_t N>
constexpr optimisation_problem<T, N>::optimisation_problem() noexcept {
  static_assert(std::is_floating_point<T>::value, "The type must be a floating point");
  static_assert(N > 0, "The number of dimensions mus be greater than 0.");
  
  lower_bounds.fill(-10.0);
  upper_bounds.fill(10.0);
  
  parameter_rotation.fill(0.0);
  for (std::size_t n = 0; n < N; n++) {
    parameter_permutation[n] = n;
    parameter_rotation[n * N + n] = 1.0;
  }
  
  parameter_scaling.fill(1.0);
  parameter_translation.fill(0.0);
  objective_value_scaling = 1.0;
  objective_value_translation = 0.0;
}

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("optimisation_problem", "[optimisation_problem]") {
  mant::optimisation_problem<double, 2> optimisation_problem;
  
  const std::array<double, 2> lower_bounds = {-10.0, -10.0};
  CHECK(optimisation_problem.lower_bounds == lower_bounds);
  
  const std::array<double, 2> upper_bounds = {10.0, 10.0};
  CHECK(optimisation_problem.upper_bounds == upper_bounds);
  
  const std::array<std::size_t, 2> parameter_permutation = {0, 1};
  CHECK(optimisation_problem.parameter_permutation == parameter_permutation);
  
  const std::array<double, 2> parameter_scaling = {1.0, 1.0};
  CHECK(optimisation_problem.parameter_scaling == parameter_scaling);
  
  const std::array<double, 2> parameter_translation = {0.0, 0.0};
  CHECK(optimisation_problem.parameter_translation == parameter_translation);
  
  const std::array<double, 4> parameter_rotation = {1.0, 0.0, 0.0, 1.0};
  CHECK(optimisation_problem.parameter_rotation == parameter_rotation);
  
  CHECK(optimisation_problem.objective_value_scaling == Approx(1.0));
  CHECK(optimisation_problem.objective_value_translation == Approx(0.0));
}
#endif