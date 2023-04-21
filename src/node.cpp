#include "node.hpp"

Node::Node(int id, int layer): id(id), layer(layer) {
	sumInput = 0;
	sumOutput = 0;
}
