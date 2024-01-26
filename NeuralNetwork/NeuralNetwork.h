#pragma once


#ifdef NEURALNETWORK_EXPORTS
#define NEURALNETWORK_API __declspec(dllexport)
#else
#define NEURALNETWORK_API __declspec(dllimport)
#endif

#define MAX_LAYER_TO_ADD 20  // max number of additional layers, its requried for output neurons

class NEURALNETWORK_API Neuron;

class NEURALNETWORK_API Weight
{
public:
	float weightValue;
	Neuron* input;
	Neuron* output;
};

class NEURALNETWORK_API InputNeuron
{
public:
	float neuronValue;
	int pos_x;
	int pos_y;
	int pos_z;
	//Weight *input_0;
	Weight* output_0;
	Weight* output_1;
	Weight* output_2;
};

class NEURALNETWORK_API OutputNeuron
{
public:
	float neuronValue;
	int pos_x;
	int pos_y;
	int pos_z;
	Weight* input_0;
	Weight* input_1;
	Weight* input_2;
	//Weight *output_0;

};

class NEURALNETWORK_API Neuron
{
public:
	float neuronValue;
	int pos_x;
	int pos_y;
	int pos_z;
	Weight* input_0;
	Weight* input_1;
	Weight* input_2;
	Weight* output_0;
	Weight* output_1;
	Weight* output_2;
	Weight* output_3;
	Weight* output_4;

	void ResetNeuron(Neuron*);

};


class NEURALNETWORK_API NeuralNetwork
{
public:
	int input;
	int output;

	// error handle
	int errnum;

	NeuralNetwork(int, int, int);

	int CreateNetwork();
	int GetInput() { return input; }

	int DuplicateSearch();				// Method for search duplicate neroun with the same cordinates
	int LayerCount();					// Showing number of nerouns on each layers	
	void ShowNeuron(int, int);			// Showing neurons from index to end index
	void ShowNeuronIn();				// Show input neuron
	void ShowNeuronOut();				// Show output neuron
	int CreateConnection();		/* TODO implement */
	int CreateConnection(int);	/* TODO implement */

	/* 
	 * TODO list
		- Add method to connect neuron, random connection
		- Add mehtod to initialize neuron and weight
		- Add method Forward for working with NN
		- Add backpropagation method
		- Add some diagnostic method
	*/

private:
	int layers;
	int conn;		// weight connection 
	int ratio;
	int layersToAdd[MAX_LAYER_TO_ADD][1];     // store layers to add and number of each added layers
	int allNeurons;
	bool init;

	Neuron** in_neuron;
	Neuron** out_neuron;
	Neuron** n_neuron;
	Weight** n_weight;

	//Calculate numbers of neuron needed for network and initialize memory
	int InitNeuron();
	// method to place neuron if column != row
	int ColumnRowNE(int, int, int, int, Neuron**, int);
	int ConnectNeuron(Neuron*, Neuron*);

};
#pragma once
