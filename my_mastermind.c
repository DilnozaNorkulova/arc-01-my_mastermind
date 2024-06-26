#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>

int wellPlacedPieces(char *code, char *input);

int misplacedPieces(char *code, char *input);

bool isEmpty(const char *str)
{
  return (str == NULL || str[0] == '\0');
}

int checkDigitRange(char *code)
{
  for (int i = 0; i < 4; i++)
  {
    if (code[i] < '0' || code[i] > '8')
    {
      return 0;
    }
  }
  return 1;
}

int getCodeLength(char *code)
{
  int length = 0;
  while (code[length] != '\0')
  {
    length++;
  }
  return length;
}

int checkDuplicateDigits(char *code)
{
  for (int i = 0; i < 3; i++)
  {
    for (int a = i + 1; a < 4; a++)
    {
      if (code[i] == code[a])
      {
        return 0;
      }
    }
  }
  return 1;
}

int checkGamerCode(char *code)
{
  int length = getCodeLength(code);
  if (length != 4)
  {
    return 0;
  }
  else if (!checkDigitRange(code))
  {
    return 0;
  }
  else if (!checkDuplicateDigits(code))
  {
    return 0;
  }
  return 1;
}

char *readStdInput()
{
  char *buffer = malloc(100);
  int i = 0;
  char c;

  while (read(0, &c, 1))
  {
    if(c == '\n'){
      return buffer;
    }
    buffer[i] = c;
    i++;
  }
  
  return "EOT";
}

int isHave(char *secretCode, char randCode)
{
  while (*secretCode != '\0')
  {
    if (*secretCode == randCode)
    {
      return 1;
    }
    secretCode++;
  }
  return 0;
}

void playGame(int attempts, char *code)
{
  int flag = 0;
  for (int i = 0; i < attempts; i++)
  {
    if (flag == 0)
    {
      printf("---\n");
      printf("Round %d\n", i);
    }
    write(1, ">", 1);
    char *input = readStdInput();
    if ( strcmp(input, "EOT") == 0)
    {
      return;
    }
    if (checkGamerCode(input) == 0)
    {
      printf("Wrong input!\n");
      flag = 1;
      i--;
      continue;
    }
    int wellp = wellPlacedPieces(code, input);
    int missp = misplacedPieces(code, input);
    if (wellp == 4)
    {
      printf("Congratz! You did it!\n");
      return;
    }
    else
    {
      printf("Well placed pieces: %d\n", wellp);
      printf("Misplaced pieces: %d\n", missp);
      flag = 0;
    }
  }
}

char *generateSecretCode()
{
  char *secretCode = malloc(5);
  srand(time(NULL));
  for (int i = 0; i < 4; i++)
  {
    char randCode;
    do
    {
      randCode = rand() % 8 + '0';
    } while (isHave(secretCode, randCode));
    secretCode[i] = randCode;
  }
  secretCode[4] = '\0';
  return secretCode;
}

int misplacedPieces(char *code, char *input)
{
  int missp = 0;
  for (int i = 0; code[i] != '\0'; i++)
  {
    for (int j = 0; input[j] != '\0'; j++)
    {
      if (code[i] == input[j] && i != j)
      {
        missp++;
      }
    }
  }
  return missp;
}

int wellPlacedPieces(char *code, char *input)
{
  int wellp = 0;
  for (int i = 0; code[i] != '\0'; i++)
  {
    if (code[i] == input[i])
    {
      wellp++;
    }
  }
  return wellp;
}

void my_mastermind(int argc, char **argv)
{
  char *code = calloc(5, sizeof(char));
  int attempts = 10;
  int flag = 0;
  printf("Will you find the secret code?\n");
  printf("Please enter a valid guess \n");
  for (int i = 1; i < argc; i++)
  {
    if (strcmp(argv[i], "-c") == 0)
    {
      i++;
      code = argv[i];
      flag = 1;
    }
    else if (strcmp(argv[i], "-t") == 0)
    {
      i++;
      attempts = atoi(argv[i]);
    }
  }
  if (flag == 0)
  {
    code = generateSecretCode();
  }
  printf("code : %s\n", code);
  playGame(attempts, code);
}

int main(int argc, char **argv)
{
  my_mastermind(argc, argv);
  return 0;
}
