# ks_http_request_performer
[![Build Status](https://travis-ci.org/KenShih522/ks_http_request_performer.svg?branch=master)](https://travis-ci.org/KenShih522/ks_http_request_performer)

A demo writtern in C++ demonstrates how to use boost library to handel HTTP requests, which allows anyone who wants to develop a cross platfrom library that can be used in different platforms. The demo is Based on Boost and OpenSSL.

# ks_http_request_performer.h

```
bool    ks_http_request_performer_init();

int64_t ks_http_request_object_create(const struct ks_http_request_setup_info* info);

bool    ks_http_request_object_perform_request(const int64_t client_id);
```

# How to use

Xcode Sample Project: ks_http_request_performer.xcodeproj

# Requirements

- OpenSSL version: 1.0.1i
- Boost version: 1.60.0
