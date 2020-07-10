#include "RequestParser.h"
#include <sstream>

RequestParser::RequestParser()
{
	reset();
}

RequestParser::RequestParser(const char* request)
{
	parser(request);
}

RequestParser::~RequestParser()
{

}

void RequestParser::reset()
{
	m_sMethod = "";
	m_sURL = "";
	m_sVersion = "";
	m_mHeader.clear();
	m_sContent = "";
}

void RequestParser::setRequest(const char* request)
{
	parser(request);
}

std::string RequestParser::getVersion()
{
	return m_sVersion;
}

std::string RequestParser::getURL()
{
	return m_sURL;
}

std::string RequestParser::getMethod()
{
	return m_sMethod;
}

std::string RequestParser::getHeader(std::string headerFieldName)
{
	return m_mHeader[headerFieldName];
}

std::string RequestParser::getContent()
{
	return m_sContent;
}

std::string RequestParser::getContentType(std::string url)
{
	if (url.size() == 0)
		return "";

	int pos = 0;
	for (int i = url.size() - 1; i >= 0; i--)
		if (url[i] == '.')
			pos = i;

	std::string exten = url.substr(pos + 1, url.size() - 1);
	
	if (exten == "jpg")
		exten = "jpeg";

	if (exten == "html" || exten == "css")
		exten = "text/" + exten;
	else if (exten == "png" || exten == "bmp" || exten == "jpeg")
		exten = "image/" + exten;

		
	return exten;
}

void RequestParser::parser(const char* request)
{
	std::stringstream ss(request);
	std::string sRequest(request);

	std::getline(ss, m_sMethod, ' ');
	std::getline(ss, m_sURL, ' ');
	std::getline(ss, m_sVersion, '\r');

	size_t posStart = 0;
	size_t posEnd = 0;
	size_t posEndHeader = 0;
	while (true)
	{
		std::string headerLine = "";

		posStart = sRequest.find("\r\n", posStart) + 2;
		posEnd = sRequest.find("\r\n", posStart);
		headerLine = sRequest.substr(posStart, posEnd - posStart);

		if (headerLine == "")
		{
			posEndHeader = posEnd + 2;
			break;
		}

		std::string headerFieldName;
		std::string value;
		std::stringstream line(headerLine);
		std::getline(line, headerFieldName, ':');
		std::getline(line, value, ' '); //remove ' '
		std::getline(line, value, '\r');
		m_mHeader[headerFieldName] = value;
	}

	size_t lenContent = sRequest.size() - posEndHeader;
	m_sContent = sRequest.substr(posEndHeader, lenContent);
}
