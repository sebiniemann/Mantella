<div class="custom-callout custom-callout-info">
#### Inheritance & Templating

Pure virtual base class for all optimisation problems.

Continuous (`float`, `double`, ...) as well as integral types (`unsigned int`, ...) can be used as template parameter `T` for the `OptimisationProblem<T>` class.
</div>

All optimisation problems are defined as single-objective, real-valued minimisation problems and may have bounds and soft-constraints.

$$\min_{X \in [C_L, C_U]} F(X) + C(X)$$

$$\begin{align}
F &:= \text{Some objective function.}\\
C &:= \text{The sum of all soft-constraints.}\\
C_L &:= \text{The lower bounds of the search space.}\\
C_U &:= \text{The upper bounds of the search space.}
\end{align}$$

A parameter \\(X\\) is within the solution space, if and only if \\(X \in [C_L, C_U]\\) and \\(C(X) = 0\\). It is further an acceptable solution, if and only if \\(F(X) \leq\\) `.getAcceptableObjectiveValue(X)`.

<div class="custom-callout custom-callout-info">
#### Default values

Unless otherwise stated, the optimisation problem has neither soft- nor bound-constraints, i.e. \\(C(X) := 0, \ \forall X\\), \\(C_L := -\infty^N\\) and \\(C_U := \infty^N\\).
</div>

It is possible to permute, scale, translate and rotate the parameter space and objective value. This is mostly necessary to avoid unintentionally biased benchmarks.

$$\min_{X \in [C_L, C_U]} F_S \left(F \left( X_R \cdot \left( X_S \circ X_P\left(X\right) - X_T\right) \right) + C\left(X_P\left(X\right)\right) \right) + F_T$$

$$\begin{align}
F, C, C_L, C_U &:= \text{Same definition as above.}\\
X_P &:= \text{The permutation of the dimensions of the parameter space.}\\
X_S &:= \text{The scaling of the parameter space (continuous-only).}\\
X_T &:= \text{The translation of the parameter space (continuous-only).}\\
X_R &:= \text{The rotation of the parameter space (continuous-only).}\\
F_S &:= \text{The scaling of the objective and soft-constraints value.}\\
F_T &:= \text{The translation of the objective value.}\\
\circ &:= \text{Element-wise multiplication.}
\end{align}$$

<div class="custom-callout custom-callout-info">
#### Default values

Unless otherwise stated, the parameter space and objective value is initially unchanged, i.e. \\(X_P\\), \\(X_R\\), \\(X_S\\), \\(X_T\\), \\(F_S\\) and \\(F_T\\) are set to a neutral element/function.
</div>

- Constraints handling<br>
  {% include reference prefix=include.anchor_prefix name="setLowerBounds" %}, {% include reference prefix=include.anchor_prefix name="setUpperBounds" %}, {% include reference prefix=include.anchor_prefix name="getLowerBounds" %}, {% include reference prefix=include.anchor_prefix name="getUpperBounds" %}, {% include reference prefix=include.anchor_prefix name="getSoftConstraintsValue" %}, {% include reference prefix=include.anchor_prefix name="isSatisfyingLowerBounds" %}, {% include reference prefix=include.anchor_prefix name="isSatisfyingUpperBounds" %}, {% include reference prefix=include.anchor_prefix name="isSatisfyingSoftConstraints" %}, {% include reference prefix=include.anchor_prefix name="isSatisfyingConstraints" %}
- Objective function<br>
  {% include reference prefix=include.anchor_prefix name="setParameterPermutation" %}, {% include reference prefix=include.anchor_prefix name="setParameterScale" %}, {% include reference prefix=include.anchor_prefix name="setParameterTranslation" %}, {% include reference prefix=include.anchor_prefix name="setParameterRotation" %}, {% include reference prefix=include.anchor_prefix name="setObjectiveValueScale" %}, {% include reference prefix=include.anchor_prefix name="setObjectiveValueTranslation" %}, {% include reference prefix=include.anchor_prefix name="setAcceptableObjectiveValue" %}, {% include reference prefix=include.anchor_prefix name="getAcceptableObjectiveValue" %}, {% include reference prefix=include.anchor_prefix name="getObjectiveValue" %}
- Results<br>
  {% include reference prefix=include.anchor_prefix name="getNumberOfEvaluations" %}, {% include reference prefix=include.anchor_prefix name="getNumberOfDistinctEvaluations" %}
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="reset" %}, {% include reference prefix=include.anchor_prefix name="getCachedObjectiveValues" %}

---
{% include label prefix=include.anchor_prefix name="setLowerBounds" %}
**<small>void</small> .setLowerBounds( <small>arma::Col&lt;T&gt;</small> L) )**

- Sets the lower bounds \\(C_L\\) of the search space to *L*.
- The *i*-te element in *L* stands for the *i*-te dimension.
- **Requirement:** The number of elements in *L* must be equal to the problem dimension.

