*Abstract base class for all optimisation problems.*

All optimisation problems are defined as minimisation problems:

$$\min_{X \in [C_L, C_U]} F(X) + C(X)$$

$$\begin{align}
F &:= \text{Some objective function.}\\
C &:= \text{The sum of all (soft-)constraints.}\\
C_L &:= \text{The lower bounds of the search space.}\\
C_U &:= \text{The upper bounds of the search space.}
\end{align}$$

<div class="custom-callout custom-callout-info">
#### Default values
Unless otherwise stated, the optimisation problem has no soft- or bound-constraints, i.e. \\(C(X) = 0 \ \forall X\\), \\(C_L = -\infty^N\\) and \\(C_U = \infty^N\\).
</div>

It is further possible to permutate, scale, translate and rotate any objective function and value.

$$\min_{X \in [C_L, C_U]} F_S \left(\left( X_R \cdot \left( X_S \circ X_P\left(X\right) - X_T\right) \right) + C\left(X_P\left(X\right)\right) \right) + F_T$$

$$\begin{align}
F, C, C_L, C_U &:= \text{Same definition as above.}\\
X_P &:= \text{The permuation of the dimensions of the objective function.}\\
X_S &:= \text{The scaling of the objective function (continuous-only).}\\
X_T &:= \text{The translation of the objective function (continuous-only).}\\
X_R &:= \text{The rotation of the objective function (continuous-only).}\\
F_S &:= \text{The scaling of the objective value.}\\
F_T &:= \text{The translation of the objective value.}\\
\circ &:= \text{Element-wise multiplication.}
\end{align}$$

<div class="custom-callout custom-callout-info">
#### Default values
The objective function and value is initially unchanged, i.e. \\(X_P\\), \\(X_R\\), \\(X_S\\), \\(X_T\\), \\(F_S\\) and \\(F_T\\) are set to a neutral element/function.
</div>

- Constraints handling<br>
  <a href="#optimisation-problems-setLowerBounds">setLowerBounds</a>, <a href="#optimisation-problems-setUpperBounds">setUpperBounds</a>, <a href="#optimisation-problems-getLowerBounds">getLowerBounds</a>, <a href="#optimisation-problems-getUpperBounds">getUpperBounds</a>, <a href="#optimisation-problems-getSoftConstraintsValue">getSoftConstraintsValue</a>, <a href="#optimisation-problems-isSatisfyingLowerBounds">isSatisfyingLowerBounds</a>, <a href="#optimisation-problems-isSatisfyingUpperBounds">isSatisfyingUpperBounds</a>, <a href="#optimisation-problems-isSatisfyingSoftConstraints">isSatisfyingSoftConstraints</a>, <a href="#optimisation-problems-isSatisfyingConstraints">isSatisfyingConstraints</a>
- Objective function<br>
  <a href="#optimisation-problems-setParameterPermutation">setParameterPermutation</a>, <a href="#optimisation-problems-setParameterScale">setParameterScaling</a>, <a href="#optimisation-problems-setParameterTranslation">setParameterTranslation</a>, <a href="#optimisation-problems-setParameterRotation">setParameterRotation</a>, <a href="#optimisation-problems-setObjectiveValueScale">setObjectiveValueScaling</a>, <a href="#optimisation-problems-setObjectiveValueTranslation">setObjectiveValueTranslation</a>, <a href="#optimisation-problems-setAcceptableObjectiveValue">setAcceptableObjectiveValue</a>, <a href="#optimisation-problems-getAcceptableObjectiveValue">getAcceptableObjectiveValue</a>, <a href="#optimisation-problems-getObjectiveValue">getObjectiveValue</a>
- Results<br>
  <a href="#optimisation-problems-getNumberOfEvaluations">getNumberOfEvaluations</a>, <a href="#optimisation-problems-getNumberOfDistinctEvaluations">getNumberOfDistinctEvaluations</a>
- Miscellaneous<br>
  <a href="#optimisation-problems-reset">reset</a>, <a href="#optimisation-problems-getCachedObjectiveValues">getCachedObjectiveValues</a>

---
<a name="optimisation-problems-setLowerBounds"></a>
**<small>void</small> .setLowerBounds( <small>arma::Col&lt;T&gt;</small> L) )**

- Sets the lower bounds \\(C_L\\) of the search space to *L*.
- **Requirement:** The number of elements in *L* must match the problem dimension.

---
<a name="optimisation-problems-setUpperBounds"></a>
**<small>void</small> .setUpperBounds( <small>arma::Col&lt;T&gt;</small> U )**

