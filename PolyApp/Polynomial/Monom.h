#pragma once
#include "../Containers/Vector.h"

class Monom {
private:
	ad::Vector<double> degrees;
	double coefficient;

public:
	Monom();
	Monom(const Monom& other);
	Monom(const double _coefficient, const ad::Vector<double> _degrees = ad::Vector<double>(1, 0.0));

	ad::Vector<double> getDegrees() const;
	double getCoefficient() const;
	void setDegrees(ad::Vector<double> _degrees);
	void setCoefficient(double _coefficient);

    Monom& operator=(const Monom& other);
    Monom operator+(const Monom& other) const;
    Monom operator-(const Monom& other) const;
    Monom operator*(const Monom& other) const;
    Monom operator*(const double _const) const;
    Monom operator/(const Monom& other) const;
	Monom operator/(const double _const) const;
	bool operator>(const Monom& other) const;
	bool operator>=(const Monom& other) const;
	bool operator<(const Monom& other) const;
	bool operator<=(const Monom& other) const;
	bool operator==(const Monom& other) const;
	bool operator!=(const Monom& other) const;
    Monom derivative(const size_t indexOfVariable) const;
	bool similiar(const Monom& other) const;
};

Monom::Monom()
{
	coefficient = 0.0;
	degrees = ad::Vector<double>(1, 0.0);
}

Monom::Monom(const Monom& other)
{
	coefficient = other.coefficient;
	degrees = other.degrees;
}

Monom::Monom(const double _coefficient, const ad::Vector<double> _degrees)
{
	coefficient = _coefficient;
	degrees = _degrees;
}

ad::Vector<double> Monom::getDegrees() const
{
	return degrees;
}

double Monom::getCoefficient() const
{
	return coefficient;
}

void Monom::setDegrees(ad::Vector<double> _degrees)
{
	degrees = _degrees;
}

void Monom::setCoefficient(double _coefficient)
{
	coefficient = _coefficient;
}

Monom& Monom::operator=(const Monom& other)
{
	if (this != &other)
	{
		degrees = other.degrees;
		coefficient = other.coefficient;
	}
	return *this;
}

Monom Monom::operator+(const Monom& other) const
{
	return Monom(coefficient + other.coefficient, degrees);
}

Monom Monom::operator-(const Monom& other) const
{
	return Monom(coefficient - other.coefficient, degrees);
}

Monom Monom::operator*(const Monom& other) const
{
	ad::Vector<double> newDegrees(other.degrees.size());
	for (size_t i = 0; i < newDegrees.size(); i++)
		newDegrees[i] = degrees[i] + other.degrees[i];
	return Monom(coefficient * other.coefficient, newDegrees);
}

Monom Monom::operator*(const double _const) const
{
	return Monom(coefficient * _const, degrees);
}

Monom Monom::operator/(const Monom& other) const
{
	ad::Vector<double> newDegrees(other.degrees.size());
	for (size_t i = 0; i < newDegrees.size(); i++)
		newDegrees[i] = degrees[i] - other.degrees[i];
	return Monom(coefficient - other.coefficient, degrees);
}

Monom Monom::operator/(const double _const) const
{
	return Monom(coefficient / _const, degrees);
}

bool Monom::operator>(const Monom& other) const
{
	if (coefficient == 0 && other.coefficient != 0)
		return false;
	else
		if (coefficient != 0 && other.coefficient == 0)
			return true;
	int posOfDifferent = -1;
	for (size_t i = 0; i < degrees.size(); i++)
		if (degrees[i] != other.degrees[i])
		{
			posOfDifferent = i;
			break;
		}
	if (posOfDifferent > -1)
		return degrees[posOfDifferent] > other.degrees[posOfDifferent];
	else
		return coefficient > other.coefficient;
}

bool Monom::operator>=(const Monom& other) const
{
	if (coefficient == 0 && other.coefficient != 0)
		return false;
	else
		if (coefficient != 0 && other.coefficient == 0)
			return true;
	int posOfDifferent = -1;
	for (size_t i = 0; i < degrees.size(); i++)
		if (degrees[i] != other.degrees[i])
		{
			posOfDifferent = i;
			break;
		}
	if (posOfDifferent > -1)
		return degrees[posOfDifferent] >= other.degrees[posOfDifferent];
	else
		return coefficient >= other.coefficient;
}

bool Monom::operator<(const Monom& other) const
{
	if (coefficient == 0 && other.coefficient != 0)
		return true;
	else
		if (coefficient != 0 && other.coefficient == 0)
			return false;
	int posOfDifferent = -1;
	for (size_t i = 0; i < degrees.size(); i++)
		if (degrees[i] != other.degrees[i])
		{
			posOfDifferent = i;
			break;
		}
	if (posOfDifferent > -1)
		return degrees[posOfDifferent] < other.degrees[posOfDifferent];
	else
		return coefficient < other.coefficient;
}

bool Monom::operator<=(const Monom& other) const
{
	if (coefficient == 0 && other.coefficient != 0)
		return true;
	else
		if (coefficient != 0 && other.coefficient == 0)
			return false;
	int posOfDifferent = -1;
	for (size_t i = 0; i < degrees.size(); i++)
		if (degrees[i] != other.degrees[i])
		{
			posOfDifferent = i;
			break;
		}
	if (posOfDifferent > -1)
		return degrees[posOfDifferent] <= other.degrees[posOfDifferent];
	else
		return coefficient <= other.coefficient;
}

bool Monom::operator==(const Monom& other) const
{
	for (size_t i = 0; i < degrees.size(); i++)
		if (degrees[i] != other.degrees[i])
			return false;
	return coefficient == other.coefficient;
}

bool Monom::operator!=(const Monom& other) const
{
	int posOfDifferent = -1;
	for (size_t i = 0; i < degrees.size(); i++)
		if (degrees[i] != other.degrees[i])
			return true;
	return coefficient != other.coefficient;
}

Monom Monom::derivative(const size_t indexOfVariable) const
{
	if (degrees[indexOfVariable] == 0)
		if (coefficient == 0)
			return Monom();
		else
			return Monom(1);
	else
	{
		ad::Vector<double> newDegrees(degrees);
		newDegrees[indexOfVariable] -= 1.0;
		return Monom(coefficient * degrees[indexOfVariable], newDegrees);
	}
}

bool Monom::similiar(const Monom& other) const
{
	for (size_t i = 0; i < degrees.size(); i++)
		if (degrees[i] != other.degrees[i])
			return false;
	return true;
}
