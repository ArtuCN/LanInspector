
#ifndef LANINSPECTOR_HPP
#define LANINSPECTOR_HPP

#include <sys/types.h>
#include <sys/socket.h>
#include <linux/if_ether.h>
#include <stdlib.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <cstdio>
#include <sstream>
#include <array>
#include <netpacket/packet.h>
#include <vector>
#include <cstdint>
#include <unistd.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <iostream>

struct NetworkInterface {
    std::string name;
    struct in_addr ip{};
    struct in_addr netmask{};
    std::array<unsigned char, 6> mac{};

    void print() const;
};

struct DefaultRoute {
    std::string gateway;
    NetworkInterface interface;
};

struct Frame {
    struct ethhdr ethernet;
    struct ether_arp arp;
};

struct sockaddr_in *populate_network_interface(
    struct ifaddrs *ifaddr,
    struct DefaultRoute *route
);

int get_router_interface_name(struct DefaultRoute *route);

// arp.cpp
std::vector<struct in_addr> get_network_hosts(
    struct in_addr ip,
    struct in_addr netmask
);

bool arp(
    std::vector<struct in_addr> *hosts,
    struct DefaultRoute *route
);

int open_arp_socket();

Frame create_arp_request(
    const NetworkInterface& interface,
    const struct in_addr& target_ip
);

int send_arp_request(
    int sockfd,
    const NetworkInterface *interface,
    const Frame *frame
);

int receive_arp_response();

#endif
