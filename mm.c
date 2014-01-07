#include <stdio.h>
#include <ctype.h>
 
#define MAXDIM 10
#define MAXCHAR 'F'
#define NUMPARAMS 3

#define DEBUG 0

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
void initialize(char model[], int dimensions, char maxchar) {
   int i;

   for (i = 0; i < dimensions; i++) {
      model[i] = rand() % (maxchar + 1 - 'A') + 'A';
   }
}
 
/* Compare "model" to "guess", assuming each contains "dimensions"
 * characters.  Print out the number of exact and inexact matches.
 * Returns the number of exact matches. 
 */
int match(char model[], char guess[], int dimensions) {
   const int mapSize = 'F' - 'A' + 1;
   char existMap[mapSize];   // 6 represents the indeces for storing instances of A-F 
   int exact = 0, inexact = 0;

   // initialize existMap with 0's
   int i;
   for (i = 0; i < mapSize; i++) {
      existMap[i] = 0;
   }

   // traverse through characters in guess and set their respective indeces
   // in existMap (existMap is just a bit vector) to true. Simultaneously
   // increment exact matches
   for (i = 0; i < dimensions; i++) {
      if (guess[i] == model[i])
         exact++;
      else
         existMap[guess[i] - 'A']++;
   }

#if DEBUG
   // check existMap
   for (i = 0; i < mapSize; i++) {
      printf("index %d (%c): %d\n", i, i + 'A', existMap[i]);
   }
#endif

   // increment inexact matches by traversing through each character in
   // model and searching for them in existMap
   for (i = 0; i < dimensions; i++) {
      if (model[i] != guess[i] && existMap[model[i] - 'A']) 
         inexact++;
   }

   printf("    %d Exact and %d Inexact\n", exact, inexact);
   return exact;
}
 
/* Return the next non_blank character from the input.  Return EOF if
 * you hit EOF while trying to find the next nonblank character
 */
int non_blank(void) {
   char value;

   while ((value = getchar()) != EOF && value == ' ') {}

   return value;
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
int get_guess(char guess[], int dimensions, char maxchar, int try) {
   char discard;
   int retry, i;

   do {
      i = retry = 0;

      // take input one character at a time and parse
      char letter;
      while (i < dimensions) {
         if ((letter = non_blank()) == EOF)
            return 0;
         else if (isalpha(letter)) {
            // make uppercase
            guess[i] = letter;
            if (islower(letter))
               guess[i] -= 32;

            // check if character is not between A-F inclusive
            if (guess[i] > maxchar) {
               printf("    Bad entry.  Try again: ");
               retry = i = dimensions;   // break hack
            }
            else
               i++;
         }
      } 

      // flush
      while ((discard = getchar()) != '\n') {}

   } while (retry);

   return 1;
}
 
int main() {
   char maxchar;
   int dim = 0, seed = 1;

   int done = 0, gameNum = 0;
   int initState = 1, guessState = 0, count = 1, anotherState = 0;
   double sum = 0;
   char discard;

   // prompt user input
   printf("Enter maxchar, dimensions, and seed => ");
   scanf(" %c %d %d", &maxchar, &dim, &seed);

   // make maxchar uppercase 
   if (islower(maxchar))
      maxchar -= 32;

   // check the maxchar if greater than MAXCHAR and dim
   // if it's greater than MAXDIM
   if (maxchar > MAXCHAR || dim < 1 || dim > MAXDIM) {
      printf("Bad initial values\n");
      return 1;
   }

#if DEBUG
   // output user input to console
   printf("\nmaxchar: %c\n", maxchar);
   printf("dimensions: %d\n", dim);
   printf("seed: %d\n", seed);
#endif

   // initialize random number generator by calling srand once
   srand(seed);

   // start game event loop

   char model[dim]; 
   while (!done) {
      // initialize model
      if (initState) {
         initialize(model, dim, maxchar);
         gameNum++;

         initState = 0;
         guessState = 1;
      }

#if DEBUG
   // output the model
   printf("model: %s\n", model);
#endif

      // prompt user for guess
      if (guessState) {
         char guess[512];

         printf("\n %d. Enter your guess: ", count);

         if (!get_guess(guess, dim, maxchar, count)) {
            printf("Unexpected EOF\n");
            return 1;
         }
         else if (match(model, guess, dim) == dim) {
            sum += count;
            anotherState = count = 1;
            guessState = 0;
         }
         else
            count++;

#if DEBUG
      printf("%s\n", guess);
#endif
      }

      // prompt user for another game
      else if (anotherState) {
         // display current average
         printf("\n\nCurrent average:  %0.3f\n", sum / gameNum);

         // ask if user wants to play again
         char another;
         printf("\nAnother game [Y/N]? ");

         if ((another = non_blank()) == EOF) {
            printf("Unexpected EOF\n");
            return 1;
         }

         if (another == 'Y' || another == 'y') 
            initState = 1;
         else
            done = 1;
         anotherState = 0;
      }
   }

   return 0;
}