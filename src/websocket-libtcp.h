#include "tcp_export.h"
#include <WinSock2.h>
#include <iostream>
#include <ws2tcpip.h>
#include <vector>

#pragma comment(lib, "Ws2_32.lib")

namespace TCP
{
    extern "C++" class TCP_EXPORT Socket
    {
    public:
        Socket();
        Socket(std::string pWebAddress, int pPort);
        bool Send(std::string pSendBuffer);
        std::vector<char> Receive();
        bool Disconnect();
        bool Connected();
        bool InitSocket();
        bool CreateSocket(std::string pWebAddress, int pPort);
        bool ConnectSocket();
        ~Socket();

    private:
        bool IsConnected = false;
        WSADATA WsaData;
        addrinfo *Response = NULL;
        SOCKET ConnectionSocket = INVALID_SOCKET;
    };
}