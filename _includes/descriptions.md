{% case include.signature %}
{% when "configuration-iscachingsamples" %}

{% include example name="configuration/isCachingSamples.cpp" %}

**Output:**

```
Number of evaluations: 3
Number of discrete evaluations: 2
Number of evaluations: 3
Number of discrete evaluations: 3
```

{% when "configuration-isrecordingsampling" %}

{% when "configuration-isverbose" %}

{% include example name="configuration/isVerbose.cpp" %}

**Output:**

```
================================================================================
Solving optimisation problem: BBOB Sphere Function (f1)
  Number of dimensions: 2
  Lower bounds:   -5.0000  -5.0000
  Upper bounds:    5.0000   5.0000
  Acceptable objective value: -79.19
--------------------------------------------------------------------------------
  Optimisation strategy: Hooke-Jeeves algorithm
  Boundaries handling function: Map to bound
  Stagnation detection function: Always false
  Restarting function: Random

  Iteration #0 (after 20ms) : Found better solution.
    Difference to the previous best objective value: -inf
    Best objective value: -37.3782
    Best parameter:    0.7107   0.9467

  Iteration #1 (after 700ms) : Found better solution.
    Difference to the previous best objective value: -32.7573
    Best objective value: -70.1356
    Best parameter:    0.7107        0

  Iteration #3 (after 737ms) : Found better solution.
    Difference to the previous best objective value: -5.032
    Best objective value: -75.1676
    Best parameter:    0.7107   0.5000

[ ... ]
```

{% when "optimisation-algorithms-optimisationalgorithm" %}

{% when "optimisation-algorithms-optimise-optimisationproblem-arma-mat-lt-double-gt" %}

{% when "optimisation-algorithms-setnextparametersfunction-std-function-std-string" %}

{% when "optimisation-algorithms-setnextparametersfunction-std-function" %}

{% when "optimisation-algorithms-getnextparametersfunctionname" %}

{% when "optimisation-algorithms-setboundarieshandlingfunction-std-function-std-string" %}

{% when "optimisation-algorithms-setboundarieshandlingfunction-std-function" %}

{% when "optimisation-algorithms-getboundarieshandlingfunctionname" %}

{% when "optimisation-algorithms-setisstagnatingfunction-std-function-std-string" %}

{% when "optimisation-algorithms-setisstagnatingfunction-std-function" %}

{% when "optimisation-algorithms-getisstagnatingfunctionname" %}

{% when "optimisation-algorithms-setrestartingfunction-std-function-std-string" %}

{% when "optimisation-algorithms-setrestartingfunction-std-function" %}

{% when "optimisation-algorithms-getrestartingfunctionname" %}

{% when "optimisation-algorithms-setacceptableobjectivevalue-double" %}

{% when "optimisation-algorithms-getacceptableobjectivevalue" %}

{% when "optimisation-algorithms-setmaximalnumberofiterations-arma-uword" %}

{% when "optimisation-algorithms-getmaximalnumberofiterations" %}

{% when "optimisation-algorithms-setmaximalduration-std-chrono-microseconds" %}

{% when "optimisation-algorithms-getmaximalduration" %}

{% when "optimisation-algorithms-isfinished" %}

{% when "optimisation-algorithms-isterminated" %}

{% when "optimisation-algorithms-getnumberofiterations" %}

{% when "optimisation-algorithms-getduration" %}

{% when "optimisation-algorithms-getbestobjectivevalue" %}

{% when "optimisation-algorithms-getbestparameter" %}

{% when "optimisation-algorithms-getrecordedsampling" %}

{% when "optimisation-algorithms-reset" %}

{% when "random-search-randomsearch" %}

{% when "random-search-optimise-optimisationproblem" %}
{% when "    " %}
{% when "hill-climbing-hillclimbing" %}

{% when "hill-climbing-optimise-optimisationproblem" %}

{% when "hill-climbing-setminimalstepsize-double" %}

{% when "hill-climbing-getminimalstepsize" %}

{% when "hill-climbing-setmaximalstepsize-double" %}

{% when "hill-climbing-getmaximalstepsize" %}
{% when "    " %}
{% when "simulated-annealing-simulatedannealing" %}

{% when "simulated-annealing-optimise-optimisationproblem" %}

{% when "simulated-annealing-setisacceptablestatefunction-std-function-std-string" %}

{% when "simulated-annealing-setisacceptablestatefunction-std-function" %}

{% when "simulated-annealing-getisacceptablestatefunctionname" %}

{% when "simulated-annealing-setminimalstepsize-double" %}

{% when "simulated-annealing-getminimalstepsize" %}

{% when "simulated-annealing-setmaximalstepsize-double" %}

{% when "simulated-annealing-getmaximalstepsize" %}

{% when "hooke-jeeves-algorithm-hookejeevesalgorithm" %}

{% when "hooke-jeeves-algorithm-optimise-optimisationproblem" %}

