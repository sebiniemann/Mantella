---
layout: page
---
{% include h1 title="Hello World" %}

{% include h2 title="Writing our first Mantella-based application" %}

Create a new source file called **hello_world.cpp** on your computer.

<ul class="nav nav-tabs" role="tablist" id="hello-world">
<li role="presentation" class="active"><a href="#hello-world-cpp" aria-controls="hello-world-cpp" role="tab" data-toggle="tab">C++</a></li>
<li role="presentation"><a href="#hello-world-compile" aria-controls="hello-world-compile" role="tab" data-toggle="tab">Compile</a></li>
<li role="presentation"><a href="#hello-world-run" aria-controls="hello-world-run" role="tab" data-toggle="tab">Run</a></li>
</ul>
<div class="tab-content">
<div role="tabpanel" class="tab-pane active" id="hello-world-cpp">
For our first Mantella-based application, we want to initialise a pre-defined optimisation problem from the black-box optimisation benchmark, optimise it with a decent optimisation algorithm and print the best found parameter to the console.

{% include source name="getting-started/hello_world.cpp" language="cpp" %}

</div>
<div role="tabpanel" class="tab-pane" id="hello-world-compile">

``` bash
c++ -std=c++11 -march=native -O3 hello_world.cpp -larmadillo -lmantella -o hello_world

# In case Armadillo and Mantella weren't installed/placed into standard included/searched system directories, use the following:
# c++ -std=c++11 -march=native -O3 hello_world.cpp -L/path/to/armaillo/lib -L/path/to/mantella/lib -larmadillo -lmantella -I/path/to/armadillo/include -I/path/to/mantella/include -o hello_world
```

</div>
<div role="tabpanel" class="tab-pane" id="hello-world-run">

``` bash
./hello_world
```

</div>
</div>

**As we didn't yet specify an termination criteria**, the optimisation process will take 1 second per default. The actual output might may vary due to randomly initialised problems.

**Possible output:**

{% include source name="getting-started/hello_world.cpp.output" language="text" %}

{% include h2 title="Understanding what we just did" %}

Our first line was to include the central Mantella include file `<mantella>`, which adds all internal Mantella headers, providing all of Mantella's features.

{% include source name="getting-started/hello_world.cpp" language="cpp" offset="0" limit="1" %}

Then we created a 2-dimensional instance of the [sphere function](/optimisation-problems/black-box-optimisation-benchmark/) from the black-box optimisation benchmark ...

{% include source name="getting-started/hello_world.cpp" language="cpp" offset="3" limit="3" %}

... and used the [Hooke-Jeeves algorithm](/optimisation-algorithms/hooke-jeeves-algorithm/) to optimise the given problem.
{% include source name="getting-started/hello_world.cpp" language="cpp" offset="7" limit="3" %}

After the optimisation process finished, we printed out the best found parameter by using the `.getBestParameter()` Getter. The`.optimise(...)` method itself does not return any value, but fills various Getter defined in the [optimisation algorithms base class](/optimisation-algorithms/).

{% include source name="getting-started/hello_world.cpp" language="cpp" offset="11" limit="2" %}

Regarding the compiler options, we added `-std=c++11` to ensure C++11 is supported (if not set per default by your compiler) and `-march=native -O3` to make use of SIMD instructions (SSE2, SSE3, AVX, ...) and compiler optimisation.

It is also crucial to add `-larmadillo` before `-lmantella`, as Mantella depends on Armadillo C++.


