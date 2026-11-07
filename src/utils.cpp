#include "../lib/LanInspector.hpp"

struct sockaddr_in *find_interface_ip(struct ifaddrs *ifaddr, struct DefaultRoute *route) {
    struct ifaddrs *ifa;
    if (!get_router_interface_name(route))
        perror("router interface not found");
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {

        if (ifa->ifa_name &&
            route->interface == ifa->ifa_name &&
            ifa->ifa_addr &&
            ifa->ifa_addr->sa_family == AF_INET) {

            struct sockaddr_in *sa =
                (struct sockaddr_in *)ifa->ifa_addr;

            return sa;
        }
    }

    return nullptr;
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
            stream >> route->interface;
        }
    }
    return (1);

}