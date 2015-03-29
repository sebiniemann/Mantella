*Extends the black-box optimisation benchmark base class*

**Objective function:**

$$\begin{align}
F(X) &:=  0.1 \max\left( \frac{\left| Z_1 \right|}{10000}, T_\text{scaled}^{100} \cdot Q \cdot \tilde{Z} \right)\\
Z &:= T_\text{scaled}^\sqrt{10} \cdot R \cdot \left( X - X_T \right) \\
\tilde{Z} &:= \begin{cases}
\left\lfloor 0.5 + Z_i\right\rfloor & \text{if } Z_i > 0.5\\
\frac{\left\lfloor 0.5 + 10Z_i\right\rfloor}{10} & \text{otherwise}\\
\end{cases}\\
X_T &:= \text{The translation of the parameter space.}\\
R &:= \text{Some rotation matrix.}\\
Q &:= \text{Some rotation matrix.}
\end{align}$$

**Soft-constraints function:**

$$C(X) := F_\text{penality}(X)$$

**Minimal parameter and objective function value:**

$$\begin{align}
X_\text{minimal} &= X_T \\
F(X_\text{minimal}) &= 0
\end{align}$$

Example code, sampling and plotting of the step ellipsoidal function.
Create a new source file called **bbob2015_step_ellipsoidal_function.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/bbob2015_step_ellipsoidal_function.cpp %}
{% endhighlight %}

Compile and build an executable from the source.
{% highlight bash %}
c++ -std=c++11 bbob2015_step_ellipsoidal_function.cpp -larmadillo -o bbob2015_step_ellipsoidal_function
./bbob2015_step_ellipsoidal_function
{% endhighlight %}

Visualisation of the sampled function using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/bbob2015_step_ellipsoidal_function.m %}
{% endhighlight %}

![Sampling of the step ellipsoidal function - surface plot]({{ site.url }}/assets/images/{{ api_reference_folder }}/bbob2015_step_ellipsoidal_function_surface.png)
![Sampling of the step ellipsoidal function - contour plot]({{ site.url }}/assets/images/{{ api_reference_folder }}/bbob2015_step_ellipsoidal_function_contour.png)

- Constructor<br>
  {% include reference prefix=include.anchor_prefix name="StepEllipsoidalFunction" %}
- Parameterisation<br>
  {% include reference prefix="optimisation-problems-" name="setParameterTranslation" %} (inherited), {% include reference prefix=include.anchor_prefix name="setParameterRotationR" %}, {% include reference prefix=include.anchor_prefix name="setParameterRotationQ" %}
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="toString" %}
  
{% include label prefix=include.anchor_prefix name="StepEllipsoidalFunction" %}
**StepEllipsoidalFunction( <small>unsigned int</small> N )** {% include continuous-only %}

- Creates an *N*-dimensional optimisation problem instance of this class.
- **Requirement:** The dimension *N* must be greater than or equal to 1.

---
{% include label prefix=include.anchor_prefix name="setParameterRotationR" %}
**<small>void</small> .setParameterRotationR( <small>arma::Mat&lt;T&gt;</small> R )**

- Parameterises the rotation by \\(R\\).
- **Requirement:** The number of rows and columns in *R* must each match the problem dimension.
- **Requirement:** *R* must be square, orthonormal (\\(R^{t} = R^{-1}\\)) and its determinant equal be to 1 or -1.

---
{% include label prefix=include.anchor_prefix name="setParameterRotationQ" %}
**<small>void</small> .setParameterRotationQ( <small>arma::Mat&lt;T&gt;</small> Q )**

- Parameterises the rotation by \\(Q\\).
- **Requirement:** The number of rows and columns in *Q* must each match the problem dimension.
- **Requirement:** *Q* must be square, orthonormal (\\(Q^{t} = Q^{-1}\\)) and its determinant equal be to 1 or -1.

---
{% include label prefix=include.anchor_prefix name="toString" %}
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, e.g. *bbob2015_step_ellipsoidal_function*.