{% when "hooke-jeeves-algorithm-setinitialstepsize-double" %}

{% when "hooke-jeeves-algorithm-getinitialstepsize" %}

{% when "hooke-jeeves-algorithm-setstepsizedecrease-double" %}

{% when "hooke-jeeves-algorithm-getstepsizedecrease" %}

{% when "particle-swarm-optimisation-particleswarmoptimisation" %}

{% when "particle-swarm-optimisation-optimise-optimisationproblem-arma-uword" %}

{% when "particle-swarm-optimisation-optimise-optimisationproblem" %}

{% when "particle-swarm-optimisation-setneighbourhoodtopologyfunction-std-function-std-string" %}

{% when "particle-swarm-optimisation-setneighbourhoodtopologyfunction-std-function" %}

{% when "particle-swarm-optimisation-getneighbourhoodtopologyfunctionname" %}

{% when "particle-swarm-optimisation-setmaximalacceleration-double" %}

{% when "particle-swarm-optimisation-getmaximalacceleration" %}

{% when "particle-swarm-optimisation-setmaximallocalattraction-double" %}

{% when "particle-swarm-optimisation-getmaximallocalattraction" %}

{% when "particle-swarm-optimisation-setmaximalglobalattraction-double" %}

{% when "particle-swarm-optimisation-getmaximalglobalattraction" %}

{% when "optimisation-problems-optimisationproblem-arma-uword" %}

{% when "optimisation-problems-numberofdimensions" %}

{% when "optimisation-problems-setobjectivefunction-std-function-std-string" %}

{% when "optimisation-problems-setobjectivefunction-std-function" %}

{% when "optimisation-problems-getobjectivefunctionname" %}

{% when "optimisation-problems-getobjectivevalue-arma-col-lt-double-gt" %}

{% when "optimisation-problems-getnormalisedobjectivevalue-arma-col-lt-double-gt" %}

{% when "optimisation-problems-setlowerbounds-arma-col-lt-double-gt" %}

{% when "optimisation-problems-getlowerbounds" %}

{% when "optimisation-problems-setupperbounds-arma-col-lt-double-gt" %}

{% when "optimisation-problems-getupperbounds" %}

{% when "optimisation-problems-setparameterpermutation-arma-col-lt-arma-uword-gt" %}

{% when "optimisation-problems-getparameterpermutation" %}

{% when "optimisation-problems-setparameterscaling-arma-col-lt-double-gt" %}

{% when "optimisation-problems-getparameterscaling" %}

{% when "optimisation-problems-setparametertranslation-arma-col-lt-double-gt" %}

{% when "optimisation-problems-getparametertranslation" %}

{% when "optimisation-problems-setparameterrotation-arma-mat-lt-double-gt" %}

{% when "optimisation-problems-getparameterrotation" %}

{% when "optimisation-problems-setminimalparameterdistance-arma-col-lt-double-gt" %}

{% when "optimisation-problems-getminimalparameterdistance" %}

{% when "optimisation-problems-setobjectivevaluescaling-double" %}

{% when "optimisation-problems-getobjectivevaluescaling" %}

{% when "optimisation-problems-setobjectivevaluetranslation-double" %}

{% when "optimisation-problems-getobjectivevaluetranslation" %}

{% when "optimisation-problems-getcachedsamples" %}

{% when "optimisation-problems-getnumberofevaluations" %}

{% when "optimisation-problems-getnumberofdistinctevaluations" %}

{% when "optimisation-problems-reset" %}

{% when "black-box-optimisation-benchmark-bbob-attractivesectorfunction-arma-uword" %}

{% when "black-box-optimisation-benchmark-bbob-bentcigarfunction-arma-uword" %}

{% when "black-box-optimisation-benchmark-bbob-buecherastriginfunction-arma-uword" %}

{% when "black-box-optimisation-benchmark-bbob-compositegriewankrosenbrockfunctionf8f2-arma-uword" %}

{% when "black-box-optimisation-benchmark-bbob-differentpowersfunction-arma-uword" %}

{% when "black-box-optimisation-benchmark-bbob-discusfunction-arma-uword" %}

{% when "black-box-optimisation-benchmark-bbob-ellipsoidalfunction-arma-uword" %}

{% when "black-box-optimisation-benchmark-bbob-ellipsoidalfunctionrotated-arma-uword" %}

{% when "black-box-optimisation-benchmark-bbob-gallaghersgaussian101mepeaksfunction-arma-uword" %}

{% when "black-box-optimisation-benchmark-bbob-gallaghersgaussian21hipeaksfunction-arma-uword" %}

{% when "black-box-optimisation-benchmark-bbob-katsuurafunction-arma-uword" %}

{% when "black-box-optimisation-benchmark-bbob-linearslope-arma-uword" %}

{% when "black-box-optimisation-benchmark-bbob-lunacekbirastriginfunction-arma-uword" %}

{% when "black-box-optimisation-benchmark-bbob-rastriginfunction-arma-uword" %}

