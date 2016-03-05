---
layout: page
---

{% include h1 title="Optimisation algorithms" %}

The `mant::OptimisationAlgorithm` class is the base of every implemented optimisation algorithm and provides methods to manage the termination criteria, handling the boundary constraints, as well as to retrieve the best solution found and information about the optimisation progress.

Each optimisation algorithm is thereby build upon the following skeleton:

``` text
call optimise(optimisationProblem, initialParameters):
  1. Initialise optimisation algorithm
  2. Boundaries handling
  3. Evaluate initial parameters
  4. Update best solution, number of iteration and duration
  5. While no termination criteria is met
    1. Check if the process is stagnating
      - If true: Restart/Reinitialise the algorithm and get new parameters
      - If false: Get the next parameters 
    2. Boundaries handling
    3. Evaluate parameters
    4. Update best solution, number of iteration and duration
```

{% include api subset=page.url %}