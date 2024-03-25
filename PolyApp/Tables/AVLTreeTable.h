
#ifndef AVLTREETABLE_H
#define AVLTREETABLE_H
#include "TableInterface.hpp"
#include <exception>
#include <functional>
#include "../Containers/Vector.h"
#include "MyExcepions.hpp"

struct TreeNode {
    std::string key;
    Polynomial* pol;
    int height;
    TreeNode* left;
    TreeNode* right;
};

class AVLTreeTable : public TableInterface
{
    public:
        AVLTreeTable();
        virtual ~AVLTreeTable();
        void addPolynomial(const std::string& key, Polynomial* const pol) override;
        void deletePolynomial(const std::string& key) override;
        Polynomial searchPolynomial(const std::string& key) override;
        Polynomial getPol(int index) override;
        string getKey(int index) override;
        size_t size() override;
    private:
        TreeNode* root;
        void inorderTraversal(TreeNode* node, ad::Vector<std::pair<std::string, Polynomial*>>& pairs);
        void visit(ad::Vector<std::pair<std::string, Polynomial*>>& _pairs, std::string key) const;
        size_t size(TreeNode* node) const;
        void clearTreeHelper(TreeNode*& treeptr);
        TreeNode* addPolynomial(TreeNode* _TreeNode, std::string _key, Polynomial* _pol);
        TreeNode* deletePolynomial(TreeNode* _TreeNode, std::string _key);
        TreeNode* findMin(TreeNode* _TreeNode);
        TreeNode* findMax(TreeNode* _TreeNode);
        int getHeight(TreeNode* _TreeNode);
        int getBalanceFactor(TreeNode* _TreeNode);
        void updateHeight(TreeNode* _TreeNode);
        TreeNode* rotateRight(TreeNode* _TreeNode);
        TreeNode* rotateLeft(TreeNode* _TreeNode);
        TreeNode* balance(TreeNode* _TreeNode);
};

AVLTreeTable::AVLTreeTable() {
        root = nullptr;
}

void AVLTreeTable::clearTreeHelper(TreeNode*& treeptr)
{
        if (treeptr != nullptr)
        {
            clearTreeHelper(treeptr->left);
            clearTreeHelper(treeptr->right);
            delete treeptr->pol;
            delete treeptr;
            treeptr = nullptr;
        }
}

size_t AVLTreeTable::size() {
        return size(root);
}

size_t AVLTreeTable::size(TreeNode* node) const {
        if (node == nullptr) {
            return 0;
        }
        return 1 + size(node->left) + size(node->right);
}

AVLTreeTable::~AVLTreeTable()
{
        clearTreeHelper(root);
}

void AVLTreeTable::visit(ad::Vector<std::pair<std::string, Polynomial*>>& _pairs, std::string key) const
{
        _pairs.push_back(std::make_pair(key, nullptr));
}

void AVLTreeTable::inorderTraversal(TreeNode* node, ad::Vector<std::pair<std::string, Polynomial*>>& pairs) {
        if (node != nullptr) {
            inorderTraversal(node->left, pairs);
            visit(pairs, node->key);
            inorderTraversal(node->right, pairs);
        }
}

Polynomial AVLTreeTable::getPol(int index) {
        if (index < 0 || index >= size()) {
            throw std::out_of_range("Index is out of range");
        }
        ad::Vector<std::pair<std::string, Polynomial*>> pairs;
        inorderTraversal(root, pairs);

        return *pairs[index].second;
}

std::string AVLTreeTable::getKey(int index) {
        if (index < 0 || index >= size()) {
            throw std::out_of_range("Index is out of range");
        }
        ad::Vector<std::pair<std::string, Polynomial*>> pairs;
        inorderTraversal(root, pairs);
        return pairs[index].first;
}

void AVLTreeTable::addPolynomial(const std::string& _key, Polynomial* const _pol) {
        root = addPolynomial(root, _key, _pol);
}

