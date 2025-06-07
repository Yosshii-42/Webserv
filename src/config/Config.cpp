#include "Config.hpp"

Config::Config(const std::string& filename)
    : tokenizer_(const_cast<std::string&>(filename)), parser_(tokenizer_) {
        // checkTree(); TODO
}

Config::~Config() {}

ConfigTokenizer& Config::getTokenizer() { return (this->tokenizer_); }

const ConfigParser& Config::getParser() const { return (this->parser_); }

void Config::printParser() {
	printServer(Config::getParser().getServer());
}

void Config::printServer(const std::vector<ServerContext>& server) {
	for (std::vector<ServerContext>::const_iterator it = server.begin(); it != server.end(); ++it) {
		std::cout << it->getValue() << std::endl;
		std::cout << " |- listen: " << (int)it->getListen() << std::endl;
		std::cout << " |- host: " << it->getHost() << std::endl;
		if (it->getClientMaxBodySize() > 0)
			std::cout << " |- client_max_body_size: " << it->getClientMaxBodySize() << std::endl;
		const std::vector<std::map<int, std::string> >& errorPages = it->getErrorPage();
		for (std::vector<std::map<int, std::string> >::const_iterator vecIt = errorPages.begin();
		vecIt != errorPages.end(); ++vecIt)
		{
			const std::map<int, std::string>& pageMap = *vecIt;
			for (std::map<int, std::string>::const_iterator mapIt = pageMap.begin(); mapIt != pageMap.end(); ++mapIt)
				std::cout << " |- error_page: " << mapIt->first << " -> " << mapIt->second << std::endl;
		}

	}
}

// void Config::printParser() {
//         printParserRecursion(Config::getParser().getRoot(), 0);
// }

// void Config::printParserRecursion(ConfigNode* node, int depth) {
//         if (!node) return;

//         for (int i = 0; i < depth * 2; ++i) std::cout << " ";
//         if (depth > 0) std::cout << "|- ";
//         std::cout << node->getKey();
//         for (std::vector<std::string>::iterator iter =
//                  node->getValues().begin();
//              iter != node->getValues().end(); ++iter)
//                 std::cout << " " << *iter;
//         std::cout << std::endl;
//         for (std::vector<ConfigNode*>::iterator it =
//                  node->getChildren().begin();
//              it != node->getChildren().end(); ++it)
//                 printParserRecursion(*it, depth + 1);
// }
