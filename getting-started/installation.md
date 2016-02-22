---
layout: page
---
{% include h1 title="Installation" %}

{% include h2 title="Prerequirements" %}

While our goal is to keep the amount of dependencies at a minimum, we still require at least a modern C++ Compiler, CMake and a linear algebra library:

- GCC 4.8.1, Clang 3.3 or any other C++11 feature complete compiler
- CMake 2.8.12
- Armadillo C++ 6.400.0

**If you already installed a newer compiler, CMake or Armadillo C++ version, this will also work**. If not, the following instructions will guide you step by step throw their installation, depending on your operating system.

{% assign titles_ = site.array %}
{% assign contents_ = site.array %}
{% assign titles_ = titles_ | push: "Linux" %}{% capture content_ %}
The following commands are based on Ubuntu, using `apt-get`. If you are using **another** Linux distributions, the names of each package might also differ.

If you are using any recent Linux distribution, you should already be equipped with a C++11 feature complete compiler, like GCC 4.8.1+ or Clang 3.3+.

1. **Install CMake**

        sudo apt-get install cmake
        
2. **Install Armadillo C++ with OpenBLAS support** (visit [Armadillo's download page](http://arma.sourceforge.net/download.html) to find more information on how to use other implementations of BLAS and LAPACK):

        sudo apt-get install libblas-dev liblapack-dev libopenblas-dev
        wget --quiet -O armadillo.tar.gz http://downloads.sourceforge.net/project/arma/armadillo-6.500.5.tar.gz
        mkdir armadillo
        tar -xzf armadillo.tar.gz -C ./armadillo --strip-components=1
        cd armadillo
        cmake .
        make
        sudo make install
        
{% endcapture %}{% assign contents_ = contents_ | push: content_ %}
{% assign titles_ = titles_ | push: "Mac OS X" %}{% capture content_ %}
The following commands are based on [Homebrew](http://homebrew.sh), a package manager for OS X.

If you are using any recent XCode version, you should already be equipped with a C++11 feature complete compiler, like Clang 3.3+.
        
1. **Install CMake**

        brew install cmake
        
2. **Install Armadillo C++ with OpenBlas support**

        brew install openblas
        brew install armadillo
        
{% endcapture %}{% assign contents_ = contents_ | push: content_ %}
{% assign titles_ = titles_ | push: "Windows" %}{% capture content_ %}
The following steps are written for 64-bit systems and tested on Windows 7. If you want to work with a 32-bit version, it should be sufficient to replace OpenBLAS and MinGW with their 32-bit counterpart and explicitly choosing `Visual Studio 14 2015` as CMake generator, instead of `Visual Studio 14 2015 Win64` (see ). It is also possible to use any other BLAS/LAPACK library beside OpenBLAS, despite not being covered in this guide.

1. **Install Visual Studio** ([download page](https://www.visualstudio.com/de-de/downloads/download-visual-studio-vs.aspx), 2015 Community version or better)
2. **Install CMake** ([download page](https://cmake.org/download/), version 2.8.12 or better)
3. **Install OpenBLAS**
    - Download the precompiled **64-bit** OpenBLAS binary package ([download page](http://www.openblas.net/), any `OpenBLAS-vX.X.XX-Win64-int32` version) and copy its contents into `C:\OpenBLAS`
4. **Install Armadillo (header-only)**
    - Download the latest version of Armadillo ([download page](http://arma.sourceforge.net/download.html)) and copy its contents into `C:\Armadillo`
    - Activate/uncomment `ARMA_USE_CXX11` in `C:\Armadillo\include\armadillo_bits\config.hpp`
    - Deactivate/outcomment `ARMA_USE_WRAPPER` in `C:\Armadillo\include\armadillo_bits\config.hpp`

The above installation is already sufficient to compile Mantella and Mantella-based projects, but they won't run under Windows just yet, as the pre-built 64-bit OpenBLAS library depends on MinGW (64-bit). You can skip this, if your BLAS/LAPCK implementations does not depend on MinGW.

5. **Install MinGW-w64**
    - Download MinGW-w64 from (http://sourceforge.net/projects/mingw-w64/) and install it into `C:\MinGW-w64`
        - Version: latest
        - Architecture: x86_64
        - Threads: win32
        - Exception: seh
        - Build revision: 1
6. Choose **either** one of the following options:
    - Add the following locations to your system path (`PATH`)
        - `C:\OpenBLAS\bin`
        - `C:\mingw-w64\mingw64\bin`
    - **or** copy the following libraries into same location as your Mantella-based executable
        - `C:\OpenBLAS\bin\libopenblas.dll`
        - `C:\mingw-w64\bin\libgcc_s_seh-1.dll`
        - `C:\mingw-w64\bin\libgfortran-3.dll`
        - `C:\mingw-w64\bin\libquadmath-0.dll`
{% endcapture %}{% assign contents_ = contents_ | push: content_ %}
{% include tabs group="Prerequirements" titles=titles_ contents=contents_ %}

{% include h2 title="Download and installation" %}

{% assign titles_ = site.array %}
{% assign contents_ = site.array %}
{% assign titles_ = titles_ | push: "Linux and Mac OS X" %}{% capture content_ %}
``` bash
git clone --depth 1 --branch master https://github.com/SebastianNiemann/Mantella.git
cd Mantella
cmake .
make -j 4
sudo make install
```
{% endcapture %}{% assign contents_ = contents_ | push: content_ %}
{% assign titles_ = titles_ | push: "Windows" %}{% capture content_ %}
``` bash
git clone --depth 1 --branch master https://github.com/SebastianNiemann/Mantella.git
cd Mantella
cmake -DCMAKE_PREFIX_PATH=C:\Armadillo\include;C:\OpenBLAS -G "Visual Studio 14 2015 Win64" .
cmake --build . --config Release
```

You should now find `mantella.dll` under `.\lib\Release\`.
{% endcapture %}{% assign contents_ = contents_ | push: content_ %}
{% include tabs group="Installation" titles=titles_ contents=contents_ %}

**Done.** You can now start building applications with Mantella.
