*Extends the black-box optimisation benchmark base class*

**Objective function:**

$$\begin{align}
F(X) &:= 100 \sum_{i=1}^{N-1} \left( Z_i^2 - Z_{i+1} \right)^2 + \sum_{i=1}^{N-1} \left( Z_i + 1 \right)^2\\
Z &:= \max\left(1, \frac{\sqrt{N}}{8}\right) \cdot \left( X - X^\text{opt} \right) + 1
N &:= \text{The number of dimensions.}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

Example code, sampling and plotting of the Rosenbrock function.
Create a new source file called **bbob2015_rosenbrock_function.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/bbob2015_rosenbrock_function.cpp %}
{% endhighlight %}

Compile and build an executable from the source.
{% highlight bash %}
c++ -std=c++11 bbob2015_rosenbrock_function.cpp -larmadillo -o bbob2015_rosenbrock_function
./bbob2015_rosenbrock_function
{% endhighlight %}

Visualisation of the sampled function using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/bbob2015_rosenbrock_function.m %}
{% endhighlight %}

![Sampling of the Rosenbrock function - surface plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob2015_rosenbrock_function_surface.png)
![Sampling of the Rosenbrock function - contour plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob2015_rosenbrock_function_contour.png)

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

- Returns a filesystem friendly name of the problem, e.g. *bbob2015_rosenbrock_function*.