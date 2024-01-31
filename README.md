# pomdp-solve

Software for performing value iteration on partially observable Markov decision processes (POMDPs).

## Table of Contents

- [About](#about)
- [Getting Started](#getting-started)
- [Contents](#contents)
- [License](#license)

## About

This is a POMDP solver that implements a number of different algorithms. Most of these are variations of exact solutions, though there is also support for aproximate finite grid solving.

## Getting Started

If you just want to compile and verify you can run the code, first build the executable with:
```
./configure
make
```

To verify it builds and you are able to run it:
```
./src/pomdp-solve -pomdp examples/pomdp-files/tiger.95.POMDP
```

## Contents

* `ChangeLog` : Changes to the code since version 4.0

* `./src` :  The main source directory for the pomdp-solve program.

* `./doc` : This directory has some useful documentation such as file formats, command line arguments, etc.  Start with the index.html file to see the various documents.

* `./src/mdp` : This directory contains the code for a library of low-level POMDP manipulation routines, including parsing files and the internal representation used by the program.  Only look at the stuff in this directory if you want to muck around with the file format and/or the internal representation.

* `./src/lp_solve` : This directory contains the code for a public domain linear programming solver. It is included it in this distribution because some minor changes were needed to integrate it with the pomdp-solve code.  If the commerical program CPLEX does not exist, this is what is used to solve the linear programs required.

* `./testing` : This directory has some test cases used by the auto-test executable. It contains some POMDP files as well as some finite and infinite horizon value functions and policy graphs. 

## License

This software is licensed under the Creative Commons Attribution-NonCommercial 4.0 International License (CC BY-NC 4.0). See the [LICENSE](LICENSE.txt) file for details.