TreeNode* AVLTreeTable::addPolynomial(TreeNode* _TreeNode, std::string _key, Polynomial* _pol) {
        if (_TreeNode == nullptr) {
        _TreeNode = new TreeNode;
        _TreeNode->key = _key;
        _TreeNode->pol = _pol;
        _TreeNode->height = 1;
        _TreeNode->left = nullptr;
        _TreeNode->right = nullptr;
        return _TreeNode;
        }
        if (_key < _TreeNode->key) {
        _TreeNode->left = addPolynomial(_TreeNode->left, _key, _pol);
        }
        else if (_key > _TreeNode->key) {
        _TreeNode->right = addPolynomial(_TreeNode->right, _key, _pol);
        }
        else {
        _TreeNode->pol = _pol;
        return _TreeNode;
        }
        return balance(_TreeNode);
}

void AVLTreeTable::deletePolynomial(const std::string& _key) {
        root = deletePolynomial(root, _key);
}

TreeNode* AVLTreeTable::deletePolynomial(TreeNode* _TreeNode, std::string _key) {
        if (_TreeNode == nullptr) {
        return _TreeNode;
        }
        if (_key < _TreeNode->key) {
        _TreeNode->left = deletePolynomial(_TreeNode->left, _key);
        }
        else if (_key > _TreeNode->key) {
        _TreeNode->right = deletePolynomial(_TreeNode->right, _key);
        }
        else {
        TreeNode* left = _TreeNode->left;
        TreeNode* right = _TreeNode->right;
        delete _TreeNode;
        if (right == nullptr) {
            return left;
        }
        TreeNode* min = findMin(right);
        min->right = findMax(right);
        min->left = left;
        return balance(min);
        }
        return balance(_TreeNode);
}

Polynomial AVLTreeTable::searchPolynomial(const std::string& _key) {
        TreeNode* _TreeNode = root;
        while (_TreeNode != nullptr) {
        if (_key < _TreeNode->key) {
            _TreeNode = _TreeNode->left;
        }
        else if (_key > _TreeNode->key) {
            _TreeNode = _TreeNode->right;
        }
        else {
            return *_TreeNode->pol;
        }
        }
        throw NothingFoundException("Nothing found");
}

TreeNode* AVLTreeTable::findMin(TreeNode* _TreeNode) {
        while (_TreeNode->left != nullptr) {
        _TreeNode = _TreeNode->left;
        }
        return _TreeNode;
}

TreeNode* AVLTreeTable::findMax(TreeNode* _TreeNode) {
        while (_TreeNode->right != nullptr) {
        _TreeNode = _TreeNode->right;
        }
        return _TreeNode;
}

int AVLTreeTable::getHeight(TreeNode* _TreeNode) {
        if (_TreeNode == nullptr) {
        return 0;
        }
        return _TreeNode->height;
}

int AVLTreeTable::getBalanceFactor(TreeNode* _TreeNode) {
        return getHeight(_TreeNode->right) - getHeight(_TreeNode->left);
}

void AVLTreeTable::updateHeight(TreeNode* _TreeNode) {
        int leftHeight = getHeight(_TreeNode->left);
        int rightHeight = getHeight(_TreeNode->right);
        _TreeNode->height = (leftHeight > rightHeight ? leftHeight : rightHeight) + 1;
}

TreeNode* AVLTreeTable::rotateRight(TreeNode* _TreeNode) {
        TreeNode* left = _TreeNode->left;
        _TreeNode->left = left->right;
        left->right = _TreeNode;
        updateHeight(_TreeNode);
        updateHeight(left);
        return left;
}

TreeNode* AVLTreeTable::rotateLeft(TreeNode* _TreeNode) {
        TreeNode* right = _TreeNode->right;
        _TreeNode->right = right->left;
        right->left = _TreeNode;
        updateHeight(_TreeNode);
        updateHeight(right);
        return right;
}

TreeNode* AVLTreeTable::balance(TreeNode* _TreeNode) {
        updateHeight(_TreeNode);
        if (getBalanceFactor(_TreeNode) == 2) {
        if (getBalanceFactor(_TreeNode->right) < 0) {
            _TreeNode->right = rotateRight(_TreeNode->right);
        }
        return rotateLeft(_TreeNode);
        }
        if (getBalanceFactor(_TreeNode) == -2) {
        if (getBalanceFactor(_TreeNode->left) > 0) {
            _TreeNode->left = rotateLeft(_TreeNode->left);
        }
        return rotateRight(_TreeNode);
        }
        return _TreeNode;
}

#endif // AVLTREETABLE_H
