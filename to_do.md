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
change header so it reads in magic number and output what it reads - not just eb
refactor ebfComp
    - understand passing structures to functions
    - understand how to use multiple instances of structure

complete tests for ebfEcho and Comp
    - find full path
    - find executable name - .c


TODO
complete function for closing file and freeing mem
    - test if it works as expected

sort malloc bug
check file closes when expected
check imageData free'd properly when expected
find memory leaks using valgrind and gdb
clean up variable names to look like foo_bar
understand why hexdumping slice0a and out are different - but correct according
to hamishs notes



QUESTIONS
unix usage - per file or different module, how to lose ./
return value on unix usage success - 10?
unix usage in main?
do we need to submit any version of original task or just 2d array