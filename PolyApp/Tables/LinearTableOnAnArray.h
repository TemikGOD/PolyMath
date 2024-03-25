#pragma once
#include "TableInterface.hpp"
#include "../Containers/Vector.h"
#include "MyExcepions.hpp"

class LinearTableOnAnArray :public TableInterface
{
private:
	struct tableRow
	{
		std::string key;
		Polynomial* value;

        tableRow()
        {
            key = "";
            value = nullptr;
        }

		tableRow(const std::string _key, Polynomial* const _value)
		{
			key = _key;
			value = _value;
		}

        ~tableRow()
        {
            delete value;
        }

        tableRow& operator=(const tableRow& other)
        {
            if (this != &other) {
                key = other.key;
                delete value;
                if (other.value != nullptr) {
                    value = new Polynomial(*other.value);
                } else {
                    value = nullptr;
                }
            }
            return *this;
        }
	};

    ad::Vector<tableRow> table;

public:
    size_t size() override { return table.size(); }
	Polynomial operator[](size_t poz) { return *table[poz].value; }

    virtual ~LinearTableOnAnArray()
    {
        table.clear();
    }

    Polynomial getPol(int index) override { return *table[index].value; }
    string getKey(int index) override { return table[index].key; }

	Polynomial searchPolynomial(const std::string& _key) override
	{
        for (auto i = 0; i < table.size(); i++)
			if (table[i].key == _key)
				return *table[i].value;
        throw NothingFoundException("Nothing found");
	}

	void addPolynomial(const std::string& _key, Polynomial* const _pol) override
	{
		bool addedCheck = false;
		try
		{
			searchPolynomial(_key);
		}
        catch (NothingFoundException& exp)
		{
			table.push_back(tableRow(_key, _pol));
			addedCheck = true;
		}
		if (!addedCheck)
		{
            throw AddException("A polynomial with this name is already in the table");
		}
	}

	void deletePolynomial(const std::string& _key) override
	{
		if (this->size() == 0)
            throw EmptyTableException("There is nothing in the table");
		int poz = -1;
		try
		{	
			for (int i = 0; i < table.size(); i++)
                if (table[i].key == _key)
                {
					poz = i;
                    break;
                }
			if (poz == -1)
                throw NothingFoundException("Nothing found");
		}
        catch (const NothingFoundException&)
		{
            throw NoPolyException("There is no polynomial with this name in the table");
		}
		table[poz] = table[size() - 1];
		table.pop_back();
	}

};

