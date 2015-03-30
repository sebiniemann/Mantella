*Extends the black-box optimisation benchmark base class*

**Objective function:**

$$\begin{align}
F(X) &:= \frac{10}{N^{2}} \left( \prod_{i=1}^{N}\left( 1 + i \sum_{j=1}^{32} \frac{\left| 2^{j}Z_i - \left[2^{j}Z_{i}\right]\right|}{2^j} \right)^{\frac{10}{N^{1.2}}} - 1 \right)\\
Z &:= Q \cdot T_\text{scaled}^{10} \cdot R \cdot \left( X - X^\text{opt} \right)\\
X_T &:= \text{The translation of the parameter space.}\\
R &:= \text{Some rotation matrix.}\\
Q &:= \text{Some rotation matrix.}\\
[\cdot] &:= \text{nearest integer value}
\end{align}$$

**Soft-constraints function:**

$$C(X) := F_\text{penality}(X)$$

Example code, sampling and plotting of the katsuura function.
Create a new source file called **bbob2015_katsuura_function.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/bbob2015_katsuura_function.cpp %}
{% endhighlight %}

Compile and build an executable from the source.
{% highlight bash %}
c++ -std=c++11 bbob2015_katsuura_function.cpp -larmadillo -o bbob2015_katsuura_function
./bbob2015_katsuura_function
{% endhighlight %}

Visualisation of the sampled function using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/bbob2015_katsuura_function.m %}
{% endhighlight %}

![Sampling of the step ellipsoidal function - surface plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob2015_katsuura_function_surface.png)
![Sampling of the step ellipsoidal function - contour plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob2015_katsuura_function_contour.png)

- Constructor<br>
  {% include reference prefix=include.anchor_prefix name="KatsuuraFunction" %}
- Parameterisation<br>
  {% include reference prefix="optimisation-problems-" name="setParameterTranslation" %} (inherited), {% include reference prefix=include.anchor_prefix name="setParameterRotationR" %}, {% include reference prefix=include.anchor_prefix name="setParameterRotationQ" %}
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="toString" %}

{% include label prefix=include.anchor_prefix name="KatsuuraFunction" %}
**KatsuuraFunction( <small>unsigned int</small> N )**

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

- Returns a filesystem friendly name of the problem, e.g. *bbob2015_katsuura_function*.