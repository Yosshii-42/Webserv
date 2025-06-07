#include "ConfigParser.hpp"

#include <stdexcept>

#include "ConfigTokenizer.hpp"
#include "Validator.hpp"

ConfigParser::ConfigParser(ConfigTokenizer& tokenizer)
    : tokens(tokenizer.getTokens()) {
	func_[0] = &ConfigParser::setPort_;
	func_[1] = &ConfigParser::setHost_;
	func_[2] = &ConfigParser::setErrPage_;
	func_[3] = &ConfigParser::setMaxBodySize_;
	// func_[3] = &ConfigParser::addtLocation_;
        std::fill(keyFlag_, keyFlag_ + 16, 0);
        makeVectorServer_();
}

ConfigParser::~ConfigParser() {}

void ConfigParser::makeVectorServer_() {
        for (size_t i = 0; i < this->tokens.size(); ++i) {
                TokenType type = tokens[i].getType();
                const std::string token = tokens[i].getText();
                // int depth = this->keyFlag_[BRACE];
                int lineNumber = tokens[i].getLineNumber();
                // if (Validator::checkSyntaxErr(tokens[i], depth) == false)
                //         throwErr(token, ": Syntax error: line ", lineNumber);

                if (type == BRACE)
                        updateDepth_(token, lineNumber);
                else if (type == SERVER)
			addServer_(&i);
		else
			continue ;
        }	
}

void ConfigParser::updateDepth_(const std::string& token, const int lineNumber) {
        std::string num = ConfigTokenizer::numberToStr((lineNumber));
        if (token == "{") {
                this->keyFlag_[BRACE]++;
        } else if (token == "}") {
                this->keyFlag_[BRACE]--;
                if (this->keyFlag_[BRACE] < 0)
                        throwErr(token, ":Config brace close error: line ",
                                 lineNumber);
        }
}

void ConfigParser::addServer_(size_t* i) {
	ServerContext current("server");
	(*i)++;
	for (; *i < this->tokens.size(); ++(*i)) {
		const std::string text = tokens[*i].getText();
		int type = tokens[*i].getType();
		int lineNum = tokens[*i].getLineNumber();
		std::cout << "string = " << text << ", type = " << type << std::endl;

		if (type == BRACE){
			updateDepth_(text, lineNum);
			if (this->keyFlag_[BRACE] == 0)
				break ;
		} else if (type >= LISTEN && type <= MAX_SIZE)
			(this->*func_[type])(current, i);
		else
			continue ;
	}
	this->servers_.push_back(current);
}

void ConfigParser::setPort_(ServerContext& current, size_t* i) {
	(*i)++;
	if (*i == this->tokens.size())
		throwErr("", "Syntax error :", tokens[*i].getLineNumber());
	std::string text = tokens[*i].getText();
	if (tokens[*i].getType() == VALUE && Validator::number(text, LISTEN) == true)
		current.setListen((uint16_t)atoi(text.c_str()));
	else{
		throwErr("", "Syntax error :", tokens[*i].getLineNumber());
	}
}

void ConfigParser::setHost_(ServerContext& current, size_t* i) {
	(*i)++;
	if (*i == this->tokens.size())
		throwErr("", "Syntax error :", tokens[*i].getLineNumber());	
	std::string text = tokens[*i].getText();
	if (tokens[*i].getType() == VALUE)
		current.setHost(text);
	else
		throwErr("", "Syntax error :", tokens[*i].getLineNumber());
}

void ConfigParser::setMaxBodySize_(ServerContext& current, size_t* i) {
	(*i)++;
	if (*i == this->tokens.size())
		throwErr("", "Syntax error :", tokens[*i].getLineNumber());
	std::string text = tokens[*i].getText();
	if (tokens[*i].getType() == VALUE && Validator::number(text, MAX_SIZE) == true)
		current.setClientMaxBodySize((size_t)atoi(text.c_str()));
	else
		throwErr("", "Syntax error :", tokens[*i].getLineNumber());
}

void ConfigParser::setErrPage_(ServerContext& current, size_t* i) {
	(*i)++;
	size_t size = this->tokens.size();
	if (*i == size)
		throwErr("", "Syntax error :", tokens[*i].getLineNumber());	
	std::string text = tokens[*i].getText();
	if (tokens[*i].getType() == VALUE && Validator::number(text, ERR_PAGE) == true) {
		(*i)++;
		if (*i == size)
			throwErr("", "Syntax error :", tokens[*i].getLineNumber());
		std::string pageName = tokens[*i].getText();
		current.addMap(atoi(text.c_str()), pageName);
	} else {
		throwErr("", "Syntax error :", tokens[*i].getLineNumber());
	}
}

const std::vector<ServerContext>& ConfigParser::getServer() const {
	return (this->servers_);
}


