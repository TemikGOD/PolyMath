#pragma once
#include "../Polynomial/polynomial.h"
#include <string>

class TableInterface
{
public:
    virtual void addPolynomial(const std::string& _key, Polynomial* const _polynomial) = 0;
    virtual void deletePolynomial(const std::string& _key) = 0;
    virtual Polynomial searchPolynomial(const std::string& _key) = 0;
    virtual ~TableInterface() { };
    virtual Polynomial getPol(int index) = 0;
    virtual string getKey(int index) = 0;
    virtual size_t size() = 0;
};

