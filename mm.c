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
   const int mapsize = 'F' - 'A' + 1;
   char existMap[mapsize]; 
   int exact = 0;
   int inexact = 0;

   // initialize existMap with 0's
   int i;
   for (i = 0; i < mapsize; i++) {
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
   for (i = 0; i < mapsize; i++) {
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

   if (scanf(" %c", &value) == EOF)
      return EOF;

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
   int retry = 0;
   int i = 0;

   do {
      i = 0;
      retry = 0;
      // take input one character at a time and parse
      // make sure each character is an uppercase letter
      char letter;
      do {
         if ((letter = non_blank()) == EOF)
            return 0;
         
         if (isalpha(letter)) {
            if (islower(letter))
               guess[i++] = letter + 'A' - 'a';
            else
               guess[i++] = letter;
         }
      } while (i < dimensions); 


      // get rid of the extra characters all the way to the EOL
      do {
         scanf("%c", &discard);
      } while (discard != '\n');

      // if user gave pattern with bad dimensions
      if (i < dimensions) {
         retry = 1;
      }

      // good dimensions so add a null to the end of guess
      guess[dimensions] = 0;

      // if user gave bad characters
      for (i = 0; i < dimensions; i++) {
         if (guess[i] > maxchar) {
            printf("    Bad entry.  Try again: ");
            retry = 1;
         }
      }
   } while (retry);

   return 1;
}
 
int main() {
   char maxchar;
   int dim;
   int seed;

   int gameNum = 1;
   double sum = 0;

   int quit = 0;
   char discard;
   int checkArgNum;

   // prompt user input
   printf("Enter maxchar, dimensions, and seed => ");

   checkArgNum = scanf(" %c %d %d", &maxchar, &dim, &seed);

   // error checking for argument input
   if (checkArgNum != NUMPARAMS) {
      printf("Bad initial values");
      return 1;
   }
   else {
      // flush 
      do {
         scanf("%c", &discard);
      } while (discard != '\n');

      // is maxchar a letter
      if (!isalpha(maxchar)) {
         printf("Bad initial values");
         return 1;
      }

      // if maxchar is lowercase
      else if (islower(maxchar))
         maxchar = maxchar + 'A' - 'a';

      // check the maxchar if greater than MAXCHAR and dim
      // if it's greater than MAXDIM
      if (maxchar > MAXCHAR || dim < 1 || dim > MAXDIM) {
         printf("Bad initial values");
         return 1;
      }
   }

#if DEBUG
   // output user input to console
   printf("\nmaxchar: %c\n", maxchar);
   printf("dimensions: %d\n", dim);
   printf("seed: %d\n", seed);
#endif

   // initialize random number generator by calling srand once
   srand(seed);

   while (!quit) {
      // initialize model
      char model[dim]; 
      initialize(model, dim, maxchar);

#if DEBUG
   // output the model
   printf("model: %s\n", model);
#endif

      // begin prompting for guess
      int prompt_for_guess = 1;
      int count = 1;

      while (prompt_for_guess) {
         char guess[512];

         printf("\n %d. Enter your guess: ", count);

         if (!get_guess(guess, dim, maxchar, count)) {
            printf("Unexpected EOF");
            return 1;
         }

         if (match(model, guess, dim) == dim) {
            prompt_for_guess = 0;
            sum += count;
         }

         count++;

#if DEBUG
      printf("%s\n", guess);
#endif
      }

      // display current average
      printf("\n\nCurrent average:  %0.3f\n", sum / gameNum);

      // ask if user wants to quit
      char quit_resp;
      printf("\nAnother game [Y/N]? ");

      if ((quit_resp = non_blank()) == EOF) {
         printf("Unexpected EOF");
         return 1;
      }

      if (quit_resp == 'Y' || quit_resp == 'y') {
         quit = 0;
         gameNum++;
      }
      else
         quit = 1;
   }

   return 0;
}
