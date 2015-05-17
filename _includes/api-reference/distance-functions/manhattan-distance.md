<div class="custom-callout custom-callout-info">
#### Inheritance

Extends the `DistanceFunction<T>` base class.

Continuous (`float`, `double`, ...) as well as integral types (`unsigned int`, ...) can be used as template parameter `T` for the `ManhattanDistance<T>` class.
</div>

**Distance function:**

$$D(X, Y) := \left\vert X - Y \right\vert$$

Example code, sampling and plotting of neighbours of the point of origin, with minimal distance 1 and maximal distance 2.

Create a new source file called **manhattan_distance.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/manhattan_distance.cpp %}
{% endhighlight %}

Compilation and execution:
{% highlight bash %}
{% include {{ api_reference_folder }}/_examples/manhattan_distance.core.sh %}
{% endhighlight %}

Visualisation of the sampled function, using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/manhattan_distance.m %}
{% endhighlight %}

![Sampling of the manhattan distance - scatter plot]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/manhattan_distance.png)

**Constructor**<br>
  {% include reference prefix=include.anchor_prefix name="ManhattanDistance" %}

---
{% include label prefix=include.anchor_prefix name="ManhattanDistance" %}
**ManhattanDistance()** {% include noexcept %}

- Creates a default instance of this class.