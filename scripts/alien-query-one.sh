#!/bin/bash

MY_PWD=`pwd`

PROJECT_DIR="$(dirname $(dirname $(readlink -m $0)))"

if [ ! -f $PROJECT_DIR/bin/alien-query ];then
  echo "alien-query was not found !!! Please compile it first !!!"
fi

FILE_TO_TEST="$1"
if [ -z "$FILE_TO_TEST" ];then
  FILE_TO_TEST="-t"
fi

URLS=$($PROJECT_DIR/bin/alien-query -d 0 -rand -rmdupli -u $USER $FILE_TO_TEST)
if [ $? -ne 0 ];then
  echo "Error in alien-query !!!"
  exit 1
fi

OUT_FILE="/tmp/test.zip"
RET_COPY=1
for url in $URLS; do
  echo "Doing xrdcp -f $url $OUT_FILE ..."
  xrdcp -f $url $OUT_FILE
  if [ $? -eq 0 ];then
    # check if it is not file with zero size
    if [ ! -s $OUT_FILE ];then
      echo "File size is zero !!! Trying next copy ..."
      continue
    fi
    # check if zip is OK
    testZip=$(zip -T $OUT_FILE)
    if [ $? -ne 0 ];then
      echo "Zip test failed !!! Trying next copy ..."
      continue
    fi
    RET_COPY=0
    break
  fi
done


if [ $RET_COPY -eq 0 ];then
  echo "Copy OK"
else
  echo "Copy failed !!!"
fi

exit $RET_COPY
