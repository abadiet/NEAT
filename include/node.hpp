#ifndef NODE_HPP
#define NODE_HPP

class Node{
	public:
		int id;
		int layer;
		float sumInput;
		float sumOutput;

		Node(int id, int layer);
		Node () {};

};

#endif	// NODE_HPP