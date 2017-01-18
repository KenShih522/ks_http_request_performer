//
//  ks_http_request.hpp
//  ks_http_request_performer
//
//  Created by Shih on 11/01/2017.
//  Copyright © 2017 kenshih522. All rights reserved.
//

#ifndef ks_http_request_hpp
#define ks_http_request_hpp

#include "ks_request.hpp"
#include <map>
#include "ks_http_request_converter.hpp"

namespace ks_http_request_performer
{
    
class ks_http_request : public ks_request{
    
public:
    
    enum type{
        ks_http_request_type_get,
        ks_http_request_type_post,
        ks_http_request_type_put,
        ks_http_request_type_delete,
        ks_http_request_type_secure_get,
        ks_http_request_type_secure_post,
        ks_http_request_type_secure_put,
        ks_http_request_type_secure_delete,
    };
    
    enum post_form_type
    {
        ks_http_request_post_form_type_text,
        ks_http_request_post_form_type_json,
    };
    
    typedef std::map<std::string, std::string> header_params;
    
public:
    ks_http_request();
    ks_http_request(const ks_http_request& request);
    ~ks_http_request();
    
    std::string         get_uri_query_string() const;
    std::string         get_uri() const;
    
    void                clone_from(const ks_http_request& request);
    void                get_all_header_params(header_params& params) const;
    bool                get_header_param(const std::string& key, std::string& value) const;
    
    // inline
    int32_t             get_type() const;
    void                set_type(const int32_t type);
    int32_t             get_post_form_type() const;
    void                set_post_form_type(const int32_t post_form_type);
    uint32_t            get_timeout() const;
    void                set_timeout(const uint32_t timeout);
    const std::string&  get_path() const;
    void                set_path(const std::string& path);
    
    const std::string&  get_content() const;
    void                set_content(const std::string& content);
    
    const std::string&  get_header_content() const;
    void                set_header_content(const std::string& header_content);
    
    template<typename value_type> \
    void add_header_param(const std::string& key, const value_type& value);
    
private:
    static const uint32_t k_default_http_request_timeout_seconds = 30;
    
private:
    int32_t     type_;
    uint32_t    timeout_seconds_;
    std::string path_;
    std::string content_;
    std::string header_content_;
    header_params header_params_;
    int32_t     post_form_type_;
    bool        gzip_flag_;
    
};

// inline

inline int32_t ks_http_request::get_type() const
{
    return type_;
}

inline void ks_http_request::set_type(const int32_t type)
{
    type_ = type;
}
inline int32_t ks_http_request::get_post_form_type() const
{
    return post_form_type_;
}

inline void ks_http_request::set_post_form_type(const int32_t post_form_type)
{
    post_form_type_ = post_form_type;
}
inline uint32_t ks_http_request::get_timeout() const
{
    return timeout_seconds_;
}

inline void ks_http_request::set_timeout(const uint32_t timeout)
{
    timeout_seconds_ = timeout;
}

inline const std::string& ks_http_request::get_path() const
{
    return path_;
}

inline void ks_http_request::set_path(const std::string& path)
{
    path_ = path;
}

inline const std::string& ks_http_request::get_content() const
{
    return content_;
}

inline void ks_http_request::set_content(const std::string& content)
{
    content_ = content;
}
    
inline const std::string& ks_http_request::get_header_content() const
{
    return header_content_;
}
inline void ks_http_request::set_header_content(const std::string& header_content)
{
    header_content_ = header_content;
}

template<typename value_type> \
void  ks_http_request::add_header_param(const std::string& key, const value_type& value)
{
    std::string value_string;
    ks_http_request_converter::convert_by_stream<value_type, std::string>(value, value_string);
    
    header_params_[key] = value_string;
}
    
}

#endif /* ks_http_request_hpp */
