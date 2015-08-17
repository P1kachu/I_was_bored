#!/bin/bash

RED="\33[31;1m"
CYAN="\33[36;1m"
NONE="\33[0m"

SIGNATURE="\nStanislas 'P1kachu' Lejay
\nEPITA 2018
\nIf you're sleeping, you're doing it wrong\n"


echo -e $RED"Input text: "$NONE
read TEXT
echo -e $CYAN"Output ticket: "$NONE
echo TEXT | fold -ws 70
echo -e $SIGNATURE