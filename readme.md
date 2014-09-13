car-game
========

We're aiming to develop a car game, which can be played against AI opponents, controlled by a neural network.

Building
--------
The project can be built with tup. http://gittup.org/tup/

To build, simply type `tup`.

Use variants for different build configurations: `tup variant build/<name>.config`.

If additional dependencies are needed, edit `Tuprules.tup`.

Usage
-----
After building:

`./bin/car-game` will start GUI with no neural network.
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

`./bin/car-game --game-type=learn` will start crunching a population of neural networks. The best one will be stored in best.car

`./bin/car-game --neural-network best.car` will start the same GUI, but this time with the neural network stored in best.car.

for full help on command line and config file parameters, run `./bin/car-game --help`.

The car physics are based on this tutorial: http://www.asawicki.info/Mirror/Car%20Physics%20for%20Games/Car%20Physics%20for%20Games.html

The neural network implementation is based on: http://www.ai-junkie.com/ann/evolved/nnt1.html

