#include <parser.hpp>

#include <fstream>
#include <vector>
#include <cstring>
#include <iostream>

std::vector<int> test;

namespace zzz {
 
    
    // Generate parser with new root node
    // Uses base tokenizer
    Parser::Parser() {
        rootNode_ = new Node(NULL, nullptr);
        workingNode_ = rootNode_;
        workingDepth_ = 0;
        tokenizer_ = new Tokenizer();

    }

    // Generate parser with given root node
    // Uses base tokenizer
    Parser::Parser(Node* rootNode) {
        rootNode_ = rootNode;
        workingNode_ = rootNode_;
        workingDepth_ = 0;
        tokenizer_ = new Tokenizer();
    }

    // Generate parser with new root node
    // Uses TokenizerClass as tokenizer
    // (TokenizerClass must be of type Tokenizer or decendent)
    template <class TokenizerClass>
    Parser::Parser() {
        // Make sure that Given tokenizer type is decendent of Tokenizer class
        static_assert(std::is_base_of<TokenizerClass, Tokenizer>::value,
                "Type T Must Inherit From Tokenizer Class"); 
        rootNode_ = new Node(NULL, nullptr);
        workingNode_ = rootNode_;
        workingDepth_ = 0;
        tokenizer_ = new TokenizerClass();
    }

    // Generate parser with given root node
    // Uses TokenizerClass as tokenizer
    // (TokenizerClass must be of type Tokenizer or decendent)
    template <class TokenizerClass>
    Parser::Parser(Node* rootNode) {
        // Make sure that Given tokenizer type is decendent of Tokenizer class
        static_assert(std::is_base_of<TokenizerClass, Tokenizer>::value,
                "Type TokenizerClass Must Inherit From Tokenizer Class"); 
        rootNode_ = rootNode;
        workingNode_ = rootNode_;
        workingDepth_ = 0;
        tokenizer_ = new TokenizerClass();
    }

   
    // Parse through string untill empty or error occurs
    // Returns: Root node
    Node* Parser::parse(std::string inputString) {
        std::vector<Token*>* tokens = tokenizer_->tokenizeString(inputString);
        bool isError = false;
        assembling_error error;

        std::string nodeString;
        
        for (auto token : *tokens) {
            std::string nodeString = inputString.substr(token->lowerIndex(), token->upperIndex()-token->lowerIndex());

            while (token->depth()-1 > workingDepth_) {
                
                if (workingNode_->childCount() == 0) {
                    if (!isError) throw assembling_error(nodeString,
                                    token->depth(), "Unexpected Depth");
                }
                
                workingNode_ = workingNode_->lastChild();
                workingDepth_++;
            }

            if (token->depth() < 1) throw assembling_error(nodeString,
                    token->depth(), "Non Positive Depth: Token Depth Must Be Atleast 1");
            while (token->depth() <= workingDepth_) {
                workingNode_ = workingNode_->parent();
                workingDepth_--;
            }

            workingNode_->appendChild(new Node(nodeString, workingNode_));
        }

        workingNode_ = rootNode_;
        return rootNode_;
    }

    // Parse stream until empty of error occurs
    Node* Parser::parse(std::istream& stream) {
        std::string inputString;
        char buffer[4096];
        while (stream.read(buffer, sizeof(buffer)))
            inputString.append(buffer, sizeof(buffer));
        inputString.append(buffer, stream.gcount());
        return parse(inputString);
    }

}



int main(int argc, char** argv) {
    // zzz::Node* root = new zzz::Node("", nullptr);
    // zzz::Parser parser(root);


    // if (argc > 1)  {

    //     std::ifstream stream;
    //     stream.open(argv[1], std::ifstream::in);

    //     try {
    //         parser.parse(stream);
    //         zzz::printNode(root);
    //     } catch (zzz::parser_error& e) {
    //         std::cerr << e.line() << ":" 
    //             << e.column() << ": "
    //             << "\033[1;31m" // Set color output to red and bold
    //             << e.what() << ": "
    //             << "\033[0m" // Return to normal text
    //             << e.info() << ".\n";
    //         return 1;
    //     } catch (zzz::assembling_error& e) {
    //         std::cerr 
    //             << "\033[1;31m" // Set color output to red and bold
    //             << e.what() << ": "
    //             << "\033[0m" // Return to normal text
    //             << e.info() << ": \n"
    //             << "String: " << e.nodeString() << "\n"
    //             << "Depth: " << e.nodeDepth() << "\n";
    //         return 1;
    //     }
    // }
    // return 0;


}
