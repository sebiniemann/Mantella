<div class="custom-callout custom-callout-info">
#### Inheritance

Extends the black-box optimisation benchmark base class
</div>

**Objective function:**

$$\begin{align}
F(X) &:= Z_{1} + 1000000 \sum_{i = 2}^{N} Z_{i}\\
Z &:= \left( Q \cdot T_\text{asymmetric}^{0.5} \cdot \left( Q \cdot X \right) \right)^{2}\\
F_0 &:= \sum_{i=0}^{11} 0.5^i \cos\left( \pi 3^j \right)\\
N &:= \text{The number of dimensions.}\\
Q &:= \text{Some rotation matrix.}\\
(\ldots)^p &:= \text{Element-wise power}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

<div class="custom-callout custom-callout-info">
#### Default values

The default values are set as specified by the black box optimisation benchmark.

- The parameter space translation \\(X_T\\) is randomly and uniformly chosen from \\([-4, 4]^N\\), rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to -0.00001 instead.
- \\(Q\\) is set to a randomly and uniformly chosen rotation matrix.
</div>

Example code, sampling and plotting of the bent cigar function.

Create a new source file called **bbob_bent_cigar_function.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/bbob_bent_cigar_function.cpp %}
{% endhighlight %}

Compilation and execution:
{% highlight bash %}
{% include {{ api_reference_folder }}/_examples/bbob_bent_cigar_function.core.sh %}
{% endhighlight %}

Visualisation of the sampled function, using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/bbob_bent_cigar_function.m %}
{% endhighlight %}

![Sampling of the bent cigar function - surface plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob_bent_cigar_function_surface.png)
![Sampling of the bent cigar function - contour plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob_bent_cigar_function_contour.png)

- Constructor<br>
  {% include reference prefix=include.anchor_prefix name="BentCigarFunction" %}
- Parameterisation<br>
  {% include reference prefix=include.anchor_prefix name="setParameterRotationQ" %}
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="toString" %}

---
{% include label prefix=include.anchor_prefix name="BentCigarFunction" %}
**BentCigarFunction( <small>unsigned int</small> N )** {% include continuous-only %}

- Creates an *N*-dimensional optimisation problem instance of this class.
- **Requirement:** The dimension *N* must be greater than or equal to 2.

---
{% include label prefix=include.anchor_prefix name="setParameterRotationQ" %}
**<small>void</small> .setParameterRotationQ( <small>arma::Mat&lt;T&gt;</small> Q )**

- Parameterises the rotation by \\(Q\\).
- **Requirement:** The number of rows and columns in *Q* must each match the problem dimension.
- **Requirement:** *R* must be square, orthonormal (\\(Q^{t} = Q^{-1}\\)) and its determinant equal be to 1 or -1.

---
{% include label prefix=include.anchor_prefix name="toString" %}
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, e.g. *bbob_bent_cigar_function*.