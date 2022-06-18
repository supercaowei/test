#!/bin/bash

# set -v
# echo "hello"
# set +v
# echo "123"

function func() {
    a="true"
    if [[ $a == "true" ]] && [ -e "./" ]; then 
        echo "true"
    else
        echo "false"
    fi
}

set -x
func
echo "1"