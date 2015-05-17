<div class="custom-callout custom-callout-info">
#### Inheritance

Extends the black-box optimisation benchmark base class
</div>

**Objective function:**

$$\begin{align}
F(X) &:= Z_1^2 + 100 \left\Vert \left(Z_2, \ldots, Z_N \right) \right\Vert_2\\
Z &:= Q \cdot T_\text{conditioning}^\sqrt{10} \cdot X\\
N &:= \text{The number of dimensions.}\\
Q &:= \text{Some rotation matrix.}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 0, \ \forall X$$

<div class="custom-callout custom-callout-info">
#### Default values

The default values are set as specified by the black box optimisation benchmark.

- The parameter space translation \\(X_T\\) is randomly and uniformly chosen from \\([-4, 4]^N\\), rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to -0.00001 instead.
- The parameter space rotation \\(X_R\\) is set to a randomly and uniformly chosen rotation matrix.
- \\(Q\\) is set to a randomly and uniformly chosen rotation matrix.
</div>

Example code, sampling and plotting of the sharpe ridge function.

Create a new source file called **bbob_sharp_ridge_function.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/bbob_sharp_ridge_function.cpp %}
{% endhighlight %}

Compilation and execution:
{% highlight bash %}
{% include {{ api_reference_folder }}/_examples/bbob_sharp_ridge_function.core.sh %}
{% endhighlight %}

Visualisation of the sampled function, using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/bbob_sharp_ridge_function.m %}
{% endhighlight %}

![Sampling of the sharp ridge function - surface plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob_sharp_ridge_function_surface.png)
![Sampling of the sharp ridge function - contour plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob_sharp_ridge_function_contour.png)

- Constructor<br>
  {% include reference prefix=include.anchor_prefix name="SharpRidgeFunction" %}
- Parameterisation<br>
  {% include reference prefix=include.anchor_prefix name="setParameterRotationQ" %}
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="toString" %}

---
{% include label prefix=include.anchor_prefix name="SharpRidgeFunction" %}
**SharpRidgeFunction( <small>unsigned int</small> N )** {% include continuous-only %}

- Creates an *N*-dimensional optimisation problem instance of this class.
- **Requirement:** The dimension *N* must be greater than or equal to 2.

---
{% include label prefix=include.anchor_prefix name="setParameterRotationQ" %}
**<small>void</small> .setParameterRotationQ( <small>arma::Mat&lt;double&gt;</small> Q )**

- Parameterises the rotation by \\(Q\\).
- **Requirement:** The number of rows and columns in *Q* must each match the problem dimension.
- **Requirement:** *Q* must be square, orthonormal (\\(Q^{t} = Q^{-1}\\)) and its determinant equal be to 1 or -1.

---
{% include label prefix=include.anchor_prefix name="toString" %}
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, e.g. *bbob_sharp_ridge_function*.
