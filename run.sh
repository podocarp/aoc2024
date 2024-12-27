#! /bin/bash

if [[ $# -eq 0 ]] || [[ ! $1 =~ ^[0-9]{2} ]]
  then
    echo "Usage: run.sh <day> [input], e.g. run.sh 02, run.sh 10 ./input, etc."
else
    set -x
    make problems/$1/sol
    INPUT="./problems/$1/input"

    if [[ ! -z "$2" ]]; then
      INPUT="$2"
    fi
    
    if [[ -z "$DEBUG" ]]; then
      ./problems/$1/sol $INPUT
    else
      lldb ./problems/$1/sol $INPUT
    fi
fi
