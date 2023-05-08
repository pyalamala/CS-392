#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int main (int argc, char **argv) {
	if (argc != 3) {
        //checks if the proper number of arguments were given
		fprintf(stderr, "Wrong number of command-line arguments\n");
		return -1;
	}


	
	FILE* in = fopen(argv[1], "rb");
	if (in == NULL) {
		printf("%s", "The binary file cannot be opened!");
		exit(-1);
	}

	FILE* out = fopen(argv[2], "w");
	if (out == NULL) {
		printf("%s", "The output file cannot be written to/opened!");
		exit(-1);
	}

    fseek(in,0, SEEK_END);
	size_t size = ftell(in);
	//gets the number of bytes in the .bin file in order to itrerate through the entire file.
	size_t i = 0;
	rewind(in);
	while(i <= size){
		int user_id;
		long item_id;
		short rating;
		long timestamp;
		//reads in the values to each respectvie variable by specifiying the proper number of bytes required for each one.
		//moves to the next ste of bytes and repeats for eahc variable. 
		fread(&user_id, 4, 1, in);
		fread(&item_id, 8, 1, in);
		fread(&rating, 1, 1, in);
		fread(&timestamp, 8, 1, in);
		if(i == size){
			//avoids adding adding an extra line if on the last line of the file.
			fprintf(out, "%d\t%ld\t%hd\t%ld", user_id, item_id, rating, timestamp);
		}
		else{
			//writes the values read in from the bin file to the output file in the proper format.
			fprintf(out, "%d\t%ld\t%hd\t%ld\n", user_id, item_id, rating, timestamp);
		}
		//iterates i by the number of bytes read.
		i = i + 4 + 8 + 1 + 8;
	}
	fclose(in);
	fclose(out);

	

	return 0;
}
