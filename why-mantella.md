---
layout: page
---
{% include h1 title="Why Mantella?" %}

Optimisation problems occur in many different fields and shapes, like model fitting (machine learning), path planning (space) or energy consumption (robotics). While these problems vary a lot, they all provide the practitioner with a common problem/question: Which algorithm would perform best (or at least sufficient)? 

Our aim is to provide all practitioners and fellow researchers alike with a library that helps them to make a decision, while being easy to use and providing a large range of competitive optimisation algorithms. 

{% include h1 title="Ease of use" %}

Before adopting to a new library, there is always a learning curve until one uses it productively for the fist time. To shorten this to a minimum, Mantella was designed to let you focus on your own application, while tackling even complex optimisation tasks with a few lines of code.

{% include h1 title="Efficiency" %}

The saying goes like: the best workman is just as good as his tools. To ensure that you will always have the right tool at hand, we continuously extending our library, adding competitive, state of the art optimisation algorithms and further optimising already included ones.

And in case you are missing an algorithm within Mantella, feel free to [contact us directly](https://github.com/SebastianNiemann/Mantella/issues/new), recommendations are always welcomed.

{% include h1 title="Scaling / Parallelelisation" %}

Mantella is built upon Armadillo C++, supporting 64 bit operation systems and speedups by SIMD vectorisation (e.g. SSE2, SSE3, SS4 and AVX). When compiled with MPI-support, Mantella automatically uses high performance cluster algorithms, without the need for source code changes.
