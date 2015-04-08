<div class="custom-callout custom-callout-info">
#### Inheritance

Extends the black-box optimisation benchmark base class
</div>

**Objective function:**

$$\begin{align}
F(X) &:= 10 \left( \frac{\sum_{i = 1}^{N} \frac{S_i}{4000} - \cos\left( S_i \right)}{N} + 1 \right)\\
M &:= \max\left( 1, \frac{\sqrt{N}}{8} \right)\\
Z &:= M \cdot X + 0.5\\
S &:= 100 \left( \sum_{i = 1}^{N - 1} \left( Z_{i}^{2} - Z_{i + 1} \right) \right)^{2} + \left( \sum_{i = 1}^{N-1} \left( 1 - Z_{i} \right) \right)^{2}\\
N &:= \text{The number of dimensions.}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

<div class="custom-callout custom-callout-info">
#### Default values

The default values are set as specified by the black box optimisation benchmark.

- The parameter space rotation \\(X_R\\) is set to a randomly and uniformly chosen rotation matrix.
</div>

Example code, sampling and plotting of the composite Griewank-Rosenbrock function f8f2.
Create a new source file called **bbob_composite_griewank_rosenbrock_function_f8f2.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/bbob_composite_griewank_rosenbrock_function_f8f2.cpp %}
{% endhighlight %}

Compile and build an executable from the source.
{% highlight bash %}
c++ -std=c++11 bbob_composite_griewank_rosenbrock_function_f8f2.cpp -larmadillo -o bbob_composite_griewank_rosenbrock_function_f8f2
./bbob_composite_griewank_rosenbrock_function_f8f2
{% endhighlight %}

Visualisation of the sampled function using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/bbob_composite_griewank_rosenbrock_function_f8f2.m %}
{% endhighlight %}

![Sampling of the composite Griewank-Rosenbrock function f8f2 - surface plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob_composite_griewank_rosenbrock_function_f8f2_surface.png)
![Sampling of the composite Griewank-Rosenbrock function f8f2 - contour plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob_composite_griewank_rosenbrock_function_f8f2_contour.png)

- Constructor<br>
  {% include reference prefix=include.anchor_prefix name="CompositeGriewankRosenbrockFunctionF8F2" %}
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="toString" %}

{% include label prefix=include.anchor_prefix name="CompositeGriewankRosenbrockFunctionF8F2" %}
**CompositeGriewankRosenbrockFunctionF8F2( <small>unsigned int</small> N )** {% include continuous-only %}

- Creates an *N*-dimensional optimisation problem instance of this class.
- **Requirement:** The dimension *N* must be greater than or equal to 2.

---
{% include label prefix=include.anchor_prefix name="toString" %}
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, e.g. *bbob_composite_griewank_rosenbrock_function_f8f2*.