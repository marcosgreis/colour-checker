#!/bin/sh

PROJECT_DIR=`pwd`
BUILD_DIR=${PROJECT_DIR}/build
EXTRA_DIR=${PROJECT_DIR}/extra
SUDO=''
SAMPLES="a b c"

prepare () {
    echo ">>> Preparing environment..."

    USER=`id -u`

    if [ "${USER}" -ne 0 ]; then
        echo "! This preparation needs to run as root"
        SUDO=sudo
    fi

    echo "This preparation will install the following packages and its dependencies:"
    echo "- cmake"
    echo "- g++"
    echo "- libboost-program-options-dev"
    echo "- libboost-test-dev"
    echo -n "Do you agree? (Y/n) "
    
    read opt
    echo 
    
    if [ "${opt}" != "n" ]; then
        ${SUDO} apt-get -y install cmake \
                                   g++ \
                                   libboost-program-options-dev \
                                   libboost-test-dev
    fi
}

build () {
    echo ">>> Building..."
    if [ ! -d ${BUILD_DIR} ]; then
        mkdir ${BUILD_DIR}
    fi
    cd ${BUILD_DIR} && \
    cmake .. && \
    make
}

run () {
    echo ">>> Running samples..."
    for i in ${SAMPLES}; do
        ${BUILD_DIR}/cmdline-checker -w ${EXTRA_DIR}/$i/wavelengths.csv \
                                -i ${EXTRA_DIR}/$i/intensities.csv \
                                -c ${EXTRA_DIR}/ciexyz31.csv \
                                -o ${EXTRA_DIR}/$i/output.csv
    done
}

sample_result() {
    for i in ${SAMPLES}; do
        echo
        echo ">>> Result for sample $i"
        cat ${EXTRA_DIR}/$i/output.csv
    done
}

unittest() {
    echo ">>> Running unit tests..."
    cd ${BUILD_DIR} 2> /dev/null && \
    ctest
}

clean () {
    echo ">>> Cleaning..."
    cd ${BUILD_DIR} 2> /dev/null && \
    make clean
}

deepclean () {
    echo ">>> Erasing build dir..."
    if [ ! -d ${BUILD_DIR} ]; then
        echo "! Build dir does not exist."
        return 
    fi
    rm -rf ${BUILD_DIR} 2> /dev/null
}

invalid() {
  echo "Invalid option: -$1"
}

param() {
  echo "Option -$1 requires an argument"
}

check() {
  if [ $1 != 0 ]; then
    echo "! Error running last command =("
    exit 1
  fi
}

usage() {
  echo "Usage: $1 [OPTIONS]"
  echo ""
  echo "  -p \t\tPrepare environment (requires sudo)"
  echo "  -b \t\tBuild"
  echo "  -r \t\tRun sample tests"
  echo "  -u \t\tRun unit tests"
  echo "  -c \t\tClean"
  echo "  -d \t\tErase build dir"
  echo ""
}

RUN=0
while getopts ":p :b :r :u :c :d" opt; do
    RUN=1
    case $opt in
        p)
            prepare
        ;;
        b)
            build
        ;;
        r)
            run
            sample_result
        ;;
        u)
            unittest
        ;;
        c)
            clean
        ;;
        d)
            deepclean
        ;;
        :)
            param $OPTARG
            RUN=0
        ;;
        \?)
            invalid $OPTARG
            RUN=0
        ;;
    esac
    check $?
done

if [ "$RUN" != "1" ]; then
  usage $0
  exit 1
fi


