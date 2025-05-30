#pragma once

class ConfigNode;
class ConfigParser;

#include "ConfigParser.hpp"
#include "Token.hpp"

class ConfigTree {
       public:
        explicit ConfigTree(const ConfigParser& parser);
        ~ConfigTree();

        ConfigParser parser;
        ConfigNode* getRoot() const;

       private:
        int keyFlag_[16];
        ConfigNode* root_;
        ConfigNode* layers_[5];

        void makeConfTree_(const ConfigParser& parser);
        void updateDepth_(const std::string& token, const int lineNumber);
        void resetKeyFlag_(const int keyType);
        void addChild_(const Token& token, ConfigNode*& current,
                       ConfigNode* parent);
        void setValue_(const Token& token, ConfigNode* node);
        void errExit_(const std::string& str1, const std::string& str2,
                      const int number);
        void deleteTree_(ConfigNode* node);
};
