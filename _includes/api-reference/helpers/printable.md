<div class="custom-callout custom-callout-info">
#### Inheritance

Abstract base class for all printable objects, i.e. classes with a filesystem friendly string representation.
</div>

The only intended usage of this class is to be inherited by `... extends Printable, ...`, in order to guarantee the presence of `.toString()`.

---
**<small>std::string</small> .toString()**
{% include noexcept %}

- Returns a filesystem friendly string representation of the derived class.