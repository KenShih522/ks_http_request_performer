//
//  ks_http_request_performer.h
//  ks_http_request_performer
//
//  Created by Shih on 11/01/2017.
//  Copyright © 2017 kenshih522. All rights reserved.
//

#ifndef ks_http_request_performer_h
#define ks_http_request_performer_h

#include "ks_http_request_performer_types.h"

#ifdef __cplusplus
extern "C"
{
#endif // #ifdef __cplusplus
    
    bool    ks_http_request_performer_init();
    
    int64_t ks_http_request_object_create(const struct ks_http_request_setup_info* info);
    
    bool    ks_http_request_object_perform_request(const int64_t client_id);

#ifdef __cplusplus
}
#endif // #ifdef __cplusplus

#endif /* ks_http_request_performer_h */
