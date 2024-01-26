// dllmain.cpp : Definuje vstupní bod pro aplikaci knihovny DLL.
#include "pch.h"
#include "NeuralNetwork.h"
#include <iostream>
#include <random>

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}



//constructor
/* 
*/
NeuralNetwork::NeuralNetwork(int in, int out, int layer)
{
    input = in;
    output = out;
    layers = layer;

    conn = 0;		// weight connection 

    in_neuron = NULL;
    out_neuron = NULL;
    n_neuron = NULL;
    n_weight = NULL;
    init = false;

    errnum = InitNeuron();
}


int NeuralNetwork::InitNeuron()
{
    int addNeuron = 0;

    // create object of neural network and initialize it 

    // initialize input neurons
    in_neuron = new Neuron * [input];

    for (int i = 0; i < input; i++)
    {
        in_neuron[i] = new Neuron;
        if (in_neuron[i] == NULL)
        {
            return 2;
        }
        //reset neuron
        in_neuron[i]->ResetNeuron(in_neuron[i]);
    }

    // initialize output neurons
    out_neuron = new Neuron * [output];

    for (int i = 0; i < output; i++)
    {
        out_neuron[i] = new Neuron;
        if (out_neuron[i] == NULL)
        {
            return 3;
        }
        //reset neuron
        out_neuron[i]->ResetNeuron(out_neuron[i]);
    }

    //last layers are depend on O/I rate 
    ratio = input / 3;

    // reset variable
    for (int i = 0; i < MAX_LAYER_TO_ADD; i++)
    {
        layersToAdd[i][0] = 0;
    }

    for (int i = 0; i < MAX_LAYER_TO_ADD; i++)    
    {

        if (ratio >= output)
        {
            int t = output * 3;
            if ((ratio - t) < 2) ratio += 2;
            layersToAdd[i][0] = ratio;

            ratio = ratio / 3;              // Neuron have 3 inputs

            if (ratio <= output)
            {
                if ((output * 3) > ratio)// && i >=  1)
                {
                    int tmp = ((output * 3) + layersToAdd[i][0]) / 2;
                    layersToAdd[i][0] = tmp;
                }
            }
        }
    }


    // Count how meny neutons is need for additional layers
    for (int i = 0; i < MAX_LAYER_TO_ADD; i++)
    {
        addNeuron += layersToAdd[i][0];
    }

    // Store Neurons needed for network
    allNeurons = (input)+(layers * (input * 5)) + addNeuron;
    // initialize pointers to neurons
    n_neuron = new Neuron * [allNeurons];

    for (int i = 0; i < allNeurons; i++)
    {
        n_neuron[i] = new Neuron;
        if (n_neuron[i] == NULL)
        {
            return 4;
        }
        //reset neuron
        n_neuron[i]->ResetNeuron(n_neuron[i]);
        // for one Neuron I need three output weight
        conn += 8;
    }

    n_weight = new Weight * [conn];

    for (int i = 0; i < conn; i++)
    {
        n_weight[i] = new Weight;
    }

    init = true;
    return 0;
}

