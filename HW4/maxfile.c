#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>
//Pranav Yalamala
//I pledge my honor that I ahve abided by the Stevens Honor System.



int main (int argc, char **argv) {

    if (argc != 2) {
        //checks if the proper number of arguments were given
		fprintf(stderr, "Wrong number of command-line arguments\n");
		return -1;
	}

    DIR *dp;
    struct dirent *dirp;

    dp = opendir(argv[1]);

    



}