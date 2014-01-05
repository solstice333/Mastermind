#include <stdio.h>
#include <ctype.h>
 
#define MAXDIM 10
#define MAXCHAR 'F'
#define NUMPARAMS 3

#define DEBUG 1

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
   model[i] = 0;
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

   // traverse through characters in model and set their respective indeces
   // in existMap (existMap is just a bit vector) to true
   for (i = 0; i < dimensions; i++) {
      if (model[i] == guess[i])
         exact++;
      else
         existMap[model[i] - 'A']++;
   }

#if DEBUG
   // check existMap
   for (i = 0; i < mapsize; i++) {
      printf("index %d (%c): %d\n", i, i + 'A', existMap[i]);
   }
#endif

   // check if exact match else see if inexact match
   for (i = 0; i < dimensions; i++) {
      if (guess[i] != model[i] && existMap[guess[i] - 'A']) 
         inexact += existMap[guess[i] - 'A'];
   }

#if DEBUG
   printf("exact matches: %d\n", exact);
   printf("inexact matches: %d\n", inexact);
#endif

   return exact;
}
 
/* Return the next non_blank character from the input.  Return EOF if
 * you hit EOF while trying to find the next nonblank character
 */
int non_blank(void) {
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
   int retry;
   int i;

   do {
      i = 0;
      retry = 0;

      // take input one character at a time and parse
      // make sure each character is an uppercase letter
      char letter;
      do {
         scanf("%c", &letter);
         
         if (isalpha(letter) && i < dimensions) {
            if (islower(letter))
               guess[i++] = letter + 'A' - 'a';
            else
               guess[i++] = letter;
         }
      } while (letter != '\n'); 

      // if user gave pattern with bad dimensions, retry
      if (i < dimensions) {
         printf("Error: Bad format. Try again\n");
         retry = 1;
         continue;
      }

      // good dimensions so add a null to the end of guess
      guess[dimensions] = 0;

      // if user gave bad characters
      for (i = 0; i < dimensions; i++) {
         if (guess[i] > maxchar) {
            printf("Error: Bad format. Try again\n");
            retry = 1;
            break;
         }
      }
      
   } while(retry);
}
 
int main() {
   char maxchar;
   int dim;
   int seed;

   int quit = 0;
   char discard;

   while (!quit) {
      int checkArgNum;

      // prompt user input
      printf("Enter maxchar, dimensions, and seed => ");
      checkArgNum = scanf("%c %d %d", &maxchar, &dim, &seed);

      if (checkArgNum == NUMPARAMS) 
         scanf("%c", &discard);
      else {
         printf("Error: Bad initial values\n");
         return 1;
      }

#if DEBUG
      // output user input to console
      printf("\nmaxchar: %c\n", maxchar);
      printf("dimensions: %d\n", dim);
      printf("seed: %d\n", seed);
#endif

      // error checking for argument input
      if (maxchar > MAXCHAR || maxchar < 'A') 
         printf("Error: Maximum character can only be between 'A' " 
          "and 'F' inclusive (case sensitive)\n");
      if (dim < 1 | dim > MAXDIM) 
         printf("Error: Bad dimensions given. Dimensions must be "
          "at least 1 or at most 10\n");
         
      // initialize the model and random number generator
      srand(seed);
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

         printf("%d. Enter your guess: ", count++);
         get_guess(guess, dim, maxchar, count);

         if (match(model, guess, dim) == dim) 
            prompt_for_guess = 0;

         printf("%s\n", guess);
      }
      
      // ask if user wants to quit
      int valid_resp = 0;
      while (!valid_resp) {
         char quit_resp;
         printf("quit? y/n ");

         scanf("%c", &quit_resp);
         scanf("%c", &discard);

         printf("%c", quit_resp);

         if (quit_resp == 'y' || quit_resp == 'n') {
            valid_resp = 1;
            if (quit_resp == 'y')
               quit = 1;
            else
               quit = 0;
         }
         else {
            printf("\nError: Invalid response\n");
         }
      }

      printf("\n\n");
   }

}
