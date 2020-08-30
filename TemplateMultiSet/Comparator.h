#pragma once
#include "MultiSet.h"
#include "Node.h"
#include<cmath>

template <typename T>
class Comparator {
public:
	bool less(const T& x, const T& y) const {
		if (x < y)
			return true;
		else
			return false;
	}

	bool equal(T x, T y) {
		if (x == y)
			return true;
		else
			return false;
	}
	
	
};
//specializarea clasei Comparator pentru double, care sa considere doua valori egale daca partea zecimala este egala
bool Comparator<double>::equal(double x, double y) {
	double a, b, f1;
	a = modf(x, &f1);
	b = modf(y, &f1);
	if (a == b)
		return true;
	else
		return false;
}

