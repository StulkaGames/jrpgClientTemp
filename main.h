#pragma once

#include <iostream>
#include <string>
#include <thread>

#ifdef _WIN32
	#include <winsock2.h>
	#pragma comment(lib, "ws2_32.lib")
	#define CLOSESOCKET closesocket
	#include <windows.h>
	static void print_utf8(const std::string& utf8_str)
	{
		SetConsoleOutputCP(CP_UTF8);  // 65001
		std::cout << utf8_str;
	}
	WSADATA wsa;
#else
	#include <sys/types.h>
	#include <sys/socket.h>
	#include <netinet/in.h>
	#include <arpa/inet.h>
	#include <unistd.h>
	#define SOCKET int
	#define INVALID_SOCKET -1
	#define SOCKET_ERROR -1
	#define CLOSESOCKET close
	static void print_utf8(const std::string& utf8_str)
	{
		std::cout << utf8_str;  // Linux/macOS по умолчанию UTF-8
	}
#endif

	static void receiveMessages(SOCKET);
