/*
 * COP 3515 – Fall Semester 2022
 *
 * Homework #4: Securing The Message
 *
 * (Your Name)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#include "SBox.h"
#include "MixColumns.h"

unsigned int rotWords[11][16] = {
    {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c},
    {0xa0, 0xfa, 0xfe, 0x17, 0x88, 0x54, 0x2c, 0xb1, 0x23, 0xa3, 0x39, 0x39, 0x2a, 0x6c, 0x76, 0x05},
    {0xf2, 0xc2, 0x95, 0xf2, 0x7a, 0x96, 0xb9, 0x43, 0x59, 0x35, 0x80, 0x7a, 0x73, 0x59, 0xf6, 0x7f},
    {0x3d, 0x80, 0x47, 0x7d, 0x47, 0x16, 0xfe, 0x3e, 0x1e, 0x23, 0x7e, 0x44, 0x6d, 0x7a, 0x88, 0x3b},
    {0xef, 0x44, 0xa5, 0x41, 0xa8, 0x52, 0x5b, 0x7f, 0xb6, 0x71, 0x25, 0x3b, 0xdb, 0x0b, 0xad, 0x00},
    {0xd4, 0xd1, 0xc6, 0xf8, 0x7c, 0x83, 0x9d, 0x87, 0xca, 0xf2, 0xb8, 0xbc, 0x11, 0xf9, 0x15, 0xbc},
    {0x6d, 0x88, 0xa3, 0x7a, 0x11, 0x0b, 0x3e, 0xfd, 0xdb, 0xf9, 0x86, 0x41, 0xca, 0x00, 0x93, 0xfd},
    {0x4e, 0x54, 0xf7, 0x0e, 0x5f, 0x5f, 0xc9, 0xf3, 0x84, 0xa6, 0x4f, 0xb2, 0x4e, 0xa6, 0xdc, 0x4f},
    {0xea, 0xd2, 0x73, 0x21, 0xb5, 0x8d, 0xba, 0xd2, 0x31, 0x2b, 0xf5, 0x60, 0x7f, 0x8d, 0x29, 0x2f},
    {0xac, 0x77, 0x66, 0xf3, 0x19, 0xfa, 0xdc, 0x21, 0x28, 0xd1, 0x29, 0x41, 0x57, 0x5c, 0x00, 0x6e},
    {0xd0, 0x14, 0xf9, 0xa8, 0xc9, 0xee, 0x25, 0x89, 0xe1, 0x3f, 0x0c, 0xc8, 0xb6, 0x63, 0x0c, 0xa6}};

/*
 * Function Title: Input Split
 *
 * Description: Separate the user entered text string into
 * integers and create a single integer for each two
 * characters in the entered string.
 *
 * Inputs: enteredKey - 32 character user entered string
 *
 * Outputs: splitKey - 16 integer array that contains the
 *                     horizontally split input
 *
 */
void InputSplit(char enteredText[], unsigned int splitText[])
{

  int indexValue = 0;
  int splitIndex = 0;
  char currentChar;
  unsigned char tempText[33] = "0"; /* holds split values */
  int locIndex = 0;                 /* used to step through array */

  for (int index = 0; index < strlen(enteredText); index = index + 2)
  {
    indexValue = 0;
    for (int pairValue = 0; pairValue < 2; pairValue++)
    {
      currentChar = enteredText[index + pairValue];
      switch (currentChar)
      {
      case '0':
        indexValue += 0;
        break;
      case '1':
        indexValue += 1;
        break;
      case '2':
        indexValue += 2;
        break;
      case '3':
        indexValue += 3;
        break;
      case '4':
        indexValue += 4;
        break;
      case '5':
        indexValue += 5;
        break;
      case '6':
        indexValue += 6;
        break;
      case '7':
        indexValue += 7;
        break;
      case '8':
        indexValue += 8;
        break;
      case '9':
        indexValue += 9;
        break;
      case 'a':
        indexValue += 10;
        break;
      case 'b':
        indexValue += 11;
        break;
      case 'c':
        indexValue += 12;
        break;
      case 'd':
        indexValue += 13;
        break;
      case 'e':
        indexValue += 14;
        break;
      case 'f':
        indexValue += 15;
        break;
      case 'A':
        indexValue += 10;
        break;
      case 'B':
        indexValue += 11;
        break;
      case 'C':
        indexValue += 12;
        break;
      case 'D':
        indexValue += 13;
        break;
      case 'E':
        indexValue += 14;
        break;
      case 'F':
        indexValue += 15;
        break;
      } /* switch */
      if (pairValue == 0)
        indexValue = indexValue << 4;
    } /* for */
    tempText[splitIndex] = indexValue;
    splitIndex++;
  } /* for */

  /* Place tempText values into splitText in a
   * horizontal fashion */
  for (int index = 0; index < 4; index++)
  {
    splitText[index + 0] = tempText[locIndex + index];
    splitText[index + 4] = tempText[locIndex + index + 1];
    splitText[index + 8] = tempText[locIndex + index + 2];
    splitText[index + 12] = tempText[locIndex + index + 3];
    locIndex += 3;
  } /* for */

  printf("\nSplit Input:\n");
  for (int index = 0; index < 16; index++)
  {
    printf("%3x ", splitText[index]);
    if (index == 3 || index == 7 || index == 11 || index == 15)
      printf("\n");
  } /* for */
} /* InputSplit */

