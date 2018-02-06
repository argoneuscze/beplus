# beplus

beplus a simple interpreter for a statically typed C-like language. The language supports dynamic
allocation and also has a simple mark-and-sweep based garbage collector.

## Installation

beplus is written in C++ and packaged using CMake, so it should be easily compilable on every major platform.

### Linux

* Clone this repository
* Create another directory for building (e.g. `mkdir build` in the project directory)
* Run CMake (`cmake ..`)
* Run the resulting Makefile (`make`)
* Run the Beplus interpreter (`./beplus`)

## Usage

You may either run the interpreter in REPL mode, or you can supply a filename as an argument.
This file will then be parsed and ran with the interpreter. The authors are as following:

* **Marek Pikna** @ <mar.pikna@gmail.com>
* **Tomáš Drbota** @ <argoneuscze@gmail.com>

## Notes

This interpreter was submitted as a semestral project for the course MI-RUN (Runtime Systems)
at FIT CTU in Prague.

## License

This program is licensed under the [MIT License](LICENSE.md).