int NeuralNetwork::CreateNetwork()
{
    // check if init was sucessed 
    if (init != true)
    {
        return 10;
    }
    // Start creating network

    // set position of Input Neuron
    int pos_x = 1;
    int pos_y = 0;          // Layer 0 is an input layer
    int pos_z = 1;

    int added_neuron = 0;
    int layersAdded = 0;                // Set counter for add layer

    Neuron* current_neuron = NULL;      // current layer 
    Neuron* next_neuron = NULL;         // next layer

    ColumnRowNE(input, pos_x, pos_y, pos_z, in_neuron, 0);

    // end position setting Input Neuron

    /* Set first layer of neurons
     * Number of layers are depend on O/I ratio
     *
     */

     // helper variable
    int used_weight = 0;    // sum of used connection  
    int used_neuron = 0;    // sum of used neruon

    for (int i = 0, k = 0; i < input; i++, k += 5)
    {
        // take each positon of input neuron
        pos_x = in_neuron[i]->pos_x;
        pos_y = in_neuron[i]->pos_y + 1;        // increment layer 
        pos_z = in_neuron[i]->pos_z;
        // add connection weight
        in_neuron[i]->output_0 = n_weight[k];
        in_neuron[i]->output_1 = n_weight[k + 1];
        in_neuron[i]->output_2 = n_weight[k + 2];
        in_neuron[i]->output_3 = n_weight[k + 3];
        in_neuron[i]->output_4 = n_weight[k + 4];

        // and back connection from weigth to input neuron
        n_weight[k]->input = in_neuron[i];
        n_weight[k + 1]->input = in_neuron[i];
        n_weight[k + 2]->input = in_neuron[i];
        n_weight[k + 3]->input = in_neuron[i];
        n_weight[k + 4]->input = in_neuron[i];

        used_weight += 5;
        /*
        * layer will be
        *    0 x 0
        *    x x x
        * |  0 x 0
        * z    x ->
        */
        // each inputs neuron have connection to 5 new neurons

        int pos_x_1 = pos_x;            // center position
        int pos_y_1 = pos_y;
        int pos_z_1 = pos_z;
        int pos_x_2 = pos_x;            // bottom position
        int pos_y_2 = pos_y;
        int pos_z_2 = pos_z - 1;
        int pos_x_3 = pos_x - 1;        // left position    
        int pos_y_3 = pos_y;
        int pos_z_3 = pos_z;
        int pos_x_4 = pos_x + 1;        // right position
        int pos_y_4 = pos_y;
        int pos_z_4 = pos_z;
        int pos_x_5 = pos_x;            // upper position
        int pos_y_5 = pos_y;
        int pos_z_5 = pos_z + 1;

        n_neuron[k]->pos_x = pos_x_1;
        n_neuron[k]->pos_y = pos_y_1;
        n_neuron[k]->pos_z = pos_z_1;
        n_neuron[k + 1]->pos_x = pos_x_2;
        n_neuron[k + 1]->pos_y = pos_y_2;
        n_neuron[k + 1]->pos_z = pos_z_2;
        n_neuron[k + 2]->pos_x = pos_x_3;
        n_neuron[k + 2]->pos_y = pos_y_3;
        n_neuron[k + 2]->pos_z = pos_z_3;
        n_neuron[k + 3]->pos_x = pos_x_4;
        n_neuron[k + 3]->pos_y = pos_y_4;
        n_neuron[k + 3]->pos_z = pos_z_4;
        n_neuron[k + 4]->pos_x = pos_x_5;
        n_neuron[k + 4]->pos_y = pos_y_5;
        n_neuron[k + 4]->pos_z = pos_z_5;

        // set connection between neuron and input neuron
        in_neuron[i]->output_0->output = n_neuron[k];
        in_neuron[i]->output_1->output = n_neuron[k + 1];
        in_neuron[i]->output_2->output = n_neuron[k + 2];
        in_neuron[i]->output_3->output = n_neuron[k + 3];
        in_neuron[i]->output_4->output = n_neuron[k + 4];

        n_neuron[k]->input_0 = in_neuron[i]->output_0;
        n_neuron[k + 1]->input_0 = in_neuron[i]->output_0;
        n_neuron[k + 2]->input_0 = in_neuron[i]->output_0;
        n_neuron[k + 3]->input_0 = in_neuron[i]->output_0;
        n_neuron[k + 4]->input_0 = in_neuron[i]->output_0;
        added_neuron += 5;
    }
    // end set of first layer  
    /*
    * Adding next layer relies on adding paralel neuton with the same cordinate like on previous layer    
    * Layers are increase on y cordinates
    *
    * First count how meny layers is required
    */
    layersAdded = layers;
    int neuronPerLayer = input * 5;
    int index = 0;                                  // store index of first neuron on previously layer
    int last_index = added_neuron;                   // Set to next free neuron


    for (int i = 0; i < layers; i++)
    {
        if (added_neuron <= allNeurons)             // when occrus bad calculation 
        {

            // if it is a last layer add only one neuron on input neuron cordinate on last layer
            if (layersAdded == 1)
            {
                last_index = added_neuron + input;
                for (int k = index; k < last_index; k++)
                {
                    // set pointers
                    current_neuron = n_neuron[k];
                    if (current_neuron == NULL) return -1;      // NULL poiner - never can hapend but...

                    // take pointer from current layer - pos_y 
                    if (current_neuron->pos_y == pos_y)
                    {
                        /* set neuron when x or y are on input sequence
                         * x = input x, z = input z
                         * using x and z = neuron (i -1) *3
                        */

                        int x = current_neuron->pos_x;
                        int z = current_neuron->pos_z;
                        int seq_x = 1 + ((k + 1) - 1) * 3;          // index starts from 0 but sequence start from 1
                        int seq_z = 1 + ((k + 1) - 1) * 3;
                        bool bseq_x = false;
                        bool bseq_z = false;

                        for (int l = 1; l <= last_index; l++) {
                            int tmp = 1 + ((l - 1) * 3);
                            if (tmp == x) bseq_x = true;
                            if (tmp == z) bseq_z = true;
                            if (bseq_x && bseq_z) break;
                        }
                        /*
                         * Add neuron when is on input cordinate sequence
                         */
                        if (bseq_x && bseq_z)
                        {
                            //Take next free neuron and set cordinates 
                            next_neuron = n_neuron[added_neuron];
                            if (next_neuron == NULL) return -1;

                            //Set cordinates of next neuron
                            next_neuron->pos_x = current_neuron->pos_x;
                            next_neuron->pos_z = current_neuron->pos_z;
                            next_neuron->pos_y = current_neuron->pos_y + 1;
                            added_neuron++;
                            
                        }
                    }//end last layer

                } // loop for ading neuron      
                pos_y++;        // increment layer 
                index = last_index;
                last_index = added_neuron;
                layersAdded--;
            }
            else if (layersAdded > 1)   // add n number of laysers
            {
                // Add neuron paraler to previously layer
                for (int k = index; k < last_index; k++)
                {
                    current_neuron = n_neuron[k];
                    if (current_neuron == NULL) return -1;      // NULL poiner - it is never can hapend but...
                    //Take next free neuron and set cordinates 
                    next_neuron = n_neuron[added_neuron];
                    if (next_neuron == NULL) return -1;

                    //simply take neuron and add a new one on the next layer                    
                    next_neuron->pos_x = current_neuron->pos_x;
                    next_neuron->pos_z = current_neuron->pos_z;
                    next_neuron->pos_y = current_neuron->pos_y + 1;
                    added_neuron++;
                } // loop for layer 
                index = last_index;
                last_index = added_neuron;
                layersAdded--;          // update count layers
                pos_y++;        // increment layer 
            }
        }
    }// end layers

    pos_y++;
    // Prepare cordinates for reduced layers
    int column = (int)std::sqrt(input);
    int center_x = column / 2;
    int center_z = column / 2;
    int side = column / 2;    // step counter
    int lb_corner_x = center_x - side;
    int lb_corner_z = center_z - side;

    if (layersToAdd[0][0] > 0)
    {
        //Add additional layers
        //Reduced for reach number of output 

        int addLayers = 0;

        for (int i = 0; i < MAX_LAYER_TO_ADD; i++)
        {
            if (layersToAdd[i][0] != 0)
            {
                addLayers++;
            }
        }

        for (int i = 0; i < addLayers; i++)
        {
            last_index = ColumnRowNE(layersToAdd[i][0], lb_corner_x, pos_y, lb_corner_z, n_neuron, added_neuron);
            added_neuron = last_index;

            index = last_index;
            last_index = added_neuron;
            pos_y++;        // increment layer 

        }

    }

    // Place output
    ColumnRowNE(output, lb_corner_x, pos_y, lb_corner_z, out_neuron, 0);

    return 0;
}


