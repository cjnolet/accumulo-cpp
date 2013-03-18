#!/bin/bash

cd "$(dirname "$0")"
java -cp ProxyServer.jar org.apache.accumulo.proxy.Proxy -p proxy.properties
