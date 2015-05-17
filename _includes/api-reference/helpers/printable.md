<div class="custom-callout custom-callout-info">
#### Inheritance

Pure virtual base class for all named classes with a string representation.
</div>

The only usage of this class is to be inherited by, in order to guarantee the presence of a `.toString()` method.

---
**<small>std::string</small> .toString()**
{% include noexcept %}

- Returns a filesystem friendly string representation of the derived class.