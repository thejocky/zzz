#pragma once

#include "zzz_exception.hpp"

#include <string>
#include <vector>

namespace zzz {

    class Token {
        size_t lowerIndex_;
        size_t upperIndex_;
        int32_t depth_;

        public:
        
        Token(size_t lowerIndex, size_t upperIndex, int32_t tokenDepth);

        size_t lowerIndex() {return lowerIndex_;}
        size_t upperIndex() {return upperIndex_;}
        int32_t depth() {return depth_;}
    };

    class Tokenizer {

        std::string buffer_;            // Buffer used store token string while reading stream
        size_t index_;            // Position of tokenizer in buffer text
        std::vector<Token*> tokenArray_; // Array of tokens filled by tokenizer

        size_t lineStartIndex_; // Index of start of line, used in error reporting
        size_t line_; // Current line of tokenizer


        enum class eTokenReturn {
            SuccessfulReturn,
            EndOfInput,
            UnexpectedChar,
        };

        enum class eSeperator {
            ChildSeperator,
            SiblingSeperator,
            ParentSeperator,
            EndOfLine,
            UnknownCharacter,
            EndOfInput,
            commentSeperator,
        };


        public:

        Tokenizer ();


        private:

        // Read in one character from buffer
        // Keeps track of line column, and buffers lines 
        char eat();

        // Appends token based on current upper and lower indexs to token array
        void appendToken();

        char* readOpenString(std::istream inputStream);

        Token nextToken();

        // Inturprets single character (won't progress buffer position)
        bool interpretChar(char character);



        int32_t readIndentation();

        void passWhiteSpace();
        Token* readOpenString(int32_t depth);
        eTokenReturn passEndOfString();
        Token* readClosedString(int32_t depth);
        Token* readMultiLineString(int32_t depth);
        void passComment();
        Token* readNode(int32_t depth);
        eSeperator interpretSeperator();
        eTokenReturn tokenizeLine();

        public:

        std::vector<Token*>* tokenizeString(std::string inputText);

        
        public:

        void setBuffer(std::string textBuffer) {buffer_ = textBuffer;}
        std::string getBuffer() {return buffer_;}

        std::vector<Token*>* getTokenVector() {return &tokenArray_;}
        void clearTokenVector() {tokenArray_.clear();}

        void tokenizeBuffer();

        

    };

} // Namesace zzz