Abstract base class for all optimisation problems.<br>
*Note:* All optimisation problems are defined as minimisation problems.

$$\min_{X \in [L, U]} F(X) + C(X)$$


$$\begin{align}
L :=&\ \text{The lower bound of the search space.}\\
U :=&\ \text{The upper bound of the search space.}\\
F :=&\ \text{Any objective function, e.g. the sphere function.}\\
C :=&\ \text{The sum of all (soft-)constraints.}\\
    &\ \text{This is always positive and only 0 if X satisfies all constraints.}\end{align}$$

- Constraints handling<br>
  <a href="#optimisation-problems-setLowerBounds">setLowerBounds</a>, <a href="#optimisation-problems-setUpperBounds">setUpperBounds</a>, <a href="#optimisation-problems-getLowerBounds">getLowerBounds</a>, <a href="#optimisation-problems-getUpperBounds">getUpperBounds</a>, <a href="#optimisation-problems-getSoftConstraintsValue">getSoftConstraintsValue</a>, <a href="#optimisation-problems-isSatisfyingLowerBounds">isSatisfyingLowerBounds</a>, <a href="#optimisation-problems-isSatisfyingUpperBounds">isSatisfyingUpperBounds</a>, <a href="#optimisation-problems-isSatisfyingSoftConstraints">isSatisfyingSoftConstraints</a>, <a href="#optimisation-problems-isSatisfyingConstraints">isSatisfyingConstraints</a>
- Objective function<br>
  <a href="#optimisation-problems-setParameterTranslation">setParameterTranslation</a>, <a href="#optimisation-problems-setParameterScale">setParameterScale</a>, <a href="#optimisation-problems-setParameterRotation">setParameterRotation</a>, <a href="#optimisation-problems-setObjectiveValueTranslation">setObjectiveValueTranslation</a>, <a href="#optimisation-problems-setObjectiveValueScale">setObjectiveValueScale</a>, <a href="#optimisation-problems-setAcceptableObjectiveValue">setAcceptableObjectiveValue</a>, <a href="#optimisation-problems-getAcceptableObjectiveValue">getAcceptableObjectiveValue</a>, <a href="#optimisation-problems-getObjectiveValue">getObjectiveValue</a>
- Results<br>
  <a href="#optimisation-problems-getNumberOfEvaluations">getNumberOfEvaluations</a>, <a href="#optimisation-problems-getNumberOfDistinctEvaluations">getNumberOfDistinctEvaluations</a>
- Miscellaneous<br>
  <a href="#optimisation-problems-reset">reset</a>, <a href="#optimisation-problems-getCachedObjectiveValues">getCachedObjectiveValues</a>

---
<a name="optimisation-problems-setLowerBounds"></a>
**<small>void</small> .setLowerBounds( <small>arma::Col&lt;T&gt;</small> L )**

- Sets the lower bounds of the search space to *L*.
- The default value is a vector with each element set to the lowest representable number.

---
<a name="optimisation-problems-setUpperBounds"></a>
**<small>void</small> .setUpperBounds( <small>arma::Col&lt;T&gt;</small> U )**

- Sets the upper bounds of the search space to *U*.
- The default value is a vector with each element set to the largest representable number.

---
<a name="optimisation-problems-getLowerBounds"></a>
**<small>arma::Col&lt;T&gt;</small> .getLowerBounds()**

- Returns the lower bounds of the search space.
- The default value (if not specified by `.setLowerBounds(...)`) is a vector with each element set to the lowest representable number.

---
<a name="optimisation-problems-getUpperBounds"></a>
**<small>arma::Col&lt;T&gt;</small> .getUpperBounds()**

- Returns the upper bounds of the search space.
- The default value (if not specified by `.setUpperBounds(...)`) is a vector with each element set to the largest representable number.

---
<a name="optimisation-problems-getSoftConstraintsValue"></a>
**<small>double</small> .getSoftConstraintsValue( <small>arma::Col&lt;T&gt;</small> X )**

- Calculates the soft constraint value for the parameter *X*.

---
<a name="optimisation-problems-isSatisfyingLowerBounds"></a>
**<small>bool</small> .isSatisfyingLowerBounds( <small>arma::Col&lt;T&gt;</small> X )**

- Returns true, if the parameter *X* is element-wise equal to or greater then the lower bound and false otherwise.
 
