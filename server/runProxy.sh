#!/bin/bash

cd "$(dirname "$0")"

if [ -z $1 ]; then
	echo "Usage: runProy.sh <version>"
else

	java -cp ProxyServer$1.jar org.apache.accumulo.proxy.Proxy -p proxy.properties

fi
