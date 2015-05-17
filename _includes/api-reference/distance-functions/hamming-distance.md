<div class="custom-callout custom-callout-info">
#### Inheritance

Extends the `DistanceFunction<T>` base class.

Continuous (`float`, `double`, ...) as well as integral types (`unsigned int`, ...) can be used as template parameter `T` for the `HammingDistance<T>` class.
</div>

**Distance function:**

$$\begin{align}
D(X, Y) &:= \left\vert\left\{i \in \left\{0, 1, \ldots, N \right\}, X_i \neq Y_i \right\}\right\vert\\
N &:= \left\vert X \right\vert
\end{align}$$

Example code, sampling and plotting of neighbours of the point of origin, with minimal distance 1 and maximal distance 2.

Create a new source file called **hamming_distance.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/hamming_distance.cpp %}
{% endhighlight %}

Compilation and execution:
{% highlight bash %}
{% include {{ api_reference_folder }}/_examples/hamming_distance.core.sh %}
{% endhighlight %}

Visualisation of the sampled function, using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/hamming_distance.m %}
{% endhighlight %}

![Sampling of the hamming distance - scatter plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/hamming_distance.png)

**Constructor**<br>
  {% include reference prefix=include.anchor_prefix name="HammingDistance" %}

---
{% include label prefix=include.anchor_prefix name="HammingDistance" %}
**HammingDistance()** {% include noexcept %}

- Creates a default instance of this class.