#include <tokenizer.hpp>
#include "zzz_exception.hpp"

#include <string>
#include <cstddef> // Access to size_t
#include <fstream>
#include <cassert>

namespace zzz {
 


    Token::Token(size_t lowerIndex, size_t upperIndex, int32_t tokenDepth) {
        lowerIndex_ = lowerIndex;
        upperIndex_ = upperIndex;
        depth_ = tokenDepth;
    }
    

    Tokenizer::Tokenizer () {
        index_ = 0;          // Position of tokenizer in buffer text

        lineStartIndex_ = 0; // Index of start of line, used in error reporting
        line_ = 0;           // Current line of tokenizer

    }

    int32_t Tokenizer::readIndentation() {
        int32_t depth = 0;
        while(buffer_[index_] == ' ') {
            if (index_ >= buffer_.length()) return 0;
            // Throw error if level of indentation isn't multiple of 2
            if (buffer_[++index_] != ' ')
                throw indentation_error(line_, index_ + 1, "Odd Indentation");
            index_++;
            depth++;
        }
        return depth;
    }

    void Tokenizer::passWhiteSpace() {
        while (index_ < buffer_.length() && buffer_[index_] == ' ') index_++;
    }

    Token* Tokenizer::readOpenString(int32_t depth) {
        size_t lowerIndex = index_;
        size_t upperIndex = lowerIndex;
        while (true) {
            if (index_ >= buffer_.length()) {
                return new Token(lowerIndex, upperIndex+1, depth);
            }
            switch (buffer_[index_]) {
                case ' ': break;
                
                // Return Token based on lower and upper index
                // Increment index if regerster shift char as
                // The seperator is 2 long instead of 1

                case '\r': index_++;
                    // fall through
                case '\n':
                case '#':
                case ':':
                case ';':
                case ',': return new Token(lowerIndex, ++upperIndex, depth);

                default: upperIndex = index_; break;
            }

            index_++;
        }
    }

    Tokenizer::eTokenReturn Tokenizer::passEndOfString() {
        passWhiteSpace();
        if (index_ >= buffer_.length()) return eTokenReturn::SuccessfulReturn;
        switch (buffer_[index_]) {
            // Continue if proper seperator between nodes reached
            // Increment index if regerster shift char as
            // as the seperator is 2 character long including \n char

            case '\r': index_++;
                // fall through
            case '\n':
            case '#':
            case ':':
            case ';':
            case ',': return eTokenReturn::SuccessfulReturn;

            default: return eTokenReturn::UnexpectedChar; 
            
        }
    }

    Token* Tokenizer::readClosedString(int32_t depth) {
        index_++; // Move past opening quotes
        size_t lowerIndex = index_;
        size_t upperIndex = lowerIndex;

        // Throw end of file error if buffer end reached
        if (index_ >= buffer_.length())
            throw syntax_error(line_, index_-lineStartIndex_ + 1,
                                "Unexpected End Of File In Closed String");
        while (buffer_[index_] != '"') {

            if (buffer_[index_] == '\n' || buffer_[index_] == '\r')
                throw syntax_error(line_, index_-lineStartIndex_+1,
                                    "Unexpected EOL In Closed String");
            
            // Ignore double quote if proceeded by back slash
            if (buffer_[index_] == '\\' && buffer_[index_+1] == '"') index_++;
    
            // Throw end of file error if buffer end reached
            if (index_ >= buffer_.length())
                throw syntax_error(line_, index_-lineStartIndex_+1,
                                "Unexpected End Of File In Closed String");
            
            index_++;
        }
        upperIndex = index_;

        // Increment index to move past the multi-line closer
        // End of buffer check will take place in passEndOfString method
        index_++;

        // Clear white space until seperator character reached (; : , \r \n)
        // Throw error if not seperator character reached
        eTokenReturn tokenRet = passEndOfString();
        if (tokenRet == eTokenReturn::UnexpectedChar) {
            throw indentation_error(line_, index_-lineStartIndex_+1,
                    "Unexpected Character at End of Closed String");
        }

        return new Token(lowerIndex, upperIndex, depth);
    }

