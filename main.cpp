#include "lib/LanInspector.hpp"

int main() {
    struct ifaddrs *ifaddr = nullptr;
    struct DefaultRoute route;

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return 1;
    }

    populate_network_interface(ifaddr, &route);
    std::vector<struct in_addr> hosts =
        get_network_hosts(
            route.interface.ip,
            route.interface.netmask
        );

    if (!arp(&hosts, &route)) {
        freeifaddrs(ifaddr);
        return 1;
    }

    freeifaddrs(ifaddr);

    return 0;
}
