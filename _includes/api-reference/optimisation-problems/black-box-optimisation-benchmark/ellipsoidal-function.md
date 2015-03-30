*Extends the black-box optimisation benchmark base class*

**Objective function:**

$$\begin{align}
F(X) &:=  T_\text{conditioning}^{1000000} \cdot T_{oscillated} \left( X \right)^{2}
(\ldots)^p &:= \text{Element-wise power}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

Example code, sampling and plotting of the ellipsoidal function.
Create a new source file called **bbob2015_ellipsoidal_function.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/bbob2015_ellipsoidal_function.cpp %}
{% endhighlight %}

Compile and build an executable from the source.
{% highlight bash %}
c++ -std=c++11 bbob2015_ellipsoidal_function.cpp -larmadillo -o bbob2015_ellipsoidal_function
./bbob2015_ellipsoidal_function
{% endhighlight %}

Visualisation of the sampled function using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/bbob2015_ellipsoidal_function.m %}
{% endhighlight %}

![Sampling of the ellipsoidal function - surface plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob2015_ellipsoidal_function_surface.png)
![Sampling of the ellipsoidal function - contour plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob2015_ellipsoidal_function_contour.png)

- Constructor<br>
  {% include reference prefix=include.anchor_prefix name="EllipsoidalFunction" %}
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="toString" %}

{% include label prefix=include.anchor_prefix name="EllipsoidalFunction" %}
**EllipsoidalFunction( <small>unsigned int</small> N )** {% include continuous-only %}

- Creates an *N*-dimensional optimisation problem instance of this class.
- **Requirement:** The dimension *N* must be greater than or equal to 1.

---
{% include label prefix=include.anchor_prefix name="toString" %}
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, e.g. *bbob2015_ellipsoidal_function*.