/*
 * Function Title: AddRoundKey
 *
 * Description: Take the current AES Key and add the
 * current rotword to it using the XOR function.
 *
 * Inputs: AESSplitKey - 16 integer array that contains
 *         the split key
 *
 *         resultMatrix - 16 unsigned int array holding result of adding Round Key
 *
 *         rotData - tabel of rotword constants
 *
 *         rotWordIndex - rot word to use in operation
 *
 * Outputs: AESSplitKey - 16 integer array that
 *          contains the XOR of the split key and the
 *          current rotword.
 *
 */

void AddRoundKey(unsigned int pTextSplit[], unsigned int resultMatrix[16], unsigned int rotData[11][16], int rotWordIndex)
{
  for (int i = 0; i < 4; i++)
  {
    for (int j = 0; j < 4; j++)
    {
      resultMatrix[j + (4 * i)] = rotData[rotWordIndex][i + (4 * j)] ^ pTextSplit[j + (4 * i)];
    }
  }

  printf("\nResult Matrix:\n");
  for (int index = 0; index < 16; index++)
  {
    printf("%3x ", resultMatrix[index]);
    if (index == 3 || index == 7 || index == 11 || index == 15)
      printf("\n");
  } /* for */

} /* AddRoundKey */

/*
 * Function Title: CalcSubBytes
 *
 * Description: Uses the result of the AddRoundKey
 * process to look up values in the sbox array which
 * are then stored in the subBytesArray array.
 *
 * Inputs: plainText - 16 integer array that contains
 *         the result of the AddRoundKey step.
 *
 * * Outputs: subBytesArray - result of looking up sbox
 *          values that match the values stored in the
 *          plainText array.
 *
 */
void CalcSubBytes(int subBytesArray[], char plainText[])
{

} /* CalcSubBytes */

/*
 * Function Title: ShiftRows
 *
 * Description: Uses the result of the SubBytes
 * process to shift where they are stored.
 *
 * Inputs: subBytes - 16 integer array that contains
 *         the result of the SubBytes step.
 *
 * Outputs: shiftRows - result of changing where the
 *            subBytes values are stored in an array.
 *
 */
void ShiftRows(int subBytes[], int shiftRows[])
{

} /* ShiftRows */

/*
 * Function Title: MixColumns
 *
 * Description: Uses the result of the SubBytes
 * process to shift where they are stored.
 *
 * Inputs: shiftRows - result of changing where the
 *         subBytes values are stored in an array.
 *
 * Output: mixCols - result of multiplying shiftRows
 *         by multiplication matrix
 *
 */
