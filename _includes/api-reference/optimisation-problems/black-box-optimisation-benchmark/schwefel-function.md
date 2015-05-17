<div class="custom-callout custom-callout-info">
#### Inheritance

Extends the black-box optimisation benchmark base class
</div>

**Objective function:**

$$\begin{align}
F(X) &:= 0.01 (418.9828872724339 - \frac{\sum_{i=1}^{N} S_i \cdot \sin\left( \sqrt{\left| S_i \right|} \right)}{N})+ 100 F_\text{penality}\left( \frac{Z}{100} \right)\\
Z_i &:= \begin{cases}
X_i + 0.25 \left( X_{i-1} - 4.2096874633 \right) & \text{for } i = 2, \ldots, N \\
X_i & \text{for } i = 1
\end{cases}\\
S &:= 100 \left( T_\text{conditioning}^\sqrt{10} \cdot \left( Z - 4.2096874633 \right) + 4.2096874633 \right)\\
N &:= \text{The number of dimensions.}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

<div class="custom-callout custom-callout-info">
#### Default values

The default values are set as specified by the black box optimisation benchmark.

- The parameter space scaling \\(X_S\\) is randomly and uniformly chosen to be either set to a posivie or negative vector of twos.
</div>

Example code, sampling and plotting of the Schwefel function.

Create a new source file called **bbob_schwefel_function.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/bbob_schwefel_function.cpp %}
{% endhighlight %}

Compilation and execution:
{% highlight bash %}
{% include {{ api_reference_folder }}/_examples/bbob_schwefel_function.core.sh %}
{% endhighlight %}

Visualisation of the sampled function, using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/bbob_schwefel_function.m %}
{% endhighlight %}

![Sampling of the schwefel function - surface plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob_schwefel_function_surface.png)
![Sampling of the schwefel function - contour plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob_schwefel_function_contour.png)

- Constructor<br>
  {% include reference prefix=include.anchor_prefix name="SchwefelFunction" %}
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="toString" %}

---
{% include label prefix=include.anchor_prefix name="SchwefelFunction" %}
**SchwefelFunction( <small>unsigned int</small> N )** {% include continuous-only %}

- Creates an *N*-dimensional optimisation problem instance of this class.
- **Requirement:** The dimension *N* must be greater than or equal to 2.

---
{% include label prefix=include.anchor_prefix name="toString" %}
**<small>std::string</small> .toString()** {% include noexcept %}


- Returns a filesystem friendly name of the problem, e.g. *bbob_schwefel_function*.