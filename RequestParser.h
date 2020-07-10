#pragma once
#include <iostream>
#include <string>
#include <map>

class RequestParser
{
public:
	RequestParser();
	RequestParser(const char* request);
	~RequestParser();

	void reset();
	void setRequest(const char* request);
	std::string getVersion();
	std::string getMethod();
	std::string getURL();
	std::string getHeader(std::string headerFieldName);
	std::string getContent();
	std::string getContentType(std::string url);

private:
	std::string m_sMethod;
	std::string m_sURL;
	std::string m_sVersion;
	std::map<std::string, std::string> m_mHeader;
	std::string m_sContent;

private:
	void parser(const char* request);
};