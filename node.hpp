#pragma once

enum nodetype {
	INPUT,
	OUTPUT,
	HIDDEN
};

class Node{
	public:
		int id;
		nodetype type;
		int layer;
		float sumInput;
		float sumOutput;
	
		Node(int id, nodetype type, int layer);
		
};
