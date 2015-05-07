
**<small>std::string</small> to_string( <small>mant::Printable</small> P )**
{% include noexcept %}

- Returns a filesystem friendly string representation of *P*, as specified by `P.toString()`.
- This mimics the C++ standard library function std::to_string(...).

---
**<small>std::string</small> to_string( <small>std::shared\_ptr&lt;mant::Printable&gt;</small> P )**
{% include noexcept %}

- Returns a filesystem friendly string representation of the implementation of *P*, as specified by `P->toString()`.
- This mimics the C++ standard library function std::to_string(...).

---
**<small>bool</small> endsWith( <small>std::string</small> S, <small>std::string</small> E )**
{% include noexcept %}

- Returns true if *E* is a suffix of *S*, and false otherwise.