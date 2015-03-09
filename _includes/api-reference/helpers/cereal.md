<small>void</small> **save( <small>T1</small> A, <small>arma::Col&lt;T2&gt;</small> D )** {% include noexcept %}

- Serialises a column vector `D` into an archive `A`.
- The serialisation (JSON, binary, ...) is defined by `A`.

{% highlight cpp %}
#include <mantella>
#include <sstream>
#include <iostream>

int main() {
  arma::Col<double> someVector(10, arma::fill::randu);
  
  
  
  
}
{% endhighlight %}

Output:
{% highlight text %}
numberOfEvaluations: 5
numberOfDistinctEvaluations: 3
{% endhighlight %}

<small>void</small> **load( <small>T1</small> A, <small>arma::Col&lt;T2&gt;</small> D )** {% include noexcept %}

- Deserialises an archive `A` into a colum vector `D`.
- Any previous data in `D` will be overwritten and its shape set to match `A`.

<small>void</small> **save( <small>T1</small> A, <small>arma::Row&lt;T2&gt;</small> D )** {% include noexcept %}

- Serialises a row vector `D` into an archive `A`.
- The serialisation (JSON, binary, ...) is defined by `A`.

<small>void</small> **load( <small>T1</small> A, <small>arma::Row&lt;T2&gt;</small> D )** {% include noexcept %}

- Deserialises an archive `A` into a row vector `D`.
- Any previous data in `D` will be overwritten and its shape set to match `A`.

<small>void</small> **save( <small>T1</small> A, <small>arma::Mat&lt;T2&gt;</small> D )** {% include noexcept %}

- Serialises a matrix `D` into an archive `A`.
- The serialisation (JSON, binary, ...) is defined by `A`.

<small>void</small> **load( <small>T1</small> A, <small>arma::Mat&lt;T2&gt;</small> D )** {% include noexcept %}

- Deserialises an archive `A` into a matrix `D`.
- Any previous data in `D` will be overwritten and its shape set to match `A`.