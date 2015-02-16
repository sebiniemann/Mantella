**save( A, D )**
{% include noexcept %}

{% highlight cpp %}
#include <cereal/cereal.hpp>

// ...

std::ostringstream output; {
  cereal::JSONOutputArchive archive(output);
  archive(OptimisationAlgorithm<ParameterType>::optimisationProblem_);
};
{% endhighlight %}

**load( A, D )**
{% include noexcept %}