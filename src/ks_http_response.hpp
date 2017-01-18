//
//  ks_http_response.hpp
//  ks_http_request_performer
//
//  Created by Shih on 11/01/2017.
//  Copyright © 2017 kenshih522. All rights reserved.
//

#ifndef ks_http_response_hpp
#define ks_http_response_hpp

#include "ks_response.hpp"
#include <inttypes.h>
#include <string>

namespace ks_http_request_performer
{
    
class ks_http_response : public ks_response{
    
public:
    ks_http_response();
    virtual ~ks_http_response();
    
    int32_t             get_code() const;
    void                set_code(const int32_t code);
    const std::string&  get_code_msg() const;
    void                set_code_msg(const std::string& msg);
    const uint8_t*      get_data_buffer() const;
    bool                copy_data_from(const uint8_t* data, const uint32_t data_size);
    uint32_t            get_data_size() const;
    
private:
    void                set_data_size(const uint32_t size);
    uint32_t            get_data_buffer_size() const;
    void                set_data_buffer_size(const uint32_t size);
    void                relase_data_buffer();
private:
    int32_t     code_;
    std::string code_msg_;
    uint8_t*    data_buffer_;
    uint32_t    data_buffer_size_;
    uint32_t    data_size_;
    
};

inline int32_t ks_http_response::get_code() const
{
    return code_;
}

inline void ks_http_response::set_code(const int32_t code)
{
    code_ = code;
}

inline const std::string& ks_http_response::get_code_msg() const
{
    return code_msg_;
}

inline void ks_http_response::set_code_msg(const std::string& msg)
{
    code_msg_ = msg;
}

inline const uint8_t* ks_http_response::get_data_buffer() const
{
    return data_buffer_;
}

inline uint32_t ks_http_response::get_data_size() const
{
    return data_size_;
}

// private
inline void ks_http_response::set_data_size(const uint32_t size)
{
    data_size_ = size;
}

inline uint32_t ks_http_response::get_data_buffer_size() const
{
    return data_buffer_size_;
}

inline void ks_http_response::set_data_buffer_size(const uint32_t size)
{
    data_buffer_size_ = size;
}
    
}

#endif /* ks_http_response_hpp */
