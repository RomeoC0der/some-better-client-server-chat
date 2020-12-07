#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <string>
#include <cassert>
#pragma comment (lib, "Ws2_32.lib")
class TcpHelper
{
public:
	TcpHelper() {
		
	};
	~TcpHelper() {};
	SOCKET& create_server();
	SOCKET& create_socket();
	bool connect_sock(SOCKET& sock);
	void init(const std::string& server_ip, const std::string& server_port);
private:
	addrinfo* info = nullptr;
	std::string ip;
	std::string port;
};

