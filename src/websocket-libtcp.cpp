#include "websocket-libtcp.h"

TCP::Socket::Socket()
{
}

TCP::Socket::Socket(std::string pWebAddress, int pPort)
{
    bool StartUp = InitSocket();

    if (StartUp)
    {
        bool CreateSock = CreateSocket(pWebAddress, pPort);

        if (CreateSock)
        {
            bool Connect = ConnectSocket();

            if (Connect)
            {
                IsConnected = true;
            }
        }
    }
}

bool TCP::Socket::InitSocket()
{
    int Result = WSAStartup(MAKEWORD(2, 2), &WsaData);

    if (Result != 0)
        return false;

    return true;
}

bool TCP::Socket::CreateSocket(std::string pWebAddress, int pPort)
{
    int Length = log10(pPort) + 2;
    char *PortInfo = new char[Length];
    _itoa_s(pPort, PortInfo, Length, 10);
    PortInfo[Length] = '\n';
    addrinfo AddressInfo;
    ZeroMemory(&AddressInfo, sizeof(AddressInfo));
    AddressInfo.ai_family = AF_UNSPEC;
    AddressInfo.ai_socktype = SOCK_STREAM;
    AddressInfo.ai_protocol = IPPROTO_TCP;
    int Result = getaddrinfo((PCSTR)&pWebAddress[0], (PCSTR)PortInfo, &AddressInfo, &this->Response);

    if (Result != 0)
    {
        WSACleanup();
        return false;
    }

    this->ConnectionSocket = socket(this->Response->ai_family, this->Response->ai_socktype, this->Response->ai_protocol);

    if (this->ConnectionSocket == INVALID_SOCKET)
    {
        freeaddrinfo(this->Response);
        WSACleanup();
        return false;
    }

    return true;
}

bool TCP::Socket::ConnectSocket()
{
    int Result = connect(this->ConnectionSocket, this->Response->ai_addr, this->Response->ai_addrlen);

    if (Result == SOCKET_ERROR)
    {
        closesocket(ConnectionSocket);
        ConnectionSocket = INVALID_SOCKET;
        WSACleanup();
        return false;
    }

    freeaddrinfo(Response);
    return true;
}

bool TCP::Socket::Send(std::string pSendBuffer)
{
    int Result = send(this->ConnectionSocket, &pSendBuffer[0], pSendBuffer.length(), 0);

    if (Result == SOCKET_ERROR)
    {
        closesocket(this->ConnectionSocket);
        WSACleanup();
        this->IsConnected = false;
        return false;
    }

    return true;
}

std::vector<char> TCP::Socket::Receive()
{
    int Result;
    std::vector<char> Buffer;
    do
    {
        int i = Buffer.size();
        Buffer.resize(Buffer.size() + 512);
        Result = recv(ConnectionSocket, &Buffer[i], 512, 0);
        Buffer.resize(i + Result + 1);

        if (Result < 0)
        {
            closesocket(ConnectionSocket);
            WSACleanup();
            IsConnected = false;
        }

    } while (Result == 512);

    return Buffer;
}

bool TCP::Socket::Disconnect()
{
    int Result = shutdown(this->ConnectionSocket, SD_SEND);

    if (Result == SOCKET_ERROR)
    {
        closesocket(this->ConnectionSocket);
        WSACleanup();
        this->IsConnected = false;
        return false;
    }

    closesocket(this->ConnectionSocket);
    WSACleanup();
    this->IsConnected = false;
    return true;
}

TCP::Socket::~Socket()
{
}