#include "../lib/LanInspector.hpp"




bool arp(std::vector<struct in_addr> *hosts)
{
    int sockfd = open_arp_socket();

    if (sockfd < 0)
        return false;
    else
        std::cout<<"andato\n";
    return true;
    for (auto it = hosts->begin(); it != hosts->end(); ) {

        create_arp_request();

        send_arp_request();

        if (receive_arp_response()) {
            ++it;
        }
        else {
            it = hosts->erase(it);
        }
    }

    close(sockfd);
    return true;
}

int open_arp_socket()
{
    int sockfd = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ARP));

    if (sockfd < 0) {
        perror("socket");
        return -1;
    }

    return sockfd;
}

std::vector<struct in_addr> get_network_hosts(
    struct in_addr ip,
    struct in_addr netmask
) {
    std::vector<struct in_addr> hosts;

    uint32_t local_ip = ntohl(ip.s_addr);
    uint32_t mask = ntohl(netmask.s_addr);

    uint32_t network = local_ip & mask;
    uint32_t broadcast = network | ~mask;

    for (uint32_t addr = network + 1; addr < broadcast; addr++) {

        if (addr == local_ip)
            continue;

        struct in_addr host;
        host.s_addr = htonl(addr);

        hosts.push_back(host);
    }

    return hosts;
}
