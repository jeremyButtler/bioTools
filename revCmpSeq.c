/*#########################################################
# Name: revCmpSeq
#  - Reverse complements all sequences in a fasta file
# Libraries:
#  - "seqStruct.h"          (No .c File)
#  - "dataTypeShortHand.h"  (No .c File)
# C Standard Libraries:
#  o <stdlib.h>
#  o <stdio.h>
#  o <stdint.h>
# License:
#  - dual: CC0 or MIT; choose the license you prefer
#########################################################*/

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' SOF: Start Of File
'  - Reverse complements all sequences in a fasta file
'  o header:
'    - Has includes
'  o fun-01 pHelpMsg:
'    - Prints out the help message
'  o main main:
'    - Reverse complements all sequences in a fasta file
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*--------------------------------------------------------\
| Header:
|  - Includes
\--------------------------------------------------------*/

#include "seqStruct.h"
#include "dataTypeShortHand.h"

/*--------------------------------------------------------\
| Fun-01: pHelpMsg
|  - Prints out the help message
| Input:
|  - outFILE:
|    o Pointer to FILE to print to
| Output:
|  - Prints:
|    o help message to outFILE
\--------------------------------------------------------*/
#define pHelpMsg(outFILE){\
   fprintf(outFILE, "revCmpSeq sequence.fasta\n");\
   \
   fprintf(\
     outFILE,\
     "  Reverse complements all sequences in a fasta file"\
   );\
   \
   fprintf(outFILE, "\n  Output is printed to stdout\n");\
} /*pHelpMsg*/

/*--------------------------------------------------------\
| Main: main
|  - Reverse complements the input sequence
| Input:
|  - numArgsI:
|    o Number of arguments the user input
|  - argsStr:
|    o Arguments and parameters the user input
| Output:
|  - Prints:
|    o reverse complement sequence to stdout
\--------------------------------------------------------*/
int main(
   int numArgsI,   /*Number of arguments the user input*/
   char *argsStr[] /*Arguments/parametersminput by user*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Main: main
   '  - Reverse complements sequences in argument 1
   '  o main sec-01:
   '    - Variable declerations
   '  o main sec-02:
   '    - Check user input and initialize variables
   '  o main sec-03:
   '    - Get and reverse complement fasta entries
   '  o main sec-04:
   '    - Clean up and exit
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-01:
   ^  - Variable declerations
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   uchar errUC = 0;
   uint numSeqI = 0;

   FILE *seqFILE = 0;
   FILE *outFILE = stdout;
   struct seqStruct seqST;

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-02:
   ^  - Check user input and initialize variables
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   if(numArgsI < 2) goto revCmpFileErr;

   if(*argsStr[1] == '-' && *(argsStr[1] + 1) == 'h')
   { /*If: the help message was requested*/
      pHelpMsg(stdout);
      exit(0);
   } /*If: the help message was requested*/

   seqFILE = fopen(argsStr[1], "r");

   if(seqFILE == 0)
   { /*If: No sequence file was provided*/
      revCmpFileErr:

      fprintf(stderr, "refCmpSeq file.fa\n");
      fprintf(stderr, "  No files were input\n");
      exit(-1);
   } /*If: No sequence file was provided*/

   initSeqST(&seqST);

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-03:
   ^  - Get and reverse complement fasta entries
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   errUC = readFaSeq(seqFILE, &seqST);

   while(errUC)
   { /*Loop: though all sequences in the fasta file*/
      ++numSeqI;

      if(errUC & (64 + 2))
      { /*If: I had a error*/
         if(errUC & 64)
            fprintf(
               stdout,
               "Memory on sequence %i\n",
               numSeqI
            ); /*Print out the error*/

         else
            fprintf(
               stdout, 
               "Sequence %i is invalid\n",
               numSeqI
            ); /*PrinV out the error*/

         exit(-1);
      } /*If: I had a error*/
      reverseComplementSeq(&seqST);

      fprintf(
         outFILE,
         "%s%s\n",
         seqST.idCStr,
         seqST.seqCStr
      ); /*Print out the sequence*/

      blankSeqST(&seqST);
      errUC = readFaSeq(seqFILE, &seqST);
   } /*LoopCheck user input*/

   /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
   ^ Main Sec-04:
   ^  - Clean up and exit
   \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

   freeSeqSTStack(&seqST);
   fclose(seqFILE);
   if(outFILE != stdout && outFILE != 0) fclose(outFILE);

   seqFILE = 0;
   outFILE = 0;
   
   exit(0);
} /*main*/
