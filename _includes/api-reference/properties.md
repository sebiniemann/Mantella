<div class="custom-callout custom-callout-info">
#### Inheritance & Templating

Abstract base class for all properties.

Continuous (`float`, `double`, ...) as well as discrete types (`unsigned int`, ...) can be used as template parameter `T` in `Properties<T>`.
</div>

Properties may be anything measurable information about an optimisation problem, e.g. its parameter to objective value correlation, Lipschitz-continuity, ... 

---
**<small>double</small> .getPlausibility()** {% include noexcept %}

- Return the plausibility of a property information to be correct.
- **Note:** The semantic meaning of this value is property specific.
- **Note:** The value is neither limited to a finite value, nor normalised between different properties.