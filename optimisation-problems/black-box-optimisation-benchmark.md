---
layout: page
---

{% include h1 title="Black-box optimisation benchmark" %}

The black-box optimisation benchmark, developed by [Nikolaus Hansen](https://www.lri.fr/~hansen/) et al., is a popular benchmark for continuous, single-objective black-box optimisation algorithms and used in competitions at the [GECCO 2009](http://coco.gforge.inria.fr/doku.php?id=bbob-2009), [2010](http://coco.gforge.inria.fr/doku.php?id=bbob-2010), [2012](http://coco.gforge.inria.fr/doku.php?id=bbob-2012), [2013](http://coco.gforge.inria.fr/doku.php?id=bbob-2013), [2015](http://coco.gforge.inria.fr/doku.php?id=bbob-2015) and [CEC 2015](http://coco.gforge.inria.fr/doku.php?id=cec-bbob-2015). It consist of 24 synthetic optimisation problems and focuses on <em>comprehensible</em> instances, in order to get a better cause-effect between the algorithms and problems at hand.

Below is one of our larger code examples, demonstrating the most common usage of this optimisation benchmark. It individually evaluates the performance of the Hooke-Jeeves algorithm for each of the 24 black box optimisation benchmark problems and rerun 100 times to address random effects.

{% include example name="black-box-optimisation-benchmark" %}

{% include notice title="Adjusting the lower and upper bound" content="It is not recommended to change the lower and upper bounds, as some optimisation problems will only perform as described within these boundaries." %}

#### Default initialisation

- The lower bounds are set to \\(-5, \ldots, -5)\\).
- The upper bounds are set to \\(5, \ldots, 5)\\).
- The objective value space is randomly translated by a Cauchy distribution with an approximate 50% chance for the optimal objective value to be within \\(\left[-100, 100\right]\\), rounded up to 2 decimal places. The optimal objective value is further bounded to be between \\(-1000\\) and \\(1000\\).
- The acceptable objective value threshold is set to be \\(1.0e-8\\) above the minimal objective value.

#### Parameter space transformations

Most functions within the benchmark perform additional transformations on the parameter space (\\(T_\text{conditioning}\\), \\(T_\text{conditioned}\\), \\(T_\text{asymmetric}\\) and \\(T_\text{oscillated}\\)), in order to adjust local optima, the conditioning and separability of a problem.

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

#### Soft-constraints value

Some functions within the benchmark have a non-zero soft-constraints function (based on \\(F_\text{penality}\\)), which acts as an additional boundary constraint and is simple added to the objective value.

$$F_\text{penality}(X) := \sum_{i = 0}^N\max\left(0, \left|X_i\right| - 5\right)^2$$

{% include api subset=page.url %}
