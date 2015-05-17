<div class="custom-callout custom-callout-info">
#### Inheritance

Extends the `DistanceFunction<T>` base class.

Continuous (`float`, `double`, ...) as well as integral types (`unsigned int`, ...) can be used as template parameter `T` for the `MaximumNorm<T>` class.
</div>

**Distance function:**

$$\begin{align}
D(X, Y) &:= \max\left(\left\vert X_0 - Y_0\right\vert, \left\vert X_1 - Y_1\right\vert, \ldots, \left\vert X_N - Y_N\right\vert\right)\\
N &:= \left\vert X \right\vert
\end{align}$$
  
Example code, sampling and plotting of neighbours of the point of origin, with minimal distance 1 and maximal distance 2.

Create a new source file called **maximum_norm.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/maximum_norm.cpp %}
{% endhighlight %}

Compilation and execution:
{% highlight bash %}
{% include {{ api_reference_folder }}/_examples/maximum_norm.core.sh %}
{% endhighlight %}

Visualisation of the sampled function, using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/maximum_norm.m %}
{% endhighlight %}

![Sampling of the maximum norm - scatter plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/maximum_norm.png)

**Constructor**<br>
  {% include reference prefix=include.anchor_prefix name="MaximumNorm" %}

---
{% include label prefix=include.anchor_prefix name="MaximumNorm" %}
**MaximumNorm()** {% include noexcept %}

- Creates a default instance of this class.