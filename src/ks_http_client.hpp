//
//  ks_http_client.hpp
//  ks_http_request_performer
//
//  Created by Shih on 11/01/2017.
//  Copyright © 2017 kenshih522. All rights reserved.
//

#ifndef ks_http_client_hpp
#define ks_http_client_hpp

#include "ks_client.hpp"
#include <boost/asio.hpp>
#include <boost/thread.hpp>

namespace ks_http_request_performer
{

class ks_http_request;
class ks_http_response;

class ks_http_client : public ks_client, public boost::enable_shared_from_this<ks_http_client>{
    
public:
    
    enum response_code
    {
        ks_http_client_response_code_success,
        ks_http_client_response_code_internal_error,
        ks_http_client_response_code_timeout,
        ks_http_client_response_code_dns_resolve_failed,
        ks_http_client_response_code_connect_to_server_failed,
        ks_http_client_response_code_send_data_failed,
        ks_http_client_response_code_ssl_handshake_failed,
        ks_http_client_response_code_response_data_wrong,
    };
    
    typedef boost::function<void(const bool success, const uint64_t client_id, const response_code& code)> ks_http_client_callback;
    
public:
    
    ks_http_client(boost::asio::io_service& io_service);
    ~ks_http_client();
    
    void setup_request(const ks_http_request& request);
    virtual bool send_request();
    
    void cancel_request();
    
    void set_ks_http_client_callback(ks_http_client_callback callback);
    void reset_ks_http_client_callback();
    
    const ks_http_request*  get_request() const;
    const ks_http_response* get_response() const;
    
protected:
    
    bool copy_response_info_to_http_response(const std::string response_string, const std::string status_code_string);
    
    bool get_cancel() const;
    void set_cancel(const bool cancel);
    
private:
    
    bool send_http_request();
    
    //thread pool task
    void start_request_process();
    void on_resolve(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
    void on_connect(const boost::system::error_code& error);
    void start_send_data();
    void send_data_end(const boost::system::error_code& error, const size_t bytes_transferred);
    void start_receive_data();
    void receive_data_end(const boost::system::error_code& error, const size_t bytes_transferred);
    bool prepare_for_response_data();
    
    //timeout timer task
    void on_timer_task(const boost::system::error_code& error);
    
    //close
    void close_socket();
    
    void prepare_http_response();
    
    void release_events();
    void release_request();
    void release_response();
    
protected:
    
    ks_http_request*               request_;
    ks_http_response*              response_;
    
    boost::asio::ip::tcp::resolver resolver_;
    boost::asio::streambuf         request_buffer_;
    boost::asio::streambuf         response_buffer_;
    boost::asio::deadline_timer    timer_;
    bool                           socket_has_connected_;
    bool                           timer_has_been_canceled_;
    
    ks_http_client_callback http_client_callback_;
    boost::mutex            http_client_callback_mutex_;
    
private:
    
    boost::asio::ip::tcp::socket   socket_;
    
    bool         cancel_;
    
};

inline const ks_http_request* ks_http_client::get_request() const
{
    return request_;
}

inline const ks_http_response* ks_http_client::get_response() const
{
    return response_;
}

inline bool ks_http_client::get_cancel() const
{
    return cancel_;
}

inline void ks_http_client::set_cancel(const bool cancel)
{
    cancel_ = cancel;
}

}

#endif /* ks_http_client_hpp */
