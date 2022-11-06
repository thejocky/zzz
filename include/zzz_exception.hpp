#pragma once

#include <exception>
#include <iostream>
#include <vector>

namespace zzz
{


    // Error parsing zzz input
    class parser_error: public std::exception {
        uint32_t line_;
        uint32_t column_;
        const char* info_;


        public:

        parser_error(uint32_t line, uint32_t column, const char* info):
            line_(line),
            column_(column),
            info_(info)
        {}

        parser_error():
            line_(0),
            column_(0),
            info_(nullptr)
        {}

        uint32_t line() {return line_;}
        uint32_t column() {return column_;}
        const char* info() {return info_;}
        virtual const char* what() {
            return "ZZZ Parser Error";
        }
    };

    class indentation_error: public parser_error {
        using parser_error::parser_error; // Inherit constructors
        const char* what() {
            return "Indentaion Error";
        }
    };

    class syntax_error: public parser_error {
        using parser_error::parser_error; // Inherit constructors
        const char* what() {
            return "Syntax Error";
        }
    };

    

    // Error assembling zzz tree
    class assembling_error: public std::exception {
        std::string nodeString_;
        int32_t nodeDepth_;
        const char* info_;

        public:
        assembling_error(std::string nodeString, 
                int32_t nodeDepth, const char* info):
            nodeString_(nodeString),
            nodeDepth_(nodeDepth),
            info_(info)
        {}

        assembling_error():
            nodeDepth_(0),
            info_(nullptr)
        {}

        std::string nodeString() {return nodeString_;}
        int32_t nodeDepth() {return nodeDepth_;}
        const char* info() {return info_;}
        virtual const char* what() {
            return "Assembling Error";
        }
    };


    // Error in serialization or deserialization
    class serial_error: public std::exception {
        std::vector<char*>* path_;
        const char* info_;

        public:
        serial_error(std::vector<char*>* errorPath,
                const char* info) :
            path_(errorPath), info_(info)
        {}

        std::vector<char*>* path() {return path_;}
        const char* info() {return info_;}
        virtual const char* what() {
            return "Serial Error";
        }
    };

    class serialize_error: public serial_error {
        using serial_error::serial_error; // Inherit constructors
        const char* what() {
            return "Serialization Error";
        }
    };

    class deserialize_error: public serial_error {
        using serial_error::serial_error; // Inherit constructors
        const char* what() {
            return "Deserialization Error";
        }
    };



    // Container for 



    



    // Functions for reporting errors are left commented as for some reason
    // functions cause a linking error for being defined twice

    // Print error to stderr
    // line:column: what: info.
    void reportError(parser_error &error);


    // Print error to stderr
    // what: info:
    // String: nodeString
    // Depth: nodeDepth
    void reportError(assembling_error &error);



} // namespace zzz