    Token* Tokenizer::readMultiLineString(int32_t depth) {
        size_t lowerIndex = index_;
        size_t upperIndex = lowerIndex;
        size_t openDepth = 0;
        size_t closeDepth = -1;

        index_++; // Pass opening bracket

        if (index_ >= buffer_.length())
            throw syntax_error(line_, index_-lineStartIndex_,
                    "Unexpected End Of Buffer In Multi-Line String");


        // Read multiline open as [===[ with openDepth being the number of '='
        while (buffer_[index_] != '[') {
            if (buffer_[index_] != '=') 
                throw syntax_error(line_, index_-lineStartIndex_+1, 
                "Unexpected Character in Multi-Line String Open: Expected '=' or '['");
            openDepth++;
            
            index_++;
            if (index_ >= buffer_.length())
                throw syntax_error(line_, index_-lineStartIndex_,
                        "Unexpected End Of File In Multi-Line String");
        }

        index_++; // Move past string open
        if (index_ >= buffer_.length())
            throw syntax_error(line_, index_-lineStartIndex_,
                    "Unexpected End Of File In Multi-Line String");

        // If starts with EOL, skip over it onto next line
        switch (buffer_[index_]) {
            case '\r': index_++;
                // fall through
            case '\n':
                index_++;
                line_++;
                lineStartIndex_ = index_;
        }

        // If starts with EOL, skip over it onto next line
        if (buffer_[index_] == '\n') index_ ++;
        else if (buffer_[index_] == '\r') index_ += 1;
        
        // Check if end of buffer reached
        if (index_ >= buffer_.length())
            throw syntax_error(line_, index_-lineStartIndex_,
                    "Unexpected End Of File In Multi-Line String");

        lowerIndex = index_;

        // Repeat loop until string close encountered with same depth as open
        while (openDepth != closeDepth) {

            
            if (buffer_[index_] == ']') {
                upperIndex = index_;
                closeDepth = 0;

                index_++;
                if (index_ >= buffer_.length())
                    throw syntax_error(line_, index_-lineStartIndex_,
                            "Unexpected End Of File In Multi-Line String");

                // Read in number of "="
                while (buffer_[index_] == '=') {
                    closeDepth++;
                    
                    index_++;
                    if (index_ >= buffer_.length())
                        throw syntax_error(line_, index_-lineStartIndex_,
                                "Unexpected End Of File In Multi-Line String");
                }


                // Prevent exiting loop if string was improperly closed
                if (buffer_[index_] != ']') closeDepth = -1;

                // Leave index without incrementing past current character
                // this is to ensure that if left on ']' it will still check
                // if character is start of the correct multiline close 
                // in the next iteration of the loop
            } else {
                
                index_++;
                if (index_ >= buffer_.length())
                    throw syntax_error(line_, index_-lineStartIndex_,
                            "Unexpected End Of File In Multi-Line String");
                
                if (buffer_[index_-1] == '\n') {
                    line_++;
                    lineStartIndex_ = index_;
                }
            }
        }

        // Increment index to move past the multi-line closer
        // End of buffer check will take place in passEndOfString method
        index_++;

        // Clear white space until seperator character reached (; : , \r \n)
        // Throw error if not seperator character reached
        eTokenReturn tokenRet = passEndOfString();
        if (tokenRet == eTokenReturn::UnexpectedChar) {
            throw syntax_error(line_, index_-lineStartIndex_,
                    "Unexpected Character at End of Multiline String");
        }

        return new Token(lowerIndex, upperIndex, depth);

    }

    void Tokenizer::passComment() {
        // Pass comment, leaving index at new line char
        while (index_ < buffer_.length() && buffer_[index_] != '\n') {
            index_++;
        }
    }



    Token* Tokenizer::readNode(int32_t depth) {
        passWhiteSpace();
        if (index_ >= buffer_.length()) return nullptr;
        switch (buffer_[index_]) {
            case '"': return readClosedString(depth);
            case '[': return readMultiLineString(depth);
            case '\r': // Fall through to '\n'
            case '\n': return nullptr;
            case '#': passComment(); return nullptr;
            case ':': // Fall through to ','
            case ',': return new Token(0, 0, depth);
            case ';':return nullptr;
            default: return readOpenString(depth);
        }
    }

    Tokenizer::eSeperator Tokenizer::interpretSeperator() {
        passWhiteSpace();
        switch (buffer_[index_]) {
            case '\r': index_++;
                // fall through
            case '\n': return eSeperator::EndOfLine;
            case '#': return eSeperator::commentSeperator;
            case ':': return eSeperator::ChildSeperator;
            case ',': return eSeperator::SiblingSeperator;
            case ';': return eSeperator::ParentSeperator;
            default: return eSeperator::UnknownCharacter;
        }
    }

    Tokenizer::eTokenReturn Tokenizer::tokenizeLine() {
        lineStartIndex_ = index_;
        int32_t depth = readIndentation() + 1;
        eSeperator nodeReturn = eSeperator::ChildSeperator;
        Token* returnToken = nullptr;
        while(nodeReturn != eSeperator::EndOfLine) {
            returnToken = readNode(depth);

            if (returnToken) tokenArray_.push_back(returnToken);
            else if (index_ >= buffer_.length()) return eTokenReturn::EndOfInput;
            
            nodeReturn = interpretSeperator();

            switch(nodeReturn) {
                case eSeperator::ChildSeperator: index_++; depth++; break;
                case eSeperator::ParentSeperator: index_++; depth--; break;
                case eSeperator::SiblingSeperator: index_++; break;
                default: break;
            }
        }
        index_++;
        if (index_ >= buffer_.length()) return eTokenReturn::EndOfInput;
        return eTokenReturn::SuccessfulReturn;
    }

    std::vector<Token*>* Tokenizer::tokenizeString(std::string inputText) {
        buffer_ = inputText;
        line_ = 1;
        lineStartIndex_ = index_;
        while (tokenizeLine() != eTokenReturn::EndOfInput) line_++;
        return &tokenArray_;
    }



} // Namesace zzz