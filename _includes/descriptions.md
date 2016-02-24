{% case include.signature %}
{% when "configuration-iscachingsamples" %}

- If set to `true`, the objective value will only be calculated once per parameter.
- The objective value of recurring parameters are than retrieved from the cache, instead of recalculating them.
- Setting this to `true` is useful if an optimisation algorithms significantly often revisits previous parameters due to its internal procedure, or because of a large minimal parameter distance.
- The number of revisits can be measured by the difference between the number of evaluations and the number of distinct evaluations.
- Default value is `false`

{% include example name=include.signature %}

**See also**

- {% include link signature="optimisation-problems-setminimalparameterdistance-arma-col-double" %}
- {% include link signature="optimisation-problems-getobjectivevalue-arma-col-double" %}
- {% include link signature="optimisation-problems-getnormalisedobjectivevalue-arma-col-double" %}
- {% include link signature="optimisation-problems-setminimalparameterdistance-arma-col-double" %}
- {% include link signature="optimisation-problems-getnumberofevaluations" %}
- {% include link signature="optimisation-problems-getnumberofdistinctevaluations" %}
- {% include link signature="optimisation-problems-getcachedsamples" %}

{% when "configuration-isrecordingsampling" %}

- If set to `true`, the evaluated parameters of an optimisation are recorded (in order of occurrence, including duplicates).
- The recording is cleared when a new optimisation is issued.
- Default value is `false`

{% include example name=include.signature %}

**See also**

- {% include link signature="optimisation-algorithms-optimise-optimisationproblem-arma-mat-double" %}
- {% include link signature="optimisation-algorithms-getrecordedsampling" %}

{% when "configuration-isverbose" %}

- If set to `true`, the optimisation process prints information on the problem and notable progress to the standard output stream `std::cout`.
- Default value is `false`

{% include example name=include.signature %}

**See also**

- {% include link signature="optimisation-algorithms-optimise-optimisationproblem-arma-mat-double" %}

{% when "optimisation-algorithms-optimisationalgorithm" %}

{% include example name=include.signature %}

{% when "optimisation-algorithms-optimise-optimisationproblem-arma-mat-double" %}

{% include example name=include.signature %}

{% when "optimisation-algorithms-setnextparametersfunction-std-function-std-string" %}

{% include example name=include.signature %}

{% when "optimisation-algorithms-setnextparametersfunction-std-function" %}

**See also**

- {% include link signature="optimisation-algorithms-setnextparametersfunction-std-function-std-string" %}

{% when "optimisation-algorithms-getnextparametersfunctionname" %}

{% include example name=include.signature %}

**See also**

- {% include link signature="optimisation-algorithms-setnextparametersfunction-std-function-std-string" %}

{% when "optimisation-algorithms-setboundarieshandlingfunction-std-function-std-string" %}

{% include example name=include.signature %}

{% when "optimisation-algorithms-setboundarieshandlingfunction-std-function" %}

**See also**

- {% include link signature="optimisation-algorithms-setboundarieshandlingfunction-std-function-std-string" %}

{% when "optimisation-algorithms-getboundarieshandlingfunctionname" %}

{% include example name=include.signature %}

**See also**

- {% include link signature="optimisation-algorithms-setboundarieshandlingfunction-std-function-std-string" %}

{% when "optimisation-algorithms-setisstagnatingfunction-std-function-std-string" %}

{% include example name=include.signature %}

{% when "optimisation-algorithms-setisstagnatingfunction-std-function" %}

**See also**

- {% include link signature="optimisation-algorithms-setisstagnatingfunction-std-function-std-string" %}

{% when "optimisation-algorithms-getisstagnatingfunctionname" %}

{% include example name=include.signature %}

**See also**

- {% include link signature="optimisation-algorithms-setisstagnatingfunction-std-function-std-string" %}

{% when "optimisation-algorithms-setrestartingfunction-std-function-std-string" %}

{% include example name=include.signature %}

{% when "optimisation-algorithms-setrestartingfunction-std-function" %}

**See also**

- {% include link signature="optimisation-algorithms-setrestartingfunction-std-function-std-string" %}

{% when "optimisation-algorithms-getrestartingfunctionname" %}

{% include example name=include.signature %}

**See also**

- {% include link signature="optimisation-algorithms-setrestartingfunction-std-function-std-string" %}

{% when "optimisation-algorithms-setacceptableobjectivevalue-double" %}

{% include example name=include.signature %}

{% when "optimisation-algorithms-getacceptableobjectivevalue" %}

{% include example name=include.signature %}

{% when "optimisation-algorithms-setmaximalnumberofiterations-arma-uword" %}

{% include example name=include.signature %}

{% when "optimisation-algorithms-getmaximalnumberofiterations" %}

{% include example name=include.signature %}

{% when "optimisation-algorithms-setmaximalduration-std-chrono-microseconds" %}

{% include example name=include.signature %}

{% when "optimisation-algorithms-getmaximalduration" %}

{% include example name=include.signature %}

{% when "optimisation-algorithms-isfinished" %}

{% include example name=include.signature %}

{% when "optimisation-algorithms-isterminated" %}

{% include example name=include.signature %}

{% when "optimisation-algorithms-getnumberofiterations" %}

{% include example name=include.signature %}

{% when "optimisation-algorithms-getduration" %}

{% include example name=include.signature %}

{% when "optimisation-algorithms-getbestobjectivevalue" %}

