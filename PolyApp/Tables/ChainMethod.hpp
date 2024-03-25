#pragma once
#include <vector>
#include <list>
#include <utility>
#include <string>
#include "TableInterface.hpp"
#include "MyExcepions.hpp"

using namespace std;

class HashTable : public TableInterface{
public:
    HashTable() : sizeT(100), table(sizeT) {}
    HashTable(int _size) : sizeT(_size), table(sizeT) {}

    virtual ~HashTable()
    {
        for(auto del: table)
        {
            del.clear();
        }
    }

    Polynomial getPol(int index) override { return *table[index].front().second; };
    string getKey(int index) override { return table[index].front().first; };

    size_t size() override {
        size_t count = 0;
        for(auto o: table)
            if(o.begin()->first != "")
                count++;
        return count;
    }

    void addPolynomial(const std::string& _key, Polynomial* const _polynomial) override {
        int index = hashFunction(_key);
        for (auto it = table[index].begin(); it != table[index].end(); it++) {
            if (it->first == _key) {
                it->second = _polynomial;
                return;
            }
        }
        table[index].push_back(std::make_pair(_key, _polynomial));
        count++;
        if (count > sizeT * loadFactor) {
            resize();
        }
    }

    void deletePolynomial(const std::string& _key) override {
        if (count == 0) {
            throw EmptyTableException("Cannot delete polynomial from an empty table.");
        }
        int index = hashFunction(_key);
        for (auto it = table[index].begin(); it != table[index].end(); it++) {
            if (it->first == _key) {
                table[index].erase(it);
                count--;
                return;
            }
        }
        throw NothingFoundException("Cannot delete polynomial with Key not found.");
    }

    Polynomial searchPolynomial(const std::string& _key) override {
        if (count == 0) {
            throw EmptyTableException("Cannot search for polynomial in an empty table.");
        }
        int index = hashFunction(_key);
        for (auto it = table[index].begin(); it != table[index].end(); it++) {
            if (it->first == _key) {
                if (it->second == nullptr) {
                    throw NoPolyException("Cannot return polynomial not found.");
                }
                return *it->second;
            }
        }
        throw NothingFoundException("Cannot return polynomial with Key not found.");
    }

private:
    const double loadFactor = 0.75;
    int sizeT;
    int count = 0;
    vector<list<pair<string, Polynomial*>>> table;

    int hashFunction(const string& _key) {
        int hash = 0;
        for (char c : _key) {
            hash = (hash * 31 + c) % sizeT;
        }
        return hash;
    }

    void resize() {
        sizeT *= 2;
        vector<list<pair<string, Polynomial*>>> newTable(sizeT);
        for (auto& bucket : table) {
            for (auto& item : bucket) {
                int index = hashFunction(item.first);
                newTable[index].push_back(item);
            }
        }
        table = newTable;
    }
};
