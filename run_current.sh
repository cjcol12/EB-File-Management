#!/bin/bash

# clear screen for readability
clear

# make
make clean
make all

# run ebfEcho with valid arguments
./ebfEcho sample_images/dodo.ebf sample_images/out.ebf

# ask the user if they want to run test script
printf "Run tests? (y/n): " 


# take user input
read test_run

# run tests if user input is "y"
if [ "$test_run" == "y" ]
then
    ./test.sh
else
    echo "all done!"
fi