{% include example name=include.signature %}

{% when "optimisation-algorithms-getbestparameter" %}

{% include example name=include.signature %}

{% when "optimisation-algorithms-getrecordedsampling" %}

{% include example name=include.signature %}

{% when "optimisation-algorithms-reset" %}

{% include example name=include.signature %}

{% when "random-search-randomsearch" %}

{% include example name=include.signature %}

{% when "random-search-optimise-optimisationproblem" %}

{% include example name=include.signature %}

{% when "hill-climbing-hillclimbing" %}

{% include example name=include.signature %}

{% when "hill-climbing-optimise-optimisationproblem" %}

{% include example name=include.signature %}

{% when "hill-climbing-setminimalstepsize-double" %}

{% include example name=include.signature %}

{% when "hill-climbing-getminimalstepsize" %}

{% include example name=include.signature %}

{% when "hill-climbing-setmaximalstepsize-double" %}

{% include example name=include.signature %}

{% when "hill-climbing-getmaximalstepsize" %}

{% include example name=include.signature %}

{% when "simulated-annealing-simulatedannealing" %}

{% include example name=include.signature %}

{% when "simulated-annealing-optimise-optimisationproblem" %}

{% include example name=include.signature %}

{% when "simulated-annealing-setisacceptablestatefunction-std-function-std-string" %}

{% include example name=include.signature %}

{% when "simulated-annealing-setisacceptablestatefunction-std-function" %}

{% when "simulated-annealing-getisacceptablestatefunctionname" %}

{% include example name=include.signature %}

{% when "simulated-annealing-setminimalstepsize-double" %}

{% include example name=include.signature %}

{% when "simulated-annealing-getminimalstepsize" %}

{% include example name=include.signature %}

{% when "simulated-annealing-setmaximalstepsize-double" %}

{% include example name=include.signature %}

{% when "simulated-annealing-getmaximalstepsize" %}

{% include example name=include.signature %}

{% when "hooke-jeeves-algorithm-hookejeevesalgorithm" %}

{% include example name=include.signature %}

{% when "hooke-jeeves-algorithm-optimise-optimisationproblem" %}

{% include example name=include.signature %}

{% when "hooke-jeeves-algorithm-setinitialstepsize-double" %}

{% include example name=include.signature %}

{% when "hooke-jeeves-algorithm-getinitialstepsize" %}

{% include example name=include.signature %}

{% when "hooke-jeeves-algorithm-setstepsizedecrease-double" %}

{% include example name=include.signature %}

{% when "hooke-jeeves-algorithm-getstepsizedecrease" %}

{% include example name=include.signature %}

{% when "particle-swarm-optimisation-particleswarmoptimisation" %}

{% include example name=include.signature %}

{% when "particle-swarm-optimisation-optimise-optimisationproblem-arma-uword" %}

{% include example name=include.signature %}

{% when "particle-swarm-optimisation-optimise-optimisationproblem" %}

{% include example name=include.signature %}

{% when "particle-swarm-optimisation-setneighbourhoodtopologyfunction-std-function-std-string" %}

{% include example name=include.signature %}

{% when "particle-swarm-optimisation-setneighbourhoodtopologyfunction-std-function" %}

{% when "particle-swarm-optimisation-getneighbourhoodtopologyfunctionname" %}

{% include example name=include.signature %}

{% when "particle-swarm-optimisation-setmaximalacceleration-double" %}

{% include example name=include.signature %}

{% when "particle-swarm-optimisation-getmaximalacceleration" %}

{% include example name=include.signature %}

{% when "particle-swarm-optimisation-setmaximallocalattraction-double" %}

{% include example name=include.signature %}

{% when "particle-swarm-optimisation-getmaximallocalattraction" %}

{% include example name=include.signature %}

{% when "particle-swarm-optimisation-setmaximalglobalattraction-double" %}

{% include example name=include.signature %}

{% when "particle-swarm-optimisation-getmaximalglobalattraction" %}

{% include example name=include.signature %}

{% when "optimisation-problems-optimisationproblem-arma-uword" %}

{% include example name=include.signature %}

{% when "optimisation-problems-numberofdimensions" %}

{% include example name=include.signature %}

{% when "optimisation-problems-setobjectivefunction-std-function-std-string" %}

{% include example name=include.signature %}

{% when "optimisation-problems-setobjectivefunction-std-function" %}

{% when "optimisation-problems-getobjectivefunctionname" %}

{% include example name=include.signature %}

{% when "optimisation-problems-getobjectivevalue-arma-col-double" %}

{% include example name=include.signature %}

{% when "optimisation-problems-getnormalisedobjectivevalue-arma-col-double" %}

{% when "optimisation-problems-setlowerbounds-arma-col-double" %}

{% include example name=include.signature %}

{% when "optimisation-problems-getlowerbounds" %}

{% include example name=include.signature %}

{% when "optimisation-problems-setupperbounds-arma-col-double" %}

{% include example name=include.signature %}

{% when "optimisation-problems-getupperbounds" %}

{% include example name=include.signature %}

{% when "optimisation-problems-setparameterpermutation-arma-col-arma-uword" %}

{% include example name=include.signature %}

{% when "optimisation-problems-getparameterpermutation" %}

{% include example name=include.signature %}

{% when "optimisation-problems-setparameterscaling-arma-col-double" %}

{% include example name=include.signature %}

{% when "optimisation-problems-getparameterscaling" %}

