#pragma once
#include<iostream>
using namespace std;
#include "Comparator.h"
#include"MultiSet.h"

#pragma once
#include <iostream>
using namespace std;
enum Color { RED, BLACK };

template <typename T, typename Comparator> class MultiSet;



template <typename T>
class Node {
	T info;
	Node<T>* left;
	Node<T>* right;
	Node<T>* parent;
	bool color;
	int nr;// numara aparitiile elementului
	

public:
	Node(const T&, Node<T>* = NULL, Node<T>* = NULL, Node<T>* = NULL, bool c = RED, int nr=0);
	Node();

	
	T getInfo() const;
	int getAparitions();
	void increaseNr();
	void decreaseNr();
	Node<T>* getLeft() const;
	Node<T>* getRight() const;
	
	

	friend class MultiSet<T, Comparator<T>>;
};

template<typename T>
Node<T>::Node(const T& t, Node<T>* l, Node<T>* r, Node<T>* p, bool ch, int nrn) : info(t), left(l), right(r), parent(p), color(ch), nr(nrn) {}

template<typename T>
Node<T>::Node() {
	this->info = NULL;
	this->left = NULL;
	this->right = NULL;
	this->parent = NULL;
	this->color = RED;
	this->nr = 0;

}
template<typename T>
T Node<T>::getInfo() const {
	return this->info;
}

template<typename T>
Node<T>* Node<T>::getLeft() const {
	return this->left;
}

template<typename T>
Node<T>* Node<T>::getRight() const {
	return this->right;
}
template<typename T>
int Node<T>::getAparitions() {
	return nr;
}

template<typename T>
void Node<T>::increaseNr() {
	nr++;
}
template<typename T>
void Node<T>::decreaseNr() {
	nr--;
}