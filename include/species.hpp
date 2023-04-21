#ifndef SPECIES_HPP
#define SPECIES_HPP

#include <vector>

using namespace std;

class Species{
	public:
		int id;
		float avgFitness;
		float avgFitnessAdjusted;
		int allowedOffspring;
		float sumFitness;
		int gensSinceImproved;
		bool isDead;
		vector<int> members;

		Species(int id);
};

#endif	// SPECIES_HPP