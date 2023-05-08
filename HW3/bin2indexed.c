#include <stdio.h>
#include <string.h>
#include <stdlib.h>
//Pranav Yalamala
//I pledge my honor that I ahve abided by the Stevens Honor System.

int main (int argc, char **argv) {
	if (argc != 4) {
        //checks if the proper number of arguments were given
		fprintf(stderr, "Wrong number of command-line arguments\n");
		return -1;
	}


	
	FILE* in = fopen(argv[1], "rb");
	if (in == NULL) {
		printf("%s", "The binary file cannot be opened!");
		exit(-1);
	}

    FILE* in2 = fopen(argv[2], "r");
    if(in2 == NULL){
        printf("%s", "The item file cannot be opened!");
		exit(-1);
    }

	FILE* out = fopen(argv[3], "wb");
	if (out == NULL) {
		printf("%s", "The output file cannot be written to/opened!");
		exit(-1);
	}

    //uses fseek to get the size of the item file to use later for writing to the output file
    fseek(in2, 0, SEEK_END);
    size_t size = ftell(in2);
    rewind(in2);

    //alloactes memory for an array to store the value of the offsets
    long* offsets = malloc(65134*sizeof(long));

    
    char line2[1000] = "";
    
    //goes line by line and sets the value of the array at the proper index to the offset at the beginnig of the line.
    int i = 0;

    while(fgets(line2, 1000, in2)){
        int item;
        //scans in item_id to use as the index to store the offset in the offsets array.
        sscanf(line2, "%d", &item);
        offsets[item] = i;
        i = ftell(in2);
		
	}


    size_t count = 0;
    while(count <= size){
        int user_id;
		int item_id;
		short rating;
		long timestamp;
		//reads in the values to each respectvie variable by specifiying the proper number of bytes required for each one.
		//moves to the next set of bytes and repeats for each variable. 
		fread(&user_id, 4, 1, in);
		fread(&item_id, 2, 1, in);
		fread(&rating, 1, 1, in);
		fread(&timestamp, 8, 1, in);

		//writes the values to the binary file using the previously read values.
        //replaces item_id with the proper offset based on it's item_id.
        fwrite(&user_id, 4, 1, out);
        fwrite(&offsets[item_id], 8, 1, out);
        fwrite(&rating, 1, 1, out);
        fwrite(&timestamp, 8, 1, out);
		//iterates count by the number of bytes read.
		count = count + 4 + 2 + 1 + 8;
    }

    fclose(in);
    fclose(in2);
    fclose(out);

    free(offsets);
    return 1;
}