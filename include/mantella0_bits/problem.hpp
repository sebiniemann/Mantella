/**
Problems
========

.. cpp:class:: problem

  .. versionadded:: 1.0.0

  Defining an optimisation problem is the first of two step necessary to solve a problem using Mantella (the second one is calling :cpp:any:`optimise` to *solve* the problem).

  Recall that a continuous black-box/derivative-free optimisation problem can be denoted as

  .. math::

    \begin{align}
      \underset{x \in R^N}{\text{minimise}} & \ f(x)                  \\
      \text{subject to}                     & \ l_i \leq x_i \leq u_i
    \end{align}

  , where ``f(x)`` is the objective function to be optimised over the ``N``-dimensional parameter ``x`` and ``l_i, u_i`` the ``i``-th lower/upper bound.

  Similar to this, :cpp:any:`problem` requires you to define

  - the objective function ``f(x)`` to be minimised (``.objective_function``) and
  - the lower (``.lower_bounds``) and upper bounds (``.upper_bounds``)

  The lower and upper bounds are set to ``[0, 1]^n`` as default, while the objective function is initially unset.

  .. container:: topic

    Using existing functions from your code base

    The most common case is to start from a pre-existing code base, having already some function to be minimised.

    There are many reasons that simply setting ``.objective_function = your_function`` might not work. While simpler cases may be solved by ``.objective_function = std::bind(your_function, ..., std::placeholders::_1, ...)``, you can use a lamdba function as wrapper around ``your_function`` for more difficult cases.

    .. code-block:: c++

      #include <mantella0>
      #include <iostream> // Used for std::cout

      // Assumes more than one output parameter.
      struct your_results {
        double objective_value;
        std::size_t other_information;
      };

      // Also assumes multiple input parameter, with the parameters under optimisation being different
      // from std::array<T, N>.
      your_results your_function(
          std::size_t other_information,
          std::vector<double> first_parameter,
          double second_parameter) {
        your_results results;
        results.objective_value =
          std::accumulate(first_parameter.cbegin(), first_parameter.cend(), 0.0) *
          second_parameter;
        results.other_information = other_information;

        return results;
      }

      int main() {
        // The additional input information.
        std::size_t other_information = 100;

        mant::problem<double, 3> problem;

        // Additional information is captured, the parameter is split into two parts, one being converted
        // to *std::vector* and one of the results is returned as objective value.
        problem.objective_function = [&other_information](const std::array<double, 3>& parameter) {
          return your_function(
            other_information,
            std::vector<double>(parameter.cbegin(), std::prev(parameter.cend())),
            std::get<2>(parameter)
          ).objective_value;
        };

        std::cout << "f(1.0, 2.0, 3.0) = " << problem.objective_function({1.0, 2.0, 3.0}) << std::endl;

        return 0;
      }

  .. container:: topic

    Adding more (non-linear) constraints

    Another common case is having more constraints than just bounded ones (sometimes also called box- or interval-constraints).

    Picking up the above problem denotion, we can extend it for arbitrary constraints as

    .. math::

      \begin{align}
        \underset{x \in R^N}{\text{minimise}} & \ f(x) + \underset{x \in R^N}{\max}(f(x)) g(x) \\
        \text{subject to}                     & \ l_i \leq x_i \leq u_i
      \end{align}

    , where ``g(x)`` is the sum of all additional constraints. Note that ``g(x)`` is weighted by the maximal objective value (``f(x)``) within the bounds (larger values will also be sufficient, it must not be the actual supremum). This ensures that non-satisfying parameters will have a worse objective value then satisfying ones.

    By reducing the constraint value, as the parameter comes closer to the satisfying parameter space, the optimiser can be guided to quicker solve the `constraint satisfaction problem <https://en.wikipedia.org/wiki/Constraint_satisfaction_problem>`_.

    Note that ``g(x)`` should be minimal (if not simply be ``0``) for satisfying parameter.

    .. code-block:: c++

      #include <mantella0>
      #include <iostream> // Used for std::cout

      int main() {
        // A optimisation problem with soft-constraints
        mant::problem<double, 3> problem;
        problem.objective_function = [](const std::array<double, 3>& parameter) {
          // The actual objective value.
          double objective_value = std::accumulate(parameter.cbegin(), parameter.cend(), 0.0);
          // An additional constraint, x_1 must be greater than or equal to x_2 * x_3.
          // The added value will become smaller as the first parameter elements come closer to satisfy the
          // constraint and be 0 for all satisfying parameters.
          objective_value += std::max(
            0.0,
            10.0 * std::get<1>(parameter) * std::get<2>(parameter) - std::get<0>(parameter));

          return objective_value;
        };

        std::cout << "f(1.0, 2.0, 3.0) = " << problem.objective_function({1.0, 2.0, 3.0}) << std::endl;

        return 0;
      }

  .. list-table:: Template parameters

    * - T

        Any floating point type
      - The value type of the parameter and objective value.
    * - N

        ``std::size_t``
      - The number of dimensions.

        Must be within ``[1, std::numeric_limits<std::size_t>::max()]``.

        For cases where the number of dimensions can be lowered during run-time (for example separable problems), the
        :cpp:any:`optimiser` can be told to focus on specific domains by specifying ``.active_dimensions``.

        You can also reduce the computational cost for time-consuming objective value calculations, by capturing the
        active dimensions:

        .. code-block:: c++

          #include <mantella0>
          #include <iostream> // Used for std::cout
          #include <iterator> // Used for std::ostream_iterator

          int main() {
            constexpr std::size_t number_of_dimensions = 3;
            const std::vector<std::size_t> active_dimensions = {0, 2};

            mant::problem<double, number_of_dimensions> problem;
            // Only the first and third dimension are active.
            // Captures *active_dimensions*.
            problem.objective_function = [&active_dimensions](const auto& parameter) {
              double objective_value = 0.0;
              for (std::size_t n = 0; n < active_dimensions.size(); ++n) {
                // Make sure this computation is actually a heavy-weight. Otherwise, the additional memory
                // lookup and code complexity might not be worth it.
                // How to ensure this? Benchmarking, benchmarking and benchmarking ;)
                objective_value += parameter.at(active_dimensions.at(n));
              }
              return objective_value;
            };
            problem.lower_bounds.fill(-5.0);
            problem.upper_bounds.fill(5.0);

            mant::hooke_jeeves_algorithm<double, number_of_dimensions> optimiser;
            // Explicitly tells the optimiser to ignore the second dimension.
            // This can also be done if the objective value is inexpensive to compute, as it speeds up
            // the optimisation process by reducing the search space's dimensionality.
            optimiser.active_dimensions = active_dimensions;

            // The best parameter will be (-5, x, -5) with objective value -10. *x* can anything, as the
            // second dimension is inactive.
            const auto&& result = mant::optimise(problem, optimiser);
            std::copy(result.parameter.cbegin(), result.parameter.cend(), std::ostream_iterator<double>(std::cout, " "));
            std::cout << "-> " << result.objective_value << std::endl;

            return 0;
          }

  .. list-table:: Member variables

    * - objective_function

        ``std::function``
      - The function to be optimised. Calculates an objective value for the given parameter.

        This variable is empty as default. Therefore, it must be set, before a :cpp:any:`problem` can be optimised.

        **Function parameters**

        - **parameter** ``(std::array<T, N>)``

          A parameter (usually generated by an :cpp:any:`optimiser`), for which its objective value is requested.

          While the parameter can contain any value (even ``NaN``), it will be bounded within ``[lower_bounds, upper_bounds]`` if generated by Mantella's optimisers.

          As The parameter is a fixed-sized ``std::array``, it will always contain ``N`` elements. However, if the :cpp:any:`optimiser`'s ``.active_dimensions`` was set, Mantella's optimisers will only optimise the active dimensions. The other dimensions may contain any value, which won't change until the optimisation processes (i.e. ``.optimisation_function``) finishes.

        **Returns** ``(T)``

          The parameter's objective value.

          As most :cpp:any:`optimiser` use this value for ranking purpose, avoiding non-comparable values (like ``NaN``) is critical.

    * - lower_bounds

        ``std::array<T, N>``
      - The search space's lower bounds.

        Must be lesser than or equal to the upper ones.

        Since most :cpp:any:`optimiser` integrate the bounds into their calculations, infinite values should be avoided as boundary.

        The default lower bound is ``(0, 0, ..., 0)``.

    * - upper_bounds

        ``std::array<T, N>``
      - The search space's upper bounds.

        Must be greater than or equal to the lower ones.

        Since most :cpp:any:`optimiser` integrate the bounds into their calculations, infinite values should be avoided as boundary.

        The default upper bound is ``(1, 1, ..., 1)``.

  .. list-table:: Member functions

    * - problem

        Constructor
      - Initializes all member variables to their default value.

        Will never throw an exception.

Below is a list of common problems we implemented, to quickly get a feeling for the library, without the need to implement your own problem yet. These problems are also used in our benchmarks to compare against other optimisation libraries.

.. include:: problem/ackley_function.include
.. include:: problem/rastrigin_function.include
.. include:: problem/rosenbrock_function.include
.. include:: problem/sphere_function.include
.. include:: problem/sum_of_different_powers.include
*/
template <typename T, std::size_t N>
struct problem {
  std::function<T(
      const std::array<T, N>& parameter)> objective_function;

  std::array<T, N> lower_bounds;
  std::array<T, N> upper_bounds;

  constexpr problem() noexcept;
};

//
// Implementation
//

template <typename T, std::size_t N>
constexpr problem<T, N>::problem() noexcept {
  static_assert(std::is_floating_point<T>::value, "");
  static_assert(N > 0, "");

  // These bounds could be anything as default, as they need user-given, context-dependent knowledge to be useful.
  // However, since we map the bounds later on internally to [0, 1] to ease maintainability in our optimisation algorithms, we use [0, 1] as default.
  lower_bounds.fill(T(0.0));
  upper_bounds.fill(T(1.0));
}

//
// Unit tests
//

#if defined(MANTELLA_BUILD_TESTS)
TEST_CASE("problem", "[problem]") {
  const mant::problem<double, 2> problem;

  CHECK((problem.lower_bounds == std::array<double, 2>({0.0, 0.0})));
  CHECK((problem.upper_bounds == std::array<double, 2>({1.0, 1.0})));
}
#endif
