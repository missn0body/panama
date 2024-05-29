#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//////////////////////////////////////////////////////////////////////////
// Interface functions and definitions
//////////////////////////////////////////////////////////////////////////

static const char *VERSION = "v.1.0.0";
static constexpr short bufsize = 256;

void usage(void)
{
	printf("panama (%s): a palindrome checker\n", VERSION);
	printf("created by anson <thesearethethingswesaw@gmail.com>\n\n");
	printf("usage:\n\tpanama (-h / --help)\n\tpanama --version\n");
	printf("\tpanama <input>\n\tcommand-to-stdout | panama\n\n");

	printf("options:\n\t%12s\t%s\n\n", "<input>", "a string to compare");

	printf("copyright (c) 2024, see LICENSE for related details\n");
	return;
}

void version(void)
{
	printf("panama (%s): a woman, a program, some canal, palindrome checker!\n", VERSION);
	return;
}

//////////////////////////////////////////////////////////////////////////
// Checking function
//////////////////////////////////////////////////////////////////////////

bool panama_str_check(const char *input)
{
	if(input == nullptr) return false;
	return true;
}

bool panama_file_check(FILE *input)
{
	if(input == nullptr) return false;
	return true;
}

//////////////////////////////////////////////////////////////////////////
// main()
//////////////////////////////////////////////////////////////////////////

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

	char infile[bufsize] = {0};
	int c;

	// Iterate through all arguments sent, character by character
	while(--argc > 0 && (*++argv)[0] != '\0')
	{
		if((*argv)[0] != '-')
		{
			if(infile[0] != '\0')
			{
				fprintf(stderr, "%s: discarded program input -- \"%s\"\n", programname, *argv);
				continue;
			}

			strncpy(infile, *argv, bufsize);
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
	FILE *input = (frompipe && infile[0] == '\0') ? stdin : fopen(infile, "r");
	if(input == nullptr) { perror(programname); exit(EXIT_FAILURE); }

	// Print out verbose information if we request it
	if(verbose) printf("%s: %s\n", programname, (input == stdin) ? "recieved input from pipe" : "recieved input from file");

	// Cleanup
	if(input != stdin) fclose(input);
	exit(EXIT_SUCCESS);
}

