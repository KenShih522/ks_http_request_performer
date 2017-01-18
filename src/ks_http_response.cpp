//
//  ks_http_response.cpp
//  ks_http_request_performer
//
//  Created by Shih on 11/01/2017.
//  Copyright © 2017 kenshih522. All rights reserved.
//

#include "ks_http_response.hpp"
#include <stdlib.h>

namespace ks_http_request_performer
{
    
ks_http_response::ks_http_response() : code_(0), \
                                       code_msg_(""), \
                                       data_buffer_(nullptr), \
                                       data_buffer_size_(0), \
                                       data_size_(0)
{
    
}

ks_http_response::~ks_http_response()
{
    relase_data_buffer();
}


bool ks_http_response::copy_data_from(const uint8_t* data, const uint32_t data_size)
{
    if (0 == data_size)
    {
        return false;
    }
    
    if (get_data_buffer_size() < data_size)
    {
        relase_data_buffer();
        data_buffer_ = (uint8_t*)malloc(static_cast<size_t>(data_size));
        memset(data_buffer_, 0, data_size);
        
        if (!data_buffer_)
        {
            return false;
        }
        
        set_data_buffer_size(data_size);
    }
    
    if (0 < data_size)
    {
        memcpy(data_buffer_, data, data_size);
    }
    
    set_data_size(data_size);
    return true;
}

void ks_http_response::relase_data_buffer()
{
    free(data_buffer_);
    data_buffer_ = nullptr;
    
    data_buffer_size_ = 0;
    data_size_ = 0;
}

}
