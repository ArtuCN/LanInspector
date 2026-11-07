#include "lib/LanInspector.hpp"

int main() {
    struct ifaddrs *ifaddr, *ifa;
    struct DefaultRoute route;
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return 1;
    }

    find_interface_ip(ifaddr, &route);
    std::cout<<route.gateway<<" "<<route.interface;
    freeifaddrs(ifaddr);

    return 0;
}