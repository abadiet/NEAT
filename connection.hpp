#pragma once

using namespace std;

class Connection{
	public:
		int innovId;
		int inNodeId;
		int outNodeId;
		float weight;
		bool enabled;
		bool isRecurrent;
	
		Connection(int innovId, int inNodeId, int outNodeId, float weight, bool enabled, bool isRecurrent);
};
