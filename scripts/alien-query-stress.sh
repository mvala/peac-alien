#!/bin/bash

MY_PWD=`pwd`

PROJECT_DIR="$(dirname $(dirname $(readlink -m $0)))"

if [ ! -f $PROJECT_DIR/bin/alien-query ];then
  echo "alien-query was not found !!! Please compile it first !!!"
fi

OUT_FILE="/tmp/alien-query-stress.out"

rm -f $OUT_FILE

for i in `seq 1 100`;do
  nohup $PROJECT_DIR/scripts/alien-query-one.sh >> $OUT_FILE &
done