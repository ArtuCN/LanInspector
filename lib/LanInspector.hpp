#ifndef LANINSPECTOR_HPP
#define LANINSPECTOR_HPP

#include <sys/types.h>
#include <iostream>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <stdlib.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>
#include <cstdio>
#include <sstream>

struct DefaultRoute {
    std::string gateway;
    std::string interface;
};

struct sockaddr_in *find_interface_ip(struct ifaddrs *ifaddr, struct DefaultRoute *route);
int get_router_interface_name(struct DefaultRoute *route);

#endif