#!/bin/bash

find . -type f -name '*.tmp' -exec rm -r {} \;
rm -rf build/ dist/ *.egg-info/

echo -e "\033[36;1m- - - - - - - - - - - - - - - - - \033[0m"


VERSION=`grep 'version=' setup.py | awk -F\' '{print $2}'`
echo -ne "\033[35;1mUpdate setup.py ? (Actual version: $VERSION)\033[0m: "
read NEW_VERSION

if [[ "$NEW_VERSION" != "n" ]]; then
    sed -i -e "s/\(version=\).*/\1\'${NEW_VERSION}\',/" setup.py
    echo -e "\t\033[36;1mVersion updated\033[0m"
fi

echo -e "\033[35;1mgit add -A\033[0m"
git add -A

echo -ne "\033[35;1mgit commit -m:\033[0m "
read MESSAGE
git commit -m "$MESSAGE"

echo -ne "\033[35;1mgit status\033[0m: "
git status -sb

if [[ "$1" == "push" ]]
then
    echo -ne "\033[35;1mgit push origin master\033[0m: "
    PUSH='git push origin master'
    if [[ $? -ne 0 ]]; then
        echo $PUSH
    fi
fi
echo -e "\033[36;1m- - - - - - - - - - - - - - - - - \033[0m"