- Sets the upper bounds \\(C_U\\) of the search space to *U*.
- **Requirement:** The number of elements in *U* must match the problem dimension.

---
<a name="optimisation-problems-getLowerBounds"></a>
**<small>arma::Col&lt;T&gt;</small> .getLowerBounds()** {% include noexcept %}

- Returns the lower bounds of the search space.
- The default value (if not parameterised by `.setLowerBounds(...)`) is a vector with each element set to the lowest representable number.

---
<a name="optimisation-problems-getUpperBounds"></a>
**<small>arma::Col&lt;T&gt;</small> .getUpperBounds()** {% include noexcept %}

- Returns the upper bounds of the search space.
- The default value (if not parameterised by `.setUpperBounds(...)`) is a vector with each element set to the largest representable number.

---
<a name="optimisation-problems-getSoftConstraintsValue"></a>
**<small>double</small> .getSoftConstraintsValue( <small>arma::Col&lt;T&gt;</small> X )**

- Calculates the soft constraint value for *X*.
- The default (if not stated otherwise by the problem definition) is to return 0 for all inputs.
- **Requirement:** The number of elements in *X* must match the problem dimension.

---
<a name="optimisation-problems-isSatisfyingLowerBounds"></a>
**<small>bool</small> .isSatisfyingLowerBounds( <small>arma::Col&lt;T&gt;</small> X )**

- Returns true, if *X* is element-wise equal to or greater then the lower bound and false otherwise.
- **Requirement:** The number of elements in *X* must match the problem dimension.
 
---
<a name="optimisation-problems-isSatisfyingUpperBounds"></a>
**<small>bool</small> .isSatisfyingUpperBounds( <small>arma::Col&lt;T&gt;</small> X )**

- Returns true, if *X* is element-wise equal to or less then the upper bound and false otherwise.
- **Requirement:** The number of elements in *X* must match the problem dimension.

---
<a name="optimisation-problems-isSatisfyingSoftConstraints"></a>
**<small>bool</small> .isSatisfyingSoftConstraints( <small>arma::Col&lt;X&gt;</small> X )**

- Returns true, if the soft-constraints value for *X* is zero and false otherwise.
- **Requirement:** The number of elements in *X* must match the problem dimension.

---
<a name="optimisation-problems-isSatisfyingConstraints"></a>
**<small>bool</small> .isSatisfyingConstraints( <small>arma::Col&lt;T&gt;</small> X )**

- Returns true, if *X* fulfills all bound- and soft-constraints and false otherwise.
- This is equivalent to `.isSatisfyingLowerBounds(X) && .isSatisfyingUpperBounds(X) && .isSatisfyingSoftConstraints(X)`.
- **Requirement:** The number of elements in *X* must match the problem dimension.

---
<a name="optimisation-problems-setParameterScale"></a>
**<small>void</small> .setParameterPermutation( <small>arma::Col&lt;unsigned int&gt;</small> P )**

- Parameterises the permutation \\(X_P\\) of the dimensions of the objective function.
- If used together with scaling, translation and rotation, the parameter is firstly permutated, than scaled, translated and lastly rotated.
- **Requirement:** The number of elements in *P* must match the problem dimension.
- **Requirement:** All elements in *X* must be unique and within [0, number of dimensions - 1]

---
<a name="optimisation-problems-setParameterScale"></a>
**<small>void</small> .setParameterScaling( <small>arma::Col&lt;T&gt;</small> S )** {% include continuous-only %}

- Parameterises the scaling \\(X_S\\) of the objective function.
- If used together with permutation, translation and rotation, the parameter is firstly permutated, than scaled, translated and lastly rotated.
- **Requirement:** The number of elements in *S* must match the problem dimension.

---
<a name="optimisation-problems-setParameterTranslation"></a>
**<small>void</small> .setParameterTranslation( <small>arma::Col&lt;T&gt;</small> T )** {% include continuous-only %}

- Parameterises the translation \\(X_T\\) of the objective function.
- If used together with permutation, scaling and rotation, the parameter is firstly permutated, than scaled, translated and lastly rotated.
- **Requirement:** The number of elements in *T* must match the problem dimension.

---
<a name="optimisation-problems-setParameterRotation"></a>
**<small>void</small> .setParameterRotation( <small>arma::Mat&lt;T&gt;</small> R )** {% include continuous-only %}

