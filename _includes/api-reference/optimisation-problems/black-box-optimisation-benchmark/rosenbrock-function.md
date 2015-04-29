<div class="custom-callout custom-callout-info">
#### Inheritance

Extends the black-box optimisation benchmark base class
</div>

**Objective function:**

$$\begin{align}
F(X) &:= 100 \sum_{i=1}^{N-1} \left( X_i^2 - X_{i+1} \right)^2 + \sum_{i=1}^{N-1} \left( X_i + 1 \right)^2\\
Z & := \max\left(1, \sqrt{\frac{N}{2}} \right) \cdot X + 1 \\
N &:= \text{The number of dimensions.}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

<div class="custom-callout custom-callout-info">
#### Default values

The default values are set as specified by the black box optimisation benchmark.

- The parameter space translation \\(X_T\\) is randomly and uniformly chosen from \\([-3, 3]^N\\), rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to -0.00001 instead.
</div>

Example code, sampling and plotting of the Rosenbrock function.
Create a new source file called **bbob_rosenbrock_function.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/bbob_rosenbrock_function.cpp %}
{% endhighlight %}

Compile and build an executable from the source.
{% highlight bash %}
{% include {{ api_reference_folder }}/_examples/bbob_rosenbrock_function.core.sh %}
{% endhighlight %}

Visualisation of the sampled function using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/bbob_rosenbrock_function.m %}
{% endhighlight %}

![Sampling of the Rosenbrock function - surface plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob_rosenbrock_function_surface.png)
![Sampling of the Rosenbrock function - contour plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob_rosenbrock_function_contour.png)

- Constructor<br>
  {% include reference prefix=include.anchor_prefix name="RosenbrockFunction" %}
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="toString" %}

**RosenbrockFunction( <small>unsigned int</small> N )** {% include continuous-only %}

- Creates an *N*-dimensional optimisation problem instance of this class.
- **Requirement:** The dimension *N* must be greater than or equal to 2.

---
{% include label prefix=include.anchor_prefix name="toString" %}
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, e.g. *bbob_rosenbrock_function*.