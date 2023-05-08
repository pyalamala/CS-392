#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//Pranav Yalamala
//I pledge my honor that I ahve abided by the Stevens Honor System.


int main (int argc, char **argv) {
	if (argc != 3) {
        //checks if the proper number of arguments were given
		fprintf(stderr, "Wrong number of command-line arguments\n");
		return -1;
	}


	
	FILE* in = fopen(argv[1], "r");
	if (in == NULL) {
		printf("%s", "The input file cannot be opened!");
		exit(-1);
	}

	FILE* out = fopen(argv[2], "wb");
	if (out == NULL) {
		printf("%s", "The output file cannot be written to/opened!");
		exit(-1);
	}


    char line[200] = "";
    while(fgets(line, 200, in)){
        int user_id;
        int item_id;
        short rating;
        long timestamp;

        //scans in each value into it respective variable.
        sscanf(line, "%d\t%d\t%hd\t%ld", &user_id, &item_id, &rating, &timestamp);


        //writes each of the given values to the .bin file using the proper number of bytes for each file
        fwrite(&user_id, 4, 1, out);
        fwrite(&item_id, 2, 1, out);
        fwrite(&rating, 1, 1, out);
        fwrite(&timestamp, 8, 1, out);
       
    }

    fclose(in);
    fclose(out);

    return 0;
    

        
    }