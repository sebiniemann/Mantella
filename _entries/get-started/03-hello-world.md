---
title: Hello World
category: get-started
---

Create a new source file called **hello.cpp** on your computer.
{% highlight cpp %}
#include <iostream>
#include <mantella>

int main() {
  // 1. Setup the optimisation problem.
  unsigned int numberOfDimensions = 4;
  std::shared_ptr<mant::OptimisationProblem<double>> optimisationProblem(new mant::bbob2015::SphereFunction(numberOfDimensions));

  // 2. Run the solver.
  mant::HookeJeevesAlgorithm optimisationAlgorithm(optimisationProblem);
  optimisationAlgorithm.optimise();

  // 3. Get your results!
  std::cout << "isFinished: " << optimisationAlgorithm.isFinished() << std::endl;
  std::cout << "isTerminated: " << optimisationAlgorithm.isTerminated() << std::endl;
  std::cout << "numberOfIterations: " << optimisationAlgorithm.getNumberOfIterations() << std::endl;
  std::cout << "numberOfEvaluations: " << optimisationProblem->getNumberOfEvaluations() << std::endl;
  std::cout << "numberOfDistinctEvaluations: " << optimisationProblem->getNumberOfDistinctEvaluations() << std::endl;
  std::cout << "bestSoftConstraintsValue: " << optimisationAlgorithm.getBestSoftConstraintsValue() << std::endl;
  std::cout << "bestObjectiveValueDelta: " << optimisationAlgorithm.getBestObjectiveValue() - optimisationProblem->getAcceptableObjectiveValue() << std::endl;
  std::cout << "bestParameter: " << optimisationAlgorithm.getBestParameter() << std::endl;
    
  return 1;
}
{% endhighlight %}

Compile and build an executable from your source.
{% highlight bash %}
c++ -std=c++11 hellp.cpp -larmadillo -o hello
# In case armadillo and mantella were not installed/placed into 
# standardly included/searched system direcories, use the following
c++ -std=c++11 hellp.cpp -L/path/to/armaillo/lib -larmadillo -I/path/to/armadillo/include -I/path/to/mantella/include -o hello
{% endhighlight %}

Run your application.
{% highlight bash %}
./hello
{% endhighlight %}

Output (may vary due to random initialised problems and algorithms):
{% highlight text %}
isFinished: 1
isTerminated: 0
numberOfIterations: 263
numberOfEvaluations: 263
numberOfDistinctEvaluations: 195
bestSoftConstraintsValue: 0
bestObjectiveValueDelta: -4.81944e-09
bestParameter:    2.7208
  -0.8456
   2.2640
   2.3872
{% endhighlight %}