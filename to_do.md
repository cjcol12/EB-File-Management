DONE 

understand test.sh further
read ebfEcho.c 
read ebfComp.c
modularise ebfEcho.c
understand why casting magic numbers to short
create structure for data
add structure syntax to variables
move definitions to new file
create clearer error messages
find BAD_DATA bugs
seperate into modules
seperate read and write functionaliy
refactor ebfComp
    - understand passing structures to functions
    - understand how to use multiple instances of structure
complete tests for ebfEcho and Comp
    - find full path
    - find executable name - .c
change header so it reads in magic number and output what it reads - not just eb
pass final two tests on output file


TODO

change output formatting in ebfEcho
comment compare_images.c
come up with my own further tests for ebfEcho and Comp
move onto other tasks


understand why hexdumping slice0a and out are different - but correct according
to hamishs notes


understand why hexdumping slice0a and out are different - but correct according
to hamishs notes



TODO
complete function for closing file and freeing mem
    - test it works as expected

sort malloc bug
check file closes when expected
check imageData free'd properly when expected
find memory leaks using valgrind and gdb
clean up variable names to look like foo_bar - mainly in struct
find \n in output when converting file
add error checking to new functions
refactor error checking into different functions
test all functionality 
extend test script
think about next task
add all prototypes to headers (or create headers)




QUESTIONS
unix usage - per file or different module, how to lose ./
return value on unix usage success - 10?
unix usage in main?
do we need to submit any version of original task or just 2d array
lines limited to 80 characters