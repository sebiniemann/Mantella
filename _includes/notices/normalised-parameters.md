{% include notice title="Normalised parameters" content="All optimisation algorithms work on a normalised parameters space, handling each optimisation problem as if the lower bounds are all \\(0\\) and the upper bounds are all \\(1\\).

The parameter is than automatically mapped to the actual parameter space by the optimisation problem's `.getNormalisedObjectiveValue(...)` method." %}