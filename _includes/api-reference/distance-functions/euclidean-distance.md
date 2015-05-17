<div class="custom-callout custom-callout-info">
#### Inheritance

Extends the `DistanceFunction<T>` base class.

Continuous (`float`, `double`, ...) as well as integral types (`unsigned int`, ...) can be used as template parameter `T` for the `EuclideanDistance<T>` class.
</div>

**Distance function:**

$$D(X, Y) := \left\Vert X - Y \right\Vert_2$$

Example code, sampling and plotting of neighbours of the point of origin, with minimal distance 1 and maximal distance 2.

Create a new source file called **euclidean_distance.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/euclidean_distance.cpp %}
{% endhighlight %}

Compilation and execution:
{% highlight bash %}
{% include {{ api_reference_folder }}/_examples/euclidean_distance.core.sh %}
{% endhighlight %}

Visualisation of the sampled function, using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/euclidean_distance.m %}
{% endhighlight %}

![Sampling of the euclidean distance - scatter plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/euclidean_distance.png)

**Constructor**<br>
  {% include reference prefix=include.anchor_prefix name="EuclideanDistance" %}

---
{% include label prefix=include.anchor_prefix name="EuclideanDistance" %}
**EuclideanDistance()** {% include noexcept %}

- Creates a default instance of this class.