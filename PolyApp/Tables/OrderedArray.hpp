#include "../Containers/Vector.h"
#include "../Polynomial/Polynomial.h"
#include "MyExcepions.hpp"
#include "TableInterface.hpp"

class OrderedTable : public TableInterface{
private:
    ad::Vector<pair<string, Polynomial*>> OrdArray;

public:
    OrderedTable() {}

    virtual ~OrderedTable()
    {
        for(const auto &del: OrdArray)
            delete del.second;
        OrdArray.clear();
    }

    Polynomial getPol(int index) override { return *OrdArray[index].second; }
    string getKey(int index) override { return OrdArray[index].first; }

    size_t size() override { return OrdArray.size();}
    
    void addPolynomial(const std::string& _key, Polynomial* const _polynomial) override {
        // ищем позицию, на которую нужно вставить новый элемент, с помощью бинарного поиска
        int left = 0, right = OrdArray.size() - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (OrdArray[mid].first == _key) {
                throw AddException("A polynomial with this name is already in the table");
            }
            else if (OrdArray[mid].first < _key) {
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }
        // вставляем новый элемент на найденную позицию
        OrdArray.insert(OrdArray.begin() + left, { _key, _polynomial });
    }

    void deletePolynomial(const std::string& _key) override {
        int temp = -1;
        ad::Vector<pair<string, Polynomial*>> tempArray;
        if (OrdArray.size() == 0)
            throw EmptyTableException("Empty table");

        int left = 0, right = OrdArray.size() - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (OrdArray[mid].first == _key) {
                temp = mid;
                break;
            }
            else if (OrdArray[mid].first < _key) {
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }
        for (int i = 0; i < temp; i++) {
            tempArray.push_back(OrdArray[i]);
        }
        for (int i = temp+1; i < OrdArray.size(); i++) {
            tempArray.push_back(OrdArray[i]);
        }
        OrdArray = std::move(tempArray);
        if (temp == -1)
            throw NothingFoundException("No polynomial found with this key");
    }
    
    Polynomial searchPolynomial(const std::string& _key) override {
        int temp = -1;
        int left = 0, right = OrdArray.size() - 1;
        while (left <= right) {
            int mid = left + (right - left) / 2;
            if (OrdArray[mid].first == _key) {
                return *OrdArray[mid].second;
            }
            else if (OrdArray[mid].first < _key) {
                left = mid + 1;
            }
            else {
                right = mid - 1;
            }
        }
        if (temp == -1)
            throw NothingFoundException("There is no such key in the table");
    }
};
