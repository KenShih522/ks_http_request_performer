//
//  ks_request.hpp
//  ks_http_request_performer
//
//  Created by Shih on 11/01/2017.
//  Copyright © 2017 kenshih522. All rights reserved.
//

#ifndef ks_request_hpp
#define ks_request_hpp

#include <string>

namespace ks_http_request_performer
{

class ks_request
{
public:
    ks_request();
    virtual ~ks_request();
    
    const std::string&  get_ip() const;
    void                set_ip(const std::string& ip);
    uint16_t            get_port() const;
    void                set_port(const uint16_t port);
    const std::string&  get_username() const;
    void                set_username(const std::string& username);
    const std::string&  get_password() const;
    void                set_password(const std::string& password);
    
private:
    std::string ip_;
    uint16_t    port_;
    std::string username_;
    std::string password_;
};

inline const std::string& ks_request::get_ip() const
{
    return ip_;
}

inline void ks_request::set_ip(const std::string& ip)
{
    ip_ = ip;
}

inline uint16_t ks_request::get_port() const
{
    return port_;
}

inline void ks_request::set_port(const uint16_t port)
{
    port_ = port;
}

inline const std::string& ks_request::get_username() const
{
    return username_;
}

inline void ks_request::set_username(const std::string& username)
{
    username_ = username;
}

inline const std::string& ks_request::get_password() const
{
    return password_;
}

inline void ks_request::set_password(const std::string& password)
{
    password_ = password;
}
    
}

#endif /* ks_request_hpp */
