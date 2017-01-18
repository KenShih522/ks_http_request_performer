//
//  ks_http_request_manager.hpp
//  ks_http_request_performer
//
//  Created by Shih on 11/01/2017.
//  Copyright © 2017 kenshih522. All rights reserved.
//

#ifndef ks_http_request_manager_hpp
#define ks_http_request_manager_hpp

#include <map>
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include "ks_http_client.hpp"

using namespace std;

struct ks_request_handle;
struct ks_http_request_setup_info;

namespace ks_http_request_performer
{
    
class ks_http_request;
class ks_http_response;
 
class ks_http_request_manager{
        
public:
    
    enum { k_http_request_invalid_id_setup_info_not_valid = -1 };
    
    typedef boost::function<void(const int64_t client_id, \
    const ks_http_request *request, \
    const ks_http_response *response,
    const int32_t response_code)> http_request_response_callback_function;
    
private:
    
    typedef struct ks_request_handle{
        
        int64_t client_id;
        
        boost::shared_ptr<ks_http_client>* client;
        
        http_request_response_callback_function callback;
        
    }ks_request_handle;
    
    typedef map<int64_t, ks_request_handle*> ks_http_requests;
    
    typedef ks_http_requests ks_http_process_requests;
    
public:
    
    ks_http_request_manager(const uint32_t num_background_threads);
    
    ~ks_http_request_manager();
    
    int64_t add_request(const struct ks_http_request_setup_info* setup_info, http_request_response_callback_function callback);
    
    bool start_process_request(const int64_t client_id);
    
private:
    
    int64_t generate_http_request_id();
    
    bool add_http_request_handle(ks_request_handle* handle);
    
    void process_request(const int64_t client_id);
    
    ks_request_handle* move_handle_to_process(const int64_t client_id);
    
    bool            remove_process_request_handle(const int64_t id);
    
    void http_client_callback(const bool success, const int64_t client_id, const ks_http_client::response_code type);
    
    void release_http_clients();
    
private:
    
    boost::asio::io_service        io_service_;
    
    boost::asio::io_service::work  worker_;
    
    boost::thread_group            threads_;
    
    int64_t                   current_max_http_request_id_ = 0;
    
    boost::mutex              current_max_http_request_id_mutex_;
    
    ks_http_requests          http_requests_;
    
    boost::mutex              http_requests_mutex_;
    
    ks_http_process_requests  http_process_requests_;
    
};
    
}

#endif /* ks_http_request_manager_hpp */
