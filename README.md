## Barrier Resilience

This repository contains implementations of multiple algorithms solving the problem of barrier resilience in a
rectangular domain. Repository is a part of my master's thesis.

#### Prerequisites

Install C++ compiler with support for C++20 standard, CMake, Google's testing framework and CGAL library. Currently,
system versions of these libraries are used.

To install them on Ubuntu, run:

```
sudo apt install build-essential cmake libgtest-dev libcgal-dev
```

On Mac, run:

```
brew install cmake googletest cgal
```

#### Instalation

Clone this repository

```
git clone git@github.com:matejm/barrier-resilience.git
```

Go to root folder of the repository, create `build` folder, run CMake and make:

```
mkdir -p build
cd build
cmake ..
make
```

#### Testing

To test the algorithms, run:

```
make tests
./../bin/tests
```

#### Examples & Experiments

Multiple examples of usage are located in `examples` folder. Simple scripts for performing experiments are located
in `experiments` folder. All binaries created by `make` are located in `bin` folder.



