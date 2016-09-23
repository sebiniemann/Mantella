
is_uniform
----------

.. cpp:function:: template <T1, T2, N1[, N2 = 10]> is_uniform(data, lower_bound, upper_bound[, p_value = 0.05])

  .. versionadded:: 1.x
  
  **Template parameters**
   
    * **T1** - The parameter, lower and upper bound type.
    * **T2** - The p-value type **(must be a floating point type)**.
    * **N1** (``std::size_t``) - The data size.
    * **N2** (``std::size_t``) - A number of bins **(must be greater than 0)**.
  
  **Function parameters**
   
    * **data** (``std::array<T, N1>``) - The data.
    * **lower_bound** (``T2``) - The lower bound.
    * **upper_bound** (``T3``) - The upper bound **(must be greater than or equal to the lower bound)**.
    * **p_value** (``T4``) - The p-value **(must be within [0, 1])**.

  **Returns**
   
    ``bool`` - True if the data is a uniform with ``N1 > 0`` and false otherwise.
    
  ``data`` is accepted as being uniform if and only if all elements are uniformly distributed within :math:`[\text{lower_bound}, \text{upper_bound}]` and there are no elements outside of these bounds.
  
  The uniformity is tested generating a histogram over ``data`` with ``N2`` bins :math:`(b_1, \ldots, b_\text{N2})`, equally sized over :math:`[\text{lower_bound}, \text{upper_bound}]` and performing an entropy-based uniformity-test, requiring that the following inequality holds true:

  .. math::

    \sum_{i = 1}^\text{N2} \log\bigg(\frac{b_i}{\text{N1}}\bigg) \frac{\text{data}[i]}{\text{N1}} > (1 - \text{p_value}) \log(\text{N2)}
  
  .. code-block:: c++
  
    #include <mantella0>
    
    int main() {
      constexpr std::size_t number_of_elements = 1000;
      std::array<double, number_of_elements> data;
      
      // Fills the data with uniformly distributed values.
      // Since we are accessing mant::random_number_generators() directly, first ensure that the element exists.
      // **Note:** The number of generator returned by mant::random_number_generators() is set by and equal to MANTELLA_MAXIMAL_NUMBER_OF_THREADS.
      static_assert(MANTELLA_MAXIMAL_NUMBER_OF_THREADS > 0, "Why should this even fail, what were you doing?");
      std::generate(data.begin(), data.end(), std::bind(std::uniform_real_distribution<double>(0.0, 1.0), std::ref(mant::random_number_generators()[0])));
      
      // Remember, data is randomly and uniformly drawn from [0, 1].
      std::cout << is_uniform(data) << "\n";
      std::cout << is_uniform(data, 0.5, 1.0) << "\n";
      // Increasing only the first halve by 1.
      std::transform(data.cbegin(), std::next(data.cbegin(), number_of_elements/2), data.begin(), [](const auto) { return auto + 1.0; });
      std::cout << is_uniform(data) << "\n";
      std::cout << std::endl;
      
      return 0;
    }
    
  .. note::
  
    The whole computation will be done in ``std::common_type<T1, T2, T3, T4>::type`` and cast the provided parameters if necessary. It is therefore advisable to ensure that ``T1 = T2 = T3 = T4`` to avoid casts at runtime.