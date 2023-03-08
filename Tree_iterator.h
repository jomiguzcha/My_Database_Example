#pragma once
#include "Nodo.h"
#include "F_ordenes.h"
#include <iostream>
#include <utility>
#include <stack>
#include <queue>

using namespace std;

template<class T,class O,class S>
class Tree_iterator{
public:
	//stack<pair<Nodo<T> *, short>> pila_x;
	S pila_x;
	O o;

	void operator =(Tree_iterator<T,O,S> i) {
		pila_x = i.pila_x;
	}

	bool operator !=(Tree_iterator<T,O,S> i) {
		return pila_x.empty() != i.pila_x.empty();
	}
	
	Tree_iterator<T, O, S> operator ++(int){
		o(pila_x);
		return *this;
	}

	T operator *() {
		return o*(pila_x);
	}

};
