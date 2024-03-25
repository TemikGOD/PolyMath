#pragma once
#include "../Containers/List.h"
#include "Monom.h"

class Polynomial : protected ad::List<Monom>
{
public:
	Polynomial();
	Polynomial(const Polynomial& other);

	//Polynomial& operator=(const Polynomial& other);
	Polynomial operator+(const Polynomial& other);
	Polynomial operator-(const Polynomial& other);
	Polynomial operator*(const Polynomial& other);
	Polynomial operator/(const Polynomial& other);

	Polynomial derivative(size_t indexOfVariable);
	void eraseZeros();
};

Polynomial::Polynomial()
{
	head = nullptr;
	tail = nullptr;
	size = 0;
}

Polynomial::Polynomial(const Polynomial& other)
{
	Node* temp = other.head;
	while (temp != nullptr)
	{
		push_back(temp->value);
		temp = temp->nextNode;
	}
}

Polynomial Polynomial::operator+(const Polynomial& other)
{
	Polynomial result;
	for (auto thisIter = begin(); thisIter != end(); ++thisIter)
		for (auto otherIter = other.begin(); otherIter != other.end(); ++otherIter)
			if ((*thisIter).similiar(*otherIter))
				result.push_back(*thisIter + *otherIter);
	MergeSort(&result.head);
	eraseZeros();
	return result;
}

Polynomial Polynomial::operator-(const Polynomial& other)
{
	Polynomial result;
	for (auto thisIter = begin(); thisIter != end(); ++thisIter)
		for (auto otherIter = other.begin(); otherIter != other.end(); ++otherIter)
			if ((*thisIter).similiar(*otherIter))
				result.push_back(*thisIter - *otherIter);
	MergeSort(&result.head);
	eraseZeros();
	return result;
}

Polynomial Polynomial::operator*(const Polynomial& other)
{
	Polynomial result;
	for (auto thisIter = begin(); thisIter != end(); ++thisIter)
		for (auto otherIter = other.begin(); otherIter != other.end(); ++otherIter)
			result.push_back(*thisIter * *otherIter);
	MergeSort(&result.head);
	eraseZeros();
	return result;
}

Polynomial Polynomial::operator/(const Polynomial& other)
{
	Polynomial result;
	for (auto thisIter = begin(); thisIter != end(); ++thisIter)
		for (auto otherIter = other.begin(); otherIter != other.end(); ++otherIter)
			result.push_back(*thisIter / *otherIter);
	MergeSort(&result.head);
	eraseZeros();
	return result;
}

Polynomial Polynomial::derivative(size_t indexOfVariable)
{
	Polynomial result;
	for (auto& elem : *this)
	{
		result.push_back(elem.derivative(indexOfVariable));
	}
	MergeSort(&result.head);
	eraseZeros();
	return result;
}

void Polynomial::eraseZeros()
{
	Node* current = tail;
	auto iter = end();
	--iter;
	while ((*iter).getCoefficient() == 0 && size > 1)
	{
		pop_back();
		--iter;
	}
}

