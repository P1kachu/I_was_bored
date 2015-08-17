#!/bin/bash

RED="\33[31;1m"
CYAN="\33[36;1m"
NONE="\33[0m"

echo -e $RED"Input text: "$NONE
read TEXT
echo -e $CYAN"Output ticket: "$NONE
echo TEXT | fold -ws 80