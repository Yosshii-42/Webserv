#pragma once

class ConfigNode;
class ConfigTokenizer;
class ServerContext;

#include "ConfigTokenizer.hpp"
#include "Token.hpp"
#include "ServerContext.hpp"
#include <vector>

class ConfigParser {
       private:
        typedef void (ConfigParser::*funcPtr)(ServerContext& current, size_t* i);
    	funcPtr func_[4];
	int keyFlag_[16];
	std::vector<ServerContext> servers_;
	void makeVectorServer_();
	void updateDepth_(const std::string& token, const int lineNumber);
	void resetKeyFlag_(const int keyType);
	void addServer_(size_t* index);
	void setPort_(ServerContext& current, size_t* i);
	void setHost_(ServerContext& current, size_t* i);
	void setErrPage_(ServerContext& current, size_t* i);
	void setMaxBodySize_(ServerContext& current, size_t* i);
	// void addLocation_();
       public:
	explicit ConfigParser(ConfigTokenizer& tokenizer);
	~ConfigParser();
	std::vector<Token> tokens;
	// ConfigTokenizer tokens;
	const std::vector<ServerContext>& getServer() const;
	// static void deleteVectorSrever(std::vector<ServerContext>& servers);
	void throwErr(const std::string& str1, const std::string& str2, const int lineNumber);
};

// class ConfigParser {
//        public:
//         explicit ConfigParser(const ConfigTokenizer& tokenizer);
//         ~ConfigParser();

//         ConfigTokenizer tokens;
//         ConfigNode* getRoot() const;
//         static void deleteTree(ConfigNode* node);
//         void throwErr(const std::string& str1, const std::string& str2,
//                       const int number);

//        private:
//         int keyFlag_[16];
//         ConfigNode* layers_[5];

//         void makeConfTree_(const ConfigTokenizer& tokens);
//         void updateDepth_(const std::string& token, const int lineNumber);
//         void resetKeyFlag_(const int keyType);
//         void addChild_(const Token& token, ConfigNode*& current,
//                        ConfigNode* parent);
//         void setValue_(const Token& token, ConfigNode* node);
// };
