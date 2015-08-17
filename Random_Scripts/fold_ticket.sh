#!/bin/bash

RED="\033[31;1m"
CYAN="\033[36;1m"
NONE="\033[0m"

SIGNATURE="\nStanislas 'P1kachu' Lejay
\nEPITA 2018
\nIf you're sleeping, you're doing it wrong\n"


echo -e $RED"\nInput text: "$NONE
read TEXT
echo -e $CYAN"\nOutput ticket: "$NONE
echo $TEXT | fold -s -w 70
echo -e $SIGNATURE
