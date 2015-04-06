*Extends the black-box optimisation benchmark base class*

**Objective function:**

$$\begin{align}
F(X) &:= T_\text{oscillated} \left( 10 - \max_{i = 1}^{101} \left( W_i \exp \left(-\frac{1}{2N} \left( X - Y_i \right) \cdot \left(R^T \cdot C_i \cdot R \cdot \left( X - Y_i \right) \right) \right) \right) \right)^{2}\\
W_i &:= \begin{cases}
1.1 + 8 \cdot \frac{i - 2}{99} & \text{for } i = 2, \ldots, 101 \\
10 & \text{for  } i = 1
\end{cases}\\
N &:= \text{The number of dimensions.}\\
R &:= \text{Some rotation matrix.}\\
C_i &:= \text{Some local parameter conditionings.}\\
Y_i &:= \text{Some local parameter translations}
\end{align}$$

**Soft-constraints function:**

$$C(X) := F_\text{penality}(X)$$

Example code, sampling and plotting of the Gallagher's Gaussian 101-me peaks function.
Create a new source file called **bbob2015_gallaghers_gaussian_101_me_peaks_function.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/bbob2015_gallaghers_gaussian_101_me_peaks_function.cpp %}
{% endhighlight %}

Compile and build an executable from the source.
{% highlight bash %}
c++ -std=c++11 bbob2015_gallaghers_gaussian_101_me_peaks_function.cpp -larmadillo -o bbob2015_gallaghers_gaussian_101_me_peaks_function
./bbob2015_gallaghers_gaussian_101_me_peaks_function
{% endhighlight %}

Visualisation of the sampled function using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/bbob2015_gallaghers_gaussian_101_me_peaks_function.m %}
{% endhighlight %}

![Sampling of the Gallagher's Gaussian 101-me peaks function - surface plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob2015_gallaghers_gaussian_101_me_peaks_function_surface.png)
![Sampling of the Gallagher's Gaussian 101-me peaks function - contour plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob2015_gallaghers_gaussian_101_me_peaks_function_contour.png)

- Constructor<br>
  {% include reference prefix=include.anchor_prefix name="GallaghersGaussian101mePeaksFunction" %}
- Parameterisation<br>
  {% include reference prefix=include.anchor_prefix name="setParameterRotationR" %}, {% include reference prefix=include.anchor_prefix name="setLocalParameterConditionings" %}, {% include reference prefix=include.anchor_prefix name="setLocalParameterTranslations" %}
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="toString" %}

{% include label prefix=include.anchor_prefix name="GallaghersGaussian101mePeaksFunction" %}
**GallaghersGaussian101mePeaksFunction( <small>unsigned int</small> N )** {% include continuous-only %}

- Creates an *N*-dimensional optimisation problem instance of this class.
- **Requirement:** The dimension *N* must be greater than or equal to 1.

---
{% include label prefix=include.anchor_prefix name="setParameterRotationR" %}
**<small>void</small> .setParameterRotationR( <small>arma::Mat&lt;double&gt;</small> R )**

- Parameterises the rotation by \\(R\\).
- **Requirement:** The number of rows and columns in *R* must each match the problem dimension.
- **Requirement:** *R* must be square, orthonormal (\\(R^{t} = R^{-1}\\)) and its determinant equal be to 1 or -1.

---
{% include label prefix=include.anchor_prefix name="setLocalParameterConditionings" %}
**<small>void</small> .setLocalParameterConditionings( <small>arma::Mat&lt;double&gt;</small> C )**

- Parameterises the local parameter conditionings \\(C_i\\).
- **Requirement:** The number of rows in *C* must match the problem dimension.
- **Requirement:** The number of columns in *C* must be equal to 101.

---
{% include label prefix=include.anchor_prefix name="setLocalParameterTranslation" %}
**<small>void</small> .setLocalParameterTranslation( <small>arma::Mat&lt;double&gt;</small> Y )**

- Parameterises the local parameter translations \\(Y_i\\).
- **Requirement:** The number of rows in *Y* must match the problem dimension.
- **Requirement:** The number of columns in *Y* must be equal to 101.

---
{% include label prefix=include.anchor_prefix name="toString" %}
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, e.g. *gallaghers-gaussian-101me-peaks-function*.


