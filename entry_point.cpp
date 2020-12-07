#include "TcpHelper.h"
#include <memory>
#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
const INT BUFFER_SIZE = 2048;
const SHORT SLEEP_TIME_MS = 25;
BOOL SERVER_STATE = true;
inline std::unique_ptr<TcpHelper>tcp(new TcpHelper());
inline std::vector<SOCKET>clients_list;
inline std::unordered_map<SOCKET, std::pair<int,char*>>map;
void entry_point_start() {
	std::string ip, port;
	std::cout << "enter the ip >> " << std::flush;
	std::cin >> ip;
	std::cout << "enter the port >> " << std::flush;
	std::cin >> port;
	tcp->init(ip, port);
}
void update_client_state( SOCKET s) {
	int is = map.count(s);
	int* bytes_recieved = nullptr;
	do {
		if (is <= 0)continue;	
		bytes_recieved = &map.find(s)->second.first;
		auto buff = &map.find(s)->second.second;
		ZeroMemory(*buff, BUFFER_SIZE);
		*bytes_recieved = 0;//reset
		*bytes_recieved = recv(s, *buff, BUFFER_SIZE, 0);
		std::cout << s << "" << *buff << std::endl;
		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME_MS + 1));
	} while (*bytes_recieved > 0 && SERVER_STATE);
	delete[] map.find(s)->second.second;
	map.erase(s);
	for (int  i =0; i < clients_list.size();i++)
	{
		if (clients_list[i] == s)clients_list.erase(clients_list.begin() + i);
	}
}
void handle_new_connection_thread(SOCKET server) {
	SOCKET client = 0;
	while (SERVER_STATE) {
		client = accept(server, nullptr, nullptr);
		if (client != SOCKET_ERROR && client != 0) {
			clients_list.emplace_back(client);		
			char* tmp = new  char[BUFFER_SIZE];
			std::pair<int, char*>data = std::pair<int, char*>(0, tmp);
			map.emplace(client, data);
			std::thread n_thread(update_client_state, client);
			n_thread.detach();
		}
	}
}
int main() {
	clients_list.reserve(25);
	entry_point_start();
	SOCKET server = tcp->create_server();
	std::thread hande_clients_thread(handle_new_connection_thread, server);
	hande_clients_thread.detach();
	while (SERVER_STATE) {
		for (SOCKET& client : clients_list)
		{
			std::pair<int, char*> tmp = map.find(client)->second;
			if (tmp.first > 1)
			{
				for (SOCKET& _client_ : clients_list)
				{
					send(_client_, tmp.second, tmp.first, 0);
				}
				break;
			}
		}

		std::this_thread::sleep_for(std::chrono::milliseconds(SLEEP_TIME_MS));
	}
	hande_clients_thread.join();
	return  0;
}