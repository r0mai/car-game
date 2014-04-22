car-game
========

We're aiming to develop a car game, which can be played against AI opponents, controlled by a nerual network.

Usage
-----
After building:

`./bin/car-game` will start GUI with a random neural network.
Press T to turn telemetry on/off. 
Press A to turn AI on/off, you can drive the car yourself
	
`./bin/car-game --ai` will start crunching a population of neural networks. The best one will be stored in best.car
	
`./bin/car-game --neural-network best.car` will start the same GUI, but this time with the neural network stored in best.car.

The car physics are based on this tutorial: http://www.asawicki.info/Mirror/Car%20Physics%20for%20Games/Car%20Physics%20for%20Games.html

The neural network implementation is based on: http://www.ai-junkie.com/ann/evolved/nnt1.html

