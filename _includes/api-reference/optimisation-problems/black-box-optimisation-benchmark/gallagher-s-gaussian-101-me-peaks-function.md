<div class="custom-callout custom-callout-info">
#### Inheritance

Extends the black-box optimisation benchmark base class
</div>

**Objective function:**

$$\begin{align}
F(X) &:= T_\text{oscillated} \left( 10 - \max_{i = 1}^{101} \left( W_i \exp \left(-\frac{1}{2N} \left( X - Y_i \right) \cdot \left(R^T \cdot C_i \cdot Q \cdot \left( X - Y_i \right) \right) \right) \right) \right)^{2}\\
W_i &:= \begin{cases}
1.1 + 8 \cdot \frac{i - 2}{99} & \text{for } i = 2, \ldots, 101 \\
10 & \text{for  } i = 1
\end{cases}\\
N &:= \text{The number of dimensions.}\\
Q &:= \text{Some rotation matrix.}\\
C_i &:= \text{Some local parameter conditionings.}\\
Y_i &:= \text{Some local parameter translations}
\end{align}$$

**Soft-constraints function:**

$$C(X) := F_\text{penality}(X)$$

<div class="custom-callout custom-callout-info">
#### Default values

The default values are set as specified by the black box optimisation benchmark.

- \\(Q\\) is set to a randomly and uniformly chosen rotation matrix.
- The first local parameter conditioning \\(C_0\\) is set to<br>
  <span style="padding-left: 20px;">
    $$\frac{T_\text{conditioning}^{10^{1.5}}}{10^{0.75}}$$
  </span><br>
  and all others are set to a randomly and uniformly selected without repetition from<br>
  <span style="padding-left: 20px;">
    $$\left\{\frac{T_\text{conditioning}^{10^\frac{c}{33}}}{10^\frac{c}{66}} : c \in \left\{ 0, \ldots, 100 \right\}\right\}$$
  </span>
- The first local parameter translation \\(Y_0\\) is randomly and uniformly chosen from \\([-4, 4]^N\\) and all others are each chosen from \\([-5, 5]^N\\).
</div>

Example code, sampling and plotting of the Gallagher's Gaussian 101-me peaks function.
Create a new source file called **bbob_gallaghers_gaussian_101_me_peaks_function.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/bbob_gallaghers_gaussian_101_me_peaks_function.cpp %}
{% endhighlight %}

Compile and build an executable from the source.
{% highlight bash %}
{% include {{ api_reference_folder }}/_examples/bbob_gallaghers_gaussian_101_me_peaks_function.core.sh %}
{% endhighlight %}

Visualisation of the sampled function using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/bbob_gallaghers_gaussian_101_me_peaks_function.m %}
{% endhighlight %}

![Sampling of the Gallagher's Gaussian 101-me peaks function - surface plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob_gallaghers_gaussian_101_me_peaks_function_surface.png)
![Sampling of the Gallagher's Gaussian 101-me peaks function - contour plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob_gallaghers_gaussian_101_me_peaks_function_contour.png)

- Constructor<br>
  {% include reference prefix=include.anchor_prefix name="GallaghersGaussian101mePeaksFunction" %}
- Parameterisation<br>
  {% include reference prefix=include.anchor_prefix name="setParameterRotationQ" %}, {% include reference prefix=include.anchor_prefix name="setLocalParameterConditionings" %}, {% include reference prefix=include.anchor_prefix name="setLocalParameterTranslations" %}
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="toString" %}

{% include label prefix=include.anchor_prefix name="GallaghersGaussian101mePeaksFunction" %}
**GallaghersGaussian101mePeaksFunction( <small>unsigned int</small> N )** {% include continuous-only %}

- Creates an *N*-dimensional optimisation problem instance of this class.
- **Requirement:** The dimension *N* must be greater than or equal to 2.

---
{% include label prefix=include.anchor_prefix name="setParameterRotationQ" %}
**<small>void</small> .setParameterRotationQ( <small>arma::Mat&lt;double&gt;</small> Q )**

- Parameterises the rotation by \\(Q\\).
- **Requirement:** The number of rows and columns in *Q* must each match the problem dimension.
- **Requirement:** *Q* must be square, orthonormal (\\(Q^{t} = Q^{-1}\\)) and its determinant equal be to 1 or -1.

---
{% include label prefix=include.anchor_prefix name="setLocalParameterConditionings" %}
**<small>void</small> .setLocalParameterConditionings( <small>arma::Mat&lt;double&gt;</small> C )**

- Parameterises the local parameter conditionings \\(C_i\\).
- **Requirement:** The number of rows in *C* must match the problem dimension.
- **Requirement:** The number of columns in *C* must be equal to 101.

---
{% include label prefix=include.anchor_prefix name="setLocalParameterTranslations" %}
**<small>void</small> .setLocalParameterTranslations( <small>arma::Mat&lt;double&gt;</small> Y )**

- Parameterises the local parameter translations \\(Y_i\\).
- **Requirement:** The number of rows in *Y* must match the problem dimension.
- **Requirement:** The number of columns in *Y* must be equal to 101.

---
{% include label prefix=include.anchor_prefix name="toString" %}
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, e.g. *bbob_gallaghers_gaussian_101me_peaks_function*.


