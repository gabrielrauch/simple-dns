#include "dns_server.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <netinet/in.h>

DNS_Server::DNS_Server()
{
    socket_fd = -1;
}

DNS_Server::~DNS_Server()
{
    if (socket_fd != -1)
    {
        close(socket_fd);
    }
}

bool DNS_Server::Initialize(const std::string &ip, int port)
{
    socket_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (socket_fd == -1)
    {
        std::cerr << "erro ao criar o socket." << std::endl;
        return false;
    }

    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.s_addr = inet_addr(ip.c_str());

    if (bind(socket_fd, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        std::cerr << "erro ao associar o socket ao endereço." << std::endl;
        return false;
    }

    return true;
}

bool DNS_Server::LoadDNSRecords(const std::string &configFileName)
{
    std::ifstream configFile(configFileName);
    if (!configFile.is_open())
    {
        std::cerr << "erro ao abrir o arquivo de configuração: " << configFileName << std::endl;
        return false;
    }

    std::string line;
    while (std::getline(configFile, line))
    {
    }

    configFile.close();
    return true;
}

bool DNS_Server::Start()
{
    if (socket_fd == -1)
    {
        std::cerr << "O servidor não foi inicializado corretamente." << std::endl;
        return false;
    }

    while (true)
    {
        struct sockaddr_in client_address;
        socklen_t client_address_len = sizeof(client_address);
        char buffer[1024];

        ssize_t bytes_received = recvfrom(socket_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_address, &client_address_len);
    }

    return true;
}

void DNS_Server::WaitForShutdown()
{
}
