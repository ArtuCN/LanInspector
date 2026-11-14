#include "lib/LanInspector.hpp"

int main() {
    struct ifaddrs *ifaddr;
    struct DefaultRoute route;
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return 1;
    }

    populate_network_interface(ifaddr, &route);
    std::vector<struct in_addr> hosts = get_network_hosts(route.interface.ip, route.interface.netmask);
    for (const struct in_addr& host : hosts) {
        char ip_str[INET_ADDRSTRLEN];

        inet_ntop(AF_INET, &host, ip_str, INET_ADDRSTRLEN);
        std::cout << ip_str << std::endl;
    }
    freeifaddrs(ifaddr);
    return 0;
}