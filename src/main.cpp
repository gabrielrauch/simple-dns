#include "dns_server.h"
#include "dns_constants.h"
#include <iostream>
#include <string>

int main()
{
    DNS_Server dnsServer;

    if (!dnsServer.Initialize("127.0.0.1", DEFAULT_DNS_PORT))
    {
        std::cerr << "Falha na inicialização do servidor DNS." << std::endl;
        return 1;
    }

    if (!dnsServer.LoadDNSRecords("dns_records.txt"))
    {
        std::cerr << "Falha ao carregar registros DNS do arquivo de configuração." << std::endl;
        return 1;
    }

    if (!dnsServer.Start())
    {
        std::cerr << "Falha ao iniciar o servidor DNS." << std::endl;
        return 1;
    }

    dnsServer.WaitForShutdown();

    return 0;
}
