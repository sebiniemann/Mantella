<div class="custom-callout custom-callout-info">
#### Inheritance

Extends the black-box optimisation benchmark base class
</div>

**Objective function:**

$$\begin{align}
F(X) &:= 10 N - \sum_{i = 1}^{N} \left( \cos{2 \pi S_{i}} \right) + \left\Vert S \right\Vert_{2}^{2}\\
Z &:= T_\text{conditioning}^\sqrt{10} \cdot T_\text{oscillated} \left( X \right)\\
S_{i} &:= \begin{cases}
10 Z_{i} & \text{if } i \text{ is odd and } Z_{i} > 0\\
Z_{i} & \text{otherwise}
\end{cases}\\
N &:= \text{The number of dimensions.}
\end{align}$$

**Soft-constraints function:**

$$C(X) := 100 \cdot F_\text{penality}(X)$$

<div class="custom-callout custom-callout-info">
#### Default values

The default values are set as specified by the black box optimisation benchmark.

- The parameter space translation \\(X_T\\) is randomly and uniformly chosen from \\([-4, 4]\\) for odd and from \\([0, 4]\\) for even dimensions, rounded up to 4 decimal places. If the translation of a dimension would be zero, it is set to -0.00001 for odd and to 0.00001 for even dimensions instead.
</div>

Example code, sampling and plotting of the Büche-Rastrigin function.
Create a new source file called **bbob2015_bueche_rastrigin_function.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/bbob2015_bueche_rastrigin_function.cpp %}
{% endhighlight %}

Compile and build an executable from the source.
{% highlight bash %}
c++ -std=c++11 bbob2015_bueche_rastrigin_function.cpp -larmadillo -o bbob2015_bueche_rastrigin_function
./bbob2015_bueche_rastrigin_function
{% endhighlight %}


Visualisation of the sampled function using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/bbob2015_bueche_rastrigin_function.m %}
{% endhighlight %}

![Sampling of the Büche-Rastrigin function - surface plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob2015_bueche_rastrigin_function_surface.png)
![Sampling of the Büche-Rastrigin function - contour plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/bbob2015_bueche_rastrigin_function_contour.png)

- Constructor<br>
  {% include reference prefix=include.anchor_prefix name="BuecheRastriginFunction" %}
- Miscellaneous<br>
  {% include reference prefix=include.anchor_prefix name="toString" %}

{% include label prefix=include.anchor_prefix name="BuecheRastriginFunction" %}
**BuecheRastriginFunction( <small>unsigned int</small> N )** {% include continuous-only %}

- Creates an *N*-dimensional optimisation problem instance of this class.
- **Requirement:** The dimension *N* must be greater than or equal to 2.

---
{% include label prefix=include.anchor_prefix name="toString" %}
**<small>std::string</small> .toString()** {% include noexcept %}

- Returns a filesystem friendly name of the problem, e.g. *bbob_bueche_rastrigin_function*.