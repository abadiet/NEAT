#ifndef POPULATION_HPP
#define POPULATION_HPP

#include "genome.hpp"
#include "species.hpp"
#include <cstring>
#include <fstream>
#include <iostream>
#include <limits>

class Population{
	private:
		vector<vector<int>> innovIds;
		int lastInnovId;

		int popSize;
		float speciationThresh;
		int threshGensSinceImproved;
		int nbInput;	// only useful for creating new genome
		int nbOutput;	// only useful for creating new genome
		int nbHiddenInit;	// only useful for creating new genome
		float probConnInit;	// only useful for creating new genome
		bool areRecurrentConnectionsAllowed;
		float weightExtremumInit;	// only useful for creating new genome

		float compareGenomes(int ig1, int ig2, float a, float b, float c);
		void updateFitnesses();
		int selectParent(int iSpe);
	public:
		int generation;
		float avgFitness;
		float avgFitnessAdjusted;
		int fitterGenomeId;
		vector<Genome> genomes;
		vector<Species> species;

		Population(int popSize, int nbInput, int nbOutput, int nbHiddenInit, float probConnInit, bool areRecurrentConnectionsAllowed = false, float weightExtremumInit = 20.0f, float speciationThreshInit = 100.0f, int threshGensSinceImproved = 15);
		Population(const string filepath) {load(filepath);};

		void loadInputs(float inputs[]);
		void loadInputs(float inputs[], int genomeId);
		void runNetwork(float activationFn(float input));
		void runNetwork(float activationFn(float input), int genomeId);
		void getOutputs(float outputs[], int genomeId);
		void runNetworkAuto(float processFn(float procInputs[], float procOutputs[], struct args* args), struct args* args, void initArgsInputs(float inputs[], struct args* args), float activationFn(float input), int maxIterationThresh, float fitnessOnMaxIteration = 0.0f);
		void runNetworkAuto(float processFn(float procInputs[], float procOutputs[], struct args* args), struct args* args, void initArgsInputs(float inputs[], struct args* args), float activationFn(float input), int maxIterationThresh, float fitnessOnMaxIteration, int genomeId);
		void setFitness(float fitness, int genomeId);
		void speciate(int target = 5, int targetThresh = 0, float stepThresh = 0.5f, float a = 1.0f, float b = 1.0f, float c = 0.4f);
		void crossover(bool elitism = false);	// git error#3
		void mutate(float mutateWeightThresh = 0.8f, float mutateWeightFullChangeThresh = 0.1f, float mutateWeightFactor = 1.2f, float addConnectionThresh = 0.05f, int maxIterationsFindConnectionThresh = 20, float reactivateConnectionThresh = 0.25f, float addNodeThresh = 0.03f, int maxIterationsFindNodeThresh = 20);
		void drawNetwork(int genomeId, sf::Vector2u windowSize = {1300, 800}, float dotsRadius = 6.5f);
		void printInfo(bool extendedGlobal = false, bool printSpecies = false, bool printGenomes = false, bool extendedGenomes = false);
		void save(const string filepath = "./neat_backup.txt");
		void load(const string filepath = "./neat_backup.txt");
};

#endif	// POPULATION_HPP
