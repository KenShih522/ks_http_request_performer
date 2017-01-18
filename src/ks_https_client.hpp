//
//  ks_https_client.hpp
//  ks_http_request_performer
//
//  Created by Shih on 11/01/2017.
//  Copyright © 2017 kenshih522. All rights reserved.
//

#ifndef ks_https_client_hpp
#define ks_https_client_hpp

#include "ks_http_client.hpp"
#include "boost/asio.hpp"
#include "boost/asio/ssl.hpp"

namespace ks_http_request_performer
{
    
class ks_http_request;

class ks_https_client : public ks_http_client
{
    
public:
    
    ks_https_client(boost::asio::io_service& io_service);
    virtual ~ks_https_client();
    
    bool send_request();
    
private:
    
    bool send_https_request();
    
    //thread pool task
    void start_request_process();
    void on_resolve(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoint_iterator);
    void on_connect(const boost::system::error_code& error);
    void on_handshake(const boost::system::error_code& error);
    void start_send_data();
    void send_data_end(const boost::system::error_code& error, const size_t bytes_transferred);
    
    void start_receive_data();
    void receive_data_end(const boost::system::error_code& error, const size_t bytes_transferred);
    bool prepare_for_response_data();
    
    //timeout timer task
    void on_timer_task(const boost::system::error_code& error);
    
    //close
    void close_socket();
    
private:
    
    boost::asio::ssl::context ctx_;
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> socket_;
    
};

}
#endif /* ks_https_client_hpp */
