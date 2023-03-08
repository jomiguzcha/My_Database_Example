#pragma once
#include <vector>

template <class T>
class Nodo{
public:
	Nodo<T> * nodos[2];
	T x;
	vector<long long> pos;
	Nodo(T _x) {
		nodos[0] = 0;
		nodos[1] = 0;
		x = _x;
	};
};

