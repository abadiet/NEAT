#include "population.hpp"

using namespace std;

Population::Population(int popSize, int nbInput, int nbOutput, int nbHiddenInit, float probConnInit, bool areRecurrentConnectionsAllowed, float weightExtremumInit, float speciationThreshInit, int threshGensSinceImproved): popSize(popSize), speciationThresh(speciationThreshInit), threshGensSinceImproved(threshGensSinceImproved), nbInput(nbInput), nbOutput(nbOutput), nbHiddenInit(nbHiddenInit), probConnInit(probConnInit), areRecurrentConnectionsAllowed(areRecurrentConnectionsAllowed), weightExtremumInit(weightExtremumInit) {
	generation = 0;
	lastInnovId = -1;
	fitterGenomeId = -1;
	for (int i = 0; i < popSize; i++) {
		genomes.push_back(Genome(nbInput, nbOutput, nbHiddenInit, probConnInit, &innovIds, &lastInnovId, weightExtremumInit));
	}
}

void Population::loadInputs(float inputs[]) {
	for (int i = 0; i < popSize; i++) {
		genomes[i].loadInputs(inputs);
	}
}

void Population::loadInputs(float inputs[], int genomeId) {
	genomes[genomeId].loadInputs(inputs);
}

void Population::runNetwork(float activationFn(float input)) {
	for (int i = 0; i < popSize; i++) {
		genomes[i].runNetwork(activationFn);
	}
}

void Population::runNetwork(float activationFn(float input), int genomeId) {
	genomes[genomeId].runNetwork(activationFn);
}

void Population::getOutputs(float outputs[], int genomeId) {
	return genomes[genomeId].getOutputs(outputs);
}

void Population::runNetworkAuto(float processFn(float procInputs[], float procOutputs[], struct args* args), struct args* args, void initArgsInputs(float inputs[], struct args* args), float activationFn(float input), int maxIterationThresh, float fitnessOnMaxIteration) {
	for (int i = 0; i < popSize; i++) {
		// init args and inputs
		float procOutputs[nbInput];
        initArgsInputs(procOutputs, args);
		
		float procInputs[nbOutput];
		float result = -1.0f;
		int iteration = 0;
		while (iteration < maxIterationThresh && result < 0.0f) {
			genomes[i].loadInputs(procOutputs);
			genomes[i].runNetwork(activationFn);
			genomes[i].getOutputs(procInputs);
			
			result = processFn(procInputs, procOutputs, args);
			
			iteration ++;
		}
		if (result < 0.0f) {
			genomes[i].fitness = fitnessOnMaxIteration;
		} else {
			genomes[i].fitness = result;
		}
	}
}

// void Population::runNetworkAuto(float processFn(), float activationFn(float inputs[]), int genomeId) {
// 	
// }


void Population::setFitness(float fitness, int genomeId) {
	genomes[genomeId].fitness = fitness;
}


void Population::speciate(int target, int targetThresh, float stepThresh, float a, float b, float c) {
	// reset species
	for (int i = 0; i < popSize; i++) {
		genomes[i].speciesId = -1;
	}
	
	// init species with leaders
	for (int iSpe = 0; iSpe < (int) species.size(); iSpe++) {
		if (!species[iSpe].isDead) {	// if the species is still alive
			int iMainGenome = species[iSpe].members[rand() % (int) species[iSpe].members.size()];	// select a random member to be the main genome of the species
			species[iSpe].members.clear();
			species[iSpe].members.push_back(iMainGenome);
			genomes[iMainGenome].speciesId = iSpe;
		}
	}
	
	// process the other genomes
	for (int genomeId = 0; genomeId < popSize; genomeId++) {
		if (genomes[genomeId].speciesId == -1) {	// if the genome not already belong to a species
			int speciesId = 0;
			while (speciesId < (int) species.size() && !(!species[speciesId].isDead && compareGenomes(species[speciesId].members[0], genomeId, a, b, c) < speciationThresh)) {
				speciesId ++;	// the genome cannot belong to this species, let's check the next one
			}
			if (speciesId == (int) species.size()) {	// no species found for the current genome, we have to create one new
				species.push_back(Species(speciesId));
			}
			species[speciesId].members.push_back(genomeId);
			genomes[genomeId].speciesId = speciesId;
		}
	}
	
	// check how many alive species we have
	int nbSpeciesAlive = 0;
	for (int iSpe = 0; iSpe < (int) species.size(); iSpe++) {
		if (!species[iSpe].isDead) {	// if the species is still alive
			nbSpeciesAlive ++;
		}
	}
	
	// update speciationThresh
	if (nbSpeciesAlive < target - targetThresh) {
		speciationThresh -= stepThresh;
	} else {
		if (nbSpeciesAlive > target + targetThresh) {
			speciationThresh += stepThresh;
		}
	}
	
	// update all the fitness
	updateFitnesses();
}

