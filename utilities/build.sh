#!/bin/sh

if [ -z ${BIN_FILE_NAME+x} ]; then
    BIN_FILE_NAME=Cypherock.bin
fi

if [ -z ${DFU_FILE_NAME+x} ]; then
    DFU_FILE_NAME=app_dfu_package.bin
fi

ACTIVE_ROOT_DIR=$(pwd)
ACTIVE_TYPE=Main
BUILD_TYPE=Debug

if [ $# -gt 0 ]; then
    case $1 in
        main)
        ACTIVE_TYPE=Main
        ;;

        initial)
        ACTIVE_TYPE=Initial
        ;;

        main-test)
        ACTIVE_TYPE=Main-test
        ;;

        initial-test)
        ACTIVE_TYPE=Initial-test
        ;;
        *)
        echo "Wrong type selection"
        echo "USAGE: $0 [type]"
        echo "type can 'main' or 'initial'"
        exit 1
        ;;
    esac
fi

if [ $# -gt 1 ]; then
    case $2 in
        dev)
        DEV=ON
        BUILD_TYPE=Debug
        ;;

        debug)
        DEV=OFF
        BUILD_TYPE=Debug
        ;;

        release)
        DEV=OFF
        BUILD_TYPE=Release
        ;;

        *)
        echo "Wrong mode selection"
        echo "USAGE: $0 <type> [mode]"
        echo "mode can be 'dev', 'debug' or 'release'"
        exit 1
        ;;
    esac
fi

if [ "${ACTIVE_TYPE}" = "" ]; then
    echo -e "\tCopy $0 into the project root directory and issue the command in following format"
    echo -e ""
    echo -e "\tUSAGE: $0 [dev|debug|release]"
    echo -e "\tParameter is optional and assumes 'debug' if not provided"
	echo -e ""
	echo -e "\tFollowing paths/value can be set:"
	echo -e "\tDFU_FILE_NAME"
	exit 1
fi

cd "${ACTIVE_ROOT_DIR}" || exit

mkdir -p "build/${ACTIVE_TYPE}"

cd "build/${ACTIVE_TYPE}" || exit

if [ -f "CMakeCache.txt" ]; then
    rm "CMakeCache.txt"
fi

CMAKE=$(which cmake)
if [ "${CMAKE}" = "" ]; then
    CMAKE=$(which mingw32-cmake)
fi

if [ "${CMAKE}" = "" ]; then
    echo -e "\tNo cmake installation found. Please install cmake to continue (or check your 'Path' Environment variable";
    exit 1;
fi

BUILD_TOOL=$(which ninja)
GEN="Ninja"
if [ "${BUILD_TOOL}" = "" ]; then
    BUILD_TOOL=$(which make)
    GEN="Unix Makefiles"
    if [ "${BUILD_TOOL}" = "" ]; then
        BUILD_TOOL=$(which mingw32-make)
        GEN="MinGW Makefiles"
    fi
fi

if [ "${BUILD_TOOL}" = "" ]; then
    echo -e "\tNo build tool (make/ninja) installation found. Please install one to continue (or check your 'Path' Environment variable";
    exit 1;
fi

"${CMAKE}" -DDEV_SWITCH=${DEV} -DDEBUG_SWITCH=${DEV} -DSIGN_BINARY=ON -DCMAKE_BUILD_TYPE:STRING="${BUILD_TYPE}" -DFIRMWARE_TYPE="${ACTIVE_TYPE}" -DCMAKE_EXPORT_COMPILE_COMMANDS:BOOL=ON -DCMAKE_BUILD_PLATFORM:STRING=Device -G "${GEN}" ../../

if [ ! $? -eq 0 ]; then exit 1; fi
"${BUILD_TOOL}" -j8 all