- Parameterises the rotation \\(X_R\\) of the objective function.
- If used together with permutation, scaling and translation, the parameter is firstly permutated, than scaled, translated and lastly rotated.
- **Requirement:** The number of rows and columns in *R* must each match the problem dimension.
- **Requirement:** *R* must be square, orthonormal (\\(R^{t} = R^{-1}\\)) and its determinant equal to 1 or -1.

---
<a name="optimisation-problems-setObjectiveValueScale"></a>
**<small>void</small> .setObjectiveValueScaling( <small>double</small> S )** {% include noexcept %}

- Parameterises the scaling \\(F_S\\) of the objective value.
- If used together with translation, the parameter is firstly scaled and then translated.

---
<a name="optimisation-problems-setObjectiveValueTranslation"></a>
**<small>void</small> .setObjectiveValueTranslation( <small>double</small> T )** {% include noexcept %}

- Parameterises the translation \\(F_T\\) of the objective value.
- If used together with scaling, the parameter is firstly scaled and then translated.

---
<a name="optimisation-problems-setAcceptableObjectiveValue"></a>
**<small>void</small> .setAcceptableObjectiveValue( <small>double</small> F )** {% include noexcept %}

- Defines the upper limit for an objective value to be acceptable by the user.
- *Note:* The acceptable objective value is not required to be reachable.

---
<a name="optimisation-problems-getAcceptableObjectiveValue"></a>
**<small>double</small> .getAcceptableObjectiveValue()** {% include noexcept %}

- Returns the the upper limit for an objective value to be acceptable by the user.
- The default value (if not parameterised by `.setAcceptableObjectiveValue(...)`) is the lowest representable number.
- *Note:* The acceptable objective value is not required to be reachable.

---
<a name="optimisation-problems-getObjectiveValue"></a>
**<small>double</small> .getObjectiveValue( <small>arma::Col&lt;T&gt;</small> X )**

- Calculates the objective value.
- The result is not re-calculated if repeatedly invoked with the same parameter, but retrieved from cache instead.
- **Requirement:** The number of elements in *X* must match the problem dimension.

---
<a name="optimisation-problems-getNumberOfEvaluations"></a>
**<small>unsigned int</small> .getNumberOfEvaluations()** {% include noexcept %}

- Counts the number of ALL objective function evaluations (i.e. invokes of `.getObjectiveValue(...)`).
- Includes results that are retrived from cache and actually calculated.
- Use the `.reset()` method to reset this counter to 0.

---
<a name="optimisation-problems-getNumberOfDistinctEvaluations"></a>
**<small>unsigned int</small> .getNumberOfDistinctEvaluations()** {% include noexcept %}

- Counts the number of disctinct, i.e. unique objective function evaluations.
- Includes actually calculated results only.
- Use the `.reset()` method to reset this counter to 0.

Example:
{% highlight cpp %}
#include <mantella>
#include <iostream>

int main() {
  std::shared_ptr<mant::OptimisationProblem> optimisationProblem(new mant::bbob2015::SphereFunction(3));
  
  optimisationProblem.getObjectiveValue({2.3, 1.2, 1.3}); // Actual computation
  optimisationProblem.getObjectiveValue({1.5, 2.6, -0.4}); // Actual computation
  optimisationProblem.getObjectiveValue({2.3, 1.2, 1.3}); // Retrived from cache
  optimisationProblem.getObjectiveValue({2.3, 1.2, 1.3}); // Retrived from cache
  optimisationProblem.getObjectiveValue({0.6, 0.8, 0.0}); // Actual computation
  
  std::cout << "numberOfEvaluations: " << optimisationProblem->getNumberOfEvaluations() << std::endl;
  std::cout << "numberOfDistinctEvaluations: " << optimisationProblem->getNumberOfDistinctEvaluations() << std::endl;
}
{% endhighlight %}

Output:
{% highlight text %}
numberOfEvaluations: 5
numberOfDistinctEvaluations: 3
{% endhighlight %}

---
<a name="optimisation-problems-reset"></a>
**<small>void</small> .reset()** {% include noexcept %}

- Resets all counters and caches.
- All other settings (lower and upper bounds, translations, scaling, ...) remain as set.

---
<a name="optimisation-problems-getCachedObjectiveValues"></a>
**<small>std::unordered_map&lt;arma::Col&lt;T&gt;, double, mant::Hash, mant::IsEqual&gt;</small> .getCachedObjectiveValues()** {% include noexcept %}

- Returns the cached mapping of parameters to objective values.
- Caches all new request to `.getObjectiveValue(...)`.