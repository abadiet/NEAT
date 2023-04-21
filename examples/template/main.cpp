#include <iostream>
#include <cmath>
#include <population.hpp>

using namespace std;

float activationFn(float input) {
    /* Used to process the output of each node relatively to their input */
    
    float output = // process of input ...
    
    return output;
}

struct args {
    /* The arguments used to conserve any data accross the iteration of processFn */
    
    // some data ...
    
} myArgs;

void setupFn(float inputsInit[], struct args* myArgs) {
    /* Setup the arguments and the first inputs given to the network */
    
    // setup myArgs ...
    
    // setup inputsInit ...
}

float processFn(float inputs[], float outputs[], struct args* myArgs) {
    /* Relatively to myArgs, process the given inputs to get the outputs which will be given to the Network next iteration. Return negative float if process not done (e.g. game not finished), else return the fitness */
    
    // load inputs ...
    // processing ...
    // get outputs ...
    
    if (wholeProcessIsDone) {
        return fitness; // which stop the network
    } else {
        return -1.0f;   // any negative float, which called the network for next iteration
    }
}


int main() {
    srand(time(0));	// init seed for random generation
    
    int popSize = ; // population size
    int nbInput = ; // number of input (bias not included)
    int nbOutput = ;    // number of output
    int nbHiddenInit = ;    // number of hidden node at initialisation
    float probConnInit = ;  // probability of a connection being created at initialisation
    bool areRecurrentConnectionsAllowed = false; // are recurrent connections allowed ? True if yes, else false
    float weightExtremumInit = 100.0f;    // weight range for connections at innitalisation: each weight will be included in [-weightExtremumInit, weightExtremumInit]
    float speciationThreshInit = 20.0f;  // The speciation threshold will be initialised to this value
    int threshGensSinceImproved = 15; // Number of generation with no improvement allowed per species, after that concerned species will be swept away
    Population myPop(popSize, nbInput, nbOutput, nbHiddenInit, probConnInit, areRecurrentConnectionsAllowed, weightExtremumInit, speciationThreshInit, threshGensSinceImproved);
    
    
    // for runNetworkAuto
    int maxIterationThresh = ;  // Maximum number of iteration of processFn, depends on your processFn
    float fitnessOnMaxIteration = 0.0f; // Fitness to give if we reach the maximum number of iteration
    
    // for speciation
    int target = 5; // Number of species targeted
    int targetThresh = 0;   // Acceptance threshold
    float stepThresh = 0.5f;    // Evolution step of speciationThresh at each speciation
    float a = 1.0f; // Coefficient for processing the comparaison between two genomes
    float b = 1.0f; // Coefficient for processing the comparaison between two genomes
    float c = 0.4f; // Coefficient for processing the comparaison between two genomes
    
    // for crossover
    bool elitism = false;    // if at each generation the fitter genome is conserve true, else false
    
    // for mutate
    float mutateWeightThresh = 0.8f;    // probability of a connection's weight being mutated
    float mutateWeightFullChangeThresh = 0.1f;    // probability of a connection's weight being fully changed
    float mutateWeightFactor = 1.2f;    // factor applied to perturbate weight will be in [-mutateWeightFactor, mutateWeightFactor]
    float addConnectionThresh = 0.05f;  // probability of a connection being added
    int maxIterationsFindConnectionThresh = 20; // maximum number of iteration to found a valid connection to create
    float reactivateConnectionThresh = 0.25f;   // probability of a connection being re-enabled
    float addNodeThresh = 0.03f;    // probability of a node being added
    int maxIterationsFindNodeThresh = 20;   // maximum number of iteration to found a valid node to create
    
    while (myPop.generation < 500) {
        // Run the network (in this example, it will automatically run the network for each genome, with the same stupFn and the same arguments
        myPop.runNetworkAuto(processFn, &myArgs, setupFn, activationFn, maxIterationThresh, fitnessOnMaxIteration);
        
        // Speciate the genomes
        myPop.speciate(target, targetThresh, stepThresh, a, b, c);
        
        // Crossover the genomes to get new generation's genomes
        myPop.crossover(elitism);
        
        // Mutate the genomes
        myPop.mutate(mutateWeightThresh, mutateWeightFullChangeThresh, mutateWeightFactor, addConnectionThresh, maxIterationsFindConnectionThresh, reactivateConnectionThresh, addNodeThresh, maxIterationsFindNodeThresh);
    }
    
    return 0;
}
