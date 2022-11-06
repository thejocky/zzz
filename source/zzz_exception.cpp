
#include <zzz_exception.hpp>


namespace zzz {

    // Print error to stderr
    // line:column: what: info.
    void reportError(parser_error &error) {       
        std::cerr << error.line() << ":" 
            << error.column() << ": "
            << "\033[1;31m" // Set color output to red and bold
            << error.what() << ": "
            << "\033[0m" // Return to normal text
            << error.info() << ".";
    }

    // Print error to stderr
    // what: info:
    // String: nodeString
    // Depth: nodeDepth
    void reportError(assembling_error &error) {
        std::cerr 
            << "\033[1;31m" // Set color output to red and bold
            << error.what() << ": "
            << "\033[0m" // Return to normal text
            << error.info() << ": "
            << "String: " << error.nodeString()
            << "Depth: " << error.nodeDepth();
    }

}