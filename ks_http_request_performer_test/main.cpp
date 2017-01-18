//
//  main.cpp
//  ks_http_request_performer_test
//
//  Created by Shih on 11/01/2017.
//  Copyright © 2017 kenshih522. All rights reserved.
//

#include <iostream>
#include <unistd.h>
#include "ks_http_request_performer.h"

void http_request_respone_callback(const ks_http_request_result* result, void* arg);

int main(int argc, const char * argv[]) {
    
    ks_http_request_performer_init();
    
    while (1) {
        
        printf("1.(http)   Returns Origin IP   (Get)\t \n");
        printf("2.(http)   Returns POST data   (Post Text)\t \n");
        printf("3.(https)  Returns POST data   (Post JSON)\t \n");
        printf("4.(https)  Returns Put data    (Put)\t \n");
        printf("5.(https)  Returns Delete data (Delete)\t \n");
        printf("6.(https)  Returns header      (Get)\t \n");
        printf("7.(https)  Returns page containing a JPEG image    (Get)\t \n");
        printf("8.(https)  Returns page containing a PNG image     (Get)\t \n");

        printf("Please select function:");
        
        int input_answer;
        
        scanf("%d",&input_answer);
        
        switch (input_answer) {
                
            case 1:{
                
                ks_http_request_setup_info setup;
                
                memset(&setup, 0, sizeof(setup));
                
                strcpy(setup.ip, "httpbin.org");
                
                strcpy(setup.uri, "/ip");
            
                setup.port = 80;
                
                setup.request_method = ks_http_request_method_get;
                
                setup.post_form_type = ks_http_request_post_form_type_text;
                
                setup.callback.response = http_request_respone_callback;
                
                setup.callback.response_user_arg = nullptr;
                
                int64_t request_id = ks_http_request_object_create(&setup);
                
                ks_http_request_object_perform_request(request_id);
                                
            }
                
            break;
                
            case 2:{
                
                ks_http_request_setup_info setup;
                
                memset(&setup, 0, sizeof(setup));
                
                strcpy(setup.ip, "httpbin.org");
                
                strcpy(setup.uri, "/post");
                
                std::string body_string = "parmater1=apple&parmater2=pen";
                
                size_t body_size = body_string.size();
                
                setup.body = (uint8_t*)malloc(static_cast<size_t>(body_size));
                
                memset(setup.body, 0, body_size);
                
                memcpy(setup.body, body_string.c_str(), body_size);
                
                setup.body_size = body_size;
                
                setup.port = 80;
                
                setup.request_method = ks_http_request_method_post;
                
                setup.post_form_type = ks_http_request_post_form_type_text;
                
                setup.callback.response = http_request_respone_callback;
                
                setup.callback.response_user_arg = nullptr;
                
                int64_t request_id = ks_http_request_object_create(&setup);
                
                ks_http_request_object_perform_request(request_id);
                
                free(setup.body);
                
            }
            
            break;
                
            case 3:{
                
                ks_http_request_setup_info setup;
                
                memset(&setup, 0, sizeof(setup));
                
                strcpy(setup.ip, "httpbin.org");
                
                strcpy(setup.uri, "/post");
                
                std::string body_string = "{\"parmater1\":\"apple\",\"parmater2\":\"pen\"}";
                
                size_t body_size = body_string.size();
                
                setup.body = (uint8_t*)malloc(static_cast<size_t>(body_size));
                
                memset(setup.body, 0, body_size);
                
                memcpy(setup.body, body_string.c_str(), body_size);
                
                setup.body_size = body_size;
                
                setup.port = 443;
                
                setup.request_method = ks_http_request_method_secure_post;
                
                setup.post_form_type = ks_http_request_post_form_type_json;
                
                setup.callback.response = http_request_respone_callback;
                
                setup.callback.response_user_arg = nullptr;
                
                int64_t request_id = ks_http_request_object_create(&setup);
                
                ks_http_request_object_perform_request(request_id);
                
                free(setup.body);
                
            }
                
            break;
                
            case 4:{
                
                ks_http_request_setup_info setup;
                
                memset(&setup, 0, sizeof(setup));
                
                strcpy(setup.ip, "httpbin.org");
                
                strcpy(setup.uri, "/put");
                
                std::string body_string = "parmater1=apple&parmater2=pen";
                
                size_t body_size = body_string.size();
                
                setup.body = (uint8_t*)malloc(static_cast<size_t>(body_size));
                
                memset(setup.body, 0, body_size);
                
                memcpy(setup.body, body_string.c_str(), body_size);
                
                setup.body_size = body_size;
                
                setup.port = 443;
                
                setup.request_method = ks_http_request_method_secure_put;
                
                setup.post_form_type = ks_http_request_post_form_type_text;
                
                setup.callback.response = http_request_respone_callback;
                
                setup.callback.response_user_arg = nullptr;
                
                int64_t request_id = ks_http_request_object_create(&setup);
                
                ks_http_request_object_perform_request(request_id);
                
                free(setup.body);
                
            }
                
            break;
                
            case 5:{
                
                ks_http_request_setup_info setup;
                
                memset(&setup, 0, sizeof(setup));
                
                strcpy(setup.ip, "httpbin.org");
                
                strcpy(setup.uri, "/delete");
                
                std::string body_string = "parmater1=apple&parmater2=pen";
                
                size_t body_size = body_string.size();
                
                setup.body = (uint8_t*)malloc(static_cast<size_t>(body_size));
                
                memset(setup.body, 0, body_size);
                
                memcpy(setup.body, body_string.c_str(), body_size);
                
                setup.body_size = body_size;
                
                setup.port = 443;
                
                setup.request_method = ks_http_request_method_secure_delete;
                
                setup.post_form_type = ks_http_request_post_form_type_text;
                
                setup.callback.response = http_request_respone_callback;
                
                setup.callback.response_user_arg = nullptr;
                
                int64_t request_id = ks_http_request_object_create(&setup);
                
                ks_http_request_object_perform_request(request_id);
                
                free(setup.body);
                
            }
                
            break;
                
            case 6:{
                
                ks_http_request_setup_info setup;
                
                memset(&setup, 0, sizeof(setup));
                
                strcpy(setup.ip, "httpbin.org");
                
                strcpy(setup.uri, "/headers");
                
                std::string body_string = "parmater1=apple&parmater2=pen";
                
                size_t body_size = body_string.size();
                
                setup.body = (uint8_t*)malloc(static_cast<size_t>(body_size));
                
                memset(setup.body, 0, body_size);
                
                memcpy(setup.body, body_string.c_str(), body_size);
                
                setup.body_size = body_size;
                
                setup.port = 443;
                
                setup.request_method = ks_http_request_method_secure_get;
                
                setup.post_form_type = ks_http_request_post_form_type_text;
                
                setup.callback.response = http_request_respone_callback;
                
                setup.callback.response_user_arg = nullptr;
                
                strcpy(setup.header_json, "{\"parmater1\":\"apple\",\"parmater2\":\"pen\"}");
                
                int64_t request_id = ks_http_request_object_create(&setup);
                
                ks_http_request_object_perform_request(request_id);
                
                free(setup.body);
                
            }
                
            break;
                
            case 7:{
                
                ks_http_request_setup_info setup;
                
                memset(&setup, 0, sizeof(setup));
                
                strcpy(setup.ip, "httpbin.org");
                
                strcpy(setup.uri, "/image/jpeg");
                
                setup.port = 443;
                
                setup.request_method = ks_http_request_method_secure_get;
                
                setup.post_form_type = ks_http_request_post_form_type_text;
                
                setup.callback.response = http_request_respone_callback;
                
                setup.callback.response_user_arg = nullptr;
                
                int64_t request_id = ks_http_request_object_create(&setup);
                
                ks_http_request_object_perform_request(request_id);
                
            }
                
            break;
                
            case 8:{
                
                ks_http_request_setup_info setup;
                
                memset(&setup, 0, sizeof(setup));
                
                strcpy(setup.ip, "httpbin.org");
                
                strcpy(setup.uri, "/image/png");
                
                setup.port = 443;
                
                setup.request_method = ks_http_request_method_secure_get;
                
                setup.post_form_type = ks_http_request_post_form_type_text;
                
                setup.callback.response = http_request_respone_callback;
                
                setup.callback.response_user_arg = nullptr;
                
                int64_t request_id = ks_http_request_object_create(&setup);
                
                ks_http_request_object_perform_request(request_id);
                
            }
                
            break;
                
        }
        
        sleep(3);
        
    }
    
    return 0;
    
}
void http_request_respone_callback(const ks_http_request_result* result, void* arg){
    
    printf("http_request_respone_callback\n");
    
    printf("response code: %d\n", result->response_code);
    
    if(result->response->response_data){
        
        uint32_t data_size = result->response->response_data_size;
        
        uint8_t* response = (uint8_t*)malloc(data_size+1);
        
        memset(response, 0, data_size+1);
        
        memcpy(response, result->response->response_data, data_size);
        
        printf("response: %s\n\n",response);
        
        if(!strcmp(result->request->uri, "/image/jpeg")){
            
            FILE* pFile;
            
            pFile = fopen("//Users//Shih//Desktop//ks_http_request_performer//ks_http_request_performer_test//123.jpeg","wb");
            
            if (pFile){
                
                fwrite(response, sizeof(uint8_t), data_size, pFile);
                
            }
            
            fclose(pFile);
            
        }
        else if(!strcmp(result->request->uri, "/image/png")){
            
            FILE* pFile;
            
            pFile = fopen("//Users//Shih//Desktop//ks_http_request_performer//ks_http_request_performer_test//123.png","wb");
            
            if (pFile){
                
                fwrite(response, sizeof(uint8_t), data_size, pFile);
                
            }
            
            fclose(pFile);
            
        }
        
        free(response);
        
    }
    
}
