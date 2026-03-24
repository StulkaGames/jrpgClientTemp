#include <iostream>
#include <string>
#include <thread>

#ifdef _WIN32
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
#define CLOSESOCKET closesocket
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
#endif

void receiveMessages(SOCKET sock) {
    char buffer[1024];
    while (true)
    {
        int bytes = recv(sock, buffer, sizeof(buffer) - 1, 0);
        if (bytes > 0)
        {
            buffer[bytes] = '\0';
            std::cout << "\n[Сервер]: " << buffer << "\n> ";
            std::cout.flush();
        }
        else
        {
            std::cout << "Отключено от сервера\n";
            break;
        }
    }
}

int main() 
{
#ifdef _WIN32
    system("chcp 65001");
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);
#endif

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(6699);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Ошибка подключения\n";
        return 1;
    }
    else
    {
        std::cout << "Подключён к серверу!\n";
    }
    
    std::thread recvThread(receiveMessages, sock);
    recvThread.detach();

    
    std::string msg;
    while (true)
    {
        std::cout << "> ";
        std::getline(std::cin, msg);
        if (msg == "exit") break;
        send(sock, msg.c_str(), msg.size(), 0);
    }

    CLOSESOCKET(sock);

#ifdef _WIN32
    WSACleanup();
#endif
    return 0;
}