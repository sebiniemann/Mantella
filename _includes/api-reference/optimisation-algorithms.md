Abstract base class for all optimisation algorithms.<br>
*Note:* All optimisation algorithms are implemented to minimise the objective value.

---
<a name="optimisation-algorithms-optimise"></a>
**<small>void</small> .setDistanceFunction( <small>std::shared_ptr&lt;mant::DistanceFunction&gt;</small> D )** {% include noexcept %}

---
<a name="optimisation-algorithms-optimise"></a>
**<small>void</small> .setMaximalNumberOfIterations( <small>unsigned int</small> N )** {% include noexcept %}

---
<a name="optimisation-algorithms-optimise"></a>
**<small>void</small> .optimise()** {% include noexcept %}

---
<a name="optimisation-algorithms-optimise"></a>
**<small>arma::Col&lt;T&gt;</small> .getBestParameter()** {% include noexcept %}

---
<a name="optimisation-algorithms-optimise"></a>
**<small>double</small> .getBestSoftConstraintsValue()** {% include noexcept %}

---
<a name="optimisation-algorithms-optimise"></a>
**<small>double</small> .getBestObjectiveValue()** {% include noexcept %}

---
<a name="optimisation-algorithms-optimise"></a>
**<small>unsigned int</small> .getNumberOfIterations()** {% include noexcept %}

---
<a name="optimisation-algorithms-optimise"></a>
**<small>bool</small> .isFinished()** {% include noexcept %}

---
<a name="optimisation-algorithms-optimise"></a>
**<small>bool</small> .isTerminated()** {% include noexcept %}