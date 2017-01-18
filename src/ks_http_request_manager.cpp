//
//  ks_http_request_manager.cpp
//  ks_http_request_performer
//
//  Created by Shih on 11/01/2017.
//  Copyright © 2017 kenshih522. All rights reserved.
//

#include "ks_http_request_manager.hpp"
#include "ks_http_request_performer_types.h"
#include "ks_http_client.hpp"
#include "ks_https_client.hpp"
#include "ks_http_request.hpp"
#include "ks_http_request_converter.hpp"

namespace ks_http_request_performer
{
    
ks_http_request_manager::ks_http_request_manager(const uint32_t num_background_threads) : worker_(io_service_) {
    
    for(int i = 0 ; i < num_background_threads ; i++){
        
        threads_.create_thread(boost::bind(&boost::asio::io_service::run, &io_service_));
        
    }
    
}

ks_http_request_manager::~ks_http_request_manager(){
    
}

int64_t ks_http_request_manager::add_request(const struct ks_http_request_setup_info* setup_info, http_request_response_callback_function callback){
    
    if(!setup_info || !setup_info->callback.response){
        
        return k_http_request_invalid_id_setup_info_not_valid;
        
    }
    
    ks_request_handle* handler = nullptr;
    
    ks_http_request_performer::ks_http_request client_request;
    
    bool success = ks_http_request_converter::convert_ks_http_request_setup_info_to_ks_http_client_request(*setup_info, client_request);
    
    if(!success){
        
        return k_http_request_invalid_id_setup_info_not_valid;
        
    }
    
    if(setup_info->request_method == ks_http_request_method_get ||
       setup_info->request_method == ks_http_request_method_post ||
       setup_info->request_method == ks_http_request_method_put ||
       setup_info->request_method == ks_http_request_method_delete){
        
        handler = new ks_request_handle;
        
        handler->client = new boost::shared_ptr<ks_http_client>(new ks_http_client(io_service_));
        
    }
    else if(setup_info->request_method == ks_http_request_method_secure_get ||
            setup_info->request_method == ks_http_request_method_secure_post ||
            setup_info->request_method == ks_http_request_method_secure_put ||
            setup_info->request_method == ks_http_request_method_secure_delete){
        
        handler = new ks_request_handle;
        
        handler->client = new boost::shared_ptr<ks_http_client>(new ks_https_client(io_service_));
        
    }
    else{
        
        return k_http_request_invalid_id_setup_info_not_valid;
        
    }
    
    handler->client->get()->setup_request(client_request);
    
    int64_t new_id = generate_http_request_id();
    
    handler->client_id = new_id;
    
    handler->callback = callback;
    
    handler->client->get()->set_client_id(new_id);
    
    add_http_request_handle(handler);
    
    return handler->client_id;
    
}

bool ks_http_request_manager::start_process_request(const int64_t client_id){
    
    io_service_.post(boost::bind(&ks_http_request_manager::process_request, this, client_id));
    
    return true;
    
}

void ks_http_request_manager::process_request(const int64_t client_id){
    
    ks_request_handle* handle = move_handle_to_process(client_id);
    
    if (!handle || !handle->client){
        
        return;
        
    }
    
    handle->client->get()->set_ks_http_client_callback(boost::bind(&ks_http_request_manager::http_client_callback, this, _1, _2, _3));
    
    bool result = handle->client->get()->send_request();
    
    if (!result){
        
        if (!handle->callback.empty()){
            
            handle->callback(handle->client_id, handle->client->get()->get_request(), handle->client->get()->get_response(), ks_http_client::ks_http_client_response_code_internal_error);
            
        }
        
        // remove process handle
        bool removed = remove_process_request_handle(client_id);
        
        if (!removed){
            
        }
        
    }
    
}

ks_http_request_manager::ks_request_handle* ks_http_request_manager::move_handle_to_process(const int64_t client_id){
    
    boost::unique_lock<boost::mutex> lock(http_requests_mutex_);
    
    ks_http_requests::iterator it = http_requests_.find(client_id);
    
    if (it == http_requests_.end()){
        
        return nullptr;
        
    }
    
    ks_request_handle* handle = it->second;
    
    if (handle && http_process_requests_.find(handle->client_id) == http_process_requests_.end()){
        
        http_process_requests_[handle->client_id] = handle;
        
    }
    
    http_requests_.erase(it);
    
    return handle;
    
}

bool ks_http_request_manager::remove_process_request_handle(const int64_t id)
{
    
    boost::unique_lock<boost::mutex> lock(http_requests_mutex_);
    
    ks_http_process_requests::iterator it = http_process_requests_.find(id);
    
    if (it == http_process_requests_.end()){
        
        return false;
        
    }
    
    ks_request_handle* handle = it->second;
    
    if (handle){
        
        delete handle->client;
        
        delete handle;
        
    }
    
    http_process_requests_.erase(it);
    
    return true;
    
}

void ks_http_request_manager::http_client_callback(const bool success, const int64_t client_id, const ks_http_client::response_code code)
{
    ks_http_process_requests::iterator it = http_process_requests_.find(client_id);
    
    if (it == http_process_requests_.end()){
        
        return;
        
    }
    
    ks_request_handle* handle = it->second;
    
    const ks_http_response* response = handle->client->get()->get_response();
    
    if (!handle->callback.empty()){
        
        handle->callback(handle->client_id, handle->client->get()->get_request(), response, code);
        
    }
    
    bool removed = remove_process_request_handle(client_id);
    
    if (!removed){
        
        return;
        
    }
    
}

int64_t ks_http_request_manager::generate_http_request_id(){
    
    boost::unique_lock<boost::mutex> lock(current_max_http_request_id_mutex_);
    
    ++current_max_http_request_id_;
    
    return current_max_http_request_id_;
    
}

bool ks_http_request_manager::add_http_request_handle(ks_request_handle* handle){
    
    if(!handle || !handle->client){
        
        return false;
        
    }
    
    boost::unique_lock<boost::mutex> lock(http_requests_mutex_);
    
    if(http_requests_.end() != http_requests_.find(handle->client_id)){
        
        return false;
        
    }
    
    http_requests_[handle->client_id] = handle;
    
    return true;
    
}

void ks_http_request_manager::release_http_clients(){
    
    boost::unique_lock<boost::mutex> lock(http_requests_mutex_);
    
    ks_request_handle* handle = nullptr;
    
    ks_http_requests::iterator it = http_requests_.begin();
    
    while(it != http_requests_.end()){
        
        handle = it->second;
        
        if(handle){
            
            delete handle->client;
            
            handle->client = nullptr;
            
            delete handle;
            
            it->second = nullptr;
            
        }
        
        ++it;
        
    }
    
    http_requests_.clear();
    
}
    
}
