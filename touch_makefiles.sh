#!/bin/bash
DPATH=$1
OLD=$2
NEW=$3

echo "Will patch .mk files within the $DPATH folder."
echo "The patch will replace $OLD with $NEW" 

find $DPATH -name *.mk -type f -exec echo "Patched " {} \;
find $DPATH -name *.mk -type f -exec sed -i "s,$OLD,$NEW,g" {} \;