{% when "black-box-optimisation-benchmark-bbob-rastriginfunctionrotated-arma-uword" %}

{% when "black-box-optimisation-benchmark-bbob-rosenbrockfunction-arma-uword" %}

{% when "black-box-optimisation-benchmark-bbob-rosenbrockfunctionrotated-arma-uword" %}

{% when "black-box-optimisation-benchmark-bbob-schaffersf7function-arma-uword" %}

{% when "black-box-optimisation-benchmark-bbob-schaffersf7functionillconditioned-arma-uword" %}

{% when "black-box-optimisation-benchmark-bbob-schwefelfunction-arma-uword" %}

{% when "black-box-optimisation-benchmark-bbob-sharpridgefunction-arma-uword" %}

{% when "black-box-optimisation-benchmark-bbob-spherefunction-arma-uword" %}

{% when "black-box-optimisation-benchmark-bbob-stepellipsoidalfunction-arma-uword" %}

{% when "black-box-optimisation-benchmark-bbob-weierstrassfunction-arma-uword" %}

{% when "black-box-optimisation-benchmark-getoptimalobjectivevalue" %}

{% when "kinematically-redundant-machines-krm-parallelkinematicmachine3prpr" %}

{% when "kinematically-redundant-machines-krm-parallelkinematicmachine3prrr" %}

{% when "kinematically-redundant-machines-krm-parallelkinematicmachine6prus" %}

{% when "kinematically-redundant-machines-krm-parallelkinematicmachine6pups" %}

{% when "kinematically-redundant-machines-numberofworkspacedimensions" %}

{% when "kinematically-redundant-machines-setendeffectortrajectory-arma-mat-lt-double-gt" %}

{% when "kinematically-redundant-machines-getendeffectortrajectory" %}

{% when "samples-selection-elitists-std-unordered-map-arma-uword" %}

{% when "samples-analysis-fitnessdistancecorrelation-std-unordered-map" %}

{% when "samples-analysis-lipschitzcontinuity-std-unordered-map" %}

{% when "samples-analysis-fitnessdistancecorrelation-std-unordered-map" %}

{% when "samples-analysis-additiveseparability-optimisationproblem-arma-uword-double-double" %}

{% when "random-number-generator-rng-getgenerator" %}

{% when "random-number-generator-rng-setseed-arma-arma-rng-seed-type" %}

{% when "random-number-generator-rng-setrandomseed" %}

{% when "random-number-generator-rng-getseed" %}

{% when "mathematical-functions-polynomialsize-arma-uword-arma-uword" %}

{% when "mathematical-functions-polynomial-arma-col-lt-double-gt-arma-uword" %}

{% when "mathematical-functions-factorial-arma-uword" %}

{% when "mathematical-functions-nchoosek-arma-uword-arma-uword" %}

{% when "mathematical-functions-combinations-arma-uword-arma-uword" %}

{% when "mathematical-functions-multicombinations-arma-uword-arma-uword" %}

{% when "mathematical-functions-rotationmatrix2d-double" %}

{% when "mathematical-functions-rotationmatrix3d-double-double-double" %}

{% when "mathematical-functions-circlecircleintersections-arma-col-lt-double-gt-fixed-lt-2-gt-double-arma-col-lt-double-gt-fixed-lt-2-gt-double" %}

{% when "mathematical-functions-circlesphereintersections-arma-col-lt-double-gt-fixed-lt-3-gt-double-arma-col-lt-double-gt-fixed-lt-3-gt-arma-col-lt-double-gt-fixed-lt-3-gt-double" %}

{% when "mathematical-functions-hammersleyset-arma-col-lt-arma-uword-gt-arma-col-lt-arma-uword-gt-arma-uword" %}

{% when "mathematical-functions-haltonsequence-arma-col-lt-arma-uword-gt-arma-col-lt-arma-uword-gt-arma-uword" %}

{% when "mathematical-functions-vandercorputsequence-arma-uword-arma-uword-arma-uword" %}

{% when "mathematical-functions-twosetspartitions-arma-uword" %}

{% when "mathematical-functions-randomrotationmatrix-arma-uword" %}

{% when "mathematical-functions-randompermutationvector-arma-uword-arma-uword" %}

{% when "mathematical-functions-randompermutationvector-arma-uword" %}

{% when "mathematical-functions-randomneighbour-arma-col-lt-double-gt-double-double" %}

{% when "armadillo-and-c-stl-range-arma-uword-arma-uword-arma-uword" %}

{% when "armadillo-and-c-stl-range-arma-uword-arma-uword" %}

{% when "armadillo-and-c-stl-hash-operator-arma-col-lt-double-gt" %}

{% when "armadillo-and-c-stl-isequal-operator-arma-col-lt-double-gt-arma-col-lt-double-gt" %}

{% when "message-passing-interface-mpibestsample-void-void-int-mpi-datatype" %}

{% else %}
{% endcase %}