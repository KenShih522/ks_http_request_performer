//
//  ks_http_request_performer_types.h
//  ks_http_request_performer
//
//  Created by Shih on 11/01/2017.
//  Copyright © 2017 kenshih522. All rights reserved.
//

#ifndef ks_http_request_performer_types_h
#define ks_http_request_performer_types_h

#include <inttypes.h>
#include <stddef.h>

enum ks_http_request_method
{
    ks_http_request_method_get,
    ks_http_request_method_post,
    ks_http_request_method_put,
    ks_http_request_method_delete,
    ks_http_request_method_secure_get,
    ks_http_request_method_secure_post,
    ks_http_request_method_secure_put,
    ks_http_request_method_secure_delete,
};

enum ks_http_request_post_form_type
{
    ks_http_request_post_form_type_text,
    ks_http_request_post_form_type_json,
};

enum ks_http_request_result_response_code
{
    ks_http_request_result_response_code_success = 0,
    ks_http_request_result_response_code_internal_error,
    ks_http_request_result_response_code_timeout,
    ks_http_request_result_response_code_network_error,
    ks_http_request_result_response_code_response_error,
};

typedef struct ks_http_request_out
{
    int32_t                          request_method; ///< ks_http_request_method
    int32_t                          post_form_type; ///< ks_http_request_post_form_type
    char                             ip[40];
    uint16_t                         port;
    char                             uri[1024];
    char                             header_json[1024];
    uint8_t*                         body;
    size_t                           body_size;
} ks_http_request_out;

typedef struct ks_http_response_out
{
    int32_t                          http_code;
    char                             http_code_msg[1024];
    uint8_t*                         response_data;
    uint32_t                         response_data_size;
} ks_http_response_out;

typedef struct ks_http_request_result
{
    int64_t                  client_id;
    int32_t                  response_code; ///< ks_http_request_result_response_code
    ks_http_request_out*     request;
    ks_http_response_out*    response;
} ks_http_request_result;

typedef void(*ks_http_request_respone_callback)(const ks_http_request_result* result, void* arg);

typedef struct ks_http_request_callbacks
{
    ks_http_request_respone_callback response;
    void*                            response_user_arg;
} ks_http_request_callbacks;

typedef struct ks_http_request_setup_info
{
    int32_t                          request_method; ///< ks_http_request_method
    int32_t                          post_form_type; ///< ks_http_request_post_form_type
    char                             ip[40];
    uint16_t                         port;
    char                             uri[1024];
    char                             header_json[1024];
    uint8_t*                         body;
    size_t                           body_size;
    ks_http_request_callbacks        callback;
} ks_http_request_setup_info;

#endif /* ks_http_request_performer_types_h */
