#ifndef linearListTable_hpp
#define linearListTable_hpp

#include "../Containers/List.h"
#include "../Polynomial/Polynomial.h"
#include "MyExcepions.hpp"
#include "TableInterface.hpp"
#include <string>

class LinearListTable : public TableInterface
{
private:
    ad::List<pair<string, Polynomial*>> items;
    
    bool isPolynomialInTable(const string& key)
    {
        if (items.empty()) { return false; }
        for (const auto& item : items) { if (item.first == key) { return true; } }
        return false;
    }
public:
    LinearListTable() {}

    virtual ~LinearListTable()
    {
        for (const auto &del: items)
            delete del.second;
    }

    size_t size() override { return items.getSize();}

    Polynomial getPol(int index) override 
    {
        auto iter = items.begin();
        size_t count = 0;
        while (count != index)
        {
            ++iter;
            ++count;
        }
        return *(*iter).second;
    }

    string getKey(int index) override
    {
        auto iter = items.begin();
        size_t count = 0;
        while (count != index)
        {
            ++iter;
            ++count;
        }
        return (*iter).first;
    }

    void addPolynomial(const string& key, Polynomial* polynomial) override
    {
        if (isPolynomialInTable(key)) { throw AddException("Polynomial already exists in table"); }
        items.push_back(std::make_pair(key, polynomial));
    }

    void deletePolynomial(const string& key) override
    {
        if (items.empty()) { throw EmptyTableException("Table is isEmpty"); }
        
        for (auto it = items.begin(); it != items.end(); ++it)
        {
            if ((*it).first == key) { items.pop_front(); return; }
        }
        throw NoPolyException("Polynomial not found in table");
    }

    Polynomial searchPolynomial(const string& key) override
    {
        for (const auto& item : items) { if (item.first == key) { return *(item.second); } }
        throw NothingFoundException("Polynomial not found in table");
    }
};

#endif /* linearListTable_hpp */
