//
//  ks_http_request_converter.cpp
//  ks_http_request_performer
//
//  Created by Shih on 11/01/2017.
//  Copyright © 2017 kenshih522. All rights reserved.
//

#include "ks_http_request_converter.hpp"
#include "ks_http_request_performer_types.h"
#include "ks_http_request.hpp"
#include "ks_http_response.hpp"
#include "ks_http_client.hpp"
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>

namespace ks_http_request_performer
{
    
namespace ks_http_request_converter
{
    
    std::string convert_ks_http_request_type_to_request_method_name_string(const int32_t type){
        
        std::string method_name_string = "";
        
        switch (type) {
                
            case ks_http_request::ks_http_request_type_get:
            case ks_http_request::ks_http_request_type_secure_get:
                
                method_name_string = "GET";
                
                break;
                
            case ks_http_request::ks_http_request_type_post:
            case ks_http_request::ks_http_request_type_secure_post:
                
                method_name_string = "POST";
                
                break;
                
            case ks_http_request::ks_http_request_type_put:
            case ks_http_request::ks_http_request_type_secure_put:
                
                method_name_string = "PUT";
                
                break;
                
            case ks_http_request::ks_http_request_type_delete:
            case ks_http_request::ks_http_request_type_secure_delete:
                
                method_name_string = "DELETE";
                
                break;
                
        }
        
        return method_name_string;
        
    }
    
    std::string convert_ks_http_request_post_form_type_to_post_form_type_name_string(const int32_t type){
        
        std::string post_form_type_name_string = "";
        
        switch (type) {
                
            case ks_http_request::ks_http_request_post_form_type_text:
                
                post_form_type_name_string = "application/x-www-form-urlencoded";
                
                break;
                
            case ks_http_request::ks_http_request_post_form_type_json:
                
                post_form_type_name_string = "application/json";
                
                break;
                
        }
        
        return post_form_type_name_string;
        
    }
    
    bool convert_ks_http_request_setup_info_to_ks_http_client_request(const struct ks_http_request_setup_info& input, ks_http_request& output){
        
        int32_t request_type = 0;
        
        int32_t post_form_type = 0;
        
        bool type_ok = convert_ks_http_request_method_setup_info_to_ks_http_request_type(input.request_method, request_type);
        
        if (!type_ok){
            
            return false;
            
        }
        
        type_ok = convert_ks_http_request_post_form_type_setup_info_to_ks_http_request_post_form_type(input.post_form_type, post_form_type);
        
        if (!type_ok){
            
            return false;
            
        }
        
        output.set_type(request_type);
        
        output.set_post_form_type(post_form_type);
        
        output.set_ip(input.ip);
        
        output.set_port(input.port);
        
        output.set_path(input.uri);
                
        const uint8_t* body_data = input.body;
        
        size_t body_data_size = input.body_size;
        
        std::string content(body_data, body_data + body_data_size);
        
        output.set_content(content);
        
        output.set_header_content(input.header_json);
        
        if(strcmp(input.header_json, "")){
            
            try{
                
                std::stringstream ss;
                
                ss << input.header_json;
                
                boost::property_tree::ptree pt;
                
                boost::property_tree::read_json(ss, pt);
                
                for (boost::property_tree::ptree::const_iterator it = pt.begin(); it != pt.end(); ++it){
                    
                    output.add_header_param(it->first, it->second.get_value<std::string>());
                    
                }
                
            }
            catch(std::exception const& e){
                
                printf("input header is not a json format\n");

                std::cerr << e.what() << std::endl;
                
                return false;
                
            }
            
        }
        
        return true;
        
    }
    
    bool convert_ks_http_request_method_setup_info_to_ks_http_request_type(const int32_t ks_http_request_post_form_type_input, int32_t& ks_http_request_post_form_type_output){
        
        bool type_ok = true;
        
        switch (ks_http_request_post_form_type_input){
                
            case ks_http_request_method_get:
                
                ks_http_request_post_form_type_output = ks_http_request::ks_http_request_type_get;
                
                break;
                
            case ks_http_request_method_post:
                
                ks_http_request_post_form_type_output = ks_http_request::ks_http_request_type_post;
                
                break;
                
            case ks_http_request_method_put:
                
                ks_http_request_post_form_type_output = ks_http_request::ks_http_request_type_put;
                
                break;
                
            case ks_http_request_method_delete:
                
                ks_http_request_post_form_type_output = ks_http_request::ks_http_request_type_delete;
                
                break;
                
            case ks_http_request_method_secure_get:
                
                ks_http_request_post_form_type_output = ks_http_request::ks_http_request_type_secure_get;
                
                break;
                
            case ks_http_request_method_secure_post:
                
                ks_http_request_post_form_type_output = ks_http_request::ks_http_request_type_secure_post;
                
                break;
                
            case ks_http_request_method_secure_put:
                
                ks_http_request_post_form_type_output = ks_http_request::ks_http_request_type_secure_put;
                
                break;
                
            case ks_http_request_method_secure_delete:
                
                ks_http_request_post_form_type_output = ks_http_request::ks_http_request_type_secure_delete;
                
                break;
                
            default:
                
                type_ok = false;
                
                break;
                
        };
        
        return type_ok;
        
    }
    
