#!/bin/sh

cd ./Common/
make
make install
cd ../
echo "\033[0;31mbuild Common complete!\033[0m"
echo


cd ./Network/
make
make install
cd ../
echo "\033[0;31mbuild Network complete!\033[0m"
echo

cd ./Service/
make
make install
cd ../
echo "\033[0;31mbuild Service complete!\033[0m"
echo

cd ./Monitor/
make
make install
cd ../
echo "\033[0;31mbuild Service complete!\033[0m"
echo

cd ./Moment/
make
make install
cd ../
echo "\033[0;31mbuild Testcase complete!\033[0m"
echo



