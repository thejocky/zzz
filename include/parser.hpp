#pragma once

#include "zzz_tree.hpp"
#include "tokenizer.hpp"
#include "zzz_exception.hpp"
#include "serialization.hpp"

#include <iostream>
#include <vector>
#include <string>


namespace zzz {


    class Parser {

        Node* rootNode_;
        Node* workingNode_;
        int32_t workingDepth_;
        Tokenizer* tokenizer_;

        public:

        // Generate parser with new root node
        // Uses base tokenizer
        Parser();

        // Generate parser with given root node
        // Uses base tokenizer
        Parser(Node* rootNode);

        // Generate parser with new root node
        // Uses TokenizerClass as tokenizer
        // (TokenizerClass must be of type Tokenizer or decendent)
        template <class TokenizerClass>
        Parser();

        // Generate parser with given root node
        // Uses TokenizerClass as tokenizer
        // (TokenizerClass must be of type Tokenizer or decendent)
        template <class TokenizerClass>
        Parser(Node* rootNode);

        // Get root node
        Node* getRoot() {return rootNode_;}
        // Set root node
        void setRoot(Node* root) {rootNode_ = root;}

        // Get the current working node
        Node* getWorkingNode() {return workingNode_;}


        // Parse through string untill empty or error occurs
        // Returns: Root node
        Node* parse(std::string inputString);

        // Parse stream into buffer then parse
        // Parses until string empty or error occurs
        // Returns: Root node
        Node* parse(std::istream& stream);

        // Parse cstring into string then parse
        // Parses until string empty or error occurs
        // Returns: Root node
        Node* parse(char* string, size_t length);

        // Load open file then read into string then parse
        // Parses until string finished or error occurs
        // Returns: Root node
        Node* parseFile(char* fileName);

    };


}