#include <stdio.h>
#include <ctype.h>
 
#define MAXDIM 10
#define MAXCHAR 'F'

static int seed;

// This will return a different random nonnegative integer on
// each successive call.
int rand() {
   seed = (seed * 131071 + 524287) % 8191;
   return seed;
}

// Call this just once in the entire program, with the seed.
void srand(int s) {
   seed = s;
}

void initialize(char model[], int dimensions, char maxchar);
int match(char model[], char guess[], int dimensions);
int non_blank();
int get_guess(char guess[], int dimensions, char maxchar, int try);

/* Initialize "model" to contain a random pattern of "dimensions"
 * letters, in the range 'A' to "maxchar".
*/
void initialize(char model[], int dimensions, char maxchar)
{
}
 
/* Compare "model" to "guess", assuming each contains "dimensions"
 * characters.  Print out the number of exact and inexact matches.
 * Returns the number of exact matches. 
 */
int match(char model[], char guess[], int dimensions)
{
}
 
/* Return the next non_blank character from the input.  Return EOF if
 * you hit EOF while trying to find the next nonblank character
 */
int non_blank(void)
{
}
 
/* Input the user's guess, putting it into "guess".  Assume that
 * there will be "dimensions" characters in the guess, in the range
 * 'A' to "maxchar".  Assume that this is the "try"th attempt on the
 * user's part.  If a guess with the wrong format or characters is
 * entered, make the user try again until a valid guess is entered.
 * After each guess, read and discard any extra characters up to and
 * including the end of line.  Return true if a guess is obtained, 
 * false if EOF is hit while trying to get a good guess.
 */
int get_guess(char guess[], int dimensions, char maxchar, int try)
{
}
 
int main()
{
}
