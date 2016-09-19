.. Mantella documentation master file, created by
   sphinx-quickstart on Mon Jul 11 09:01:51 2016.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Welcome to Mantella's documentation!
====================================

Contents:

.. toctree::
   
   
Code-Block Test
===============

.. code-block:: c++

   #include <mantella>

   int main() {
     constexpr number_of_dimensions = 2;

     mant::optimisation_problem<double, number_of_dimensions> optimisation_problem;
     optimisation_problem.objective_functions = 
       {{[optimisation_problem](
           const auto parameter) {
         return std::accumulate(parameter, std::next(parameter, optimisation_problem.number_of_dimensions), 0);
       },
       "My first objective function"},
       {[optimisation_problem](
           const auto parameter) {
         return std::accumulate(parameter, std::next(parameter, optimisation_problem.number_of_dimensions), 1, std::multiplies<double>());
       },
       "My second objective function"}};

     return 0;
   }
	
Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`

