#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main (int argc, char **argv) {
    if (argc != 4) {
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

    FILE* out2 = fopen(argv[3], "w");
	if (out == NULL) {
		printf("%s", "The output file cannot be written to/opened!");
		exit(-1);
	}


    char line[200] = "";
    while(fgets(line, 200, in)){
        const char s[2] = "\t";
        const char n[2] = "\n"; 
        char* token;

        int user_id;
        int item_id;
        int rating;
        int timestamp;

        //scans in each value into it respective variable.
        sscanf(line, "%d\t%d\t%d\t%d", &user_id, &item_id, &rating, &timestamp);

        // printf("user_Id:%d\t", user_id);
        // printf("item_Id:%d\t", item_id);
        // printf("rating:%d\t", rating);
        // printf("timestamp:%d\t\n", timestamp);

        fwrite(&user_id, sizeof(user_id), 1, out);
        fwrite(&s, sizeof(s), 1, out);
        fwrite(&item_id, sizeof(item_id), 1, out);
        fwrite(&s, sizeof(s), 1, out);
        fwrite(&rating, sizeof(rating), 1, out);
        fwrite(&s, sizeof(s), 1, out);
        fwrite(&timestamp, sizeof(timestamp), 1, out);
        fwrite(&n, sizeof(n), 1, out);
       
    }

    fclose(in);
    fclose(out);

    in = fopen(argv[1], "rb");
    if (in == NULL) {
		printf("%s", "The input file cannot be opened!");
		exit(-1);
	}

    char lines[200] = "";
    while (fgets(line,200, in)){
        int user_id;
        const char s[3] = "\t\n";
        char* token;
        token = strtok(line, s);

        while (token != NULL){
            printf("%d\t", (int)token);
            token = strtok(NULL, s);
        }

        printf("\n");
    }
}