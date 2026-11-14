#include "../lib/LanInspector.hpp"

void NetworkInterface::print() const {
    char ip_buffer[INET_ADDRSTRLEN];
    char mask_buffer[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &ip, ip_buffer, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &netmask, mask_buffer, INET_ADDRSTRLEN);

    std::cout << "Interface: " << name << "\n";
    std::cout << "IP: " << ip_buffer << "\n";
    std::cout << "Netmask: " << mask_buffer << "\n";

    std::cout << "MAC: ";

    for (int i = 0; i < mac.size(); i++) {
        printf("%02X", mac[i]);

        if (i < mac.size() - 1)
            printf(":");
    }

    std::cout << "\n";
}