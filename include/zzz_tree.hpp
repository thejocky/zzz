#pragma once

#include <vector>
#include <string>

namespace zzz {

    




    class Node {
        std::string _string;
        std::vector<Node*> _children;
        Node* _parent;
        Node* _sibling;

        public:

        Node(std::string string, Node* parent, Node* sibling = nullptr);

        ~Node() {}

        // Free internal string and set pointer to NULL
        void clearString();

        // Set internal string pointer to given pointer (doesn't delete array)
        void setString(std::string string) {_string = string;}

        // Get internal string pointer
        std::string& string() {return _string;}

        // Set sibling of the node
        void setSibling(Node* sibling) {_sibling = sibling;}

        // Get sibling of node
        Node* sibling() {return _sibling;}

        // Append child to internal vector given pointer to child
        // Returns appended child
        Node* appendChild(Node* child);

        // Generate child node given child string, append child to internal vector
        // Returns appended child
        Node* appendChild(std::string childString)
            {return appendChild(new Node(childString, this));}

        // Delete child at given index, aswell as all decendants
        void deleteChild(int childIndex);

        // Remove child from internal vector then return it
        Node* popChild(int childIndex);

        // Returns number of children nodes
        int childCount() {return _children.size();}

        // Returns pointer to child node at given index
        Node* child(int index) {return _children[index];}

        // Returns pointer to nth child node with given name
        // Returns -1 if no child found with given name
        Node* child(std::string target, int n=1);

        // Returns first child of node
        Node* firstChild();

        // Returns last child of node
        Node* lastChild();

        // Returns pointer to parent node
        Node* parent() {return _parent;}
        

    };

    void printNode(Node* node, int depth = 0);

} // Namespace zzz