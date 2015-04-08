<div class="custom-callout custom-callout-info">
#### Inheritance

Extends the black-box optimisation benchmark base class
</div>

**Objective function:**

$$\begin{align}
F(X) &:= 10 \left( N - \sum_{i=1}^N \cos\left( 2\pi Z_i \right) \right) + \left\Vert Z \right\Vert_2^2\\
Z &:= R \cdot T_\text{conditioning}^\sqrt{10} \cdot Q \cdot T_\text{asymmetric}^{0.2} \left(T_\text{oscillated} \left( R \cdot X \right) \right)\\
N &:= \text{The number of dimensions.}\\
R &:= \text{Some rotation matrix.}\\
Q &:= \text{Some rotation matrix.}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

<div class="custom-callout custom-callout-info">
#### Default values

The default values are set as specified by the black box optimisation benchmark.

- The parameter space translation \\(X_T\\) is randomly and uniformly chosen from \\([-4, 4]^N\\), rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to -0.00001 instead.
- \\(R\\) is set to a randomly and uniformly chosen rotation matrix.
- \\(Q\\) is set to a randomly and uniformly chosen rotation matrix.
</div>

Example code, sampling and plotting of the Rastrigin function (rotated).
Create a new source file called **bbob_rastrigin_function_rotated.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/bbob_rastrigin_function_rotated.cpp %}
{% endhighlight %}

Compile and build an executable from the source.
{% highlight bash %}
c++ -std=c++11 bbob_rastrigin_function_rotated.cpp -larmadillo -o bbob_rastrigin_function_rotated
./bbob_rastrigin_function_rotated
{% endhighlight %}

Visualisation of the sampled function using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/bbob_rastrigin_function_rotated.m %}
{% endhighlight %}

![Sampling of the Rastrigin function (rotated) - surface plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob_rastrigin_function_rotated_surface.png)
![Sampling of the Rastrigin function (rotated) - contour plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob_rastrigin_function_rotated_contour.png)

- Constructor<br>
  {% include reference prefix=include.anchor_prefix name="RastriginFunctionRotated" %}
- Parameterisation<br>
  {% include reference prefix=include.anchor_prefix name="setParameterRotationR" %}, {% include reference prefix=include.anchor_prefix name="setParameterRotationQ" %}
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="toString" %}

{% include label prefix=include.anchor_prefix name="RastriginFunctionRotated" %}
**RastriginFunctionRotated( <small>unsigned int</small> N )** {% include continuous-only %}

- Creates an *N*-dimensional optimisation problem instance of this class.
- **Requirement:** The dimension *N* must be greater than or equal to 2.

---
{% include label prefix=include.anchor_prefix name="setParameterRotationR" %}
**<small>void</small> .setParameterRotationR( <small>arma::Mat&lt;double&gt;</small> R )**

- Parameterises the rotation by \\(R\\).
- **Requirement:** The number of rows and columns in *R* must each match the problem dimension.
- **Requirement:** *R* must be square, orthonormal (\\(R^{t} = R^{-1}\\)) and its determinant equal be to 1 or -1.

---
{% include label prefix=include.anchor_prefix name="setParameterRotationQ" %}
**<small>void</small> .setParameterRotationQ( <small>arma::Mat&lt;double&gt;</small> Q )**

- Parameterises the rotation by \\(Q\\).
- **Requirement:** The number of rows and columns in *Q* must each match the problem dimension.
- **Requirement:** *Q* must be square, orthonormal (\\(Q^{t} = Q^{-1}\\)) and its determinant equal be to 1 or -1.

---
{% include label prefix=include.anchor_prefix name="toString" %}
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, e.g. *bbob_rastrigin_function_rotated*.