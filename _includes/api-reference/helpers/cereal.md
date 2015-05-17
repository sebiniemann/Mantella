Add support to convert a 

Example code, **saving** an `arma::Mat` instance.

Create a new source file called **cereal\_save.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/cereal_save.cpp %}
{% endhighlight %}

Compilation and execution:
{% highlight bash %}
{% include {{ api_reference_folder }}/_examples/cereal_save.core.sh %}
{% endhighlight %}

Output:
{% highlight text %}
{% include {{ api_reference_folder }}/_examples/cereal_save.serialised %}
{% endhighlight %}

Example code, **loading** an `arma::Mat` instance.

Create a new source file called **cereal\_load.cpp**:
{% highlight cpp %}
{% include {{ api_reference_folder }}/_examples/cereal_load.cpp %}
{% endhighlight %}

Compilation and execution:
{% highlight bash %}
{% include {{ api_reference_folder }}/_examples/cereal_load.core.sh %}
{% endhighlight %}

Output:
{% highlight text %}
{% include {{ api_reference_folder }}/_examples/cereal_load.output %}
{% endhighlight %}

---
**<small>void</small> save( <small>cereal::JSONOutputArchive</small> A, <small>arma::Col&lt;T&gt;</small> D )** {% include noexcept %}

- Serialises a column vector *D* into an archive *A*.

---
**<small>void</small> load( <small>cereal::JSONInputArchive</small> A, <small>arma::Col&lt;T&gt;</small> D )** {% include noexcept %}

- Deserialises an archive *A* into a column vector *D*.
- **Requirement:** *D* must be empty.
- **Requirement:** The archive must contain a valid JSON-based serialisation.

---
**<small>void</small> save( <small>cereal::JSONOutputArchive</small> A, <small>arma::Row&lt;T&gt;</small> D )** {% include noexcept %}

- Serialises a row vector *D* into an archive *A*.

---
**<small>void</small> load( <small>cereal::JSONInputArchive</small> A, <small>arma::Row&lt;T&gt;</small> D )**

- Deserialises an archive *A* into a row vector *D*.
- **Requirement:** *D* must be empty.
- **Requirement:** The archive must contain a valid JSON-based serialisation.

---
**<small>void</small> save( <small>cereal::JSONOutputArchive</small> A, <small>arma::Mat&lt;T&gt;</small> D )** {% include noexcept %}

- Serialises a matrix *D* into an archive *A*.

---
**<small>void</small> load( <small>cereal::JSONInputArchive</small> A, <small>arma::Mat&lt;T&gt;</small> D )**

- Deserialises an archive *A* into a matrix *D*.
- **Requirement:** *D* must be empty.
- **Requirement:** The archive must contain a valid JSON-based serialisation.