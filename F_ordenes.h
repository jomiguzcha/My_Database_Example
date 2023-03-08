#pragma once
#include "Nodo.h"
#include "Tree_iterator.h"
#include <stack>
#include <queue>
#include <utility>

template <class T>
struct Inorden {
	void operator()(stack<pair<Nodo<T> *, short>>& a) {
		while ((a.top()).second != 1) {
			if ((a.top()).second == 0) {
				(a.top()).second = 1;
				if (((a.top()).first)->nodos[0] != NULL) {
					a.push(pair <Nodo<T> *, short>(((a.top()).first)->nodos[0], 0));
				}
			}
			else if ((a.top()).second == 2) {
				(a.top()).second = 3;
				if (((a.top()).first)->nodos[1] != NULL) {
					a.push(pair <Nodo<T> *, short>(((a.top()).first)->nodos[1], 0));
				}
			}
			else if ((a.top()).second == 3) {
				a.pop();
				if (a.empty()) {
					break;
				}
			}
		}
	}

	T operator *(stack<pair<Nodo<T> *, short>>& a){
		if ((a.top()).second == 1) {
			(a.top()).second = 2;
			return ((a.top()).first)->x;
		}
	}

};

template <class T>
struct Preorden {
	void operator()(stack<pair<Nodo<T> *, short>>& a) {
		while ((a.top()).second != 0) {
			if ((a.top()).second == 1) {
				(a.top()).second = 2;
				if (((a.top()).first)->nodos[0] != NULL) {
					a.push(pair <Nodo<T> *, short>(((a.top()).first)->nodos[0], 0));
				}
			}
			else if ((a.top()).second == 2) {
				(a.top()).second = 3;
				if (((a.top()).first)->nodos[1] != NULL) {
					a.push(pair <Nodo<T> *, short>(((a.top()).first)->nodos[1], 0));
				}
			}
			else if ((a.top()).second == 3) {
				a.pop();
				if (a.empty()) {
					break;
				}
			}
		}
	}

	T operator *(stack<pair<Nodo<T> *, short>>& a) {
		if ((a.top()).second == 0) {
			(a.top()).second = 1;
			return ((a.top()).first)->x;
		}
	}

};

template <class T>
struct Postorden {
	void operator()(stack<pair<Nodo<T> *, short>>& a) {
		while ((a.top()).second != 2) {
			if ((a.top()).second == 0) {
				(a.top()).second = 1;
				if (((a.top()).first)->nodos[0] != NULL) {
					a.push(pair <Nodo<T> *, short>(((a.top()).first)->nodos[0], 0));
				}
			}
			else if ((a.top()).second == 1) {
				(a.top()).second = 2;
				if (((a.top()).first)->nodos[1] != NULL) {
					a.push(pair <Nodo<T> *, short>(((a.top()).first)->nodos[1], 0));
				}
			}
			else if ((a.top()).second == 3) {
				a.pop();
				if (a.empty()) {
					break;
				}
			}
		}
	}

	T operator *(stack<pair<Nodo<T> *, short>>& a) {
		if ((a.top()).second == 2) {
			(a.top()).second = 3;
			return ((a.top()).first)->x;
		}
	}

};

template <class T>
struct Reverse {
	void operator()(stack<pair<Nodo<T> *, short>>& a) {
		while ((a.top()).second != 1) {
			if ((a.top()).second == 2) {
				(a.top()).second = 3;
				if (((a.top()).first)->nodos[0] != NULL) {
					a.push(pair <Nodo<T> *, short>(((a.top()).first)->nodos[0], 0));
				}
			}
			else if ((a.top()).second == 0) {
				(a.top()).second = 1;
				if (((a.top()).first)->nodos[1] != NULL) {
					a.push(pair <Nodo<T> *, short>(((a.top()).first)->nodos[1], 0));
				}
			}
			else if ((a.top()).second == 3) {
				a.pop();
				if (a.empty()) {
					break;
				}
			}
		}
	}

	T operator *(stack<pair<Nodo<T> *, short>>& a) {
		if ((a.top()).second == 1) {
			(a.top()).second = 2;
			return ((a.top()).first)->x;
		}
	}

};

template <class T>
struct Niveles {
	void operator()(queue<pair<Nodo<T> *, short>>& a) {
		if (((a.front()).first)->nodos[0] != 0) {
			a.push(pair<Nodo<T> *, short>(((a.front()).first)->nodos[0], 0));
		}
		if (((a.front()).first)->nodos[1] != 0) {
			a.push(pair<Nodo<T> *, short>(((a.front()).first)->nodos[1], 0));
		}
		a.pop();
	}

	T operator *(queue<pair<Nodo<T> *, short>>& a) {
		return ((a.front()).first)->x;
	}

};