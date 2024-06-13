#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

//////////////////////////////////////////////////////////////////////////
// Interface functions and definitions
//////////////////////////////////////////////////////////////////////////

static const char *VERSION = "v.1.0.1";
static constexpr short bufsize = 256;

void usage(void)
{
	printf("panama (%s): a palindrome checker\n", VERSION);
	printf("created by anson <thesearethethingswesaw@gmail.com>\n\n");
	printf("usage:\n\tpanama (-h / --help)\n\tpanama --version\n");
	printf("\tpanama [-qv] <input file>\n\tcommand-to-stdout | panama [-qv]\n\n");

	printf("options:\n\t%12s\t%s\n", "-q, --quiet", "only note if entirely consists of palindromes, do not output anything else");
	printf("\t%12s\t%s\n", "-v, --verbose", "prints verbose details on each line of a file");
	printf("\t%12s\t%s\n\n", "<input file>", "a string representing a filename");

	printf("copyright (c) 2024, see LICENSE for related details\n");
	return;
}

void version(void)
{
	printf("panama (%s): a woman, a program, some canal, palindrome checker!\n", VERSION);
	return;
}

//////////////////////////////////////////////////////////////////////////
// Checking functions
//////////////////////////////////////////////////////////////////////////

void panama_strip_str(const char *input, char *output)
{
	while (*input != '\0')
	{
		if(isalpha(*input))
		{
			*output = toupper(*input);
			output++;
		}

		input++;
	}

	*output = '\0';
	return;
}

bool panama_str_check(const char *input)
{
	if(input == nullptr) return false;

	size_t len = strlen(input);
	size_t mid = (size_t)(len / 2);

	for(size_t i = 0, j = len - 1; i < mid; i++, j--)
	{
		if(input[i] != input[j]) return false;
	}

	return true;
}

bool panama_file_check(FILE *input, bool isverbose, bool isquiet)
{
	if(input == nullptr) return false;

	char line[bufsize] = {0}, stripped[bufsize] = {0};
	bool retval = true, ispure = true;

	for(size_t i = 1; fgets(line, bufsize, input) != nullptr; i++)
	{
		// Remove the trailing newline and strip any other
		// whitespace present in the line. panama only judges
		// characters
		line[strcspn(line, "\r\n")] = '\0';
		panama_strip_str(line, stripped);

		// Check the return value of the single-string function,
		// so we can tell the user which lines are not palindromes
		retval = panama_str_check(stripped);
		if(!retval)
		{
			ispure = false;
			if(isverbose && !isquiet) printf(" ---\tline %ld is not a palindrome\n", i);
		}
		else if(!isquiet) printf(" +++\tline %ld is a palindrome\n", i);
	}

	return ispure;
}

//////////////////////////////////////////////////////////////////////////
// main()
//////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
	const char *programname = argv[0];
	bool frompipe = !isatty(STDIN_FILENO);
	bool verbose = false;
	bool quiet = false;

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
				if(strcmp((*argv) + 2, "verbose") == 0) { verbose = true; continue; }
				if(strcmp((*argv) + 2, "quiet") == 0)
				{
					if(verbose)
					{
						fprintf(stderr, "%s: quiet flag counters verbose flag\n", programname);
						verbose = false;
					}

					quiet = true;
					continue;
				}
			}
			while((c = *++argv[0]))
			{
				// Single character option testing here.
				switch(c)
				{
					case 'h': usage(); exit(EXIT_SUCCESS);
					case 'v':
						verbose = true;
						break;

					case 'q':
						if(verbose)
						{
							fprintf(stderr, "%s: quiet flag counters verbose flag\n", programname);
							verbose = false;
						}

						quiet = true;
						break;

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
	FILE *input = (frompipe || infile[0] == '\0') ? stdin : fopen(infile, "r");
	if(input == nullptr) { perror(programname); exit(EXIT_FAILURE); }

	// Print out verbose information if we request it
	if(verbose) printf("%s: %s\n", programname, (input == stdin) ? "recieved input from pipe" : "recieved input from file");

	// Actual palindrome checking
	bool ret = panama_file_check(input, verbose, quiet);
	if	(!ret)   printf("%s: \"%s\" is not a, or does not consist entirely of palindromes\n", programname, (input == stdin) ? "stdin" : infile);
	else if (!quiet) printf("%s: \"%s\" is a, or consists entirely of palindromes\n",	      programname, (input == stdin) ? "stdin" : infile);

	// Cleanup
	if(input != stdin) fclose(input);
	exit(EXIT_SUCCESS);
}

