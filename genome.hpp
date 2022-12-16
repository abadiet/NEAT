#pragma once

#include "node.cpp"
#include "connection.cpp"
#include <vector>
#include <SFML/Graphics.hpp>

using namespace std;

class Genome{
	private:
		float weightExtremumInit;
	
		int getInnovId(vector<vector<int>>* innovIds, int* lastInnovId, int inNodeId, int outNodeId);
		void mutateWeights(float mutateWeightFullChangeThresh, float mutateWeightFactor);
		bool addConnection(vector<vector<int>>* innovIds, int* lastInnovId, int maxIterationsFindConnectionThresh, bool areRecurrentConnectionsAllowed, float reactivateConnectionThresh);
		int isValidNewConnection(int inNodeId, int outNodeId, bool areRecurrentConnectionsAllowed);
		bool addNode(vector<vector<int>>* innovIds, int* lastInnovId, int maxIterationsFindNodeThresh, bool areRecurrentConnectionsAllowed);
		void updateLayersRec(int nodeId);
	public:
		int nbInput;
		int nbOutput;
		float fitness;
		int speciesId;
		
		vector<Node> nodes;
		vector<Connection> connections;
		
		Genome(int nbInput, int nbOutput, int nbHiddenInit, float probConnInit, vector<vector<int>>* innovIds, int* lastInnovId, float weightExtremumInit = 20.0f);
		void loadInputs(float inputs[]);
		void runNetwork(float activationFn(float input));
		void getOutputs(float outputs[]);
		void mutate(vector<vector<int>>* innovIds, int* lastInnovId, bool areRecurrentConnectionsAllowed = false, float mutateWeightThresh = 0.8f, float mutateWeightFullChangeThresh = 0.1f, float mutateWeightFactor = 1.2f, float addConnectionThresh = 0.05f, int maxIterationsFindConnectionThresh = 20, float reactivateConnectionThresh = 0.25f, float addNodeThresh = 0.03f, int maxIterationsFindNodeThresh = 20);
		void drawNetwork(sf::Vector2u windowSize = {1300, 800}, float dotsRadius = 6.5f);
};
