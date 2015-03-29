*Extends the black-box optimisation benchmark base class*

**Objective function:**

$$\begin{align}
F(X) &:= \left\Vert X - X_T \right\Vert_2\\
X_T &:= \text{The translation of the parameter space.}\\
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

**Minimal parameter and objective function value:**

$$\begin{align}
X_\text{minimal} &= X_T \\
F(X_\text{minimal}) &= 0
\end{align}$$

Example code, sampling and plotting of the sphere function.
Create a new source file called **bbob2015_sphere_function.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/bbob2015_sphere_function.cpp %}
{% endhighlight %}

Compile and build an executable from the source.
{% highlight bash %}
c++ -std=c++11 bbob2015_sphere_function.cpp -larmadillo -o bbob2015_sphere_function
./bbob2015_sphere_function
{% endhighlight %}

Visualisation of the sampled function using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/bbob2015_sphere_function.m %}
{% endhighlight %}

![Sampling of the step ellipsoidal function - surface plot]({{ site.url }}/assets/images/{{ api_reference_folder }}/bbob2015_sphere_function_surface.png)
![Sampling of the step ellipsoidal function - contour plot]({{ site.url }}/assets/images/{{ api_reference_folder }}/bbob2015_sphere_function_contour.png)

- Constructor<br>
  {% include reference prefix=include.anchor_prefix name="SphereFunction" %}
- Parameterisation<br>
  {% include reference prefix="optimisation-problems-" name="setParameterTranslation" %} (inherited)
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="toString" %}

{% include label prefix=include.anchor_prefix name="SphereFunction" %}
**SphereFunction( <small>unsigned int</small> N )** {% include continuous-only %}

- Creates an *N*-dimensional optimisation problem instance of this class.
- **Requirement:** The dimension *N* must be greater than or equal to 1.

---
{% include label prefix=include.anchor_prefix name="toString" %}
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, e.g. *bbob2015_sphere_function*.