Adds support for `arma::Col<T>` to be used as a key inside an `std::unordered_map` (added with C++11) with `std::unordered_map<arma::Col<T>, U, Hash<T>, IsKeyEqual<T>>`.

---
{% include label prefix=include.anchor_prefix name="IsKeyEqual" %}
**<small>std::size\_t</small> Hash.operator()( <small>arma::Col&lt;T&gt;</small> X )** {% include noexcept %}

- Generates a hash value for `arma::Col<T>` with a very small collision probability.

Example code, sampling and plotting of the collision probability.
Create a new source file called **hash.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/hash.cpp %}
{% endhighlight %}

Compile and build an executable from the source.
{% highlight bash %}
{% include {{ api_reference_folder }}/_examples/hash.core.sh %}
{% endhighlight %}

Visualisation of the sampled function using Matlab:
{% highlight matlab %}
{% include {{ api_reference_folder }}/_examples/hash.m %}
{% endhighlight %}

![Sampling of the collision probability - histogram]({{ site.baseurl }}/assets/images/{{ api_reference_folder }}/hash.png)

---
{% include label prefix=include.anchor_prefix name="IsKeyEqual" %}
**<small>bool</small> IsKeyEqual.operator()( <small>arma::Col&lt;T&gt;</small> A, <small>arma::Col&lt;T&gt;</small> B )**

- Returns true if both vectors are equal, i.e. contain the same elements, in the same order, and false otherwise.
- **Requirement:** The number of elements in *A* must be equal to the number of elements in *B*.