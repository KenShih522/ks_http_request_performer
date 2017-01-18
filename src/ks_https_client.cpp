//
//  ks_https_client.cpp
//  ks_http_request_performer
//
//  Created by Shih on 11/01/2017.
//  Copyright © 2017 kenshih522. All rights reserved.
//

#include "ks_https_client.hpp"
#include "ks_http_request.hpp"
#include "ks_http_response.hpp"
#include "boost/asio/ssl.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/asio/placeholders.hpp"

namespace ks_http_request_performer
{
    
ks_https_client::ks_https_client(boost::asio::io_service& io_service) : ctx_(boost::asio::ssl::context::sslv23), socket_(io_service, ctx_), ks_http_client(io_service){
    
}

ks_https_client::~ks_https_client(){
    
}

bool ks_https_client::send_request() {
    
    if (!request_){
        
        return false;
        
    }
    
    bool result = false;
    
    switch (request_->get_type()){
            
        case ks_http_request::ks_http_request_type_secure_get:
        case ks_http_request::ks_http_request_type_secure_post:
        case ks_http_request::ks_http_request_type_secure_put:
        case ks_http_request::ks_http_request_type_secure_delete:
            
            result = send_https_request();
            
        break;
            
    }
    
    return result;
    
}

bool ks_https_client::send_https_request(){
    
    timer_.expires_from_now(boost::posix_time::seconds(request_->get_timeout()));
    
    timer_.async_wait(boost::bind(&ks_https_client::on_timer_task, this, _1));
    
    start_request_process();
    
    return true;
    
}

void ks_https_client::start_request_process(){
    
    std::string port = boost::lexical_cast<std::string>(request_->get_port());
    
    boost::asio::ip::tcp::resolver::query query(request_->get_ip().c_str(), port.c_str());
    
    resolver_.async_resolve(
                            query,
                            boost::bind(&ks_https_client::on_resolve, boost::static_pointer_cast<ks_https_client>(shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::iterator)
                            );
    
}

void ks_https_client::on_resolve(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoint_iterator){
    
    if (error) {
        
        http_client_callback_(false, get_client_id(), ks_http_client_response_code_dns_resolve_failed);
        
        return;
        
    }
    
    try {
        
        socket_.lowest_layer().async_connect(*endpoint_iterator, boost::bind(&ks_https_client::on_connect, boost::static_pointer_cast<ks_https_client>(shared_from_this()), boost::asio::placeholders::error));
        
    }
    catch (std::exception& e) {
        
        http_client_callback_(false, get_client_id(), ks_http_client_response_code_connect_to_server_failed);
        
    }
    
}

void ks_https_client::on_connect(const boost::system::error_code& error){
    
    if (error) {
        
        http_client_callback_(false, get_client_id(), ks_http_client_response_code_connect_to_server_failed);
        
        return;
        
    }
    
    socket_has_connected_ = true;
    
    socket_.async_handshake(boost::asio::ssl::stream_base::client, boost::bind(&ks_https_client::on_handshake, boost::static_pointer_cast<ks_https_client>(shared_from_this()), boost::asio::placeholders::error));
    
}

void ks_https_client::on_handshake(const boost::system::error_code& error){
    
    if (error) {
        
        http_client_callback_(false, get_client_id(), ks_http_client_response_code_ssl_handshake_failed);
        
        close_socket();
        
    }
    
    start_send_data();
    
}

void ks_https_client::start_send_data(){
    
    ks_http_request_converter::convert_ks_http_request_to_request_buffer(request_, request_buffer_);
    
    boost::asio::async_write(socket_, request_buffer_.data(), boost::bind(&ks_https_client::send_data_end, boost::static_pointer_cast<ks_https_client>(shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    
}

void ks_https_client::send_data_end(const boost::system::error_code& error, const size_t bytes_transferred){
    
    if (error || bytes_transferred == 0) {
        
        close_socket();
        
        http_client_callback_(false, get_client_id(), ks_http_client_response_code_send_data_failed);
        
        return;
        
    }
    
    start_receive_data();
    
}

void ks_https_client::start_receive_data(){
    
    boost::asio::async_read(socket_, response_buffer_, boost::asio::transfer_at_least(1), boost::bind(&ks_https_client::receive_data_end, boost::static_pointer_cast<ks_https_client>(shared_from_this()), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    
    
}

void ks_https_client::receive_data_end(const boost::system::error_code& error, const size_t bytes_transferred){
    
    if (error || bytes_transferred == 0) {
        
        close_socket();
        
        http_client_callback_(false, get_client_id(), ks_http_client_response_code_response_data_wrong);
        
        return;
        
    }
    
    timer_.cancel();
    
    timer_has_been_canceled_ = true;
    
    if (prepare_for_response_data() == false) {
        
        http_client_callback_(false, get_client_id(), ks_http_client_response_code_response_data_wrong);
        
        return;
        
    }
    
    http_client_callback_(true, get_client_id(), ks_http_client_response_code_success);
    
}

bool ks_https_client::prepare_for_response_data(){
    
    std::istream response_data_stream(&response_buffer_);
    
    std::string line1;
    
    std::getline(response_data_stream, line1);
    
    if (!response_data_stream) {
        
        return false;
        
    }
    
    std::stringstream response_stream(line1);
    
    std::string http_version;
    
    response_stream >> http_version;
    
    std::string status_code;
    
    response_stream >> status_code;
    
    std::string status_message;
    
    std::getline(response_stream, status_message);
    
    if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
        
        return false;
        
    }
    
    boost::system::error_code error;
    
    while (boost::asio::read(socket_, response_buffer_, boost::asio::transfer_at_least(1), error)) {
        
        if (error) {
            
            return false;
            
        }
        
    }
    
    boost::asio::streambuf::const_buffers_type bufs = response_buffer_.data();
    
    std::string response_string(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + response_buffer_.size());
    
    std::size_t pos = response_string.find("\r\n\r\n");
    
    if (pos != std::string::npos) {
        
        response_string = response_string.substr(pos + 4);
        
    }
    
    bool success = copy_response_info_to_http_response(response_string, status_code);
    
    if (!success) {
        
        return false;
        
    }
    
    if (http_client_callback_.empty()) {
        
        return false;
        
    }
    
    return true;
}

void ks_https_client::on_timer_task(const boost::system::error_code& error){
    
    if (!error && !timer_has_been_canceled_) {
        
        http_client_callback_(false, get_client_id(), ks_http_client_response_code_timeout);
        
    }
    
    if (!error && !timer_has_been_canceled_ && socket_has_connected_) {
        
        close_socket();
        
    }
    
}

void ks_https_client::close_socket(){
    
    try {
        
        if (socket_.lowest_layer().is_open()) {
            
            boost::system::error_code ec;
            
            socket_.lowest_layer().shutdown(boost::asio::ip::tcp::socket::shutdown_send, ec);
            
            socket_.lowest_layer().close(ec);
            
            if (ec) {
                
                
            }
            
        }
        
    }
    catch (std::exception& e){
        

    }
    
}

}
