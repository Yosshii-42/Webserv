#pragma once

class ConfigTokenizer;

#include "ConfigTokenizer.hpp"
#include "Token.hpp"
#include <iostream>
#include <map>
#include "data.hpp"


class ServerContext {
	private:
	 std::string value_;
         uint16_t listen_;
         std::string host_;
         std::vector<std::map<int, std::string> > errorPage_;
         size_t clientMaxBodySize_;
        //  std::vector<LocationContext> locations_;
	public:
	 ServerContext(const std::string& text);
         ~ServerContext();
         void setListen(const uint16_t port);
         void setHost(const std::string& host);
         void addMap(const int number, const std::string& fileName);
         void setClientMaxBodySize(const size_t size);
        //  void addLocation();
	 std::string getValue() const;
         uint16_t getListen() const;
         const std::string& getHost() const;
         const std::vector<std::map<int, std::string> >& getErrorPage() const;
         size_t getClientMaxBodySize() const;
        //  const LocationContext& getLocation(string& path) const;
};
