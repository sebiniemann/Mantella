

{% highlight cpp %}
std::shared_ptr<mant::PropertyAnalysis<double>> continousPropertyAnalysis;
std::shared_ptr<mant::PropertyAnalysis<unsigned int>> discretePropertiesAnalysis;
{% endhighlight %}


{% highlight cpp %}
#include <memory>

std::shared_ptr<mant::PropertyAnalysis<double>> propertyAnalysis;
propertyAnalysis.setDistanceFunction(mant::EuclideanDistance);
{% endhighlight %}

**.analyse( F )**

{% highlight cpp %}
#include <memory>

std::shared_ptr<mant::OptimisationProblem> optimisationProblem;

mant::PropertyAnalysis propertyAnalysis;
propertyAnalysis.analyse(optimisationProblem);
{% endhighlight %}

**.getPlausibility()**

{% highlight cpp %}
#include <memory>

std::shared_ptr<mant::OptimisationProblem> optimisationProblem;

mant::PropertyAnalysis propertyAnalysis;
propertyAnalysis.analyse(optimisationProblem);
propertyAnalysis.getPlausibility();
{% endhighlight %}