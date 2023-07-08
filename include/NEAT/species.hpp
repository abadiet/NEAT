#ifndef SPECIES_HPP
#define SPECIES_HPP

#include <vector>

namespace neat {

class Species{
	public:
		int id;
		float avgFitness;
		float avgFitnessAdjusted;
		int allowedOffspring;
		float sumFitness;
		int gensSinceImproved;
		bool isDead;
		std::vector<int> members;

		Species(int id);
};

}

#endif	// SPECIES_HPP