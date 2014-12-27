#!/bin/bash
export ANDTOOLCHAIN=./android-cmake/android.toolchain.cmake
NDK_LOCATION=~/opt/android-ndk
ABI=armeabi-v7a
if [[ $1 ]] 
then 
    NDK_LOCATION=$1 
fi

if [[ $2 ]] 
then 
    ABI=$2 
fi

cmake .. -DANDROID_ABI=$ABI  -DANDROID_TOOLCHAIN_NAME=arm-linux-androideabi-4.8 -DCMAKE_TOOLCHAIN_FILE=$ANDTOOLCHAIN -DANDROID_NDK=$NDK_LOCATION -DANDROID_NATIVE_API_LEVEL=android-9
