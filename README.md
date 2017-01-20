# ks_http_request_performer
[![Build Status](https://travis-ci.org/KenShih522/ks_http_request_performer.svg?branch=master)](https://travis-ci.org/KenShih522/ks_http_request_performer)

A demo writtern in C++ demonstrates how to use boost library to handel HTTP requests, which allows anyone who wants to develop a cross platfrom library that can be used in different platforms.

# ks_http_request_performer.h

```
bool    ks_http_request_performer_init();

int64_t ks_http_request_object_create(const struct ks_http_request_setup_info* info);

bool    ks_http_request_object_perform_request(const int64_t client_id);
```

# How to use

See below for a simple post request example:

Step1: initial ks_http_request_performer by calling following function

```
ks_http_request_performer_init();
```

Step2: fill in ks_http_request_setup_info and create a ks_http_request_object

```

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

```
Step3: perform the request_id which is returned by ks_http_request_object_create function

```
ks_http_request_object_perform_request(request_id);
```

Xcode Sample Project: ks_http_request_performer.xcodeproj

# Requirements

- OpenSSL version: 1.0.1i
- Boost version: 1.60.0
