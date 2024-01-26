# Neural network 3D library

This is the basic library to creating a neural network, but with some different way.
I was have an idea to create network as 3D model. Neural network is create from neuron object
it is place with cordinate x,y,z to the space, depending on different number of inputs and outputs 
and layers it generate 3D map. For now the library only puts the neuron on area map. Project is on the begining.

## Whay I wrote this?

When I was start learning about NN I don´t find any model of topology with 3D structure but only with matrix.
I want to try do this in different way and of course learn more about AI and ML.

## How it is work.

On first stage is to create oject of Neural Network depends of input, outputs and layers is calculate required numbers
of neurons and initialize memory. Next it is need to call method CreateNetwork to create 3D map of neurons, later I want
to add choising topology. Each neuron has 3 inputs and 5 outpust (for now its for testing). Next methods will be comming soon.

## Technology

The project is writen in Visual Studio 2022 in C++.

## Usage

Use library is very easy, you have to only create object NeuralNetwork with parametrs input, outputs and numbers of layers
 and call method .CreateNetwork();

"NeuralNetwork myNN(25, 5, 3);

myNN.CreateNetwork();"

For now working method are:

- LayerCount();
- DuplicateSearch();

Project is on the begining so for now only generate map of neurons. More in TODO.


## TODO:

- Add method to connect neuron, random connection
- Add mehtod to initialize neuron and weight 
- Add method Forward for working with NN
- Add backpropagation method 
- Add some diagnostic method

