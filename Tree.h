#pragma once
#include "Nodo.h"
#include "F_ordenes.h"
#include "Tree_iterator.h"
#include <iostream>
#include <stack>
#include <vector>
#include <queue>
#include <map>

using namespace std;

template <class T,class O>
class Tree{
public:
	Tree() { root = 0; };
	~Tree() {};

	void Rep(Nodo<T>**& q, Nodo<T>** p);
	Nodo<T>* find(T x);
	bool find(T x, Nodo<T>** &p);
	bool insert(T x);
	bool remove(T x);
	void mostrar(Nodo<T> *);
	void inorden_r(Nodo<T> *);
	void inorden_x(Nodo<T> *);
	void print_endl();

	typedef Tree_iterator<T, Inorden<T>, stack<pair<Nodo<T> *, short>>> inorden_iterator;
	typedef Tree_iterator<T, Preorden<T>, stack<pair<Nodo<T> *, short>>> preorden_iterator;
	typedef Tree_iterator<T, Postorden<T>, stack<pair<Nodo<T> *, short>>> postorden_iterator;
	typedef Tree_iterator<T, Reverse<T>, stack<pair<Nodo<T> *, short>>> reverse_iterator;
	typedef Tree_iterator<T, Niveles<T>, queue<pair<Nodo<T> *, short>> > niv_iterator;

	inorden_iterator inorden_begin() {
		inorden_iterator i;
		i.pila_x.push(pair <Nodo<T> *, short>(root, 0));
		i++;
		return i;
	}

	inorden_iterator inorden_end() {
		return inorden_iterator();
	}

	preorden_iterator preorden_begin() {
		preorden_iterator i;
		i.pila_x.push(pair <Nodo<T> *, short>(root, 0));
		i++;
		return i;
	}

	preorden_iterator preorden_end() {
		return preorden_iterator();
	}

	postorden_iterator postorden_begin() {
		postorden_iterator i;
		i.pila_x.push(pair <Nodo<T> *, short>(root, 0));
		i++;
		return i;
	}

	postorden_iterator postorden_end() {
		return postorden_iterator();
	}

	reverse_iterator reverse_begin() {
		reverse_iterator i;
		i.pila_x.push(pair <Nodo<T> *, short>(root, 0));
		i++;
		return i;
	}

	reverse_iterator reverse_end() {
		return reverse_iterator();
	}

	niv_iterator niv_begin() {
		niv_iterator i;
		i.pila_x.push(pair <Nodo<T> *, short>(root, 0));
		return i;
	}

	niv_iterator niv_end() {
		return niv_iterator();
	}
	
	string nombre;
	string columna;
	string tabla;
	map<int, Nodo<T>*> listado;

	Nodo<T>* root;
	O o;
	stack<Nodo<T> *> pila;
	stack<pair<Nodo<T> *, short>> p_x;
	queue<Nodo<T> *> cola;
};

template<class T, class O>
inline void Tree<T, O>::Rep(Nodo<T>**& q, Nodo<T>** p){
	q = &((*p)->nodos[(*p)->nodos[0] == 0]);
	while ((*q)->nodos[0] || (*q)->nodos[1]) {
		q = &((*q)->nodos[(*q)->nodos[1] != 0]);
	}
}

template<class T, class O>
inline Nodo<T>* Tree<T, O>::find(T x){
	Nodo<T>** p = &root;
	while (*p && ((*p)->x != x)) {
		p = &((*p)->nodos[o((*p)->x, x)]);
	}

	if (!!*p) {
		return *p;
	}
	else {
		return NULL;
	}
	   	
}

template<class T, class O>
inline bool Tree<T, O>::find(T x, Nodo<T>**& p){
	p = &root;
	while (*p && ((*p)->x != x)) {
		p = &((*p)->nodos[o((*p)->x,x)]);
	}

	return !!*p;
}

template<class T, class O>
inline bool Tree<T, O>::insert(T x){
	Nodo<T>** p;
	if (find(x, p)) { return 0; }
	*p = new Nodo<T>(x);

	return 1;
}

template<class T, class O>
inline bool Tree<T, O>::remove(T x){
	Nodo<T>** p;
	if (!find(x, p)) { return 0; }
	if ((*p)->nodos[0] && (*p)->nodos[1]) {
		Nodo<T>** q;
		Rep(q, p);
		(*p)->x = (*q)->x;
		p = q;
	}

	Nodo<T>* t = *p;
	*p = (*p)->nodos[(*p)->nodos[0]==0];
	delete t;

	return 1;
}

template<class T, class O>
inline void Tree<T, O>::mostrar(Nodo<T> *y) {
	cola.push(y);
	while (!cola.empty()) {
		cout << (cola.front())->x << " ";
		if ((cola.front())->nodos[0] != 0) {
			cola.push((cola.front())->nodos[0]);
		}
		if ((cola.front())->nodos[1] != 0) {
			cola.push((cola.front())->nodos[1]);
		}
		cola.pop();
	}
	cout << endl;
}

template<class T, class O>
inline void Tree<T, O>::inorden_r(Nodo<T> *y){
	if (y==NULL) {	return;	}
	pila.push(y);
	inorden_r((pila.top())->nodos[0]);
	cout << (pila.top())->x << " ";
	inorden_r((pila.top())->nodos[1]);
	pila.pop();
}

template<class T, class O>
inline void Tree<T, O>::inorden_x(Nodo<T> *y){
	p_x.push(pair <Nodo<T> *, short> (y, 0));
	
	while (!p_x.empty()) {
		if ((p_x.top()).second == 0) {
			(p_x.top()).second = 1;
			if (((p_x.top()).first)->nodos[0] != NULL) {
				p_x.push(pair <Nodo<T> *, short> (((p_x.top()).first)->nodos[0], 0));
			}
		}
		else if ((p_x.top()).second == 1) {
			cout << ((p_x.top()).first)->x << " ";
			(p_x.top()).second = 2;
		}
		else if ((p_x.top()).second == 2) {
			(p_x.top()).second = 3;
			if (((p_x.top()).first)->nodos[1] != NULL) {
				p_x.push(pair <Nodo<T> *, short>(((p_x.top()).first)->nodos[1], 0));
			}
		}
		else if ((p_x.top()).second == 3) {
			p_x.pop();
		}
	}
}

template<class T, class O>
inline void Tree<T, O>::print_endl(){
	vector <Nodo<T>* > c;
	c.push_back(root);
	int len = 0; int hojas = 0;
	while (c.size() != 0) {
		len = c.size();
		for (int i = 0; i < len; i++) {
			if (c[i]) {
				if (c[i]->nodos[0] == 0 && c[i]->nodos[1] == 0) {
					hojas++;
				}
				cout << c[i]->x << "("<< c[i]->pos[0] <<") ";
				c.push_back(c[i]->nodos[0]);
				c.push_back(c[i]->nodos[1]);
			}
			else{
				cout << "- ";
			}
			
		}
		cout << endl;
		c.erase(c.begin(), c.begin() + len);
	}
	cout <<"Hojas: "<< hojas << endl;
}
	