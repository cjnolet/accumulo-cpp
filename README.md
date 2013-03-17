Accumulo++
==========

A C++ library for Apache Accumulo

## Why a C++ Library for Accumulo?

Simple- When I'm running logic in InfoSphere Streams, I'd like to talk to Accumulo in C/C++. If I'm running code that needs to utilize CUDA and the GPU, I'd like to stay in C/C++ also. In Accumulo 1.5, we were given an Apache Thrift-based proxy that allows us to generate clients in differing languages. This is one such client which has been augmented to look as close as possible to the Java-based API that we've all come to know and love.

## Requirements

1. A running Accumulo cluster
2. The new Accumulo Thrift Proxy server (included in this codebase)
3. Thrift C++ library installed
4. Boost C++ Library installed
