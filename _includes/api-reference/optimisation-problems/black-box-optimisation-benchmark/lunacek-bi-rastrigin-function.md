<div class="custom-callout custom-callout-info">
#### Inheritance

Extends the black-box optimisation benchmark base class
</div>

**Objective function:**

$$\begin{align}
F(X) &:= \min\left( \sum_{i=1}^{N} \left( X_i - 2.5 \right)^2, N + S \sum_{i=1}^{N} \left( X_i - \mu \right)^2 \right) + 10 \left( N - \sum_{i=1}^N \cos\left( 2 \pi Z_i \right)\right)\\
Z &:= Q \cdot T_\text{conditioning}^\sqrt{10} \cdot R \cdot \left( X - 2.5 \right)\\
S &:= 1 - \frac{0.5}{\sqrt{N + 20} - 4.1}\\
\mu &:= -\sqrt{5.25 / S}\\
N &:= \text{The number of dimensions.}\\
R &:= \text{Some rotation matrix.}\\
Q &:= \text{Some rotation matrix.}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 10000 F_\text{penality}(X)$$

<div class="custom-callout custom-callout-info">
#### Default values

The default values are set as specified by the black box optimisation benchmark.

- The parameter space scaling \\(X_S\\) is randomly and uniformly chosen to be either set to a posivie or negative vector of twos.
- \\(R\\) is set to a randomly and uniformly chosen rotation matrix.
- \\(Q\\) is set to a randomly and uniformly chosen rotation matrix.
</div>

Example code, sampling and plotting of the Lunacek bi-Rastrigin function.
Create a new source file called **bbob_lunacek_bi_rastrigin_function.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/bbob_lunacek_bi_rastrigin_function.cpp %}
{% endhighlight %}

Compile and build an executable from the source.
{% highlight bash %}
c++ -std=c++11 bbob_lunacek_bi_rastrigin_function.cpp -larmadillo -o bbob_lunacek_bi_rastrigin_function
./bbob_lunacek_bi_rastrigin_function
{% endhighlight %}

Visualisation of the sampled function using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/bbob_lunacek_bi_rastrigin_function.m %}
{% endhighlight %}

![Sampling of the Lunacek bi-Rastrigin function - surface plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob_lunacek_bi_rastrigin_function_surface.png)
![Sampling of the Lunacek bi-Rastrigin function - contour plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob_lunacek_bi_rastrigin_function_contour.png)

- Constructor<br>
  {% include reference prefix=include.anchor_prefix name="LunacekBiRastriginFunction" %}
- Parameterisation<br>
  {% include reference prefix=include.anchor_prefix name="setParameterRotationR" %}, {% include reference prefix=include.anchor_prefix name="setParameterRotationQ" %}
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="toString" %}

{% include label prefix=include.anchor_prefix name="LunacekBiRastriginFunction" %}
**LunacekBiRastriginFunction( <small>unsigned int</small> N )** {% include continuous-only %}

- Creates an *N*-dimensional optimisation problem instance of this class.
- **Requirement:** The dimension *N* must be greater than or equal to 1.

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

- Returns a filesystem friendly name of the problem, e.g. *lunacek-bi-rastrigin-function*.