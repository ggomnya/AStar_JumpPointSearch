#pragma once
struct Node {
	int X;
	int Y;
	Node* pParent;
	double F;
	double G;
	double H;
};

static bool compare(Node* a, Node* b) {
	return a->F < b->F;
}