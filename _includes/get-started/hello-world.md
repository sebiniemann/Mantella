Create a new source file called **hello.cpp** on your computer.
{% highlight cpp %}
{% include get-started/_examples/hello.cpp %}
{% endhighlight %}

Compile and build an executable from your source.
{% highlight bash %}
c++ -std=c++11 hello.cpp -larmadillo -o hello
# In case armadillo and mantella were not installed/placed into 
# standardly included/searched system direcories, use the following
c++ -std=c++11 hello.cpp -L/path/to/armaillo/lib -larmadillo -I/path/to/armadillo/include -I/path/to/mantella/include -o hello
{% endhighlight %}

Run your application.
{% highlight bash %}
./hello
{% endhighlight %}

Output (may vary due to random initialised problems and algorithms):
{% highlight text %}
{% include get-started/_examples/hello.output %}
{% endhighlight %}