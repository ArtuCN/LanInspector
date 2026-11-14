#include "../lib/LanInspector.hpp"

struct sockaddr_in *populate_network_interface(
    struct ifaddrs *ifaddr,
    struct DefaultRoute *route
) {
    struct ifaddrs *ifa;
    struct sockaddr_in *sa = nullptr;

    if (!get_router_interface_name(route)) {
        perror("router interface not found");
        return nullptr;
    }

    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {

        if (!ifa->ifa_name ||
            route->interface.name != ifa->ifa_name ||
            !ifa->ifa_addr) {
            continue;
        }

        // Recupero IP e netmask IPv4
        if (ifa->ifa_addr->sa_family == AF_INET) {
            sa = (struct sockaddr_in *)ifa->ifa_addr;
            route->interface.ip = sa->sin_addr;

            if (ifa->ifa_netmask &&
                ifa->ifa_netmask->sa_family == AF_INET) {
                struct sockaddr_in *mask =
                    (struct sockaddr_in *)ifa->ifa_netmask;

                route->interface.netmask = mask->sin_addr;
            }
        }

        // Recupero MAC
        if (ifa->ifa_addr->sa_family == AF_PACKET) {
            struct sockaddr_ll *mac =
                (struct sockaddr_ll *)ifa->ifa_addr;

            for (int i = 0; i < 6; i++) {
                route->interface.mac[i] = mac->sll_addr[i];
            }
        }
    }
    route->interface.print();
    return sa;
}

int get_router_interface_name(struct DefaultRoute *route) {
    FILE* pipe = popen("ip -4 route show default", "r");

    if (pipe == nullptr) {
        return 0;
    }

    char buffer[100];
    std::string output;

    while (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
        output += buffer;
    }

    pclose(pipe);

    std::istringstream stream(output);
    std::string word;

    while (stream >> word) {

        if (word == "via") {
            stream >> route->gateway;
        }

        if (word == "dev") {
            stream >> route->interface.name;
        }
    }
    return (1);

}