#include "node.hpp"

Node::Node(int id, nodetype type, int layer): id(id), type(type), layer(layer) {
	sumInput = 0;
	sumOutput = 0;
}