{% include example name=include.signature %}

{% when "optimisation-problems-setparametertranslation-arma-col-double" %}

{% include example name=include.signature %}

{% when "optimisation-problems-getparametertranslation" %}

{% include example name=include.signature %}

{% when "optimisation-problems-setparameterrotation-arma-mat-double" %}

{% include example name=include.signature %}

{% when "optimisation-problems-getparameterrotation" %}

{% include example name=include.signature %}

{% when "optimisation-problems-setminimalparameterdistance-arma-col-double" %}

{% include example name=include.signature %}

{% when "optimisation-problems-getminimalparameterdistance" %}

{% include example name=include.signature %}

{% when "optimisation-problems-setobjectivevaluescaling-double" %}

{% include example name=include.signature %}

{% when "optimisation-problems-getobjectivevaluescaling" %}

{% include example name=include.signature %}

{% when "optimisation-problems-setobjectivevaluetranslation-double" %}

{% include example name=include.signature %}

{% when "optimisation-problems-getobjectivevaluetranslation" %}

{% include example name=include.signature %}

{% when "optimisation-problems-getcachedsamples" %}

{% include example name=include.signature %}

{% when "optimisation-problems-getnumberofevaluations" %}

{% include example name=include.signature %}

{% when "optimisation-problems-getnumberofdistinctevaluations" %}

{% include example name=include.signature %}

{% when "optimisation-problems-reset" %}

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-attractivesectorfunction-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional attractive sector function.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/attractive-sector-function-surface.png" %}{% assign _alts = _alts | push: "Attractive sector function - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/attractive-sector-function-contour.png" %}{% assign _alts = _alts | push: "Attractive sector function - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:= T_\text{oscillated}\left(\left\Vert S \right\Vert_2^2\right)^{0.9} \\
  Z &:= Q \cdot T_\text{conditioning}^\sqrt{10} \cdot X\\
  S_i &:= \begin{cases}
    100 Z_{i} & \text{if } Z_{i} \cdot {X_T}_i > 0 \\
    Z_{i} & \text{otherwise}
  \end{cases}\\
  Q &:= \text{A rotation matrix}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

**Parameter initialisation**

- The parameter space is randomly and uniformly translated to be within \\([-4, 4]^N\\), rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to \\(-0.00001\\) instead.
- The parameter space is randomly and uniformly rotated.
- \\(Q\\) is set to a randomly and uniformly chosen rotation matrix.

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-bentcigarfunction-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional bent cigar function.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/bent-cigar-function-surface.png" %}{% assign _alts = _alts | push: "Bent cigar function - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/bent-cigar-function-contour.png" %}{% assign _alts = _alts | push: "Bent cigar function - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:= Z_{1} + 1000000 \sum_{i = 2}^{N} Z_{i}\\
  Z &:= \left( Q \cdot T_\text{asymmetric}^{0.5} \cdot \left( Q \cdot X \right) \right)^{2}\\
  F_0 &:= \sum_{i=0}^{11} 0.5^i \cos\left( \pi 3^j \right)\\
  N &:= \text{The number of dimensions}\\
  Q &:= \text{A rotation matrix}\\
  (\ldots)^p &:= \text{Element-wise power}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

**Parameter initialisation**

- The parameter space is randomly and uniformly translated to be within \\([-4, 4]^N\\), rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to \\(-0.00001\\) instead.
- \\(Q\\) is set to a randomly and uniformly chosen rotation matrix.

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-buecherastriginfunction-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional Büche-Rastrigin function.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/b-che-rastrigin-function-surface.png" %}{% assign _alts = _alts | push: "Büche-Rastrigin function - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/b-che-rastrigin-function-contour.png" %}{% assign _alts = _alts | push: "Büche-Rastrigin function - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:= 10 N - \sum_{i = 1}^{N} \left( \cos{2 \pi S_{i}} \right) + \left\Vert S \right\Vert_{2}^{2}\\
  Z &:= T_\text{conditioning}^\sqrt{10} \cdot T_\text{oscillated} \left( X \right)\\
  S_{i} &:= \begin{cases}
    10 Z_{i} & \text{if } i \text{ is odd and } Z_{i} > 0\\
    Z_{i} & \text{otherwise}
  \end{cases}\\
  N &:= \text{The number of dimensions}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 100 \cdot F_\text{penality}(X)$$

**Parameter initialisation**

- is randomly and uniformly translated to be within \([-4, 4]\) for odd and within \([0, 4]\) for even dimensions, rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to \(-0.00001\) for odd and to \(0.00001\) for even dimensions instead.

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-compositegriewankrosenbrockfunctionf8f2-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional composite Griewank Rosenbrock function f8f2.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/composite-griewank-rosenbrock-function-f8f2-surface.png" %}{% assign _alts = _alts | push: "Composite Griewank Rosenbrock function f8f2 - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/composite-griewank-rosenbrock-function-f8f2-contour.png" %}{% assign _alts = _alts | push: "Composite Griewank Rosenbrock function f8f2 - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:= 10 \left( \frac{\sum_{i = 1}^{N} \frac{S_i}{4000} - \cos\left( S_i \right)}{N} + 1 \right)\\
  M &:= \max\left( 1, \frac{\sqrt{N}}{8} \right)\\
  Z &:= M \cdot X + 0.5\\
  S &:= 100 \left( \sum_{i = 1}^{N - 1} \left( Z_{i}^{2} - Z_{i + 1} \right) \right)^{2} + \left( \sum_{i = 1}^{N-1} \left( 1 - Z_{i} \right) \right)^{2}\\
  N &:= \text{The number of dimensions}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

