#ifndef DNS_SERVER_H
#define DNS_SERVER_H

#include <string>
#include <vector>
#include <map>
#include <netinet/in.h>

class DNS_Server
{
public:
    DNS_Server();
    ~DNS_Server();

    bool Initialize(const std::string &ip, int port);

    bool LoadDNSRecords(const std::string &configFileName);

    bool Start();

    void WaitForShutdown();

private:
    int socket_fd;
    struct sockaddr_in server_address;
    std::map<std::string, std::string> dns_records;

    void HandleDNSRequest(int client_socket);
};

#endif
