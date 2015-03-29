*Extends the black-box optimisation benchmark base class*

**Objective function:**

$$\begin{align}
F(X) &:= 100 \sum_{i=1}^{N-1} \left( Z_i^2 - Z_{i+1} \right)^2 + \sum_{i=1}^{N-1} \left( Z_i + 1 \right)^2\\
Z &:= \max\left(1, \frac{\sqrt{N}}{8}\right) \cdot R \cdot X + 0.5\\
R &:= \text{Some rotation matrix.}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

**Minimal parameter and objective function value:**
    
$$\begin{align}
X_\text{minimal} &= \frac{R \cdot 0.5}{\max\left(1, \frac{\sqrt{N}}{8}\right)} \\
F(X_\text{minimal}) &= 0
\end{align}$$

Example code, sampling and plotting of the rosenbrock function (rotated).
Create a new source file called **bbob2015_rosenbrock_function_rotated.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/bbob2015_rosenbrock_function_rotated.cpp %}
{% endhighlight %}

Compile and build an executable from the source.
{% highlight bash %}
c++ -std=c++11 bbob2015_rosenbrock_function_rotated.cpp -larmadillo -o bbob2015_rosenbrock_function_rotated
./bbob2015_rosenbrock_function_rotated
{% endhighlight %}

Visualisation of the sampled function using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/bbob2015_rosenbrock_function_rotated.m %}
{% endhighlight %}

![Sampling of the step ellipsoidal function - surface plot]({{ site.url }}/assets/images/{{ api_reference_folder }}/bbob2015_rosenbrock_function_rotated_surface.png)
![Sampling of the step ellipsoidal function - contour plot]({{ site.url }}/assets/images/{{ api_reference_folder }}/bbob2015_rosenbrock_function_rotated_contour.png)

- Constructor<br>
  {% include reference prefix=include.anchor_prefix name="RosenbrockFunctionRotated" %}
- Parameterisation<br>
  {% include reference prefix=include.anchor_prefix name="setParameterRotationR" %}
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="toString" %}
  
{% include label prefix=include.anchor_prefix name="RosenbrockFunctionRotated" %}
**RosenbrockFunctionRotated( <small>unsigned int</small> N )** {% include continuous-only %}

- Creates an *N*-dimensional optimisation problem instance of this class.
- **Requirement:** The dimension *N* must be greater than or equal to 2.

---
{% include label prefix=include.anchor_prefix name="setParameterRotationR" %}
**<small>void</small> .setParameterRotationR( <small>arma::Mat&lt;T&gt;</small> R )**

- Parameterises the rotation by \\(R\\).
- **Requirement:** The number of rows and columns in *R* must each match the problem dimension.
- **Requirement:** *R* must be square, orthonormal (\\(R^{t} = R^{-1}\\)) and its determinant equal be to 1 or -1.

---
{% include label prefix=include.anchor_prefix name="toString" %}
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, e.g. *bbob2015_rosenbrock_function_rotated*.