**Parameter initialisation**

- The parameter space is randomly and uniformly rotated.

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-differentpowersfunction-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional different powers function.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/different-powers-function-surface.png" %}{% assign _alts = _alts | push: "Different powers function - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/different-powers-function-contour.png" %}{% assign _alts = _alts | push: "Different powers function - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:= \left\Vert \prod_{i = 1}^{N} \left| X_i \right| Z_i \right\Vert_{2}\\
  Z &:= T_\text{conditioned} \left( X^{2} \right)\\
  N &:= \text{The number of dimensions}\\
  (\ldots)^p &:= \text{Element-wise power}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

**Parameter initialisation**

- The parameter space is randomly and uniformly translated to be within \\([-4, 4]^N\\), rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to \\(-0.00001\\) instead.
- The parameter space is randomly and uniformly rotated.

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-discusfunction-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional discus function.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/discus-function-surface.png" %}{% assign _alts = _alts | push: "Discus function - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/discus-function-contour.png" %}{% assign _alts = _alts | push: "Discus function - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:= 1000000 Z_{1} + \sum_{i = 2}^{N} Z_{i}\\
  Z &:= T_\text{oscillated} \left( X \right)^{2}\\
  N &:= \text{The number of dimensions}\\
  (\ldots)^p &:= \text{Element-wise power}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

**Parameter initialisation**

- The parameter space is randomly and uniformly translated to be within \\([-4, 4]^N\\), rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to \\(-0.00001\\) instead.
- The parameter space is randomly and uniformly rotated.

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-ellipsoidalfunction-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional ellipsoidal function.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/ellipsoidal-function-surface.png" %}{% assign _alts = _alts | push: "Ellipsoidal function - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/ellipsoidal-function-contour.png" %}{% assign _alts = _alts | push: "Ellipsoidal function - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:=  T_\text{conditioning}^{1000000} \cdot T_{oscillated} \left( X \right)^{2} \\
  (\ldots)^p &:= \text{Element-wise power}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

**Parameter initialisation**

- The parameter space is randomly and uniformly translated to be within \\([-4, 4]^N\\), rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to \\(-0.00001\\) instead.

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-ellipsoidalfunctionrotated-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional ellipsoidal function, rotated.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/ellipsoidal-function-rotated-surface.png" %}{% assign _alts = _alts | push: "Ellipsoidal function, rotated - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/ellipsoidal-function-rotated-contour.png" %}{% assign _alts = _alts | push: "Ellipsoidal function, rotated - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:=  T_\text{conditioning}^{1000000} \cdot T_{oscillated} \left( X \right)^{2}\\
  (\ldots)^p &:= \text{Element-wise power}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

**Parameter initialisation**

- The parameter space is randomly and uniformly translated to be within \\([-4, 4]^N\\), rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to \\(-0.00001\\) instead.
- The parameter space is randomly and uniformly rotated.

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-gallaghersgaussian101mepeaksfunction-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional Gallagher's Gaussian 101-me peaks function.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/gallagher-s-gaussian-101-me-peaks-function-surface.png" %}{% assign _alts = _alts | push: "Gallagher's Gaussian 101-me peaks function - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/gallagher-s-gaussian-101-me-peaks-function-contour.png" %}{% assign _alts = _alts | push: "Gallagher's Gaussian 101-me peaks function - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:= T_\text{oscillated} \left( 10 - \max_{i = 1}^{101} \left( W_i \exp \left(-\frac{1}{2N} \left( X - Y_i \right) \cdot \left(R^T \cdot C_i \cdot Q \cdot \left( X - Y_i \right) \right) \right) \right) \right)^{2}\\
  W_i &:= \begin{cases}
  1.1 + 8 \cdot \frac{i - 2}{99} & \text{for } i = 2, \ldots, 101 \\
  10 & \text{for  } i = 1
  \end{cases}\\
  N &:= \text{The number of dimensions}\\
  Q &:= \text{A rotation matrix}\\
  C_i &:= \text{Local parameter conditionings}\\
  Y_i &:= \text{Local parameter translations}
\end{align}$$

**Soft-constraints function:**

$$C(X) := F_\text{penality}(X)$$

**Parameter initialisation**

- \\(Q\\) is set to a randomly and uniformly chosen rotation matrix.
- The first local parameter conditioning \\(C_0\\) is set to<br>
  <span style="padding-left: 20px;">
  $$\frac{T_\text{conditioning}^{10^{1.5}}}{10^{0.75}}$$
  </span><br>
  and all others are set to a randomly and uniformly selected without repetition from<br>
  <span style="padding-left: 20px;">
  $$\left\{\frac{T_\text{conditioning}^{10^\frac{c}{33}}}{10^\frac{c}{66}} : c \in \left\{ 0, \ldots, 100 \right\}\right\}$$
  </span>
