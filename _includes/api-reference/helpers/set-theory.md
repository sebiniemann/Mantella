**<small>std::vector&lt;std::pair&lt;arma::Col&lt;unsigned int&gt;, arma::Col&lt;unsigned int&gt;&gt;&gt;</small> getTwoSetsPartitions( <small>unsigned int</small> N )**

- Calculates all partitions of \\(\left\\{0, 1, \ldots, N-1\right\\}\\) into exactly two sets.
- **Note:** The partitions are not guaranteed to be ordered.
- **Requirement:** The number of elements *N* must be greater than or equal to 2.

Example code, generating all two set partitions of \\(\left\\{0, 1, 2, 3, 4, 5\right\\}\\).

Create a new source file called **partitions.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/two_sets_partitions.cpp %}
{% endhighlight %}

Compilation and execution:
{% highlight bash %}
{% include {{ api_reference_folder }}/_examples/two_sets_partitions.core.sh %}
{% endhighlight %}

Output:
{% highlight text %}
{% include {{ api_reference_folder }}/_examples/two_sets_partitions.output %}
{% endhighlight %}