float Population::compareGenomes(int ig1, int ig2, float a, float b, float c) {
	int maxInnovId1 = 0;
	vector<int> connEnabled1;
	for (int i = 0; i < (int) genomes[ig1].connections.size(); i++) {
		if (genomes[ig1].connections[i].enabled) {
			connEnabled1.push_back(i);
			if (genomes[ig1].connections[i].innovId > maxInnovId1) {
				maxInnovId1 = genomes[ig1].connections[i].innovId;
			}
		}
	}
	
	int maxInnovId2 = 0;
	vector<int> connEnabled2;
	for (int i = 0; i < (int) genomes[ig2].connections.size(); i++) {
		if (genomes[ig2].connections[i].enabled) {
			connEnabled2.push_back(i);
			if (genomes[ig2].connections[i].innovId > maxInnovId2) {
				maxInnovId2 = genomes[ig2].connections[i].innovId;
			}
		}
	}
	
	int excessGenes = 0;
	int disjointGenes = 0;
	float sumDiffWeights = 0.0f;
	int nbCommonGenes = 0;
	
	for (int i1 = 0; i1 < (int) connEnabled1.size(); i1++) {
		if (genomes[ig1].connections[connEnabled1[i1]].innovId > maxInnovId2) {
			excessGenes += 1;
		} else {
			int i2 = 0;
			
			while (i2 < (int) connEnabled2.size() && genomes[ig2].connections[connEnabled2[i2]].innovId != genomes[ig1].connections[connEnabled1[i1]].innovId) {
				i2++;
			}
			if (i2 == (int) connEnabled2.size()) {
				disjointGenes += 1;
			} else {
				nbCommonGenes += 1;
				float diff = genomes[ig2].connections[connEnabled2[i2]].weight - genomes[ig1].connections[connEnabled1[i1]].weight;
				if (diff > 0) {
					sumDiffWeights += diff;
				} else {
					sumDiffWeights += -1 * diff;
				}
			}
		}
	}
	
	for (int i2 = 0; i2 < (int) connEnabled2.size(); i2++) {
		if (genomes[ig2].connections[connEnabled2[i2]].innovId > maxInnovId1) {
			excessGenes += 1;
		} else {
			int i1 = 0;
			while (i1 < (int) connEnabled1.size() && genomes[ig2].connections[connEnabled2[i2]].innovId != genomes[ig1].connections[connEnabled1[i1]].innovId) {
				i1++;
			}
			if (i1 == (int) connEnabled1.size()) {
				disjointGenes += 1;
			}
		}
	}
	
	if (nbCommonGenes > 0) {
		return (a * excessGenes + b * disjointGenes) / max((int) connEnabled1.size(), (int) connEnabled2.size()) + c * sumDiffWeights / nbCommonGenes;
	} else {
		return numeric_limits<float>::max();	// is there a better way?
	}
}

void Population::updateFitnesses() {
	fitterGenomeId = 0;
	avgFitness = 0;
	avgFitnessAdjusted = 0;
	for (int i = 0; i < popSize; i++) {
		avgFitness += genomes[i].fitness;
		
		if (genomes[i].fitness > genomes[fitterGenomeId].fitness) {
			fitterGenomeId = i;
		}
	}
	
	avgFitness /= popSize;
	
	for (int i = 0; i < (int) species.size(); i++) {
		if (!species[i].isDead) {
			species[i].sumFitness = 0;
			for (int j = 0; j < (int) species[i].members.size(); j++) {
				species[i].sumFitness += genomes[species[i].members[j]].fitness;
			}
			
			if (species[i].sumFitness / (int) species[i].members.size() > species[i].avgFitness) {	// the avgFitness of the species has increased
				species[i].gensSinceImproved  = 0;
			} else {
				species[i].gensSinceImproved += 1;
			}
			
			species[i].avgFitness = species[i].sumFitness / (int) species[i].members.size();
			species[i].avgFitnessAdjusted = species[i].avgFitness / (int) species[i].members.size();
			
			avgFitnessAdjusted += species[i].avgFitness;
		}
	}
	
	avgFitnessAdjusted /= popSize;
	
	for (int i = 0; i < (int) species.size(); i++) {
		if (!species[i].isDead) {
			if (species[i].gensSinceImproved < threshGensSinceImproved) {
				species[i].allowedOffspring = (int) ((int) species[i].members.size() * species[i].avgFitnessAdjusted / (avgFitnessAdjusted + numeric_limits<float>::min()));	// note that (int) 0.9f == 0.0f	// numeric_limits<float>::min() = minimum positive value of float
			} else {
				species[i].allowedOffspring = 0;
			}
		}
	}
}

