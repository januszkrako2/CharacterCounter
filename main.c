#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

struct pair {
  char character;
  int occurrences;
};

typedef struct pair Pair;

int main(int argc, char *argv[]) {

  if (argc != 2) {
    printf("Format: executablename file.txt\n");
    return 1;
  }

  FILE *filePointer = fopen(argv[1], "r");
  if (filePointer == NULL) {
    printf("File named '%s' not found.\n", argv[1]);
    return 1;
  }

  // Get the number of characters in the file.
  fseek(filePointer, 0L, SEEK_END);
  unsigned int characterCount = ftell(filePointer);
  rewind(filePointer);

  // Move its contents into the buffer.
  char data[characterCount];
  int i;
  for (i = 0; i < sizeof(data); i++) {
    data[i] = fgetc(filePointer);
  }
  fclose(filePointer);
  data[i] = '\0';

  Pair uniquePairs[128];
  int uniqueCharacters = 0;

  for (int i = 0; i < (sizeof(uniquePairs) / sizeof(uniquePairs[0])); i++) {
    uniquePairs[i].character = '\0';
    uniquePairs[i].occurrences = 0;
  }

  // Loop to store unique characters and their occurrence count.
  for (int i = 0; i < strlen(data); i++) {
    if (isspace(data[i])) {
      continue;
    }

    // If the character is in the array, add an occurrence to it and skip.
    for (int j = 0; j < (sizeof(uniquePairs) / sizeof(uniquePairs[0])); j++) {
      if (uniquePairs[j].character == data[i]) {
        uniquePairs[j].occurrences++;
        goto next_character;
      }
    }

    // If the character is new, add it to the struct array with one occurrence.
    uniquePairs[uniqueCharacters].character = data[i];
    uniquePairs[uniqueCharacters].occurrences = 1;
    uniqueCharacters++;

  next_character:
    continue;
  }

  // Use selection sort to sort the unique pairs.
  for (int i = 0; i < uniqueCharacters; i++) {
    for (int j = i; j < uniqueCharacters; j++) {
      if (uniquePairs[j].occurrences > uniquePairs[i].occurrences) {
        Pair highestPair = uniquePairs[j];
        uniquePairs[j] = uniquePairs[i];
        uniquePairs[i] = highestPair;
      }
    }
  }

  printf("Unique characters: %u\n", uniqueCharacters);
  for (int i = 0; i < 10; i++) {
    // Don't display 10 characters if there aren't that many.
    if (uniquePairs[i].character == '\0') {
      break;
    }
    printf("%c (%u)\n", uniquePairs[i].character, uniquePairs[i].occurrences);
  }

  return 0;
}
