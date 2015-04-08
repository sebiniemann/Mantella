<div class="custom-callout custom-callout-info">
#### Inheritance

Extends the black-box optimisation benchmark base class
</div>

**Objective function:**

$$\begin{align}
F(X) &:= 100 \sum_{i=1}^{N-1} \left( X_i^2 - X_{i+1} \right)^2 + \sum_{i=1}^{N-1} \left( X_i + 1 \right)^2
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

Example code, sampling and plotting of the Rosenbrock function (rotated).
Create a new source file called **bbob_rosenbrock_function_rotated.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/bbob_rosenbrock_function_rotated.cpp %}
{% endhighlight %}

Compile and build an executable from the source.
{% highlight bash %}
c++ -std=c++11 bbob_rosenbrock_function_rotated.cpp -larmadillo -o bbob_rosenbrock_function_rotated
./bbob_rosenbrock_function_rotated
{% endhighlight %}

Visualisation of the sampled function using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/bbob_rosenbrock_function_rotated.m %}
{% endhighlight %}

![Sampling of the Rosenbrock function (rotated) - surface plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob_rosenbrock_function_rotated_surface.png)
![Sampling of the Rosenbrock function (rotated) - contour plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob_rosenbrock_function_rotated_contour.png)

- Constructor<br>
  {% include reference prefix=include.anchor_prefix name="RosenbrockFunctionRotated" %}
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="toString" %}
  
{% include label prefix=include.anchor_prefix name="RosenbrockFunctionRotated" %}
**RosenbrockFunctionRotated( <small>unsigned int</small> N )** {% include continuous-only %}

- Creates an *N*-dimensional optimisation problem instance of this class.
- **Requirement:** The dimension *N* must be greater than or equal to 2.

---
{% include label prefix=include.anchor_prefix name="toString" %}
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, e.g. *bbob2015_rosenbrock_function_rotated*.