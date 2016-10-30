#!/bin/bash
desc=$(git describe --long --dirty=-D)
IFS=- read -ra args <<< "$desc"
mkdir -p build
prefix=""
if [ "${#args[@]}" -eq 4 ]
    then    
        prefix='D'
fi
ctmp=$(echo ${args[0]}| head -c 1)
if [ $ctmp = "v" ] || [ $ctmp = "V" ] ; then
    args[0]=$(echo ${args[0]} |cut -b 2- )
fi   

file=".tar"
file=build/mathlibra-${args[0]}$prefix$file
./gradlew createGitHeader
git archive -o $file HEAD   
tar -C build/ -rf $file GIT_VERSION.h
cat $file | gzip > $file.gz
rm $file

