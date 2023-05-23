# Assignment 1

## Quick Links
- [Key Information](#key-information)
- [The Task](#the-task)
- [File Formats](#file-format-details)
    - [ebf](#ebf-files)
    - [ebu](#ebu-files)
    - [ebc](#ebc-files)
- [Running the Code](#using-the-files)
- [Hints and Tips](#hints-and-tips)
    - [Where to Start](#where-do-i-begin)
    - [Branching in Git](#branching-in-git)
- [Common Errors](#common-issues)
    - [Compilation Errors](#compilation-errors)
    - [Runtime Errors](#runtime-errors)

## Key Information

# The Task

## Testing

You will be provided with a basic shellscript which tests the tools up to the end of Task 3. You should extend this to test Task 4, as well as adding any additional tests which you think appropriate. When your code is submitted, we will test it using our own comprehensive test script. We may test things that you haven't though of.
All programmes should return 0 in the case of successful exit or non-0 where the code fails, with an appropriate string to describe the result. This is a Unix convention- 0 means success because there is only one way in which a program can succeed, but there are many different ways to fail.

The messages and return codes which you should use are:


| Value  | String | Condition |
| ------------- | ------------- | ------------- |
| 1  | ERROR: Bad Argument Count | Program given wrong # of arguments |
| 2 | ERROR: Bad File Name (fname) | Program fails to open file |
| 3 | ERROR: Bad Magic Number (fname) | File has an illegal magic number |
| 4 | ERROR: Bad Dimensions (fname) | File has illegal dimensions |
| 5 | ERROR: Image Malloc Failed | Malloc failed to allocate memory |
| 6 | ERROR: Bad Data (fname) | Reading in data failed |
| 7 | ERROR: Output Failed (fname) | Writing out data failed |
| 100 | ERROR: Miscellaneous (describe) | Any other error which is detected |

In all cases, fname should be replaced by the name of the file which caused the error.
In the case of Miscellaneous errors, in 100 chars or less please describe what the error was.

All other print statements will be ignored by the final test script.

You should also use these standard success messages, for which the value is always 0.

| String | Condition |
| ------------- | ------------- |
| Usage: executablename file1 file2 | Program run with no arguments |
| ECHOED | xEcho file successfully completes |
| IDENTICAL | xComp file successfully completes and files are identical |
| DIFFERENT | xComp file successfully completes and files are not identical |
| CONVERTED | x2y file successfully completes |

Usage messages are also a Unix convention - when an executable is run with no arguments, it is assumed that the user does not know the correct syntax and wishes to see a prompt. As this is not an error state, the return value is 0.

# File Format Details

The three file formats you will be using are ebf, ebu and ebc - these are all **new file formats**

They are loosely based on the PGM image format

## ebf files

ebf files are a basic greyscale image file. They contain a short header of metadata, followed by pixel values. The basic format is as follows:

```
eb              - this is the magic number - for ebf files this is always eb.
height width    - two space-separated integers which give the height and width of the image.
p1 p2 p3 ...    - the pixel data which is separated by spaces. This is usually (but not always) separated into rows by a newline.
```

ebf files have a maximum pixel value of 31.

## ebu files

ebu (ebf uncompressed) are the binary equivalent of ebf files. They have a very similar header (written in ASCII) followed by the pixel data written out as **binary**. If you open an ebu file in a text editor, you will generally see a random string of data as it is not encoded in a human-readable format.

```
eu
height width
p1 p2 p3 ...
```

ebu files have a maximum pixel value of 31.

## ebc files

ebc (ebf compressed) are compressed binary ebf files. They also have a similar header, but the pixel data is written out in **compressed** binary. As ebf files have a maximum pixel value of 31, each pixel can be represented by **5 bits of binary**. C can only write out blocks of 8 bits (1 byte) at once, so you will need to design an algorithm to pack your pixel data into the minimum number of bytes. This should compress an ebu file to just over 0.6 times its original size.

```
ec
height width
packed binary pixel data
```

ebc files have a maximum pixel value of 255.

# Using The Files

You can compile the provided files by using:
```
make all
```
Executables can then be run the standard way.

To run the testing script, the first time:
```
chmod +x tests.sh
```
This gives you permission to execute the file - you only need to do this once.

You can then run the test suite using:
```
./tests.sh
```

# Hints and Tips

# Common Issues