---
<a name="optimisation-problems-isSatisfyingUpperBounds"></a>
**<small>bool</small> .isSatisfyingUpperBounds( <small>arma::Col&lt;T&gt;</small> X )**

- Returns true, if the parameter *X* is element-wise equal to or less then the upper bound and false otherwise.

---
<a name="optimisation-problems-isSatisfyingSoftConstraints"></a>
**<small>bool</small> .isSatisfyingSoftConstraints( <small>arma::Col&lt;T&gt;</small> X )**

- Returns true, if the soft-constraints value for parameter *X* is zero and false otherwise.

---
<a name="optimisation-problems-isSatisfyingConstraints"></a>
**<small>bool</small> .isSatisfyingConstraints( <small>arma::Col&lt;T&gt;</small> X )**

- Returns true, if the parameter *X* fulfills all bound- and soft-constraints and false otherwise.
- This is equivalent to `.isSatisfyingLowerBounds(X) && .isSatisfyingUpperBounds(X) && .isSatisfyingSoftConstraints(X)`.

---
<a name="optimisation-problems-setParameterTranslation"></a>
**<small>void</small> .setParameterTranslation( <small>arma::Col&lt;T&gt;</small> T )**

- Translates (i.e. shifts) the search space along the parameter axis.
- If used together with scaling and rotation, the parameter is first scaled, then translated and lastly rotated.
- The default value is a zero vector (no translation)

$$F_\text{translated}(X) := F(X + T)$$

---
<a name="optimisation-problems-setParameterScale"></a>
**<small>void</small> .setParameterScale( <small>arma::Col&lt;T&gt;</small> S )** {% include continuous-only %}

- Scales the search space along the parameter axis.
- If used together with translation and rotation, the parameter is first scaled, then translated and lastly rotated.
- The default value is a vector of ones (no scaling)

$$F_\text{scaled}(X) := F(S \circ X), \circ := \text{element-wise multiplication}$$

---
<a name="optimisation-problems-setParameterRotation"></a>
**<small>void</small> .setParameterRotation( <small>arma::Mat&lt;T&gt;</small> R )** {% include continuous-only %}

- Rotates the search space along the parameter axis by a rotation matrix *R*.
- The rotation matrix must be orthonormal (\\(R^{t} = R^{-1}\\)) with determinant 1 or -1.
- If used together with translation and scaling, the parameter is first scaled, then translated and lastly rotated.
- The default value is an indentity matrix (no rotation)

$$F_\text{rotated}(X) := F(R \cdot X)$$

---
<a name="optimisation-problems-setObjectiveValueTranslation"></a>
**<small>void</small> .setObjectiveValueTranslation( <small>double</small> T )** {% include noexcept %}

- Translates the objective value space.
- If used together with scaling, the parameter is first scaled and then translated.
- The default value is 0 (no translation)

$$F_\text{translated}(X) := F(X) + T$$

---
<a name="optimisation-problems-setObjectiveValueScale"></a>
**<small>void</small> .setObjectiveValueScale( <small>double</small> S )** {% include noexcept %}

- Scales the objective value space.
- If used together with scaling, the parameter is first scaled and then translated.
- The default value is 1 (no scaling)

$$F_\text{scaled}(X) := S \cdot F(X)$$

---
<a name="optimisation-problems-setAcceptableObjectiveValue"></a>
**<small>void</small> .setAcceptableObjectiveValue( <small>double</small> F )** {% include noexcept %}

- Defines the the upper limit for an objective value to be acceptable by the user.
- The default value is the lowest representable number.
- *Note:* The acceptable objective value is not required to be reachable.

---
<a name="optimisation-problems-getAcceptableObjectiveValue"></a>
**<small>double</small> .getAcceptableObjectiveValue()** {% include noexcept %}

- Returns the the upper limit for an objective value to be acceptable by the user.
- *Note:* The acceptable objective value is not required to be reachable.

---
<a name="optimisation-problems-getObjectiveValue"></a>
**<small>double</small> .getObjectiveValue( <small>arma::Col&lt;T&gt;</small> X )**

- Calculates the objective value for the parameter *X*.
- The result is not re-calculated if repeatedly invoked with the same parameter, but retrieved from cache instead.

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

Code example:
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