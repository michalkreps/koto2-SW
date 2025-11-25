#!/bin/bash                                                                     
DIR=
if [ -n "$BASH_VERSION" ]; then
    #bash                                                                       
    if [ "x${BASH_ARGV[0]}" = "x" ]; then
        if [ ! -d e14 ]; then
            echo ERROR: must "cd ../E14_TOP_DIR" before calling "source setup.sh" for this version of bash!
            export E14_TOP_DIR=
            return 1
        fi
        DIR="$PWD"
    else
        DIR=$(cd $(dirname ${BASH_ARGV[0]});pwd)
    fi
elif [ -n "$ZSH_VERSION" ]; then
    #zsh                                                                        
    if [ "x$0" = "x" ]; then
        if [ ! -d e14 ]; then
            echo ERROR: must "cd ../E14_TOP_DIR" before calling "source setup.s\
h" for this version of zsh!
            export E14_TOP_DIR=
            return 1
        fi
        DIR="$PWD"
    else
        DIR=$(cd $(dirname $0);pwd)
    fi
else
    echo "Not supported shell. bash and zsh are supported."
    export E14_TOP_DIR=
    return 1
fi

export E14_TOP_DIR=${DIR}/e14
source ${E14_TOP_DIR}/scripts/e14.sh

