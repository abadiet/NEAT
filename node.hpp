#pragma once

class Node{
	public:
		int id;
		int layer;
		float sumInput;
		float sumOutput;

		Node(int id, int layer);
		Node () {};

};
