<div class="custom-callout custom-callout-info">
#### Inheritance

Extends the black-box optimisation benchmark base class
</div>

**Objective function:**

$$\begin{align}
F(X) &:= \left( \frac{\sum_{i=1}^{N-1} S_i \cdot \left(1 + \sin\left(50 S_i^{0.4}\right)^2\right)}{N - 1} \right)^2\\

Z &:= T_\text{conditioning}^\sqrt{10} \cdot Q \cdot T_\text{asymmetric}^{0.5}\left( X \right)\\
S_i &:= \left(Z_i^2 + Z_{i+1}^2 \right)^{0.25}\\
N &:= \text{The number of dimensions.}\\
Q &:= \text{Some rotation matrix.}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 10 F_\text{penality}(X)$$

Example code, sampling and plotting of the Schaffers f7 function.
Create a new source file called **bbob_schaffers_f7_function.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/bbob_schaffers_f7_function.cpp %}
{% endhighlight %}

Compile and build an executable from the source.
{% highlight bash %}
c++ -std=c++11 bbob_schaffers_f7_function.cpp -larmadillo -o bbob_schaffers_f7_function
./bbob_schaffers_f7_function
{% endhighlight %}

Visualisation of the sampled function using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/bbob_schaffers_f7_function.m %}
{% endhighlight %}

![Sampling of the Schaffers f7 function - surface plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob_schaffers_f7_function_surface.png)
![Sampling of the Schaffers f7 function - contour plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob_schaffers_f7_function_contour.png)

- Constructor<br>
  {% include reference prefix=include.anchor_prefix name="SchaffersF7Function" %}
- Parameterisation<br>
  {% include reference prefix=include.anchor_prefix name="setParameterRotationQ" %}
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="toString" %}

{% include label prefix=include.anchor_prefix name="SchaffersF7Function" %}
**SchaffersF7Function( <small>unsigned int</small> N )** {% include continuous-only %}

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

- Returns a filesystem friendly name of the problem, e.g. *bbob2015_schaffers_f7_function*.