    bool convert_ks_http_request_post_form_type_setup_info_to_ks_http_request_post_form_type(const int32_t ks_http_request_post_form_type_input, int32_t& ks_http_request_post_form_type_output){
        
        bool type_ok = true;
        
        switch (ks_http_request_post_form_type_input){
                
            case ks_http_request_post_form_type_text:
                
                ks_http_request_post_form_type_output = ks_http_request::ks_http_request_post_form_type_text;
                
                break;
                
            case ks_http_request_post_form_type_json:
                
                ks_http_request_post_form_type_output = ks_http_request::ks_http_request_post_form_type_json;
                
                break;
                
            default:
                
                type_ok = false;
                
                break;
                
        };
        
        return type_ok;
        
    }
    
    void convert_ks_http_client_response_code_to_ks_http_request_response_code(const int32_t ks_http_client_response_code, \
                                                                            int32_t& ks_http_request_response_code){
        switch (ks_http_client_response_code){
                
            case ks_http_client::ks_http_client_response_code_success:
                
                ks_http_request_response_code = ks_http_request_result_response_code_success;
                
                break;
                
            case ks_http_client::ks_http_client_response_code_internal_error:
                
                ks_http_request_response_code = ks_http_request_result_response_code_internal_error;
                
                break;
                
                
            case ks_http_client::ks_http_client_response_code_timeout:
                
                ks_http_request_response_code = ks_http_request_result_response_code_timeout;
                
                break;
                
            case ks_http_client::ks_http_client_response_code_dns_resolve_failed:
            case ks_http_client::ks_http_client_response_code_connect_to_server_failed:
            case ks_http_client::ks_http_client_response_code_send_data_failed:
            case ks_http_client::ks_http_client_response_code_ssl_handshake_failed:
                
                ks_http_request_response_code = ks_http_request_result_response_code_network_error;
                
                break;
                
            case ks_http_client::ks_http_client_response_code_response_data_wrong:
                
                ks_http_request_response_code = ks_http_request_result_response_code_response_error;
                
                break;
                
        };
        
    }
    
    void convert_ks_http_request_to_request_buffer(const ks_http_request* request, \
                                                   boost::asio::streambuf& request_buffer){
        
        std::string host = request->get_ip();
        
        std::string uri = request->get_uri();
        
        int32_t request_type = request->get_type();
        
        int32_t request_post_form_type = request->get_post_form_type();
        
        std::string request_method_name = ks_http_request_converter::convert_ks_http_request_type_to_request_method_name_string(request_type);
        
        std::string content_type_name = ks_http_request_converter::convert_ks_http_request_post_form_type_to_post_form_type_name_string(request_post_form_type);
        
        std::string content = request->get_content();
        
        std::string::size_type content_length = request->get_content().size();
        
        std::ostream request_stream(&request_buffer);
        
        request_stream << request_method_name << " " << uri.c_str() << " HTTP/1.0\r\n";
        
        request_stream << "Host: " << host.c_str() << "\r\n";
        
        request_stream << "Accept: */*\r\n";
        
        request_stream << "Content-Type: " << content_type_name.c_str() << "\r\n";
        
        ks_http_request::header_params header;
        
        request->get_all_header_params(header);
        
        for (ks_http_request::header_params::iterator it = header.begin(); it != header.end(); it++){
            
            request_stream << it->first << ": " << it->second << "\r\n";
            
        }
        
        if(request_type == ks_http_request::ks_http_request_type_post ||
           request_type == ks_http_request::ks_http_request_type_delete ||
           request_type == ks_http_request::ks_http_request_type_put ||
           request_type == ks_http_request::ks_http_request_type_secure_post ||
           request_type == ks_http_request::ks_http_request_type_secure_delete ||
           request_type == ks_http_request::ks_http_request_type_secure_put){
            
            request_stream << "Content-Length: " << content_length << "\r\n";
            
            request_stream << "Connection: close\r\n";
            
            request_stream << "\r\n";
            
            request_stream << content.c_str() << "\r\n";
            
            request_stream << "\r\n";
            
        }
        else{
            
            request_stream << "Connection: close\r\n";
            
            request_stream << "\r\n";
            
        }
        
    }
    
    void convert_ks_http_request_to_ks_http_request_out(const ks_http_request& input, \
                                                 struct ks_http_request_out& output){
        
        strncpy(output.ip, input.get_ip().c_str(), sizeof(output.ip));
        
        output.port = input.get_port();
        
        strncpy(output.uri, input.get_uri().c_str(), sizeof(output.uri));
        
        output.request_method = input.get_type();
        
        output.post_form_type = input.get_post_form_type();
        
        strncpy(output.header_json, input.get_header_content().c_str(), sizeof(output.header_json));

        output.body = nullptr;
        
        uint8_t* data = const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(input.get_content().c_str()));
        
        size_t data_size = input.get_content().size();
        
        if (data && 0 < data_size){
            
            output.body_size = data_size;
            
            output.body = (uint8_t*)malloc(data_size+1);
            
            memset(output.body, 0, data_size+1);
            
            memcpy(output.body, data, output.body_size);
            
        }
        
    }
    
    void convert_ks_http_response_to_ks_http_response_out(const ks_http_response& input, struct ks_http_response_out& output){
        
        output.http_code = input.get_code();
        
        strncpy(output.http_code_msg, input.get_code_msg().c_str(), sizeof(output.http_code_msg));
        
        const uint8_t* data = input.get_data_buffer();
        
        uint32_t data_size = input.get_data_size();
        
        output.response_data = nullptr;
        
        if (data && 0 < data_size){
            
            output.response_data_size = data_size;
            
            output.response_data = (uint8_t*)malloc(data_size+1);
            
            memset(output.response_data, 0, data_size+1);
            
            memcpy(output.response_data, data, output.response_data_size);
            
        }
        
    }
    
}
    
}
