#!/bin/bash

if [[ -z $1 ]]
then
    echo "Usage: $0 url"
    exit 
fi

if [ ! -a previous_version.tmp ]
then
    echo "Creating file"
    curl -s $1 > previous_version.tmp
    echo -e "Created\n"
fi

while [ 1 ];
do
    echo "Updating..."
    curl -s $1 > actual_website.tmp
    DIFF=$(diff actual_website.tmp previous_version.tmp)
    if [ "$DIFF" != "" ]
    then
        echo "$1 has been updated!"
        echo -e '\a'
        cat $DIFF
        echo ""
    else
        echo -e "$1 has not been updated\n"
    fi
    rm previous_version.tmp
    mv actual_website.tmp previous_version.tmp
    sleep 300 # 5 min
done