---
{% include label prefix=include.anchor_prefix name="setUpperBounds" %}
**<small>void</small> .setUpperBounds( <small>arma::Col&lt;T&gt;</small> U )**

- Sets the upper bounds \\(C_U\\) of the search space to *U*.
- The *i*-te element in *U* stands for the *i*-te dimension.
- **Requirement:** The number of elements in *U* must be equal to the problem dimension.

---
{% include label prefix=include.anchor_prefix name="getLowerBounds" %}
**<small>arma::Col&lt;T&gt;</small> .getLowerBounds()** {% include noexcept %}

- Returns the lower bounds of the search space.
- The *i*-te element stands for the *i*-te dimension.

---
{% include label prefix=include.anchor_prefix name="getUpperBounds" %}
**<small>arma::Col&lt;T&gt;</small> .getUpperBounds()** {% include noexcept %}

- Returns the upper bounds of the search space.
- The *i*-te element stands for the *i*-te dimension.

---
{% include label prefix=include.anchor_prefix name="getSoftConstraintsValue" %}
**<small>double</small> .getSoftConstraintsValue( <small>arma::Col&lt;T&gt;</small> X )**

- Calculates the soft constraints value for *X*.
- The soft constraints value is always positive (including 0). If and only if no constraint is violated (except lower and upper bounds), the return value is 0.
- The *i*-te element in *X* stands for the *i*-te dimension.
- **Note:** The soft constraints value is not required to correlate with the distance between *X* and the solution space.
- **Requirement:** The number of elements in *X* must be equal to the problem dimension.

---
{% include label prefix=include.anchor_prefix name="isSatisfyingLowerBounds" %}
**<small>bool</small> .isSatisfyingLowerBounds( <small>arma::Col&lt;T&gt;</small> X )**

- Returns true, if and only if *X* is element-wise equal to or greater then the lower bound, and false otherwise.
- The *i*-te element in *X* is compared with the *i*-te lower bound.
- **Requirement:** The number of elements in *X* must be equal to the problem dimension.
 
---
{% include label prefix=include.anchor_prefix name="isSatisfyingUpperBounds" %}
**<small>bool</small> .isSatisfyingUpperBounds( <small>arma::Col&lt;T&gt;</small> X )**

- Returns true, if and only if *X* is element-wise equal to or less then the upper bounds, and false otherwise.
- The *i*-te element in *X* is compared with the *i*-te upper bound.
- **Requirement:** The number of elements in *X* must be equal to the problem dimension.

---
{% include label prefix=include.anchor_prefix name="isSatisfyingSoftConstraints" %}
**<small>bool</small> .isSatisfyingSoftConstraints( <small>arma::Col&lt;T&gt;</small> X )**

- Returns true, if and only if the soft-constraints value for *X* is equal to 0, and false otherwise.
- The *i*-te element in *X* stands for the *i*-te dimension.
- **Requirement:** The number of elements in *X* must be equal to the problem dimension.

---
{% include label prefix=include.anchor_prefix name="isSatisfyingConstraints" %}
**<small>bool</small> .isSatisfyingConstraints( <small>arma::Col&lt;T&gt;</small> X )**

- Returns true, if and only if *X* is within the solution space, i.e. ful
- fils all bound- and soft-constraints, and false otherwise.
- This is equivalent to `.isSatisfyingLowerBounds(X) && .isSatisfyingUpperBounds(X) && .isSatisfyingSoftConstraints(X)`.
- The *i*-te element in *X* stands for the *i*-te dimension.
- **Requirement:** The number of elements in *X* must be equal to the problem dimension.

---
{% include label prefix=include.anchor_prefix name="setParameterPermutation" %}
**<small>void</small> .setParameterPermutation( <small>arma::Col&lt;unsigned int&gt;</small> P )**

- Parametrises the permutation \\(X_P\\) of the dimensions of the parameter space.
- The *i*-te dimension will be permuted with the *i*-te element in P.
- If used together with scaling, translation or rotation, the parameter is firstly permuted, than scaled, translated and lastly rotated.
- **Requirement:** The number of elements in *P* must be equal to the problem dimension.
- **Requirement:** All elements in *P* must be unique and within [0, number of dimensions - 1].

---
{% include label prefix=include.anchor_prefix name="setParameterScaling" %}
**<small>void</small> .setParameterScaling( <small>arma::Col&lt;T&gt;</small> S )** {% include continuous-only %}

- Parametrises the scaling \\(X_S\\) of the parameter space.
- The *i*-te dimension will be scaled by the *i*-te element in S.
- If used together with permutation, translation or rotation, the parameter is firstly permuted, than scaled, translated and lastly rotated.
- **Requirement:** The number of elements in *S* must be equal to the problem dimension.

