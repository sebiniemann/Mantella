<div class="custom-callout custom-callout-info">
#### Inheritance

Extends the black-box optimisation benchmark base class
</div>

**Objective function:**

$$\begin{align}
F(X) &:= F_0 - S \cdot Z\\
S &:= T_\text{conditioning}^\sqrt{10}\\
Z_i &:= \begin{cases}
X_i & \text{if } X_{i} > {C_L}_i \\
{C_L}_i & \text{otherwise}
\end{cases}\\
F_0 &:= \left\Vert 5 S \right\Vert_2^2\\
C_L &:= \text{The lower bounds of the search space.}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

<div class="custom-callout custom-callout-info">
#### Default values

The default values are set as specified by the black box optimisation benchmark.

- The parameter space rotation \\(X_R\\) is randomly and uniformly chosen to be either set to a 180° rotation for each dimension, or no rotation at all.
</div>

Example code, sampling and plotting of the linear slope function.
Create a new source file called **bbob_linear_slope.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/bbob_linear_slope.cpp %}
{% endhighlight %}

Compile and build an executable from the source.
{% highlight bash %}
{% include {{ api_reference_folder }}/_examples/bbob_linear_slope.core.sh %}
{% endhighlight %}

Visualisation of the sampled function using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/bbob_linear_slope.m %}
{% endhighlight %}

![Sampling of the linear slope - surface plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob_linear_slope_surface.png)
![Sampling of the linear slope - contour plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob_linear_slope_contour.png)

- Constructor<br>
  {% include reference prefix=include.anchor_prefix name="LinearSlope" %}
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="toString" %}

{% include label prefix=include.anchor_prefix name="LinearSlope" %}
**LinearSlope( <small>unsigned int</small> N )** {% include continuous-only %}

- Creates an *N*-dimensional optimisation problem instance of this class.
- **Requirement:** The dimension *N* must be greater than or equal to 2.

---
{% include label prefix=include.anchor_prefix name="toString" %}
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, e.g. *bbob_linear_slope*.