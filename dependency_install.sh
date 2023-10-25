#!/bin/bash
echo starting;
mkdir dependencies build external;
mkdir external/include;

cd dependencies
git clone https://github.com/ashtum/lazycsv.git
git clone https://github.com/HowardHinnant/date.git
git clone https://github.com/mmahnic/argumentum.git

cp lazycsv/include/lazycsv.hpp ../external/include
cp date/include/date/date.h ../external/include

mkdir ../build/argumentum
cd ../build/argumentum
cmake ../../dependencies/argumentum/ -DCMAKE_INSTALL_PREFIX=../../external/
make install