void ConfigParser::throwErr(const std::string& str1, const std::string& str2, const int lineNumber) {

        std::string num = ConfigTokenizer::numberToStr(lineNumber);
        // deleteVectorSrever(this->servers_);
        throw(std::runtime_error(str1 + str2 + num));
}

void ConfigParser::resetKeyFlag_(const int keyType) {
        this->keyFlag_[keyType] = 0;
}



// ConfigParser::ConfigParser(const ConfigTokenizer& tokenizer)
//     : tokens(tokenizer) {
//         std::fill(keyFlag_, keyFlag_ + 16, 0);
//         makeConfTree_(tokenizer);
// }

// ConfigParser::~ConfigParser() {
//         if (this->layers_[0]) {
//                 deleteTree(this->layers_[0]);
//                 this->layers_[0] = NULL;
//         }
// }

// ConfigNode* ConfigParser::getRoot() const { return (this->layers_[0]); }

// void ConfigParser::makeConfTree_(const ConfigTokenizer& parser) {
//         this->layers_[0] = new ConfigNode(Token("root", 99));
//         std::vector<Token> tokens = parser.getTokens();

//         for (size_t i = 0; i < tokens.size(); ++i) {
//                 TokenType type = tokens[i].getType();
//                 const std::string token = tokens[i].getText();
//                 int depth = this->keyFlag_[BRACE];
//                 int lineNumber = tokens[i].getLineNumber();
//                 if (Validator::checkSyntaxErr(tokens[i], depth) == false)
//                         throwErr(token, ": Syntax error: line ", lineNumber);

//                 if (type == BRACE)
//                         updateDepth_(token, lineNumber);
//                 else if (type >= SERVER && type <= RETURN)
//                         addChild_(tokens[i], layers_[depth + 1],
//                                   layers_[depth]);
//                 else if (i > 0 && tokens[i - 1].getType() == ERR_PAGE)
//                         addChild_(tokens[i], layers_[depth + 2],
//                                   layers_[depth + 1]);
//                 else if (this->keyFlag_[ERR_PAGE] == 1)
//                         setValue_(tokens[i], layers_[depth + 2]);
//                 else
//                         setValue_(tokens[i], layers_[depth + 1]);
//         }
// }

// void ConfigParser::updateDepth_(const std::string& token,
//                                 const int lineNumber) {
//         std::string num = ConfigTokenizer::numberToStr((lineNumber));
//         if (token == "{") {
//                 this->keyFlag_[BRACE]++;
//         } else if (token == "}") {
//                 this->keyFlag_[BRACE]--;
//                 if (this->keyFlag_[BRACE] < 0)
//                         throwErr(token, ":Config brace close error: line ",
//                                  lineNumber);
//                 if (this->keyFlag_[BRACE] == 0 && keyFlag_[LOCATION] == 0)
//                         throwErr(token, ":Config location error: line ",
//                                  lineNumber);
//                 if (this->keyFlag_[BRACE] == 0) resetKeyFlag_(LOCATION);
//                 if (this->keyFlag_[BRACE] == 0) resetKeyFlag_(SERVER);
//         }
// }

// void ConfigParser::resetKeyFlag_(const int keyType) {
//         this->keyFlag_[keyType] = 0;
// }

// void ConfigParser::addChild_(const Token& token, ConfigNode*& current,
//                              ConfigNode* parent) {
//         current = new ConfigNode(token);
//         parent->getChildren().push_back(current);
//         int keyType = token.getType();
//         this->keyFlag_[keyType]++;
// }

// void ConfigParser::setValue_(const Token& token, ConfigNode* node) {
//         std::string text = token.getText();
//         Validator::checkSyntaxErr(token, this->keyFlag_[BRACE]);

//         if (text.size() == 1 && text[0] == ';')
//                 throwErr(text, ": Can't find value: line ",
//                          token.getLineNumber());
//         if (text[text.size() - 1] == ';') {
//                 text = text.substr(0, text.size() - 1);
//                 if (this->keyFlag_[ERR_PAGE] == 1) resetKeyFlag_(ERR_PAGE);
//         }
//         node->getValues().push_back(text);
// }

// void ConfigParser::deleteTree(ConfigNode* node) {
//         if (!node) return;
//         for (std::vector<ConfigNode*>::iterator it =
//                  node->getChildren().begin();
//              it != node->getChildren().end(); ++it)
//                 deleteTree(*it);
//         delete (node);
//         node = NULL;
// }

// void ConfigParser::throwErr(const std::string& str1, const std::string& str2,
//                             const int number) {
//         std::string num = ConfigTokenizer::numberToStr(number);
//         deleteTree(this->layers_[0]);
//         throw(std::runtime_error(str1 + str2 + num));
// }
