**.getDistance( X )**
{% include continuous-only %}
{% include noexcept %}
<br>
**.getDistance( X, Y )**
{% include continuous-only %}

- *getDistance( X, Y )* is a metric
- *getDistance( X )* is equivalent to *getDistance( X, 0 )*, with 0 as a zero vector.

$$\sqrt{\sum_{i = 1}^{n} (x_{i} - y_{i})^{2}}$$

**.getRandomNeighbour( X, min_distance, max_distance )**
{% include continuous-only %}