void Population::crossover(bool elitism) {
	vector<Genome> newGenomes;
	
	if (elitism) {	// elitism mode on = we conserve during generations the fitter genome
		Genome newGenome(nbInput, nbOutput, nbHiddenInit, probConnInit, &innovIds, &lastInnovId, weightExtremumInit);
		newGenome.nodes = genomes[fitterGenomeId].nodes;
		newGenome.connections = genomes[fitterGenomeId].connections;
		newGenome.speciesId = genomes[fitterGenomeId].speciesId;
		newGenomes.push_back(newGenome);
	}
	
	for (int iSpe = 0; iSpe < (int) species.size() ; iSpe++) {
		for (int k = 0; k < species[iSpe].allowedOffspring; k++) {
			// choose pseudo-randomly two parents. Don't care if they're identical as the child will be mutated...
			int iParent1 = selectParent(iSpe);
			int iParent2 = selectParent(iSpe);
			
			// clone the fitter
			Genome newGenome(nbInput, nbOutput, nbHiddenInit, probConnInit, &innovIds, &lastInnovId, weightExtremumInit);
			int iMainParent;
			int iSecondParent;
			if (genomes[iParent1].fitness > genomes[iParent2].fitness) {
				iMainParent = iParent1;
				iSecondParent = iParent2;
			} else {
				iMainParent = iParent2;
				iSecondParent = iParent1;
			}
			newGenome.nodes = genomes[iMainParent].nodes;
			
			newGenome.connections = genomes[iMainParent].connections;
			newGenome.speciesId = iSpe;
			
			// connections shared by both of the parents must be randomly wheighted
			for (int iMainParentConn = 0; iMainParentConn < (int) genomes[iMainParent].connections.size(); iMainParentConn++) {
				for (int iSecondParentConn = 0; iSecondParentConn < (int) genomes[iSecondParent].connections.size(); iSecondParentConn++) {
					if (genomes[iMainParent].connections[iMainParentConn].innovId == genomes[iSecondParent].connections[iSecondParentConn].innovId) {
						if (rand() % 2 == 0) {	// 50 % of chance for each parent, newGenome already have the wheight of MainParent
							newGenome.connections[iMainParentConn].weight = genomes[iSecondParent].connections[iSecondParentConn].weight;
						}
					}
				}
			}
			
			newGenomes.push_back(newGenome);
		}
	}
	
	int previousSize = (int) newGenomes.size();
	// add genomes if some are missing
	for (int k = 0; k < popSize - previousSize; k++) {
		newGenomes.push_back(Genome(nbInput, nbOutput, nbHiddenInit, probConnInit, &innovIds, &lastInnovId, weightExtremumInit));
	}
	
	// or remove some genomes if there is too many genomes
	for (int k = 0; k < previousSize - popSize; k++) {
		newGenomes.pop_back();
	}
	
	genomes = newGenomes;
	
	// reset species members
	for (int i = 0; i < (int) species.size(); i++) {
		species[i].members.clear();
		species[i].isDead = true;
	}
	for (int i = 0; i < popSize; i++) {
		if (genomes[i].speciesId > -1) {
			species[genomes[i].speciesId].members.push_back(i);
			species[genomes[i].speciesId].isDead = false;	// empty species will stay to isDead = true
		}
	}
	
	fitterGenomeId = -1;	// avoid to missuse fitterGenomeId
	
	generation ++;
}

