# Parallel Merge Sort ##

This is a simple multi threaded version of merge sort, that utilizes the pthread library.

## Specifications ##
+ Merge sort recursively sorts halves of the input and then merges the 2 sorted halves.
+ The base case being when there is only 1 element.

## Multi-threading ##
+ An intial thread performs the first call to merge sort.
+ Thereafter, a thread is spanned for each half of the input on which merges sort must be performed.

## Compiling the Program ##
The program can be complied by running the `make` command.
