//
//  ks_http_request_performer.cpp
//  ks_http_request_performer
//
//  Created by Shih on 11/01/2017.
//  Copyright © 2017 kenshih522. All rights reserved.
//

#include "ks_http_request_performer.h"
#include "ks_http_request_performer_types.h"
#include "ks_http_request_manager.hpp"
#include "ks_http_request_converter.hpp"

static const uint32_t k_default_number_of_http_request_background_threads = 10; ///< the background threads number

typedef struct ks_http_request_handle{
    
    ks_http_request_performer::ks_http_request_manager*  request_manager;
    
    ks_http_request_callbacks callbacks;
    
} ks_http_request_handle;

ks_http_request_handle* http_request_handler = nullptr;

void ks_http_request_manager_callback(const int64_t client_id, \
                                   const ks_http_request_performer::ks_http_request *request, \
                                   const ks_http_request_performer::ks_http_response *response, \
                                   const int32_t response_code);

using namespace std;

using namespace ks_http_request_performer;

bool ks_http_request_performer_init(){
    
    if(http_request_handler == nullptr){
        
        http_request_handler = new ks_http_request_handle;
        
        http_request_handler->request_manager = new ks_http_request_performer::ks_http_request_manager(k_default_number_of_http_request_background_threads);
        
        http_request_handler->callbacks.response = nullptr;
        
        http_request_handler->callbacks.response_user_arg = nullptr;
        
        return true;
        
    }
    else{
        
        return false;
        
    }
    
}

int64_t ks_http_request_object_create(const struct ks_http_request_setup_info* info){
    
    if(!http_request_handler || !http_request_handler->request_manager){
        
        return -1;
        
    }
    
    http_request_handler->callbacks.response = info->callback.response;
    
    http_request_handler->callbacks.response_user_arg = info->callback.response_user_arg;
    
    return http_request_handler->request_manager->add_request(info, ks_http_request_manager_callback);
    
}

bool ks_http_request_object_perform_request(const int64_t client_id){
    
    if(!http_request_handler || !http_request_handler->request_manager){
        
        return false;
        
    }
    
    return http_request_handler->request_manager->start_process_request(client_id);
    
}

void ks_http_request_manager_callback(const int64_t client_id, const ks_http_request_performer::ks_http_request *request, const ks_http_request_performer::ks_http_response *response, const int32_t response_code){
    
    ks_http_request_result request_result;
    
    memset(&request_result, 0, sizeof(ks_http_request_result));
    
    request_result.client_id = client_id;
    
    ks_http_request_performer::ks_http_request_converter::convert_ks_http_client_response_code_to_ks_http_request_response_code(response_code, request_result.response_code);
    
    request_result.request = new ks_http_request_out;
    
    request_result.response = new ks_http_response_out;
    
    ks_http_request_performer::ks_http_request_converter::convert_ks_http_request_to_ks_http_request_out(*request, *(request_result.request));
    
    ks_http_request_performer::ks_http_request_converter::convert_ks_http_response_to_ks_http_response_out(*response, *(request_result.response));
    
    http_request_handler->callbacks.response(&request_result, http_request_handler->callbacks.response_user_arg);
    
    if(request_result.request->body){
        
        free(request_result.request->body);
        
    }
    
    if(request_result.response->response_data){
        
        free(request_result.response->response_data);
        
    }
    
    delete request_result.request;
    
    delete request_result.response;
    
}
