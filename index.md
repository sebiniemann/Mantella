---
layout: base
---
<div class="container-fluid text-center landing-page landing-page-first">
# ![Logo](/assets/images/logo.png) Mantella

<p class="lead">An efficiency-focused, modern C++ library for analysing<br> and solving optimisation problems.</p>
<p>
<a role="button" class="btn btn-primary btn-lg" href="https://github.com/SebastianNiemann/Mantella/archive/master.zip"><span class="glyphicon glyphicon-save" aria-hidden="true"></span> Download</a>
<a role="button" class="btn btn-primary btn-lg" href="/getting-started/">Get Started <span class="glyphicon glyphicon-send" aria-hidden="true"></span></a>
</p>
</div>

<div class="container-fluid text-center landing-page landing-page-second">
## Mantella is fast and scales with your (parallel) hardware<br><small>We are not in the 90's, support of SIMD vectorisation, multi-cores and HPC are a given!</small>

<p class="lead">Mantella is built upon [Armadillo C++](http://arma.sourceforge.net/), supporting 64 bit operation systems and speedups by<br> SIMD vectorisation (e.g. SSE2, SSE3, SS4 and AVX).</p>  
<p class="lead">When compiled with MPI-support, Mantella automatically uses<br> high performance cluster algorithms, no need for source code changes.</p>
</div>

<div class="container-fluid text-center landing-page landing-page-third">
## Mantella is new, but easy to use and master<br><small>Mantella helps you being productive and lets you focus on your own application.</small>

{% include example name="getting-started/hello_world.cpp" %}

Mantella also provides a wide range of competitive, state of the art optimisation algorithms<br> and can easily be extended using functional programming.
</div>

<div class="container-fluid text-center landing-page landing-page-fourth">
## Painless installation within a few minutes<br><small>Mantella is keeping a small dependency footprint, only requiring Armadillo C++ and C++11.</small>

{% highlight bash %}
git clone --depth 1 --branch master https://github.com/SebastianNiemann/Mantella.git
cd Mantella
cmake .
make -j 4
sudo make install
{% endhighlight %} 

[Install Armadillo C++](/getting-started/installation/), Install Mantella (see above), **Done!**
</div>

<div class="container-fluid text-center landing-page landing-page-fifth">
## Mantella has a permissive license<br><small>No need to be restrictive.</small>

<p class="lead">Mantella is licensed under the [MIT license](http://opensource.org/licenses/MIT).</p>
</div>

<div class="container-fluid text-center landing-page landing-page-sixth">
## So, what are you waiting for?<br><small>Download Mantella and get started.</small>
<p class="lead">
<a role="button" class="btn btn-primary btn-lg" href="https://github.com/SebastianNiemann/Mantella/archive/master.zip"><span class="glyphicon glyphicon-save" aria-hidden="true"></span> Download</a>
<strong>Mantella and<strong>
<a role="button" class="btn btn-primary btn-lg" href="/getting-started/">Get Started <span class="glyphicon glyphicon-send" aria-hidden="true"></span></a>
</p>
</div>