int Population::selectParent(int iSpe) {
	/* Chooses player from the population to return randomly(considering fitness). This works by randomly choosing a value between 0 and the sum of all the fitnesses then go through all the dots and add their fitness to a running sum and if that sum is greater than the random value generated that dot is chosen since players with a higher fitness function add more to the running sum then they have a higher chance of being chosen */
	// build a random threshold in [0, sumFitness)
	float randThresh = rand()/(double) (RAND_MAX);
	while (randThresh == 1.0f) {
		randThresh = rand()/(double) (RAND_MAX);
	}
	randThresh *= species[iSpe].sumFitness;
	
	float runningSum = 0.0f;
	
	for (int i = 0; i < (int) species[iSpe].members.size(); i++) {
		runningSum += genomes[species[iSpe].members[i]].fitness;
		if (runningSum > randThresh) {
			return species[iSpe].members[i];
		}
	}
	cout << "Error : don't find a parent during crossover." << endl;
	return -1;	// impossible
}

void Population::mutate(float mutateWeightThresh, float mutateWeightFullChangeThresh, float mutateWeightFactor, float addConnectionThresh, int maxIterationsFindConnectionThresh, float reactivateConnectionThresh, float addNodeThresh, int maxIterationsFindNodeThresh) {
	for (int i = 0; i < popSize; i++) {
		genomes[i].mutate(&innovIds, &lastInnovId, areRecurrentConnectionsAllowed, mutateWeightThresh, mutateWeightFullChangeThresh, mutateWeightFactor, addConnectionThresh, maxIterationsFindConnectionThresh, reactivateConnectionThresh, addNodeThresh, maxIterationsFindNodeThresh);
	}
}

void Population::drawNetwork(int genomeId, sf::Vector2u windowSize, float dotsRadius) {
	genomes[genomeId].drawNetwork(windowSize, dotsRadius);
}

void Population::printInfo(bool extendedGlobal, bool printSpecies, bool printGenomes, bool extendedGenomes) {
	cout << "GENERATION " << generation << endl;
	
	cout << "	" << "Global" << endl;
	cout << "	" << "	" << "Average fitness: " << avgFitness << endl;
	cout << "	" << "	" << "Average fitness (adjusted): " << avgFitnessAdjusted << endl;
	cout << "	" << "	" << "Best fitness: " << genomes[fitterGenomeId].fitness << endl;
	if (extendedGlobal) {
		cout << "	" << "	" << "Population size: " << popSize << endl;
		cout << "	" << "	" << "Number of inputs: " << nbInput << endl;
		cout << "	" << "	" << "Number of outputs: " << nbOutput << endl;
		cout << "	" << "	" << "Speciation threshold: " << speciationThresh << endl;
		cout << "	" << "	" << "Are recurrent connections allowed: ";
		if (areRecurrentConnectionsAllowed) {
			cout << "yes" << endl;
		} else {
			cout << "no" << endl;
		}
		cout << "	" << "	" << "When initializing a new genome" << endl;
		cout << "	" << "	" << "	" << "Number of hidden nodes: " << nbHiddenInit << endl;
		cout << "	" << "	" << "	" << "Proability of a connection to be created: " << probConnInit << endl;
		cout << "	" << "	" << "	" << "Weight bounds: " << weightExtremumInit << endl;
	
	}
	
	if (printSpecies) {
		cout << "	" << "Species [id, average fitness, average fitness (adjusted), number of allowed offspring(s), number of generations since improved, number of members, dead]" << endl;
		for (int i = 0; i < (int) species.size(); i++) {
			cout << "	" << "	" << i << "	" << species[i].avgFitness << "	" << species[i].avgFitnessAdjusted << "	" << species[i].allowedOffspring << "	" << species[i].gensSinceImproved << "	" << (int) species[i].members.size() << "	";
			if (species[i].isDead) {
				cout << "yes" << endl;
			} else {
				cout << "no" << endl;
			}
		}
	}
	
	if (printGenomes) {
		cout << "	" << "Genomes [id, fitness, id of the species]" << endl;
		for (int i = 0; i < popSize; i++) {
			cout << "	" << "	" << i << "	" << genomes[i].fitness << "	" << genomes[i].speciesId << endl;
			if (extendedGenomes) {
				for (int k = 0; k < (int) genomes[i].connections.size(); k++) {
					cout << "	" << "	" << "	" << genomes[i].connections[k].inNodeId << " -> " << genomes[i].connections[k].outNodeId << "	(W: " << genomes[i].connections[k].weight << ", Innov: " << genomes[i].connections[k].innovId << ")";
					if (genomes[i].connections[k].isRecurrent) {
						cout << " R ";
					}
					if (!genomes[i].connections[k].enabled) {
						cout << " D ";
					}
					cout << endl;
				}
			}
		}
	}
}
