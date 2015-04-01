*Extends the black-box optimisation benchmark base class*

**Objective function:**

$$\begin{align}
F(X) &:= 10 \left( \frac{\sum_{i=1}^N \sum_{j=0}^{11} 0.5^j \cos\left( 2 \pi 3^j \left(Z_i + 0.5 \right) \right)}{N}  - F_0 \right)^3\\
Z &:= X_R \cdot T_\text{conditioning}^\frac{1}{10} \cdot Q \cdot T_\text{oscillated} \left( X \right)\\
F_0 &:= \sum_{i=0}^{11} 0.5^i \cos\left( \pi 3^j \right)\\
N &:= \text{The number of dimensions.}\\
X_R &:= \text{The rotation of the parameter space.}\\
Q &:= \text{Some rotation matrix.}
\end{align}$$

**Soft-constraints function:**

$$C(X) := \frac{10 F_\text{penality}(X)}{N}$$

Example code, sampling and plotting of the Weierstrass function.
Create a new source file called **bbob2015_weierstrass_function.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/bbob2015_weierstrass_function.cpp %}
{% endhighlight %}

Compile and build an executable from the source.
{% highlight bash %}
c++ -std=c++11 bbob2015_weierstrass_function.cpp -larmadillo -o bbob2015_weierstrass_function
./bbob2015_weierstrass_function
{% endhighlight %}

Visualisation of the sampled function using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/bbob2015_weierstrass_function.m %}
{% endhighlight %}

![Sampling of the Weierstrass function - surface plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob2015_weierstrass_function_surface.png)
![Sampling of the Weierstrass function - contour plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob2015_weierstrass_function_contour.png)

- Constructor<br>
  {% include reference prefix=include.anchor_prefix name="WeierstrassFunction" %}
- Parameterisation<br>
  {% include reference prefix=include.anchor_prefix name="setParameterRotationQ" %}
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="toString" %}

{% include label prefix=include.anchor_prefix name="WeierstrassFunction" %}
**WeierstrassFunction( <small>unsigned int</small> N )** {% include continuous-only %}

- Creates an *N*-dimensional optimisation problem instance of this class.
- **Requirement:** The dimension *N* must be greater than or equal to 1.

---
{% include label prefix=include.anchor_prefix name="setParameterRotationQ" %}
**<small>void</small> .setParameterRotationQ( <small>arma::Mat&lt;double&gt;</small> Q )**

- Parameterises the rotation by \\(Q\\).
- **Requirement:** The number of rows and columns in *Q* must each match the problem dimension.
- **Requirement:** *Q* must be square, orthonormal (\\(Q^{t} = Q^{-1}\\)) and its determinant equal be to 1 or -1.

---
{% include label prefix=include.anchor_prefix name="toString" %}
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, e.g. *bbob2015_weierstrass_function*.


