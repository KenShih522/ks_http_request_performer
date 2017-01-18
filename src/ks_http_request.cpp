//
//  ks_http_request.cpp
//  ks_http_request_performer
//
//  Created by Shih on 11/01/2017.
//  Copyright © 2017 kenshih522. All rights reserved.
//

#include "ks_http_request.hpp"

namespace ks_http_request_performer
{
    
    ks_http_request::ks_http_request() : type_(ks_http_request_type_get), \
                                         timeout_seconds_(k_default_http_request_timeout_seconds)
    {
        
    }
    
    ks_http_request::ks_http_request(const ks_http_request& request)
    {
        clone_from(request);
    }
    
    ks_http_request::~ks_http_request()
    {
        header_params_.clear();
    }
    
    std::string ks_http_request::get_uri() const
    {
        std::string uri = path_;
        
        return uri;
    }
    
    void ks_http_request::clone_from(const ks_http_request& request)
    {
        set_ip(request.get_ip());
        set_port(request.get_port());
        set_username(request.get_username());
        set_password(request.get_password());
        set_type(request.get_type());
        set_post_form_type(request.get_post_form_type());
        set_timeout(request.get_timeout());
        set_path(request.get_path());
        
        request.get_all_header_params(header_params_);
        set_content(request.get_content());
        set_header_content(request.get_header_content());
    }
    
    void ks_http_request::get_all_header_params(header_params& params) const
    {
        params = header_params_;
    }
    
    bool ks_http_request::get_header_param(const std::string& key, std::string& value) const
    {
        header_params::const_iterator it = header_params_.find(key);
        if (header_params_.end() == it)
        {
            return false;
        }
        
        value = it->second;
        return true;
    }
    
}