int NeuralNetwork::DuplicateSearch()
{
    /* test if neruron is duplicate
*/
    int  count = 0;
    int  y = 0;
    for (int i = 0; i < allNeurons; i++)
    {
        if (n_neuron[i]->pos_y == y)
        {
            int x = n_neuron[i]->pos_x;
            int z = n_neuron[i]->pos_z;

            for (int j = i + 1; j < allNeurons; j++)
            {
                if (x == n_neuron[j]->pos_x && y == n_neuron[j]->pos_y && z == n_neuron[j]->pos_z)
                {
                    std::cout << " Duplicate : " << j << " X : " << n_neuron[j]->pos_x << " Y : " << n_neuron[j]->pos_y << " Z : " << n_neuron[j]->pos_z << std::endl;
                    std::cout << " Duplicate 2: " << i << " X : " << n_neuron[i]->pos_x << " Y : " << n_neuron[i]->pos_y << " Z : " << n_neuron[i]->pos_z << std::endl;

                }

            }
            count++;

        }
        else
        {
            y++;
        }

    }
    return count;
}

int NeuralNetwork::LayerCount()
{
    /* test if neruron is duplicate
*/
    int  count = 0;
    int  y = 1;
    int index = 0;
    int last_index = 0;
    for (size_t i = y; i < MAX_LAYER_TO_ADD; i++)
    {
        for (int i = 0; i < allNeurons; i++)
        {
            if (n_neuron[i]->pos_y == y)
            {
                if (count == 0)
                {
                    index = i;
                }
                count++;
            }

        }
        last_index = index + count;
        std::cout << " Neuron on layer: " << y << ": " << count << " S: " << index << " E:" << last_index << std::endl;
        count = 0;
        y++;
    }
    return count;
}


int NeuralNetwork::CreateConnection()
{
    std::random_device r;
    std::uniform_real_distribution<float> uniform_dist(-0.5, 0.5);
    std::mt19937 e2(r());
    float gen = 0;
    //gen = uniform_dist(e2);

    int y = 0;
    Neuron* current_neuron = NULL;      // current layer 
    Neuron* next_neuron = NULL;         // next layer
    Weight* w_conn = NULL;
    int long add_conn = 0;
    int x = 0;
    int z = 0;

    for (size_t i = y; i < MAX_LAYER_TO_ADD; i++)
    {
        for (int i = 0; i < allNeurons; i++)
        {
            // first layer 
            if (n_neuron[i]->pos_y == 1)
            {
                x = n_neuron[i]->pos_x;
                z = n_neuron[i]->pos_z;
                // TODO implement creating connections 

            }

            if (n_neuron[i]->pos_y == y)
            {

            }

        }
        y++;
    }
    return 0;
}

