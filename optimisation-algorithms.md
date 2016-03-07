---
layout: page
---

{% include h1 title="Optimisation algorithms" %}

The `mant::OptimisationAlgorithm` class is the base of every implemented optimisation algorithm and provides methods to manage the termination criteria, handling the boundary constraints, as well as to retrieve the best solution found and information about the optimisation progress.

Each optimisation algorithm is thereby build upon the following skeleton:

``` text
call optimise(optimisationProblem, initialParameters):
  1. Set all counters (number of iterations and duration) to 0
  2. Initialise optimisation algorithm
  3. Boundaries handling
  4. Evaluate initial parameters
  5. While no termination criteria is met
    1. Check if the process is stagnating
      - If true: Restart/Reinitialise the algorithm and get new parameters
      - If false: Get the next parameters 
    2. Boundaries handling
    3. Evaluate all parameters

call evaluate(parameters):
  1. For each parameter in parameters
    1. Evaluate the objective value
    2. Update the best solution and duration
    3. Break the loop if any termination criteria is met
  2. Increase the number of iterations by 1
```

{% include api subset=page.url %}