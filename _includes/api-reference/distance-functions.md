<div class="custom-callout custom-callout-info">
#### Inheritance & Templating

Pure virtual base class for all distance functions.

Continuous (`float`, `double`, ...) as well as integral types (`unsigned int`, ...) can be used as template parameter `T` for the `DistanceFunction<T>` class.
</div>

---
**<small>T</small> .getLength(<small>arma::Col&lt;T&gt;</small> X)** {% include noexcept %}

---
**<small>double</small> .getDistance(<small>arma::Col&lt;T&gt;</small> X,  <small>arma::Col&lt;T&gt;</small> Y)**

---
**<small>arma::Col&lt;T&gt;</small> .getRandomNeighbour(<small>arma::Col&lt;T&gt;</small> X, <small>T</small> A, <small>T</small> B )**