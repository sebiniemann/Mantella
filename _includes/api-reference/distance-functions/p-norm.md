<div class="custom-callout custom-callout-info">
#### Inheritance

Extends the `DistanceFunction<T>` base class.

Continuous (`float`, `double`, ...) as well as integral types (`unsigned int`, ...) can be used as template parameter `T` for the `PNorm<T>` class.
</div>

**Distance function:**

$$D(X, Y) := \left\Vert X - Y \right\Vert_P$$

Example code, sampling and plotting of neighbours of the point of origin, with minimal distance 1 and maximal distance 2.

Create a new source file called **p_norm.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/p_norm.cpp %}
{% endhighlight %}

Compilation and execution:
{% highlight bash %}
{% include {{ api_reference_folder }}/_examples/p_norm.core.sh %}
{% endhighlight %}

Visualisation of the sampled function, using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/p_norm.m %}
{% endhighlight %}

![Sampling of the p-norm - scatter plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/p_norm.png)

**Constructor**<br>
  {% include reference prefix=include.anchor_prefix name="PNorm" %}

---
{% include label prefix=include.anchor_prefix name="PNorm" %}
**PNorm(<small>double</small> P)**

- Creates a default instance of this class.
- **Requirement:** *P* must be greater than or equal to 1.