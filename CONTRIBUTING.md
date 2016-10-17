Contributing to Mantella
=======================

This guide will accompany you on your first contribution to Mantella, covering everything from setting up a development system to committing your changes.

In case you only want to submit a pull request, go directly to **Filing a pull request** (at the end of the document).

**If you are in a hurry**, feel free to skip any part at your own account, but be aware that this might delay accepting your pull request (as someone needs to invest the time to finish it up :wink:), especially if the implementation is not yet working.

Let us know what you are planning
---------------------------------

If you found a bug in the source code, a mistake in any kind of documentation or you are missing some features within Mantella and want to implement/fix it yourself, please let us know in advance, so we can discuss the implementation or handle the bug beforehand. This way, we can also give you early support and advices.

To do this, simply add an issue to the [GitHub issue tracker](https://github.com/Mantella/Mantella/issues), describing what you plan to do and make clear whether you are already working on it or not. You can also contact us directly on [Gitter](https://gitter.im/Mantella/Mantella) at any point of your contribution.

In case you found a bug and want to fix it, please give us informations on what got wrong and how to reproduce it.

Forking Mantella
----------------

### Contributors

Your first step will be to fork Mantella on GitHub, for example by hitting `Fork` on the GitHub website and download your fork onto your local machine using the [Git tool](https://git-scm.com/downloads) of your choice.

### Team members

If you are a long-time contributor to Mantella and got write permissions for the repository, you don't need to fork Mantella to your account, but can directly add a new branch to Mantella 

``` bash
git checkout master
git pull
git checkout -b YourUsername/YourAwesomeBranch
```

Setting up a development system
-------------------------------

**The recommended and most easiest way** to set up a Mantella development system (including all dependencies), is to install 
[Docker](https://www.docker.com) and run:

```
git clone http://github.com/Mantella/Mantella.git
cd Mantella
sudo docker build -t ubuntu/mantella:latest .
sudo docker run -v .:/mantella -w /mantella --name mantella -t -d ubuntu/mantella
```

**Done!**

This will install a virtual machine and set up all required dependencies, together with other useful tools for testing or debugging. You can then connect to the Docker machine using:

```
sudo docker exec -i -t mantella /bin/bash
```

In case you have problems installing Docker on your operation system (typically on non-Linux machines), you can also install [Vagrant](https://www.vagrantup.com) after cloning Mantella and run:

```
vagrant up
```

to install a virtual machine including our Docker machine.

You can then connect to the Vagrant machine using your SSH client of choice (using `localhost:2222` as host:port and `vagrant` as username as well as password) and access the Docker machine inside Vagrant using `sudo docker exec -i -t mantella /bin/bash` as before.

Coding guidelines
-----------------

**Most important: Read some existing code, to get a better understanding of our code style :wink:**

### Adding new headers

When adding new files, put your headers under `include/mantella_bits/`. To include them into the library, add your headers to `include/mantella`.

### Comments

Describe complex parts of your code as well as [magic numbers](https://en.wikipedia.org/wiki/Magic_number_(programming)) as specific as possible and avoid vague descriptions.

**If you had to think about how to implement something or needed to learn it on [StackOverflow](http://stackoverflow.com/) (or any other site) about how to do it, it is a complex part.**

However, if you find yourself documenting code that is hard to read or understand, you should consider to rewrite such tricky or unclear code first. In general, you should strive for readable, maintainable code, by using well-known patterns and simple structures.

Testing your work
-----------------

### Running tests

To install your changes, build and execute the text, run

``` bash
./.develop --install --test
```

Executing the tests can also be further configured by using [Catch's commandline options](https://github.com/philsquared/Catch/blob/master/docs/command-line.md). For example, running just a subset of tests

``` bash
./test/build/tests [myNewFunction]
``` 

Filing a pull request
---------------------

Pull request messages should then be organised as followed:

``` text
<tag>: <subject>

<body>

<licence statement>
```

### Tag
Add one of the following tags, that describe your changes the best.

- `api`: Your change will break the existing API in any way.
- `feature`: You added a new feature, without breaking the existing API.
- `fix`: You fixed a bug.
- `test`: You changed or added a test.
- `doc`: You extended or corrected the documentation.
- `misc`: For anything else.

### Subject
Add a description about the content of your commit.

### Body (optional)
In case the subject does not fully covers your changes, fell free to describe the details of your changes as specific as as you can/want. Do not be afraid to add multiple lines.

### Licence statement
End the commit message of your pull request adding a clear statement under which your licence your contribution is licensed.

Note that we cannot accept contributions that are either missing a licence statement or have chosen a licence incompatible to the MIT licence.

``` text
My contribution is licensed under the MIT license.
```