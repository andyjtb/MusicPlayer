//
//  J_IPAdress.cpp
//  AGP
//
//  Created by Jfitzpat on the JUCE Forum on 7/13/12.
//  Copyright (c) 2012 yourcompanyname. All rights reserved.
//

#if JUCE_MSVC
#pragma warning (push)
#pragma warning (disable : 4127 4389 4018)
#endif

#ifndef AI_NUMERICSERV  // (missing in older Mac SDKs)
#define AI_NUMERICSERV 0x1000
#endif

#include "../JuceLibraryCode/JuceHeader.h"

#if JUCE_WINDOWS

#include <Windows.h>
#include <iphlpapi.h>

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>

#endif

#include "J_IPAddress.h"


//==============================================================================
//==============================================================================
IpAddress::IpAddress()
: ipAddress (0)
{
}

IpAddress::IpAddress (uint32 addr)
: ipAddress (addr)
{
}

IpAddress::IpAddress (const IpAddress& other)
: ipAddress (other.ipAddress)
{
}

IpAddress& IpAddress::operator= (const IpAddress& other)
{
    ipAddress = other.ipAddress;
    return *this;
}

IpAddress::IpAddress (const String& addr)
{
    uint32 temp = inet_addr (addr.toUTF8());
    ipAddress = ntohl (temp);
}

String IpAddress::toString() const
{
    String s;
    
    s = String ((ipAddress >> 24) & 0xFF);
    s << '.';
    s << String ((ipAddress >> 16) & 0xFF);
    s << '.';
    s << String ((ipAddress >> 8) & 0xFF);
    s << '.';
    s << String (ipAddress & 0xFF);
    
    return s;
}

uint32 IpAddress::toUint32() const noexcept
{
    return ipAddress;
}

uint32 IpAddress::toNetworkUint32() const noexcept
{
    return htonl (ipAddress);
}

bool IpAddress::isAny() const noexcept          { return ipAddress == 0; }
bool IpAddress::isBroadcast() const noexcept    { return ipAddress == 0xFFFFFFFF; }
bool IpAddress::isLocal() const noexcept        { return ipAddress == 0x7F000001; }

bool IpAddress::operator== (const IpAddress& other) const noexcept
{
    return ipAddress == other.ipAddress;
}

bool IpAddress::operator!= (const IpAddress& other) const noexcept
{
    return ipAddress != other.ipAddress;
}

#if JUCE_WINDOWS
void IpAddress::findAllIpAddresses (Array<IpAddress>& result)
{
    // For consistancy
    result.addIfNotAlreadyThere (IpAddress ("127.0.0.1"));
    
    ULONG len = sizeof (IP_ADAPTER_INFO);
    HeapBlock<IP_ADAPTER_INFO> adapterInfo (1);
    
    if (GetAdaptersInfo (adapterInfo, &len) == ERROR_BUFFER_OVERFLOW)
        adapterInfo.malloc (len, 1);
    
    if (GetAdaptersInfo (adapterInfo, &len) == NO_ERROR)
    {
        for (PIP_ADAPTER_INFO adapter = adapterInfo; adapter != nullptr; adapter = adapter->Next)
        {
            IpAddress ip (adapter->IpAddressList.IpAddress.String);
            if (! ip.isAny())
                result.addIfNotAlreadyThere (ip);
        }
    }
}
#endif

#if JUCE_MAC || JUCE_IOS
// Oh joy, two incompatible SIOCGIFCONF interfaces...
void IpAddress::findAllIpAddresses (Array<IpAddress>& result)
{
    struct ifconf cfg;
    size_t buffer_capacity;
    HeapBlock<char> buffer;
    int sock = -1;
    
    // Compute the sizes of ifreq structures
    const size_t ifreq_size_in = IFNAMSIZ + sizeof (struct sockaddr_in);
    const size_t ifreq_size_in6 = IFNAMSIZ + sizeof (struct sockaddr_in6);
    
    // Poor man's try since we can be in an existing noexcept
    do
    {
        // Create a dummy socket to execute the IO control on
        sock = socket (AF_INET, SOCK_DGRAM, 0);
        if (sock < 0)
            break;
        
        // Repeatedly call the IO control with increasing buffer sizes until success
        // Ugly, old school...
        bool success = true;
        buffer_capacity = ifreq_size_in6;
        do
        {
            buffer_capacity *= 2;
            buffer.realloc (buffer_capacity);
            
            cfg.ifc_len = buffer_capacity;
            cfg.ifc_buf = buffer;
            
            if ((ioctl (sock, SIOCGIFCONF, &cfg) < 0) && (errno != EINVAL))
            {
                success = false;
                break;
            }
            
        } while ((buffer_capacity - cfg.ifc_len) < 2 * ifreq_size_in6);
        
        // How did we do?
        if (success == false)
            break;
        
        // Copy the interface addresses into the result array
        while (cfg.ifc_len >= ifreq_size_in)
        {
            // Skip entries for non-internet addresses
            if (cfg.ifc_req->ifr_addr.sa_family == AF_INET)
            {
                const struct sockaddr_in* addr_in = (const struct sockaddr_in*) &cfg.ifc_req->ifr_addr;
                in_addr_t addr = addr_in->sin_addr.s_addr;
                // Skip entries without an address
                if (addr != INADDR_NONE)
                    result.addIfNotAlreadyThere (IpAddress (ntohl(addr)));
            }
            
            // Move to the next structure in the buffer
            // CANNOT just use sizeof (ifreq) because entries vary in size
            cfg.ifc_len -= IFNAMSIZ + cfg.ifc_req->ifr_addr.sa_len;
            cfg.ifc_buf += IFNAMSIZ + cfg.ifc_req->ifr_addr.sa_len;
        }
    } while (0);
    
    // Free the buffer and close the socket if necessary
    buffer.free();
    
    if (sock >= 0)
        close(sock);
}
#endif

#if JUCE_LINUX || JUCE_ANDROID
void IpAddress::findAllIpAddresses (Array<IpAddress>& result)
{
    const int s = socket (AF_INET, SOCK_DGRAM, 0);
    if (s != -1)
    {
        char buf [1024];
        struct ifconf ifc;
        ifc.ifc_len = sizeof (buf);
        ifc.ifc_buf = buf;
        ioctl (s, SIOCGIFCONF, &ifc);
        
        struct ifreq *ifr = ifc.ifc_req;
        int nInterfaces = ifc.ifc_len / sizeof(struct ifreq);
        for(int i = 0; i < nInterfaces; i++)
        {
            struct ifreq *item = &ifr[i];
            
            if (item->ifr_addr.sa_family == AF_INET)
            {
                const struct sockaddr_in* addr_in = (const struct sockaddr_in*) &item->ifr_addr;
                in_addr_t addr = addr_in->sin_addr.s_addr;
                // Skip entries without an address
                if (addr != INADDR_NONE)
                    result.addIfNotAlreadyThere (IpAddress (ntohl(addr)));
            }
        }
        close (s);
    }
}
#endif

const IpAddress IpAddress::any (0);
const IpAddress IpAddress::broadcast (0xFFFFFFFF);
const IpAddress IpAddress::localhost (0x7F000001);