int NeuralNetwork::CreateConnection(int layer)
{

    return 0;
}



void NeuralNetwork::ShowNeuron(int start, int end)
{
    int line = 1;
    for (int i = start; i < end; i++)
    {
        std::cout << " Neuron " << i << ": X=" << n_neuron[i]->pos_x << " Y=" << n_neuron[i]->pos_y << " Z=" << n_neuron[i]->pos_z;
        if (line == 4)
        {
            std::cout << std::endl;
            line = 0;
        }
        line++;
    }
}

void NeuralNetwork::ShowNeuronIn()
{
    int line = 1;
    for (int i = 0; i < input; i++)
    {
        std::cout << " Neuron " << i << ": X=" << in_neuron[i]->pos_x << " Y=" << in_neuron[i]->pos_y << " Z=" << in_neuron[i]->pos_z;
        if (line == 4)
        {
            std::cout << std::endl;
            line = 0;
        }
        line++;
    }
}

void NeuralNetwork::ShowNeuronOut()
{
    int line = 1;
    for (int i = 0; i < output; i++)
    {
        std::cout << " Neuron " << i << ": X=" << out_neuron[i]->pos_x << " Y=" << out_neuron[i]->pos_y << " Z=" << out_neuron[i]->pos_z;
        if (line == 4)
        {
            std::cout << std::endl;
            line = 0;
        }
        line++;
    }
}

int NeuralNetwork::ColumnRowNE(int count, int pos_x, int pos_y, int pos_z, Neuron** neuron, int index)
{
    int last_pos_x = 0;
    int last_pos_z = 0;
    int row = count;
    int column = (int)std::sqrt(row);   // Divided to row and column 
    int tmp = column * column;
    int tmp_row = 0;                    // Calculate rest of neuron - row != column

    if (tmp != row)
    {
        tmp_row = std::abs(tmp - row);
    }


    int added = index;

    // Set position of input neuron - row = column
    for (int i = 0; i < column; i++)
    {
        for (int j = 0; j < column; j++)
        {
            // set position of first input 
            // first input is on position 
            neuron[added]->pos_x = pos_x;
            neuron[added]->pos_y = pos_y;
            neuron[added]->pos_z = pos_z;
            //increse position for next input 
            added++;
            pos_x += 3;
            last_pos_x = pos_x;
            
        }
        pos_z += 3;
        pos_x = 1;
    }
    // end set position of input neuron

    // when input neuron is not equal: row = column, add rest input neuron 
    if (tmp_row != 0)
    {
        //add last inputs
        float tmp = (float)tmp_row / column;
        if (tmp <= 1)
        {
            count = column * column;    // used neuron
            //Put neurons to right column
            last_pos_z = pos_z;
            pos_z = 1;                      //set z to begin
            pos_x = last_pos_x;
            for (int i = count; i < row; i++)
            {
                neuron[added]->pos_x = pos_x;
                neuron[added]->pos_y = pos_y;
                neuron[added]->pos_z = pos_z;
                pos_z += 3;
                last_pos_z = pos_z;
                added++;
                
            }
        }
        else if (tmp > 1)
        {
            //put neurons to right column and to upper row
            count = tmp_row;    // used neuron

            //Put neurons to right column
            last_pos_z = pos_z;
            pos_z = 1;                      //set z to begin
            pos_x = last_pos_x;
            // put to right column
            for (int i = 0; i < column; i++)
            {
                neuron[added]->pos_x = pos_x;
                neuron[added]->pos_y = pos_y;
                neuron[added]->pos_z = pos_z;
                pos_z += 3;
                added++;
                count--;
                
            }
            //put to upper row
            last_pos_x = pos_x;
            pos_x = 1;

            for (int i = 0; i < count; i++)
            {
                neuron[added]->pos_x = pos_x;
                neuron[added]->pos_y = pos_y;
                neuron[added]->pos_z = pos_z;
                pos_x += 3;
                added++;
                
            }
        }

    }
    return added;
}

/*
* Definition Neuron class
*
*/

void Neuron::ResetNeuron(Neuron* neuron)
{
    // Set position to 0
    neuron->pos_x = 0;
    neuron->pos_y = 0;
    neuron->pos_z = 0;

    // Reset Neuron value
    neuron->neuronValue = 0;

    // Set connection to NULL
    neuron->input_0 = NULL;
    neuron->input_1 = NULL;
    neuron->input_2 = NULL;
    neuron->output_0 = NULL;
    neuron->output_1 = NULL;
    neuron->output_2 = NULL;
    neuron->output_3 = NULL;
    neuron->output_4 = NULL;

}