- The first local parameter translation \\(Y_0\\) is randomly and uniformly chosen from \\([-4, 4]^N\\) and all others are each chosen from \\([-5, 5]^N\\).

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-gallaghersgaussian21hipeaksfunction-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional Gallagher's Gaussian 21-hi peaks function.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/gallagher-s-gaussian-21-hi-peaks-function-surface.png" %}{% assign _alts = _alts | push: "Gallagher's Gaussian 21-hi peaks function - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/gallagher-s-gaussian-21-hi-peaks-function-contour.png" %}{% assign _alts = _alts | push: "Gallagher's Gaussian 21-hi peaks function - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:= T_\text{oscillated} \left( 10 - \max_{i = 1}^{21} \left( W_i \exp \left(-\frac{1}{2N} \left( X - Y_i \right) \cdot \left(R^T \cdot C_i \cdot Q \cdot \left( X - Y_i \right) \right) \right) \right) \right)^{2}\\
  W_i &:= \begin{cases}
  1.1 + 8 \cdot \frac{i - 2}{19} & \text{for } i = 2, \ldots, 21 \\
  10 & \text{for  } i = 1
  \end{cases}\\
  N &:= \text{The number of dimensions}\\
  Q &:= \text{A rotation matrix}\\
  C_i &:= \text{Local parameter conditionings}\\
  Y_i &:= \text{Local parameter translations}
\end{align}$$

**Soft-constraints function:**

$$C(X) := F_\text{penality}(X)$$

**Parameter initialisation**

- \\(Q\\) is set to a randomly and uniformly chosen rotation matrix.
- The first local parameter conditioning \\(C_0\\) is set to<br>
  <span style="padding-left: 20px;">
  $$\frac{T_\text{conditioning}^{1000}}{\sqrt{1000}}$$
  </span><br>
  and all others are set to a randomly and uniformly selected without repetition from<br>
  <span style="padding-left: 20px;">
  $$\left\{\frac{T_\text{conditioning}^{1000^\frac{c}{19}}}{1000^\frac{c}{38}} : c \in \left\{ 0, \ldots, 20 \right\}\right\}$$
  </span>
- The first local parameter translation \\(Y_0\\) is randomly and uniformly chosen from \\([-3.92, 3.92]^N\\) and all others are each chosen from \\([-4.9, 4.9]^N\\).

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-katsuurafunction-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional Katsuura function.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/katsuura-function-surface.png" %}{% assign _alts = _alts | push: "Katsuura function - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/katsuura-function-contour.png" %}{% assign _alts = _alts | push: "Katsuura function - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:= \frac{10}{N^{2}} \left( \prod_{i=1}^{N}\left( 1 + i \sum_{j=1}^{32} \frac{\left| 2^{j}Z_i - \left[2^{j}Z_{i}\right]\right|}{2^j} \right)^{\frac{10}{N^{1.2}}} - 1 \right)\\
  Z &:= Q \cdot T_\text{conditioned}^{10} \cdot X\\
  N &:= \text{The number of dimensions}\\
  Q &:= \text{A rotation matrix}\\
  [\cdot] &:= \text{Rounding to the nearest integer value}
\end{align}$$

**Soft-constraints function:**

$$C(X) := F_\text{penality}(X)$$

**Parameter initialisation**

- The parameter space is randomly and uniformly translated to be within \\([-4, 4]^N\\), rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to \\(-0.00001\\) instead.
- The parameter space is randomly and uniformly rotated.
- \\(Q\\) is set to a randomly and uniformly chosen rotation matrix.

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-linearslope-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional linear slope.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/linear-slope-surface.png" %}{% assign _alts = _alts | push: "Linear slope - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/linear-slope-contour.png" %}{% assign _alts = _alts | push: "Linear slope - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:= F_0 - S \cdot Z\\
  S &:= T_\text{conditioning}^\sqrt{10}\\
  Z_i &:= \begin{cases}
  X_i & \text{if } X_{i} > {C_L}_i \\
  {C_L}_i & \text{otherwise}
  \end{cases}\\
  F_0 &:= \left\Vert 5 S \right\Vert_2^2\\
  C_L &:= \text{The lower bounds of the search space}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

**Parameter initialisation**

- The parameter space is randomly and uniformly rotated by either 180° or 0° (no rotation).

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-lunacekbirastriginfunction-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional Lunacek bi-Rastrigin function.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/lunacek-bi-rastrigin-function-surface.png" %}{% assign _alts = _alts | push: "Lunacek bi-Rastrigin function - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/lunacek-bi-rastrigin-function-contour.png" %}{% assign _alts = _alts | push: "Lunacek bi-Rastrigin function - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:= \min\left( \sum_{i=1}^{N} \left( X_i - 2.5 \right)^2, N + S \sum_{i=1}^{N} \left( X_i + \mu \right)^2 \right) + 10 \left( N - \sum_{i=1}^N \cos\left( 2 \pi Z_i \right)\right)\\
  Z &:= Q \cdot T_\text{conditioning}^\sqrt{10} \cdot R \cdot \left( X - 2.5 \right)\\
  S &:= 1 - \frac{0.5}{\sqrt{N + 20} - 4.1}\\
  \mu &:= \sqrt{5.25 / S}\\
  N &:= \text{The number of dimensions}\\
  R &:= \text{A rotation matrix}\\
  Q &:= \text{A rotation matrix}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 10000 F_\text{penality}(X)$$

**Parameter initialisation**

