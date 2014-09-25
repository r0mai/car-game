car-game
========

We're aiming to develop a car game, which can be played against AI opponents, controlled by a neural network.

Building
--------
The project can be built with tup. http://gittup.org/tup/

The program depends on the following libraries:
- SFML (http://www.sfml-dev.org/)
- Boost (http://www.boost.org/)
- liblua (http://www.lua.org/)

To build, first create a variant: `tup variant build/<name>.config`. Then simply type `tup`.

Variants control the compiler and the flags. Supported compilers: gcc, clang.

If additional dependencies are needed, set the environment variables `EXTRA_CPP_FLAGS` (for compilation flags) and `EXTRA_LD_FLAGS` (for linker flags).

Usage
-----
After building:

`./build-<profile>/bin/car-game` starts GUI with no neural network.
- Press T to turn telemetry graphs on/off.
- Press X to turn telemetry text on/off.
- Press A to turn AI on/off, you can drive the car yourself.
- Press C to show/hide the car.
- Press Y to show/hide rays.
- Press P to show/hide checkpoints.
- Press R to show/hide track boundary.
- Press G to show/hide track area.
- Press E to show/hide the trace of the current car.
- Press PgUp/PgDown to select different cars.

`./build-<profile>/bin/car-game --game-type=learn` starts crunching a population of neural networks. The best one is stored in `best.car`. This file is saved after each generation, so it can be viewed even while learning is in progress.

`./build-<profile>/bin/car-game --neural-network best.car` starts the same GUI, but this time with the neural network stored in best.car.

for full help on command line and config file parameters, run `./build-<profile>/bin/car-game --help`.

The car physics are based on this tutorial: http://www.asawicki.info/Mirror/Car%20Physics%20for%20Games/Car%20Physics%20for%20Games.html

The neural network implementation is based on: http://www.ai-junkie.com/ann/evolved/nnt1.html

