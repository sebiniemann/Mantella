<div class="custom-callout custom-callout-info">
#### Inheritance

Extends the `OptimisationProblem<double>` base class.

Pure virtual base class for all black-box optimisation benchmark problems.
</div>

The black-box optimisation benchmark, developed by [Nikolaus Hansen](https://www.lri.fr/~hansen/) et al., is a popular benchmark for continuous, single-objective black-box optimisation algorithms and used in competitions at the [GECCO 2009](http://coco.gforge.inria.fr/doku.php?id=bbob-2009), [2010](http://coco.gforge.inria.fr/doku.php?id=bbob-2010), [2012](http://coco.gforge.inria.fr/doku.php?id=bbob-2012), [2013](http://coco.gforge.inria.fr/doku.php?id=bbob-2013), [2015](http://coco.gforge.inria.fr/doku.php?id=bbob-2015) and [CEC 2015](http://coco.gforge.inria.fr/doku.php?id=cec-bbob-2015). It consist of 24 synthetic optimisation problems and focuses on *comprehensible* instances, in order to get a better cause-effect understanding between the algorithm and problem at hand.

- Separable functions<br>
  {% include reference prefix=include.namespace name="Sphere function" %}, {% include reference prefix=include.namespace name="Ellipsoidal function" %}, {% include reference prefix=include.namespace name="Rastrigin function" %}, {% include reference prefix=include.namespace name="Büche-Rastrigin function" %}, {% include reference prefix=include.namespace name="Linear Slope" %}
- Functions with low or moderate conditioning<br>
  {% include reference prefix=include.namespace name="Attractive sector function" %}, {% include reference prefix=include.namespace name="Step ellipsoidal function" %}, {% include reference prefix=include.namespace name="Rosenbrock function" %}, {% include reference prefix=include.namespace name="Rosenbrock function (rotated)" %}
- Unimodal functions with high conditioning<br>
  {% include reference prefix=include.namespace name="Ellipsoidal function (rotated)" %}, {% include reference prefix=include.namespace name="Discus function" %}, {% include reference prefix=include.namespace name="Bent cigar function" %}, {% include reference prefix=include.namespace name="Sharp ridge function" %}, {% include reference prefix=include.namespace name="Different powers function" %}
- Multi-modal functions with adequate global structure<br>
  {% include reference prefix=include.namespace name="Rastrigin function (rotated)" %}, {% include reference prefix=include.namespace name="Weierstrass function" %}, {% include reference prefix=include.namespace name="Schaffers F7 function" %}, {% include reference prefix=include.namespace name="Schaffers F7 function (ill-conditioned)" %}, {% include reference prefix=include.namespace name="Composite Griewank-Rosenbrock function F8F2" %}
- Multi-modal functions with weak global structure<br>
  {% include reference prefix=include.namespace name="Schwefel function" %}, {% include reference prefix=include.namespace name="Gallagher's Gaussian 101-me peaks function" %}, {% include reference prefix=include.namespace name="Gallagher's Gaussian 21-hi peaks function" %}, {% include reference prefix=include.namespace name="Katsuura function" %}, {% include reference prefix=include.namespace name="Lunacek bi-Rastrigin function" %}
  
  
<div class="custom-callout custom-callout-info">
#### Default values

- Each lower bound in \\(C_L\\) is set to -5.
- Each upper bound in \\(C_U\\) is set to 5.
- The parameter space translation \\(X_T\\) is randomly chosen from a Cauchy distribution with an approximate 50% chance to be within \\(\left[-100, 100\right]\\), rounded up to 2 decimal places. The translation is further bounded between -1000 and 1000.
- The acceptable objective value threshold is set to be 1.0e-8 above the minimal objective value within the solution space.
- Contrary to the base class `OptimisationProblem`, each black-box optimisation benchmark problem is randomly and uniformly initialised.

**Note:** It is not recommended to change the lower and upper bounds, as some optimisation problems will only perform as described within these boundaries.
</div>

<div class="custom-callout custom-callout-info">
#### Parameter space transformations

Most functions within the benchmark perform the additional transformations on the parameter space (\\(T\_\text{conditioning}\\), \\(T\_\text{conditioned}\\), \\(T\_\text{asymmetric}\\) and \\(T\_\text{oscillated}\\)), in order to adjust local optima, the conditioning and separability of a problem.

<div class="MathJax_Display" style="text-align: center;">
$$\begin{align}
{T_\text{conditioning}^C}_i &:= C^\frac{i}{N - 1}\\
{T_\text{conditioned}(X)}_i &:= X_i^\frac{i}{N - 1}\\
{T_\text{asymmetric}^A(X)}_i &:= \begin{cases}
X_i^{1 + A \cdot \frac{i}{N - 1} \cdot \sqrt{X_i}} & \text{if } X > 0 \\
X_i & \text{otherwise}
\end{cases}\\
T_\text{oscillated}(X), X \text{ is a scalar} &:= \begin{cases}
e^{\text{log}\left(\left|X\right|\right) + 0.049 \left(\sin\left(10 \text{log}\left(\left|X\right|\right)\right) + \sin\left(7.9 \text{log}\left(\left|X\right|\right)\right)\right)} & \text{if } X > 0 \\
-e^{\text{log}\left(\left|X\right|\right) + 0.049 \left(\sin\left(5.5 \text{log}\left(\left|X\right|\right)\right) + \sin\left(3.1 \text{log}\left(\left|X\right|\right)\right)\right)} & \text{if } X < 0 \\
0 & \text{otherwise}
\end{cases}\\
T_\text{oscillated}(X)_i, X \text{ is a vector} &:= T_\text{oscillated}(X_i)
\end{align}$$
</div>
</div>

<div class="custom-callout custom-callout-info">
#### Soft-constraints value

Some functions within the benchmark have a non-zero soft-constraints function (based on \\(F_\text{penality}\\)), which acts as an additional boundary constraint.

<div class="MathJax_Display" style="text-align: center;">
$$F_\text{penality}(X) := \sum_{i = 0}^N\max\left(0, \left|X_i\right| - 5\right)^2$$
</div>
</div>