- The parameter space is randomly and uniformly translated to be within \\([-4, 4]^N\\), rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to \\(-0.00001\\) instead.
- \\(R\\) is set to a randomly and uniformly chosen rotation matrix.
- \\(Q\\) is set to a randomly and uniformly chosen rotation matrix.

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-rastriginfunction-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional Rastrigin function.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/rastrigin-function-surface.png" %}{% assign _alts = _alts | push: "Rastrigin function - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/rastrigin-function-contour.png" %}{% assign _alts = _alts | push: "Rastrigin function - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:= 10 \left( N - \sum_{i=1}^N \cos\left( 2\pi Z_i \right) \right) + \left\Vert Z \right\Vert_2^2\\
  Z &:= T_\text{conditioning}^\sqrt{10} \cdot T_\text{asymmetric}^{0.2} \left(T_\text{oscillated} \left( X \right) \right)\\
  N &:= \text{The number of dimensions.}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

**Parameter initialisation**

- The parameter space is randomly and uniformly translated to be within \\([-4, 4]^N\\), rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to \\(-0.00001\\) instead.

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-rastriginfunctionrotated-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional Rastrigin function, rotated.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/rastrigin-function-rotated-surface.png" %}{% assign _alts = _alts | push: "Rastrigin function, rotated - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/rastrigin-function-rotated-contour.png" %}{% assign _alts = _alts | push: "Rastrigin function, rotated - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:= 10 \left( N - \sum_{i=1}^N \cos\left( 2\pi Z_i \right) \right) + \left\Vert Z \right\Vert_2^2\\
  Z &:= R \cdot T_\text{conditioning}^\sqrt{10} \cdot Q \cdot T_\text{asymmetric}^{0.2} \left(T_\text{oscillated} \left( R \cdot X \right) \right)\\
  N &:= \text{The number of dimensions}\\
  R &:= \text{A rotation matrix}\\
  Q &:= \text{A rotation matrix}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

**Parameter initialisation**

- The parameter space is randomly and uniformly translated to be within \\([-4, 4]^N\\), rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to \\(-0.00001\\) instead.
- \\(R\\) is set to a randomly and uniformly chosen rotation matrix.
- \\(Q\\) is set to a randomly and uniformly chosen rotation matrix.

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-rosenbrockfunction-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional Rosenbrock function.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/rosenbrock-function-surface.png" %}{% assign _alts = _alts | push: "Rosenbrock function - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/rosenbrock-function-contour.png" %}{% assign _alts = _alts | push: "Rosenbrock function - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:= 100 \sum_{i=1}^{N-1} \left( X_i^2 - X_{i+1} \right)^2 + \sum_{i=1}^{N-1} \left( X_i + 1 \right)^2\\
  Z & := \max\left(1, \sqrt{\frac{N}{2}} \right) \cdot X + 1 \\
  N &:= \text{The number of dimensions}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

**Parameter initialisation**

- The parameter space is randomly and uniformly translated to be within \\([-4, 4]^N\\), rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to \\(-0.00001\\) instead.

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-rosenbrockfunctionrotated-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional Rosenbrock function, rotated.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/rosenbrock-function-rotated-surface.png" %}{% assign _alts = _alts | push: "Rosenbrock function, rotated - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/rosenbrock-function-rotated-contour.png" %}{% assign _alts = _alts | push: "Rosenbrock function, rotated - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:= 100 \sum_{i=1}^{N-1} \left( Z_i^2 - Z_{i+1} \right)^2 + \sum_{i=1}^{N-1} \left( Z_i + 1 \right)^2 \\
  Z & := \max\left(1, \sqrt{\frac{N}{2}} \right) \cdot X + 0.5 \\
  N &:= \text{The number of dimensions}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

**Parameter initialisation**

- The parameter space is randomly and uniformly translated to be within \\([-4, 4]^N\\), rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to \\(-0.00001\\) instead.
- The parameter space is randomly and uniformly rotated.

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-schaffersf7function-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional Schaffers f7 function.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/schaffers-f7-function-surface.png" %}{% assign _alts = _alts | push: "Schaffers f7 function - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/schaffers-f7-function-contour.png" %}{% assign _alts = _alts | push: "Schaffers f7 function - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:= \left( \frac{\sum_{i=1}^{N-1} S_i \cdot \left(1 + \sin\left(50 S_i^{0.4}\right)^2\right)}{N - 1} \right)^2\\
  Z &:= T_\text{conditioning}^\sqrt{10} \cdot Q \cdot T_\text{asymmetric}^{0.5}\left( X \right)\\
  S_i &:= \left(Z_i^2 + Z_{i+1}^2 \right)^{0.25}\\
  N &:= \text{The number of dimensions}\\
  Q &:= \text{A rotation matrix}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 10 F_\text{penality}(X)$$

**Parameter initialisation**

- The parameter space is randomly and uniformly translated to be within \\([-4, 4]^N\\), rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to \\(-0.00001\\) instead.
- The parameter space is randomly and uniformly rotated.
- \\(Q\\) is set to a randomly and uniformly chosen rotation matrix.

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-schaffersf7functionillconditioned-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional Schaffers f7 function, ill-conditioned.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/schaffers-f7-function-ill-conditioned-surface.png" %}{% assign _alts = _alts | push: "Schaffers f7 function, ill-conditioned - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/schaffers-f7-function-ill-conditioned-contour.png" %}{% assign _alts = _alts | push: "Schaffers f7 function, ill-conditioned - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:= \left( \frac{\sum_{i=1}^{N-1} S_i \cdot \left(1 + \sin\left(50 S_i^{0.4}\right)^2\right)}{N - 1} \right)^2\\
  Z &:= T_\text{conditioning}^\sqrt{1000} \cdot Q \cdot T_\text{asymmetric}^{0.5}\left( X \right)\\
  S_i &:= \left(Z_i^2 + Z_{i+1}^2 \right)^{0.25}\\
  N &:= \text{The number of dimensions}\\
  Q &:= \text{A rotation matrix}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 10 F_\text{penality}(X)$$

