#include "main.h"


int main() 
{
#ifdef _WIN32
    system("chcp 65001");
#endif

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(6699);
    serverAddr.sin_addr.s_addr = inet_addr("188.233.81.96");

    if (connect(sock, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR)
    {
        std::cerr << "Ошибка подключения\n";
        return 1;
    }
    else
    {
        std::cout << "Подключен к серверу\n";
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

    return 0;
}


static void receiveMessages(SOCKET sock)
{
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