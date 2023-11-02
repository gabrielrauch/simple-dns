#include "dns_server.h"
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <cstring>
#include <arpa/inet.h>
#include <netdb.h>
#include <mutex>
#include <netinet/in.h>
#include <condition_variable>
#include <regex>

DNS_Server::DNS_Server()
{
    socket_fd = -1;
    validDomainPattern = std::regex("^[a-zA-Z0-9][a-zA-Z0-9-]{1,61}[a-zA-Z0-9]\\.[a-zA-Z]{2,}$");
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
        std::string domain;
        std::string ip;

        std::istringstream lineStream(line);
        lineStream >> domain >> ip;

        if (!domain.empty() && !ip.empty())
        {
            AddToCache(domain, ip);
        }
    }

    configFile.close();
    return true;
}

bool DNS_Server::IsValidDomain(const std::string &domain)
{
    return std::regex_match(domain, validDomainPattern);
}

bool DNS_Server::IsQuerySizeValid(const ssize_t querySize)
{
    return querySize < 1024;
}

std::string DNS_Server::LookupInCache(const std::string &domain)
{
    std::lock_guard<std::mutex> lock(cacheMutex);
    auto it = dnsCache.find(domain);
    if (it != dnsCache.end())
    {
        return it->second;
    }
    return "";
}

void DNS_Server::AddToCache(const std::string &domain, const std::string &ip)
{
    std::lock_guard<std::mutex> lock(cacheMutex);
    dnsCache[domain] = ip;
}

std::string DNS_Server::ProcessDNSQuery(const std::string &query)
{
    std::string domain;
    std::string recordType;

    size_t spacePos = query.find(" ");
    if (spacePos != std::string::npos)
    {
        domain = query.substr(0, spacePos);
        recordType = query.substr(spacePos + 1);
    }
    else
    {
        return "";
    }

    if (recordType == "A")
    {
        struct addrinfo hints;
        struct addrinfo *result, *rp;
        int ret;

        memset(&hints, 0, sizeof(struct addrinfo));
        hints.ai_family = AF_INET; // IPv4
        hints.ai_socktype = SOCK_STREAM;

        ret = getaddrinfo(domain.c_str(), NULL, &hints, &result);
        if (ret == 0)
        {
            for (rp = result; rp != NULL; rp = rp->ai_next)
            {
                struct sockaddr_in *addr = (struct sockaddr_in *)rp->ai_addr;
                std::string ip = inet_ntoa(addr->sin_addr);

                freeaddrinfo(result);

                return ip;
            }
        }
    }

    return "";
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

        if (!IsQuerySizeValid(bytes_received))
        {
            std::cerr << "Tamanho da query excedido, ignorando..." << std::endl;
            continue;
        }

        std::string query = std::string(buffer, bytes_received);
        if (!IsValidDomain(query))
        {
            std::cerr << "Query inválida, ignorando..." << std::endl;
        }

        std::string cachedResult = LookupInCache(query);

        if (!cachedResult.empty())
        {
            sendto(socket_fd, cachedResult.c_str(), cachedResult.size(), 0, (struct sockaddr *)&client_address, client_address_len);
        }
        else
        {
            std::string queryResult = ProcessDNSQuery(query);

            if (!queryResult.empty())
            {
                AddToCache(query, queryResult);

                sendto(socket_fd, queryResult.c_str(), queryResult.size(), 0, (struct sockaddr *)&client_address, client_address_len);
            }
            else
            {
                std::cerr << "Consulta DNS não encontrada ou erro na pesquisa DNS." << std::endl;
            }
        }
    }

    return true;
}

void DNS_Server::WaitForShutdown()
{
    std::mutex mutex;
    std::unique_lock<std::mutex> lock(mutex);
    std::condition_variable cv;

    cv.wait_for(lock, std::chrono::seconds(30));

    std::cout << "Encerrando o servidor..." << std::endl;
}