**Parameter initialisation**

- The parameter space is randomly and uniformly translated to be within \\([-4, 4]^N\\), rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to \\(-0.00001\\) instead.
- The parameter space is randomly and uniformly rotated.
- \\(Q\\) is set to a randomly and uniformly chosen rotation matrix.

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-schwefelfunction-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional Schwefel function.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/schwefel-function-surface.png" %}{% assign _alts = _alts | push: "Schwefel function - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/schwefel-function-contour.png" %}{% assign _alts = _alts | push: "Schwefel function - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:= 0.01 (418.9828872724339 - \frac{\sum_{i=1}^{N} S_i \cdot \sin\left( \sqrt{\left| S_i \right|} \right)}{N})+ 100 F_\text{penality}\left( \frac{Z}{100} \right)\\
  Z_i &:= \begin{cases}
  X_i + 0.25 \left( X_{i-1} - 4.2096874633 \right) & \text{for } i = 2, \ldots, N \\
  X_i & \text{for } i = 1
  \end{cases}\\
  S &:= 100 \left( T_\text{conditioning}^\sqrt{10} \cdot \left( Z - 4.2096874633 \right) + 4.2096874633 \right)\\
  N &:= \text{The number of dimensions}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

**Parameter initialisation**

- The parameter space is randomly and uniformly translated to be within \\([-4, 4]^N\\), rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to \\(-0.00001\\) instead.

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-sharpridgefunction-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional sharp ridge function.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/sharp-ridge-function-surface.png" %}{% assign _alts = _alts | push: "Sharp ridge function - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/sharp-ridge-function-contour.png" %}{% assign _alts = _alts | push: "Sharp ridge function - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:= Z_1^2 + 100 \left\Vert \left(Z_2, \ldots, Z_N \right) \right\Vert_2\\
  Z &:= Q \cdot T_\text{conditioning}^\sqrt{10} \cdot X\\
  N &:= \text{The number of dimensions}\\
  Q &:= \text{A rotation matrix}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

**Parameter initialisation**

- The parameter space is randomly and uniformly translated to be within \\([-4, 4]^N\\), rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to \\(-0.00001\\) instead.
- The parameter space is randomly and uniformly rotated.
- \\(Q\\) is set to a randomly and uniformly chosen rotation matrix.

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-spherefunction-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional sphere function.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/sphere-function-surface.png" %}{% assign _alts = _alts | push: "Sphere function - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/sphere-function-contour.png" %}{% assign _alts = _alts | push: "Sphere function - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:= \left\Vert X \right\Vert_2
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

**Parameter initialisation**

- The parameter space is randomly and uniformly translated to be within \\([-4, 4]^N\\), rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to \\(-0.00001\\) instead.

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-stepellipsoidalfunction-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional step ellipsoidal function.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/step-ellipsoidal-function-surface.png" %}{% assign _alts = _alts | push: "Step ellipsoidal function - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/step-ellipsoidal-function-contour.png" %}{% assign _alts = _alts | push: "Step ellipsoidal function - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:=  0.1 \max\left( \frac{\left| Z_1 \right|}{10000}, T_\text{conditioning}^{100} \cdot Q \cdot \tilde{Z} \right)\\
  Z &:= T_\text{conditioning}^\sqrt{10} \cdot X \\
  \tilde{Z} &:= \begin{cases}
  \left\lfloor 0.5 + Z_i\right\rfloor & \text{if } Z_i > 0.5\\
  \frac{\left\lfloor 0.5 + 10Z_i\right\rfloor}{10} & \text{otherwise}\\
  \end{cases}\\
  Q &:= \text{A rotation matrix}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

**Parameter initialisation**

- The parameter space is randomly and uniformly translated to be within \\([-4, 4]^N\\), rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to \\(-0.00001\\) instead.
- The parameter space is randomly and uniformly rotated.
- \\(Q\\) is set to a randomly and uniformly chosen rotation matrix.

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-bbob-weierstrassfunction-arma-uword" %}

- Instantiates an `numberOfDimensions`-dimensional Weierstrass function.

