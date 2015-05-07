---
layout: api-reference

title: API Reference
subtitle: Reference to Mantella's API

table_of_contents:
---

<div class="custom-callout custom-callout-info">
#### Can this interface be used for my problem?

While we use many commen interfaces for continuous and discrete optimisation problems, some calculations will only make sense for one of them.

<dl class="dl-horizontal" markdown="0">
  <dt>{% include continuous-only %}</dt>
  <dd>These interfaces can only be used for continuous optimisation problems.</dd>
  
  <dt>{% include discrete-only %}</dt>
  <dd>These interfaces can only be used for discrete optimisation problems.</dd>
</dl>

Interfaces marked without any specifiers can be used for all problem types.
</div>

<div class="custom-callout custom-callout-info">
#### Notable interface guarantees

<dl class="dl-horizontal" markdown="0">
  <dt>{% include noexcept %}</dt>
  <dd>This interface won't throw any exception, as it accepts all possible inputs.</dd>
</dl>
</div>