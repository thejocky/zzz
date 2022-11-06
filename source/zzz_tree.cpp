#include <zzz_tree.hpp>

#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <fstream>

namespace zzz {

    // Initialize Node methods ------------------------------------------------

    Node::Node(std::string string, Node* parent, Node* sibling) :
        _string(string),
        _parent(parent),
        _sibling(sibling)
    {}

    // Clear internal string
    void Node::clearString() {
        _string.clear();
    }

    // Append child to internal vector given pointer to child
    // Returns appended child
    Node* Node::appendChild(Node* child) {
        if (!_children.empty())
            _children.back()->setSibling(child);
        _children.push_back(child);
        return child;
    }
 
    // Delete child at given index, aswell as all decendants
    void Node::deleteChild(int index) {
        delete[] _children[index];
        _children.erase(_children.begin() + index);
        _children[index-1]->setSibling(_children[index]);
    }

    // Remove child from internal vector then return it
    Node* Node::popChild(int index) {
        Node* tmp = _children[index];
        delete[] _children[index];
        _children.erase(_children.begin() + index);
        _children.back()->setSibling(nullptr);
        return tmp;
    }
    
    // Returns pointer to nth child node with given name
    // Returns nullptr if no child found with given name
    Node* Node::child(std::string target, int n) {
        int count = 0; // Count of number of children found with target name
        for (auto itr = firstChild(); itr; itr = itr->sibling()) {
            if (itr->string() == target) {
                count++;
                if (count == n) return itr;
            }
        }
        return nullptr;
    }

    // Returns first child of node
    Node* Node::firstChild() {
        if (_children.empty()) return nullptr;
        return _children.front();
    }

    // Returns last child of node
    Node* Node::lastChild() {
        if (_children.empty()) return nullptr;
        return _children.back();
    }

   
    void printNode(Node* node, int depth) {
        for (int i = 0; i < depth; i++) std::cout << "  ";
        std::cout << "String: " << node->string() << "\n";
        for (int i = 0; i < node->childCount(); i++) {
            printNode(node->child(i), depth+1);
        }
    }



} // Namespace zzz