---
{% include label prefix=include.anchor_prefix name="setParameterTranslation" %}
**<small>void</small> .setParameterTranslation( <small>arma::Col&lt;T&gt;</small> T )** {% include continuous-only %}

- Parametrises the translation \\(X_T\\) of the parameter space.
- The *i*-te dimension will be translated by the *i*-te element in T.
- If used together with permutation, scaling or rotation, the parameter is firstly permuted, than scaled, translated and lastly rotated.
- **Requirement:** The number of elements in *T* must be equal to the problem dimension.

---
{% include label prefix=include.anchor_prefix name="setParameterRotation" %}
**<small>void</small> .setParameterRotation( <small>arma::Mat&lt;T&gt;</small> R )** {% include continuous-only %}

- Parametrises the rotation \\(X_R\\) of the parameter space.
- If used together with permutation, scaling or translation, the parameter is firstly permuted, than scaled, translated and lastly rotated.
- **Requirement:** The number of rows and columns in *R* must each be equal to the problem dimension.
- **Requirement:** *R* must be square, orthonormal (\\(R^{t} = R^{-1}\\)) and its determinant equal be to 1 or -1.

---
{% include label prefix=include.anchor_prefix name="setObjectiveValueScaling" %}
**<small>void</small> .setObjectiveValueScaling( <small>double</small> S )** {% include noexcept %}

- Parametrises the scaling \\(F_S\\) of the objective value.
- If used together with translation, the parameter is firstly scaled and then translated.

---
{% include label prefix=include.anchor_prefix name="setObjectiveValueTranslation" %}
**<small>void</small> .setObjectiveValueTranslation( <small>double</small> T )** {% include noexcept %}

- Parametrises the translation \\(F_T\\) of the objective value.
- If used together with scaling, the parameter is firstly scaled and then translated.

---
{% include label prefix=include.anchor_prefix name="setAcceptableObjectiveValue" %}
**<small>void</small> .setAcceptableObjectiveValue( <small>double</small> F )** {% include noexcept %}

- Defines the upper limit for an objective value to be acceptable by the user.
- **Note:** The acceptable objective value is not required to be reachable.

---
{% include label prefix=include.anchor_prefix name="getAcceptableObjectiveValue" %}
**<small>double</small> .getAcceptableObjectiveValue()** {% include noexcept %}

- Returns the the upper limit for an objective value to be acceptable by the user.
- **Note:** The acceptable objective value is not required to be reachable.

---
{% include label prefix=include.anchor_prefix name="getObjectiveValue" %}
**<small>double</small> .getObjectiveValue( <small>arma::Col&lt;T&gt;</small> X )**

- Calculates the objective value.
- The result is not re-calculated if repeatedly invoked with the same parameter, but retrieved from cache instead.
- The *i*-te element in *X* stands for the *i*-te dimension.
- **Requirement:** The number of elements in *X* must be equal to the problem dimension.

---
{% include label prefix=include.anchor_prefix name="getNumberOfEvaluations" %}
**<small>unsigned int</small> .getNumberOfEvaluations()** {% include noexcept %}

- Counts the number of ALL objective function evaluations (i.e. invokes of `.getObjectiveValue(...)`).
- Includes results that are retrieved from cache or actually calculated.
- Use the `.reset()` method to reset this counter to 0.

---
{% include label prefix=include.anchor_prefix name="getNumberOfDistinctEvaluations" %}
**<small>unsigned int</small> .getNumberOfDistinctEvaluations()** {% include noexcept %}

- Counts the number of distinct objective function evaluations.
- Includes actually calculated results only.
- Use the `.reset()` method to reset this counter to 0.

Example code, demonstrating the difference between `.getNumberOfEvaluations()` and `.getNumberOfDistinctEvaluations()`.
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/optimisation_problem.cpp %}
{% endhighlight %}

Compilation and execution:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/optimisation_problem.core.sh %}
{% endhighlight %}

Output:
{% highlight text %}
{% include {{ api_reference_folder }}/_examples/optimisation_problem.output %}
{% endhighlight %}

---
{% include label prefix=include.anchor_prefix name="reset" %}
**<small>void</small> .reset()** {% include noexcept %}

- Resets all counters (`.getNumberOfEvaluations()` and `.getNumberOfDistinctEvaluations()`) and caches (`.getCachedObjectiveValues()`).
- All other settings (lower and upper bounds, translations, scaling, ...) remain unchanged.

---
{% include label prefix=include.anchor_prefix name="getCachedObjectiveValues" %}
**<small>std::unordered_map&lt;arma::Col&lt;T&gt;, double, mant::Hash, mant::IsEqual&gt;</small> .getCachedObjectiveValues()** {% include noexcept %}

- Returns the cached mapping of parameters to objective values.
- Caches all request to `.getObjectiveValue(...)`.