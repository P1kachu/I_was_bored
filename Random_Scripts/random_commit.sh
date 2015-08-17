#!/bin/bash
# Usage ./commit [push]

COMMIT_MESSAGE=$(curl http://whatthecommit.com/ 2> /dev/null | sed '40q;d'| cut -c 4-)

echo -e "\033[35;1mgit add -A\033[0m"
git add -A

echo -e "\033[35;1mgit commit -m \033[0m\033[33;1m$COMMIT_MESSAGE\033[0m"
git commit -m "$COMMIT_MESSAGE"

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