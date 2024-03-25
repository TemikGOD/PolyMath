#ifndef OpenHashTable_hpp
#define OpenHashTable_hpp

#include "TableInterface.hpp"
#include "MyExcepions.hpp"
#include "QtAlgorithms"

#include <vector>

struct PolyData
{
    string key;
    Polynomial* polynomial;

    ~PolyData()
    {
        delete polynomial;
    }
};

class OpenHashTable : public TableInterface
{
private:
    size_t sizeT;
    vector<vector<PolyData>> data;

    size_t iSearch(const string& key) const { return hash<string>{ }(key) % sizeT; }
public:

    explicit OpenHashTable(const size_t _size) : sizeT(_size), data(_size) {}

    Polynomial getPol(int index) override { return *data[index].front().polynomial; };
    string getKey(int index) override { return data[index].front().key; }

    virtual ~OpenHashTable()
    {
        for (auto del: data)
        {
            del.clear();
        }
    }
    
    size_t size() override {
        size_t count = 0;
        for(auto o: data)
            if(o.begin()->key != "")
                count++;
        return count;
    }

    void addPolynomial(const string& key, Polynomial* const polynomial) override
    {
        PolyData obj = { key, polynomial };
        size_t i = iSearch(key);

        if (!data[i].empty()) { throw AddException("Элемент с ключом уже существует"); }

        data[i].push_back(obj);
    }

    void deletePolynomial(const string& key) override
    {
        size_t i = iSearch(key);

        auto& vec = data[i];
        auto it = find_if(vec.begin(), vec.end(), [key](const PolyData& obj) { return obj.key == key; });

        if (it == vec.end()) { throw NoPolyException("Элемент с ключом не найден"); }

        vec.erase(it);
    }

    Polynomial searchPolynomial(const string& key) override
    {
        size_t i = iSearch(key);

        const auto& vec = data[i];
        auto it = find_if(vec.begin(), vec.end(), [key](const PolyData& obj) { return obj.key == key; });

        if (it != vec.end()) { return *(it->polynomial); }

        throw NothingFoundException("Элемент с ключом не найден");
    }
};

#endif /* OpenHashTable_hpp */
