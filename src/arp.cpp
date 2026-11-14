#include "../lib/LanInspector.hpp"

#include "../lib/LanInspector.hpp"

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