#pragma once
#include "RequestParser.h"
#include "SimpleHTTPServer.h"
#include <fstream>
#include <sstream>

#define DEFAULT_LENGTH_BUFFER 51200

class MyHTTPServer :public SimpleHTTPServer
{
public:
	MyHTTPServer() :SimpleHTTPServer() {}

	void clientHandler(SOCKET clientSocket)
	{
		while (true)
		{
			int recvBufflen = DEFAULT_LENGTH_BUFFER;
			char* recvBuff = new char[recvBufflen];
			int bytesReceived = 0;

			bytesReceived = recv(clientSocket, recvBuff, recvBufflen, 0);
			if (bytesReceived <= 0)
			{
				closesocket(clientSocket);
				break;
			}

			RequestParser requestParser(recvBuff);
			std::string responseContent;

			if (requestParser.getMethod() == "GET")
			{
				responseContent = doGetRequest(requestParser);
			}
			else if (requestParser.getMethod() == "POST")
			{
				responseContent = doPostRequest(requestParser);
			}
			int byteSend = send(clientSocket, responseContent.c_str(), responseContent.size(), 0);

			delete[] recvBuff;
		}

	}

	std::string doGetRequest(RequestParser requestParser)
	{
		std::ostringstream oss;

		std::ifstream inFile("." + requestParser.getURL(), std::ios::binary);
		if (!inFile.fail())
		{
			oss << "HTTP/1.1 200 OK\r\n";
			oss << "Cache-control: no-cache, private \r\n";
			oss << "Connection: " << requestParser.getHeader("Connection") << "\r\n";
			oss << "Content-type: " << requestParser.getContentType(requestParser.getURL()) << "\r\n";

			std::string fileContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());

			// complete respone message
			oss << "Content-Length: " << fileContent.size() << "\r\n";
			oss << "\r\n";
			oss << fileContent;
		}
		else
		{
			inFile.open("./404.html");
			oss << "HTTP/1.1 404 Not Found\r\n";
			oss << "Cache-control: no-cache, private \r\n";
			oss << "Connection: " << requestParser.getHeader("Connection") << "\r\n";
			oss << "Content-type: text/html\r\n";

			std::string fileContent((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());

			// complete respone message
			oss << "Content-Length: " << fileContent.size() << "\r\n";
			oss << "\r\n";
			oss << fileContent;
		}

		return oss.str();
	}

	std::string doPostRequest(RequestParser requestParser)
	{
		std::stringstream ss(requestParser.getHeader("Content-Length"));
		size_t lenContentRequest = 0;
		ss >> lenContentRequest;

		// user have not special character
		std::string content = requestParser.getContent();
		size_t posStartUser = content.find("username=", 0) + strlen("username=");
		size_t posEndUser = content.find("&password=");
		size_t posStartPassword = posEndUser + strlen("&password=");

		std::string user = content.substr(posStartUser, posEndUser - posStartUser);
		std::string pass = content.substr(posStartPassword, lenContentRequest - posStartPassword);

		std::ostringstream oss;
		oss << "HTTP/1.1 302 Found\r\n";
		oss << "Connection: keep-alive\r\n";

		if (user == "admin" && pass == "admin")
		{
			
			oss << "Location: /info.html\r\n";
			oss << "\r\n";
		}
		else
		{
			oss << "Location: /404.html\r\n";
			oss << "\r\n";
		}

		return oss.str();
	}
};