//
//  J_IPAddress.h
//  AGP
//
//  Created by Jfitzpat on the JUCE Forum on 7/13/12.
//  Copyright (c) 2012 yourcompanyname. All rights reserved.
//

#ifndef AGP_J_IPAddress_h
#define AGP_J_IPAddress_h


#include "JuceHeader.h"

/** The jfitzpat IpAddress class
 */
class JUCE_API IpAddress
{
public:
    //==============================================================================
    /** Populates a list of the IPv4 addresses of all the available network cards. */
    static void findAllIpAddresses (Array<IpAddress>& results);
    
    //==============================================================================
    /** Creates a null address (0.0.0.0). */
    IpAddress();
    
    /** Creates from a host order uint32. */
    IpAddress (uint32 addr);
    
    /** Creates from another address. */
    IpAddress (const IpAddress& other);
    
    /** Creates a copy of another address. */
    IpAddress& operator= (const IpAddress& other);
    
    /** Creates an address from a string ("1.2.3.4"). */
    explicit IpAddress (const String& addr);
    
    /** Returns a dot-separated string in the form "1.2.3.4". */
    String toString() const;
    
    /** Return as host order uint32. */
    uint32 toUint32() const noexcept;
    
    /** Return as network order uint32. */
    uint32 toNetworkUint32() const noexcept;
    
    /** Returns true if this address is ANY (0.0.0.0). */
    bool isAny() const noexcept;
    
    /** Returns true if this address is BROADCAST (255.255.255.255). */
    bool isBroadcast() const noexcept;
    
    /** Returns true if this address is LOOPBACK (127.0.0.1). */
    bool isLocal() const noexcept;
    
    bool operator== (const IpAddress& other) const noexcept;
    bool operator!= (const IpAddress& other) const noexcept;
    
    //==============================================================================
    /** IPv4 Any Address. */
    static const IpAddress any;
    static const IpAddress broadcast;
    static const IpAddress localhost;
    
    //==============================================================================
private:
    uint32 ipAddress;
};

#endif

