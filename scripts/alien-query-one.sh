#!/bin/bash

MY_PWD=`pwd`

PROJECT_DIR="$(dirname $(dirname $(readlink -m $0)))"

if [ ! -f $PROJECT_DIR/bin/alien-query ];then
  echo "alien-query was not found !!! Please compile it first !!!"
fi

URLS=$($PROJECT_DIR/bin/alien-query -t -d 0 -rand -rmdupli -u $USER)
if [ $? -ne 0 ];then
	echo "Error in alien-query !!!"
	exit 1
fi

for url in $URLS; do
  echo "Doing  xrdcp -f $url /tmp/test.zip ..."
  xrdcp -f $url /tmp/test.zip
  if [ $? -eq 0 ];then
    break
  fi
done
