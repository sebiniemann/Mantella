*Abstract base class for the black-box optimisation benchmark.*
*Extends the optimisation problem base class*

- Separable functions<br>
  {% include reference prefix=include.namespace name="Sphere function" %}, {% include reference prefix=include.namespace name="Ellipsoidal function" %}, {% include reference prefix=include.namespace name="Rastrigin function" %}, {% include reference prefix=include.namespace name="Büche-Rastrigin function" %}, {% include reference prefix=include.namespace name="Linear Slope" %}
- Functions with low or moderate conditioning<br>
  {% include reference prefix=include.namespace name="Attractive sector function" %}, {% include reference prefix=include.namespace name="Step ellipsoidal function" %}, {% include reference prefix=include.namespace name="Rosenbrock function" %}, {% include reference prefix=include.namespace name="Rosenbrock function (rotated)" %}
- Unimodal functions with high conditioning<br>
  {% include reference prefix=include.namespace name="Ellipsoidal function (rotated)" %}, {% include reference prefix=include.namespace name="Discus function" %}, {% include reference prefix=include.namespace name="Bent cigar function" %}, {% include reference prefix=include.namespace name="Sharp ridge function" %}, {% include reference prefix=include.namespace name="Different powers function" %}
- Multi-modal functions with adequate global structure<br>
  {% include reference prefix=include.namespace name="Rastrigin function (rotated)" %}, {% include reference prefix=include.namespace name="Weierstrass function" %}, {% include reference prefix=include.namespace name="Schaffers F7 function" %}, {% include reference prefix=include.namespace name="Schaffers F7 function (ill-conditioned)" %}, {% include reference prefix=include.namespace name="Composite Griewank-Rosenbrock function F8F2" %}
- Multi-modal functions with weak global structure<br>
  {% include reference prefix=include.namespace name="Schwefel function" %}, {% include reference prefix=include.namespace name="Gallagher's Gaussian 101-me peaks function" %}, {% include reference prefix=include.namespace name="Gallagher's Gaussian 21-hi peaks function" %}, {% include reference prefix=include.namespace name="Katsuura function" %}, {% include reference prefix=include.namespace name="Lunacek bi-Rastrigin function" %}