//
//  ks_http_client.cpp
//  ks_http_request_performer
//
//  Created by Shih on 11/01/2017.
//  Copyright © 2017 kenshih522. All rights reserved.
//

#include "ks_http_client.hpp"
#include "ks_http_request.hpp"
#include "ks_http_response.hpp"
#include "boost/lexical_cast.hpp"
#include "boost/asio/placeholders.hpp"

namespace ks_http_request_performer
{
    
ks_http_client::ks_http_client(boost::asio::io_service& io_service)  : request_(nullptr), \
    response_(nullptr), \
    resolver_(io_service), \
    socket_(io_service), \
    timer_(io_service), \
    socket_has_connected_(false), \
    timer_has_been_canceled_(false){
    
    prepare_http_response();
    
}

ks_http_client::~ks_http_client(){
    
    release_request();
    
    release_response();
    
}

void ks_http_client::setup_request(const ks_http_request& request){
    
    if (!request_){
        
        request_ = new ks_http_request;
    }
    
    request_->clone_from(request);
    
}

bool ks_http_client::send_request(){
    
    if (!request_){
        
        return false;
        
    }
    
    bool result = false;
    
    switch (request_->get_type()){
            
        case ks_http_request::ks_http_request_type_get:
        case ks_http_request::ks_http_request_type_post:
        case ks_http_request::ks_http_request_type_put:
        case ks_http_request::ks_http_request_type_delete:
            
            result = send_http_request();

        break;
            
    }
    
    return result;
    
}

void ks_http_client::cancel_request(){
    
    //not implement
    
}

bool ks_http_client::send_http_request(){
    
    timer_.expires_from_now(boost::posix_time::seconds(request_->get_timeout()));
    
    timer_.async_wait(boost::bind(&ks_http_client::on_timer_task, this, _1));
    
    start_request_process();
    
    return true;
    
}

void ks_http_client::start_request_process(){
    
    std::string port = boost::lexical_cast<std::string>(request_->get_port());
    
    boost::asio::ip::tcp::resolver::query query(request_->get_ip().c_str(), port.c_str());
    
    resolver_.async_resolve(
                            query,
                            boost::bind(&ks_http_client::on_resolve, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::iterator)
                            );
    
}

void ks_http_client::on_resolve(const boost::system::error_code& error, boost::asio::ip::tcp::resolver::iterator endpoint_iterator){
    
    if (error) {
        
        http_client_callback_(false, get_client_id(), ks_http_client_response_code_dns_resolve_failed);
        
        return;
        
    }
    
    boost::asio::async_connect(
                               socket_,
                               endpoint_iterator,
                               boost::bind(&ks_http_client::on_connect, shared_from_this(), boost::asio::placeholders::error)
                               );
    
}

void ks_http_client::on_connect(const boost::system::error_code& error){
    
    if (error) {
        
        http_client_callback_(false, get_client_id(), ks_http_client_response_code_connect_to_server_failed);
        
        return;
        
    }
    
    socket_has_connected_ = true;
    
    start_send_data();
    
}

void ks_http_client::start_send_data(){
    
    ks_http_request_converter::convert_ks_http_request_to_request_buffer(request_, request_buffer_);
    
    boost::asio::async_write(socket_, request_buffer_.data(), boost::bind(&ks_http_client::send_data_end, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    
}

void ks_http_client::send_data_end(const boost::system::error_code& error, const size_t bytes_transferred){
    
    if (error || bytes_transferred == 0) {
        
        close_socket();
        
        http_client_callback_(false, get_client_id(), ks_http_client_response_code_send_data_failed);
        
        return;
        
    }
    
    start_receive_data();
    
}

void ks_http_client::start_receive_data(){
    
    boost::asio::async_read(socket_, response_buffer_, boost::asio::transfer_at_least(1), boost::bind(&ks_http_client::receive_data_end, shared_from_this(), boost::asio::placeholders::error, boost::asio::placeholders::bytes_transferred));
    
}

void ks_http_client::receive_data_end(const boost::system::error_code& error, const size_t bytes_transferred){
    
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

bool ks_http_client::prepare_for_response_data(){
    
    std::istream response_data_stream(&response_buffer_);
    
    std::string line1;
    
    std::getline(response_data_stream, line1);
    
    if (!response_data_stream) {
        
        return false;
        
    }
    
    std::stringstream response_stream(line1);
    
    std::string http_version;
    
    response_stream >> http_version;
    
    std::string status_code_string;
    
    response_stream >> status_code_string;
    
    std::string status_message;
    
    std::getline(response_stream, status_message);
    
    if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
        
        return false;
        
    }
    
    boost::system::error_code error;
    
    while (boost::asio::read(socket_, response_buffer_, boost::asio::transfer_at_least(1), error)){
        
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
    
    bool success = copy_response_info_to_http_response(response_string, status_code_string);
    
    if (!success) {
        
        return false;
        
    }
    
    if (http_client_callback_.empty()){
        
        return false;
        
    }
    
    return true;
    
}

void ks_http_client::on_timer_task(const boost::system::error_code& error){
    
    if (!error && !timer_has_been_canceled_) {
        
        http_client_callback_(false, get_client_id(), ks_http_client_response_code_timeout);
        
    }
    
    if (!error && !timer_has_been_canceled_ && socket_has_connected_) {
        
        close_socket();
        
    }
    
}

void ks_http_client::close_socket(){
    
    if (socket_.is_open()) {
        
        socket_.close();
        
    }
    
}

void ks_http_client::set_ks_http_client_callback(ks_http_client_callback callback){
    
    boost::unique_lock<boost::mutex> lock(http_client_callback_mutex_);
    
    http_client_callback_ = callback;
    
}

void ks_http_client::reset_ks_http_client_callback(){
    
    boost::unique_lock<boost::mutex> lock(http_client_callback_mutex_);
    
    http_client_callback_.clear();
    
}

void ks_http_client::release_request(){
    
    delete request_;
    
    request_ = nullptr;
    
}

void ks_http_client::release_response(){
    
    delete response_;
    
    response_ = nullptr;
    
}

void ks_http_client::prepare_http_response(){
    
    if (!response_){
        
        response_ = new ks_http_response;
        
    }
    
}

bool ks_http_client::copy_response_info_to_http_response(const std::string response_string, const std::string status_code_string){
    
    uint8_t* temp = (uint8_t *)malloc(response_string.length() + 1);
    
    memset(temp, 0, response_string.length() + 1);
    
    memcpy(temp, response_string.c_str(), response_string.size());
    
    int32_t status_code = -1;
    
    try {
        
        status_code = boost::lexical_cast<int32_t>(status_code_string.c_str());
        
    }
    catch (boost::bad_lexical_cast const&) {
        
        return false;
        
    }
    
    response_->set_code(status_code);
    
    bool data_copied = response_->copy_data_from(temp, response_string.size() + 1);
    
    if (!data_copied){
        
    }
    
    free(temp);
    
    return true;
    
}

}
