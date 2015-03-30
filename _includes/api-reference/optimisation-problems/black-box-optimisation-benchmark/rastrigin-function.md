*Extends the black-box optimisation benchmark base class*

**Objective function:**

$$\begin{align}
F(X) &:= 10 \left( N - \sum_{i=1}^N \cos\left( 2\pi Z_i \right) \right) + \left\Vert Z \right\Vert_2^2\\
Z &:= T_\text{conditioning}^\sqrt{10} \cdot T_\text{asymmetric}^{0.2} \left(T_\text{oscillated} \left( X \right) \right)\\
N &:= \text{The number of dimensions.}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

Example code, sampling and plotting of the rastrigin function.
Create a new source file called **bbob2015_rastrigin_function.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/bbob2015_rastrigin_function.cpp %}
{% endhighlight %}

Compile and build an executable from the source.
{% highlight bash %}
c++ -std=c++11 bbob2015_rastrigin_function.cpp -larmadillo -o bbob2015_rastrigin_function
./bbob2015_rastrigin_function
{% endhighlight %}

Visualisation of the sampled function using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/bbob2015_rastrigin_function.m %}
{% endhighlight %}

![Sampling of the step ellipsoidal function - surface plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob2015_rastrigin_function_surface.png)
![Sampling of the step ellipsoidal function - contour plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob2015_rastrigin_function_contour.png)

- Constructor<br>
  {% include reference prefix=include.anchor_prefix name="RastriginFunction" %}
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="toString" %}

{% include label prefix=include.anchor_prefix name="RastriginFunction" %}
**RastriginFunction( <small>unsigned int</small> N )** {% include continuous-only %}

- Creates an *N*-dimensional optimisation problem instance of this class.
- **Requirement:** The dimension *N* must be greater than or equal to 1.

---
{% include label prefix=include.anchor_prefix name="toString" %}
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, e.g. *bbob2015_rastrigin_function*.