void MixColumns(int shiftRows[], int mixCols[])
{

  int currentValue, lookUpValue;
  unsigned char tmp[15];

  int multMatrix[4][4] =
      {{2, 3, 1, 1},
       {1, 2, 3, 1},
       {1, 1, 2, 3},
       {3, 1, 1, 2}}; /* used to multiply shiftRows */

  mixCols[0] = (unsigned char)(table_2[shiftRows[0]] ^ table_3[shiftRows[4]] ^ shiftRows[8] ^ shiftRows[12]);
  mixCols[1] = (unsigned char)(shiftRows[0] ^ table_2[shiftRows[4]] ^ table_3[shiftRows[8]] ^ shiftRows[12]);
  mixCols[2] = (unsigned char)(shiftRows[0] ^ shiftRows[4] ^ table_2[shiftRows[8]] ^ table_3[shiftRows[12]]);
  mixCols[3] = (unsigned char)(table_3[shiftRows[0]] ^ shiftRows[4] ^ shiftRows[8] ^ table_2[shiftRows[12]]);

  mixCols[4] = (unsigned char)(table_2[shiftRows[1]] ^ table_3[shiftRows[5]] ^ shiftRows[9] ^ shiftRows[13]);
  mixCols[5] = (unsigned char)(shiftRows[1] ^ table_2[shiftRows[5]] ^ table_3[shiftRows[9]] ^ shiftRows[13]);
  mixCols[6] = (unsigned char)(shiftRows[1] ^ shiftRows[5] ^ table_2[shiftRows[9]] ^ table_3[shiftRows[13]]);
  mixCols[7] = (unsigned char)(table_3[shiftRows[1]] ^ shiftRows[5] ^ shiftRows[9] ^ table_2[shiftRows[13]]);

  mixCols[8] = (unsigned char)(table_2[shiftRows[2]] ^ table_3[shiftRows[6]] ^ shiftRows[10] ^ shiftRows[14]);
  mixCols[9] = (unsigned char)(shiftRows[2] ^ table_2[shiftRows[6]] ^ table_3[shiftRows[10]] ^ shiftRows[14]);
  mixCols[10] = (unsigned char)(shiftRows[2] ^ shiftRows[6] ^ table_2[shiftRows[10]] ^ table_3[shiftRows[14]]);
  mixCols[11] = (unsigned char)(table_3[shiftRows[2]] ^ shiftRows[6] ^ shiftRows[10] ^ table_2[shiftRows[14]]);

  mixCols[12] = (unsigned char)(table_2[shiftRows[3]] ^ table_3[shiftRows[7]] ^ shiftRows[11] ^ shiftRows[15]);
  mixCols[13] = (unsigned char)(shiftRows[3] ^ table_2[shiftRows[7]] ^ table_3[shiftRows[11]] ^ shiftRows[15]);
  mixCols[14] = (unsigned char)(shiftRows[3] ^ shiftRows[7] ^ table_2[shiftRows[11]] ^ table_3[shiftRows[15]]);
  mixCols[15] = (unsigned char)(table_3[shiftRows[3]] ^ shiftRows[7] ^ shiftRows[11] ^ table_2[shiftRows[15]]);

  printf("\n==> Result of Mix Columns step:\n");
  for (int index = 0; index < 16; index++)
  {
    printf("%x ", mixCols[index]);
    if (index == 3 || index == 7 || index == 11 || index == 15)
      printf("\n");
  } /* for */

} /* MixColumns */

int main(void)
{

  unsigned char clearText[4]; /* text to be encoded */
  char action;                /* encrypt/decrypt flag */
  char AESKey[33] = "0";      /* user AES key value */
  char plainText[33] = "0";   /* user plain text value */

  unsigned int AESSplitKey[16];     /* AES key split into 16 bytes */
  unsigned int plainTextSplit[16];  /* plain text split into 16 bytes */
  unsigned int encryptedText[16];   /* string copy holder */
  unsigned int subBytes[16] = {0};  /* holds results of subytes step */
  unsigned int shiftRows[16] = {0}; /* holds results of shift rows step */
  unsigned int mixCols[16] = {0};   /* holds results of mix columns step */
  unsigned int temp[16] = {0};      /* holds mixColss when they are being flipped from horiz to vert */
  int currentRotWord = 0;           /* next rotword to use */
  int encryptRound = 1;             /* current encryption round */

  /* Load the hard coded AES key value */
  strcpy(AESKey, "2b7e151628aed2a6abf7158809cf4f3c");
  printf("\nThe AES key is: %s\n", AESKey);

  /* Split the user entered AES key string into 16
   * separate integers */

  InputSplit(AESKey, AESSplitKey);

  /*
   * Ask the user to enter their 16 byte plaintext
   * Trap errors if they don't enter enough
   * information
   */

  while (strlen(plainText) < 32 && plainText[0] != 'x')
  {
    printf("Please enter your 32 character, 16 hexidecinal digit plaintext \n(x for default):\n");
    scanf("%s", &plainText);
  }

  if (plainText[0] == 'x')
  {
    strcpy(plainText, "3243f6a8885a308d313198a2e0370734");
  } /* if */
  printf("\nThe plain text is: %s\n", plainText);

  /* Split the user entered plain text string into
   * 16 separate integers */
  InputSplit(plainText, plainTextSplit);

  printf("\n*** Starting round %d of encryption\n", encryptRound);

  /* AddRoundKey */
  AddRoundKey(plainTextSplit, temp, rotWords, currentRotWord);

  for (int encryptLoop = 0; encryptLoop < 9; encryptLoop++)
  {

    /* SubBytes */

    /* ShiftRows */

    /* MixColumns */

    /* AddRoundKey */
    currentRotWord++;
    AddRoundKey(plainTextSplit, temp, rotWords, currentRotWord);

    encryptRound++;
    printf("\n*** Starting round %d of encryption\n", encryptRound);

  } /* for */

  /* SubBytes */

  /* ShiftRows */

  /* AddRoundKey */
  currentRotWord++;

  /* Display final encoded text */

  return 0;
}