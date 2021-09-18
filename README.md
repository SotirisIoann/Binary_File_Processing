# Binary_File_Processing

C project for processing binary files.

More specifically, it implements a simple name registration system by managing binary files.Contains procedures for entering, deleting, accessing, searching and sorting.

The program needs two files to run. The first contains the entries(data.bin) and the second the bytes covered by each entry(pos.data).With the help of the second file we can read all the records so these two files change at the same time based on whether a record has been deleted or inserted.

Compile and run

    gcc -o b bfile.c
  
Execute

    ./b data.bin pos.bin



