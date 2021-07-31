# Copyright (C) 2018 RDA Technologies Limited and/or its affiliates("RDA").
# All rights reserved.
#
# This software is supplied "AS IS" without any warranties.
# RDA assumes no responsibility or liability for the use of the software,
# conveys no license or title under any patent, copyright, or mask work
# right to the product. RDA reserves the right to make changes in the
# software without notification.  RDA also make no representation or
# warranty that such application will be suitable for the specified use
# without further testing or modification.

export PROJECT_ROOT=$PWD
export BUILD_TARGET=$1

if test -d $1;then
    export BUILD_TARGET=$1
else
    export BUILD_TARGET=demo/$1
    export BUILD_DEMO=$1
fi

if  test -z ${BUILD_RELEASE_TYPE};then
    export BUILD_RELEASE_TYPE=debug
fi

if test -n ${BUILD_DEMO};then
    export PROJECT_OUT=$PROJECT_ROOT/out/${BUILD_TARGET}_${BUILD_RELEASE_TYPE}
fi

function remove_from_var
{
    local VAR=$1
    local VAL="$2"
    if test -n "$VAL"; then
        export $VAR="$(echo "${!VAR}" | tr ':' '\n' | grep -v "$VAL" | paste -d: -s)"
    fi
}

function prepend_var
{
    local VAR=$1
    local VAL="$2"
    if test -n "$VAL"; then
        remove_from_var $VAR "$VAL"
        if test -n "${!VAR}"; then
            export $VAR="$VAL:${!VAR}"
        else
            export $VAR="$VAL"
        fi
    fi
}

case `uname` in
Linux)
    prepend_var PATH $PROJECT_ROOT/prebuilts/linux/bin
    prepend_var PATH $PROJECT_ROOT/prebuilts/linux/cmake/bin
    prepend_var PATH $PROJECT_ROOT/prebuilts/linux/gcc-arm-none-eabi/bin
    prepend_var PATH $PROJECT_ROOT/tools
    prepend_var PATH $PROJECT_ROOT/tools/linux
    ;;
*_NT*)
    prepend_var PATH $PROJECT_ROOT/prebuilts/win32/bin
    prepend_var PATH $PROJECT_ROOT/prebuilts/win32/cmake/bin
    prepend_var PATH $PROJECT_ROOT/prebuilts/win32/python3
    prepend_var PATH $PROJECT_ROOT/prebuilts/win32/gcc-arm-none-eabi/bin
    prepend_var PATH $PROJECT_ROOT/tools
    prepend_var PATH $PROJECT_ROOT/tools/win32
    ;;
esac

cout() {
    if test -n ${PROJECT_OUT}; then
        mkdir -p $PROJECT_OUT && cd $PROJECT_OUT
    fi
}

croot() {
    cd $PROJECT_ROOT
}
