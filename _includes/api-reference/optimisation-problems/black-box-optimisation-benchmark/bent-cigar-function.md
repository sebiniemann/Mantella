*Extends the black-box optimisation benchmark base class*

**Objective function:**

$$\begin{align}
F(X) &:= Z_{1} + 1000000 \sum_{i = 2}^{N} Z_{i}\\
Z &:= \left( R \cdot T_\text{asymmetric}^{0.5} \cdot \left( R \cdot \left( X - X^\text{opt} \right) \right) \right)^{2}\\
F_0 &:= \sum_{i=0}^{11} 0.5^i \cos\left( \pi 3^j \right)\\
N &:= \text{The number of dimensions.}\\
X_T &:= \text{The translation of the parameter space.}\\
R &:= \text{Some rotation matrix.}\\
(\ldots)^p &:= \text{Element-wise power}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

**Minimal parameter and objective function value:**

$$\begin{align}
X_\text{minimal} &= X_T\\
F(X_\text{minimal}) &= 0
\end{align}$$

Example code, sampling and plotting of the bent cigar function.
Create a new source file called **bbob2015_bent_cigar_function.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/bbob2015_bent_cigar_function.cpp %}
{% endhighlight %}

Compile and build an executable from the source.
{% highlight bash %}
c++ -std=c++11 bbob2015_bent_cigar_function.cpp -larmadillo -o bbob2015_bent_cigar_function
./bbob2015_bent_cigar_function
{% endhighlight %}

Visualisation of the sampled function using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/bbob2015_bent_cigar_function.m %}
{% endhighlight %}

![Sampling of the bent cigar function - surface plot]({{ site.url }}/assets/images/{{ api_reference_folder }}/bbob2015_bent_cigar_function_surface.png)
![Sampling of the bent cigar function - contour plot]({{ site.url }}/assets/images/{{ api_reference_folder }}/bbob2015_bent_cigar_function_contour.png)

- Constructor<br>
  {% include reference prefix=include.anchor_prefix name="BentCigarFunction" %}
- Parameterisation<br>
  {% include reference prefix="optimisation-problems-" name="setParameterTranslation" %} (inherited), {% include reference prefix=include.anchor_prefix name="setParameterRotationR" %}
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="toString" %}

{% include label prefix=include.anchor_prefix name="BentCigarFunction" %}
**BentCigarFunction( <small>unsigned int</small> N )** {% include continuous-only %}

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

- Returns a filesystem friendly name of the problem, e.g. *bbob2015_bent_cigar_function*.