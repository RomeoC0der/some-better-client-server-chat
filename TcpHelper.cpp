#include "TcpHelper.h"

SOCKET& TcpHelper::create_server()
{
	SOCKET sock = create_socket();
	int b_result = bind(sock, info->ai_addr, info->ai_addrlen);
	if (b_result == SOCKET_ERROR)printf("Unable to bind a socket");
	freeaddrinfo(info);
	int rez = listen(sock, SOMAXCONN);
	if (rez == SOCKET_ERROR)printf("Unable to listen a socket");
	return sock;
}
SOCKET& TcpHelper::create_socket()
{
	SOCKET sock;
	struct addrinfo* result = NULL;
	struct addrinfo hints;
	ZeroMemory(&hints, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;//TCP
	hints.ai_protocol = IPPROTO_TCP;
	hints.ai_flags = AI_PASSIVE;
	int rez = getaddrinfo(this->ip.c_str(), this->port.c_str(), &hints, &result);
	assert(!rez);//rez must be  0
	sock = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	assert(sock && "Unable to create a socket");
	this->info = result;
	return sock;	
}
bool TcpHelper::connect_sock(SOCKET& sock)
{
	auto host_ = gethostbyname(this->ip.c_str());
	SOCKADDR_IN SockAddr;
	SockAddr.sin_port = htons(std::stoi(this->port));
	SockAddr.sin_family = AF_INET;
	SockAddr.sin_addr.s_addr = *((unsigned long*)host_->h_addr);
	int connect_code = connect(sock, (SOCKADDR*)(&SockAddr), sizeof(SockAddr));
	assert(!connect_code && "Unable to estabilish connection");//connect code MUST BE 0 IF SUCCESS
	return true;
}
void TcpHelper::init(const std::string& server_ip, const std::string& server_port)
{
	this->ip = server_ip;
	this->port = server_port;
	WSAData data;
	if (WSAStartup(MAKEWORD(2, 2), &data) == WSAVERNOTSUPPORTED)
	{
		MessageBox(NULL, "Unable to WSAStartup", "Ban", MB_OK);
		Sleep(1000);
		//make crash
		DestroyWindow(NULL);
	}
}
