#ifndef DNS_SERVER_H
#define DNS_SERVER_H

#include <string>
#include <vector>
#include <map>
#include <netinet/in.h>
#include <unordered_map>
#include <mutex>
#include <regex>

class DNS_Server
{

private:
    int socket_fd;
    struct sockaddr_in server_address;
    std::map<std::string, std::string> dns_records;
    std::unordered_map<std::string, std::string> dnsCache;
    std::mutex cacheMutex;
    std::regex validDomainPattern;

public:
    DNS_Server();

    ~DNS_Server();

    bool Initialize(const std::string &ip, int port);

    bool LoadDNSRecords(const std::string &configFileName);

    bool IsValidDomain(const std::string &domain);

    bool IsQuerySizeValid(const ssize_t querySize);

    bool Start();

    void WaitForShutdown();

    std::string LookupInCache(const std::string &domain);

    void AddToCache(const std::string &domain, const std::string &ip);

    std::string ProcessDNSQuery(const std::string &query);
};

#endif
