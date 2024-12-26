#! /bin/bash

if [[ ! $# -eq 1 ]] || [[ ! $1 =~ ^[0-9]{2} ]]
  then
    echo "Usage: run.sh <day>, e.g. run.sh 02, run.sh 10, etc."
else
    set -x
    make problems/$1/sol
    if [[ -z "$DEBUG" ]]; then
      ./problems/$1/sol ./problems/$1/input
    else
      lldb ./problems/$1/sol ./problems/$1/input
    fi
fi
