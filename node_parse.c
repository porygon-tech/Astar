#include <stdio.h>
#include <stdlib.h>
//gcc node_parse.c -o NODEPARSE -lm && time ./NODEPARSE


int main(int argc, char const *argv[]){
	char filename[] = "cataluna.csv";
	char *line_buf = NULL;
	size_t line_buf_size = 0;
	int line_count = 0;
	ssize_t line_size;


	FILE *fp = fopen(filename, "r");
	if (!fp){
		fprintf(stderr, "Error opening file '%s'\n", filename);
		return EXIT_FAILURE;
	}

	// Get the first line of the file. 
	line_size = getline(&line_buf, &line_buf_size, fp);

	// Loop through until we are done with the file. 
	while (line_size >= 0){
		line_count++;
		printf("line[%06d]: chars=%06zd, buf size=%06zu, contents: %s", line_count,
				line_size, line_buf_size, line_buf);
		line_size = getline(&line_buf, &line_buf_size, fp); //Get the next line 
	}
}
