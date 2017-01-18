//
//  ks_client.hpp
//  ks_http_request_performer
//
//  Created by Shih on 11/01/2017.
//  Copyright © 2017 kenshih522. All rights reserved.
//

#ifndef ks_client_hpp
#define ks_client_hpp

#include <inttypes.h>

namespace ks_http_request_performer
{
    
class ks_client
{
public:
    ks_client();
    virtual ~ks_client();
    
    // inline
    int64_t  get_client_id() const;
    void     set_client_id(const int64_t client_id);
    
private:
    
    int64_t     client_id_;
    
};

inline int64_t ks_client::get_client_id() const
{
    return client_id_;
}

inline void ks_client::set_client_id(const int64_t client_id)
{
    client_id_ = client_id;
}
    
}

#endif /* ks_client_hpp */
