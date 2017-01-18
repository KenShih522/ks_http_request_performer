//
//  ks_http_request_converter.hpp
//  ks_http_request_performer
//
//  Created by Shih on 11/01/2017.
//  Copyright © 2017 kenshih522. All rights reserved.
//

#ifndef ks_http_request_converter_hpp
#define ks_http_request_converter_hpp

#include <sstream>
#include <string>
#include <boost/asio.hpp>

struct ks_http_request_setup_info;
struct ks_http_request_out;
struct ks_http_response_out;

namespace ks_http_request_performer
{
    
class ks_http_request;
class ks_http_response;

namespace ks_http_request_converter
{
    
    template<typename type_in, typename type_out> void convert_by_stream(const type_in& input, type_out& output){
        
        std::stringstream stream;
        
        stream << input;
        
        stream >> output;
        
    }
    
    std::string   convert_ks_http_request_type_to_request_method_name_string(const int32_t type);
    
    std::string   convert_ks_http_request_post_form_type_to_post_form_type_name_string(const int32_t type);
    
    bool          convert_ks_http_request_setup_info_to_ks_http_client_request(const struct ks_http_request_setup_info& input, \
                                                                            ks_http_request& output);
    
    bool          convert_ks_http_request_method_setup_info_to_ks_http_request_type(const int32_t ks_http_request_method_input, \
                                                                      int32_t& ks_http_request_type_output);
    
    bool          convert_ks_http_request_post_form_type_setup_info_to_ks_http_request_post_form_type(const int32_t ks_http_request_post_form_type_input, \
                                                                                        int32_t& ks_http_request_post_form_type_output);
    
    void          convert_ks_http_client_response_code_to_ks_http_request_response_code(const int32_t ks_http_client_response_code, \
                                                                                     int32_t& ks_http_request_response_code);
    
    void          convert_ks_http_request_to_request_buffer(const ks_http_request* input, \
                                                            boost::asio::streambuf& request_buffer);
    
    void          convert_ks_http_request_to_ks_http_request_out(const ks_http_request& input, \
                                                          struct ks_http_request_out& output);
    
    void          convert_ks_http_response_to_ks_http_response_out(const ks_http_response& input, \
                                                            struct ks_http_response_out& output);
    
    
}
    
}

#endif /* ks_http_request_converter_hpp */