{% assign _paths = site.array %}
{% assign _alts = site.array %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/weierstrass-function-surface.png" %}{% assign _alts = _alts | push: "Weierstrass function - surface plot" %}
{% assign _paths = _paths | push: "api/black-box-optimisation-benchmark/weierstrass-function-contour.png" %}{% assign _alts = _alts | push: "Weierstrass function - contour plot" %}
{% include images paths=_paths alts=_alts %}

**Objective function:**

$$\begin{align}
  F(X) &:= 10 \left( \frac{\sum_{i=1}^N \sum_{j=0}^{11} 0.5^j \cos\left( 2 \pi 3^j \left(Z_i + 0.5 \right) \right)}{N}  - F_0 \right)^3\\
  Z &:= R \cdot T_\text{conditioning}^\frac{1}{10} \cdot Q \cdot T_\text{oscillated} \left( R \cdot X \right)\\
  F_0 &:= -1.99951171875\\
  N &:= \text{The number of dimensions}\\
  R &:= \text{The parameter space rotation matrix}\\
  Q &:= \text{A rotation matrix}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

**Parameter initialisation**

- The parameter space is randomly and uniformly translated to be within \\([-4, 4]^N\\), rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to \\(-0.00001\\) instead.
- The parameter space is randomly and uniformly rotated.
- \\(Q\\) is set to a randomly and uniformly chosen rotation matrix.

{% include example name=include.signature %}

{% when "black-box-optimisation-benchmark-getoptimalobjectivevalue" %}

- Returns the optimal (minimal) objective value for this problem **within the default bounds**.
- There will be at least one parameter with this objective value **within the default bounds**.
- The optimal objective value is not adjusted to any boundary changes.

**See also**

- {% include link signature="optimisation-algorithms-setacceptableobjectivevalue-double" %}

{% include example name=include.signature %}

{% when "kinematically-redundant-machines-krm-parallelkinematicmachine3prpr" %}

{% include example name=include.signature %}

{% when "kinematically-redundant-machines-krm-parallelkinematicmachine3prrr" %}

{% include example name=include.signature %}

{% when "kinematically-redundant-machines-krm-parallelkinematicmachine6prus" %}

{% include example name=include.signature %}

{% when "kinematically-redundant-machines-krm-parallelkinematicmachine6pups" %}

{% include example name=include.signature %}

{% when "kinematically-redundant-machines-numberofworkspacedimensions" %}

{% include example name=include.signature %}

{% when "kinematically-redundant-machines-setendeffectortrajectory-arma-mat-double" %}

{% include example name=include.signature %}

{% when "kinematically-redundant-machines-getendeffectortrajectory" %}

{% include example name=include.signature %}

{% when "samples-selection-elitists-std-unordered-map-arma-uword" %}

{% include example name=include.signature %}

{% when "samples-analysis-lipschitzcontinuity-std-unordered-map" %}

{% include example name=include.signature %}

{% when "samples-analysis-fitnessdistancecorrelation-std-unordered-map" %}

{% include example name=include.signature %}

{% when "samples-analysis-additiveseparability-optimisationproblem-arma-uword-double-double" %}

{% include example name=include.signature %}

{% when "random-number-generator-rng-getgenerator" %}

{% include example name=include.signature %}

{% when "random-number-generator-rng-setseed-arma-arma-rng-seed-type" %}

{% include example name=include.signature %}

{% when "random-number-generator-rng-setrandomseed" %}

{% include example name=include.signature %}

{% when "random-number-generator-rng-getseed" %}

{% include example name=include.signature %}

{% when "mathematical-functions-polynomialsize-arma-uword-arma-uword" %}

{% include example name=include.signature %}

{% when "mathematical-functions-polynomial-arma-col-double-arma-uword" %}

{% include example name=include.signature %}

{% when "mathematical-functions-factorial-arma-uword" %}

{% include example name=include.signature %}

{% when "mathematical-functions-nchoosek-arma-uword-arma-uword" %}

{% include example name=include.signature %}

{% when "mathematical-functions-combinations-arma-uword-arma-uword" %}

{% include example name=include.signature %}

{% when "mathematical-functions-multicombinations-arma-uword-arma-uword" %}

{% include example name=include.signature %}

{% when "mathematical-functions-rotationmatrix2d-double" %}

{% include example name=include.signature %}

{% when "mathematical-functions-rotationmatrix3d-double-double-double" %}

{% include example name=include.signature %}

{% when "mathematical-functions-circlecircleintersections-arma-col-double-fixed-2-double-arma-col-double-fixed-2-double" %}

{% include example name=include.signature %}

{% when "mathematical-functions-circlesphereintersections-arma-col-double-fixed-3-double-arma-col-double-fixed-3-arma-col-double-fixed-3-double" %}

{% include example name=include.signature %}

{% when "mathematical-functions-hammersleyset-arma-col-arma-uword-arma-col-arma-uword-arma-uword" %}

{% include example name=include.signature %}

{% when "mathematical-functions-haltonsequence-arma-col-arma-uword-arma-col-arma-uword-arma-uword" %}

{% include example name=include.signature %}

{% when "mathematical-functions-vandercorputsequence-arma-uword-arma-uword-arma-uword" %}

{% include example name=include.signature %}

{% when "mathematical-functions-twosetspartitions-arma-uword" %}

{% include example name=include.signature %}

{% when "mathematical-functions-randomrotationmatrix-arma-uword" %}

{% include example name=include.signature %}

{% when "mathematical-functions-randompermutationvector-arma-uword-arma-uword" %}

{% include example name=include.signature %}

{% when "mathematical-functions-randompermutationvector-arma-uword" %}

{% when "mathematical-functions-randomneighbour-arma-col-double-double-double" %}

{% include example name=include.signature %}

{% when "armadillo-and-c-stl-range-arma-uword-arma-uword-arma-uword" %}

{% include example name=include.signature %}

{% when "armadillo-and-c-stl-range-arma-uword-arma-uword" %}

{% when "armadillo-and-c-stl-hash-operator-arma-col-double" %}

{% include example name=include.signature %}

{% when "armadillo-and-c-stl-isequal-operator-arma-col-double-arma-col-double" %}

{% when "message-passing-interface-mpibestsample-void-void-int-mpi-datatype" %}

{% include example name=include.signature %}

{% else %}
{% endcase %}