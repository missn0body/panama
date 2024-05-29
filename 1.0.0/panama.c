#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

static const char *VERSION = "v.1.0.0";
static constexpr short bufsize = 256;

void usage(void)
{
	printf("usage\n");
	return;
}

void version(void)
{
	printf("panama (%s): a woman, a program, some canal, palindrome checker!\n", VERSION);
	return;
}

int main(int argc, char *argv[])
{
	const char *programname = argv[0];
	bool frompipe = !isatty(STDIN_FILENO);
	bool verbose = false;

	if(!(frompipe || argc > 1))
	{
		fprintf(stderr, "%s: too few arguments, try \"--help\"\n", programname);
		exit(EXIT_FAILURE);
	}

	char infile[bufsize] = {0}, outfile[bufsize] = {0};
	int c;

	// Iterate through all arguments sent, character by character
	while(--argc > 0 && (*++argv)[0] != '\0')
	{
		if((*argv)[0] != '-')
		{
			if(outfile[0] != '\0')
			{
				fprintf(stderr, "%s: discarded program input -- \"%s\"\n", programname, *argv);
				continue;
			}

			strncpy((infile[0] == '\0') ? infile : outfile, *argv, bufsize);
		}

		if((*argv)[0] == '-')
		{
			// If there's another dash, then it's a long option.
			// Move the pointer up 2 places and compare the word itself.
			if((*argv)[1] == '-')
			{
				// Using continue statements here so that the user
				// can use both single character and long options
				// simultaniously, and the loop can test both.
				if(strcmp((*argv) + 2, "help")    == 0) { usage(); exit(EXIT_SUCCESS);   }
				if(strcmp((*argv) + 2, "version") == 0) { version(); exit(EXIT_SUCCESS); }
			}
			while((c = *++argv[0]))
			{
				// Single character option testing here.
				switch(c)
				{
					case 'h': usage(); exit(EXIT_SUCCESS);
					// This error flag can either be set by a
					// completely unrelated character inputted,
					// or you managed to put -option instead of
					// --option.
					default:
						fprintf(stderr, "%s: unknown option. try \"--help\"\n", programname);
						exit(EXIT_FAILURE);
				}
			}

			continue;
		}
	}

	// Determine where to output and where to get our input
	FILE *input =  (frompipe && infile[0] == '\0')  ? stdin  : fopen(infile, "r");
	FILE *output = (outfile[0] == '\0') 		? stdout : fopen(outfile, "a");
	if(input == nullptr || output == nullptr) { perror(programname); exit(EXIT_FAILURE); }

	// Print out verbose information if we request it
	if(verbose) printf("%s: %s\n", programname, (input == stdin)   ? "recieved input from pipe" : "recieved input from file");
	if(verbose) printf("%s: %s\n", programname, (output == stdout) ? "writing to stdout"	    : "writing to file");

	// Cleanup
	if(input  != stdin)  fclose(input);
	if(output != stdout) fclose(output);
	exit(EXIT_SUCCESS);
}

