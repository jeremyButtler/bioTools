/*########################################################
# Name: trimSam
# Use:
#  - trims soft mask regions off all alignments with
#    sequences in a sam file. Aligments without sequences
#    are ignored & not printed out.
# Libraries:
#   - "samEntryStruct.h"
#   o "base10StrToNum.h"
#   o "dataTypeShortHand.h"
# C Standard Libraries:
#   o <stdlib.h>
#   o <stdint.h>
#   o <stdio.h>
#   o <string.h>
########################################################*/

#ifndef TRIMSAM_H
#define TRIMSAM_H

#include "oldSamEntryStruct.h"

/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
| trimSam SOF: Start Of Functions
|  - fun-01 trimSamEntry:
|    o Trim soft mask regions off end of sam entry
|  - fun-02 trimSamReads:
|    o Trims soft mask regions for all reads with a
|      sequence in a sam file
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Name: trimSamEntry (Fun-01:)
| Use:
|  - Trims off the soft masked regions of a sam entry
| Input:
|  - samST:
|    o samEntry struct with sam entry to trim
| Output:
|  - Returns:
|    o 0 if suceeded
|    o 2 if header (invalid and ignored)
|    o 4 if an unmapped read (no reference)
|    o 8 if no sequence line
|  - Modifies:
|    o Trims cigar, sequence, & q-score entries in samST.
\-------------------------------------------------------*/
static uchar trimSamEntry(
    struct samEntry *samST   /*has sam line to trim*/
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-01 TOC: trimSamEntry
   '  - Trims soft masked regions at start & end of sam
   '    entry
   '  o fun-01 sec-01:
   '    - Variable declerations
   '  o fun-01 sec-02:
   '    - Find how much to trim & trim cigar entry
   '  o fun-01 sec-03:
   '    - Trim the sequence entry
   '  o fun-01 sec-04:
   '    - Trim the q-score entry
   '  o fun-01 sec-05:
   '    - Shift characters for other parts of the cigar
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-01 Sec-01:
    ^  - Variable declerations
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    char *incSamUCStr = samST->cigarCStr;
    char *delUCStr = samST->cigarCStr; /*Bases to remove*/

    unsigned char uCharTabCnt =0;/*Counts number of tabs*/

    /*Number of bases soft masked at start or end*/
    uint lenStartTrimUInt = 0;
    uint lenEndTrimUInt = 0;

    /*Loop Iterators*/
    uint uIntSeq=0;
    uchar uCharCnt = uCharTabCnt;

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-01 Sec-02:
    ^  - Find how much to trim & trim cigar entry
    ^  o fun-01 sec-02 sub-01:
    ^    - Check start of cigar & trim if needed
    ^  o fun-01 sec-02 sub-02:
    ^    - Check end of cigar & trim if needed
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    /****************************************************\
    * Fun-01 Sec-02 Sub-01:
    *  - Check start of cigar & trim if needed
    \****************************************************/

    /*Check if is an header is an header entry*/
    if(*(samST->samEntryCStr) == '@') return 2;

    /*Check if is an unmapped read*/
    if(samST->flagUSht & 4) return 4;

    /*Check if has a sequence to trim*/
    if(*(samST->seqCStr) == '*') return 8;

    incSamUCStr =
       base10StrToUI(incSamUCStr, lenStartTrimUInt);

    if(*incSamUCStr != 'S')
    { /*If: no softmasking at start*/
        lenStartTrimUInt = 0;

        /*Move to end of cigar entry*/
        while(*incSamUCStr > 32) ++incSamUCStr;
        delUCStr = incSamUCStr;
    } /*If: no softmasking at start*/

    else
    { /*Else: this is a soft mask and need to remove*/
        ++incSamUCStr; /*Move off soft mask marker*/

        while(*incSamUCStr > 32)
        { /*Loop: Get to end of the cigar*/
            *delUCStr = *incSamUCStr;
            ++incSamUCStr;/*Move to end of cigar entry*/
            ++delUCStr;   /*Move to next base to replace*/
        } /*Loop: Get to end of the cigar*/
    } /*Else: this is a soft mask and need to remove*/

    /****************************************************\
    * Fun-01 Sec-02 Sub-02:
    *  - Check end of cigar & trim if needed
    \****************************************************/

    --delUCStr;        /*Move back to last cigar entry*/

    if(*delUCStr != 'S')
    { /*If: their is not softmasking at the end*/
        lenEndTrimUInt = 0;/*If no soft masking at start*/

        /*Check if there is anyting to trim*/
        if(lenStartTrimUInt == 0) return 0;
        ++delUCStr;           /*Account for minus one*/
    } /*If: their is not softmasking at the end*/

    else
    { /*Else: I am trimming bases off the end*/
        /*Get number of bases to trim*/
        --delUCStr; /*Move soft masking 'S' marker*/

        delUCStr =
           base10BackwardsStrToUI(
              delUCStr,
              lenEndTrimUInt
           );

        ++delUCStr; /*Move off entry before soft mask*/
    } /*Else: I am trimming bases off the end*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-01 Sec-03:
    ^  - Trim the sequence entry
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    if(uCharTabCnt < 4)
    { /*If: I need to find the sequence entry*/
        /*Find the sequence entry*/
        for(
           uCharCnt = uCharTabCnt;
           uCharCnt < 4;
           ++uCharCnt
        ){ /*Loop: get past RNEXT, PNEXT, & TLEN*/
            while(*incSamUCStr > 32) /*Move next entry*/
            { /*Loop: get to end of entry*/
                *delUCStr = *incSamUCStr;
                ++delUCStr;
                ++incSamUCStr;
            } /*Loop: get to end of entry*/

            /*Move leader of the tab*/
            *delUCStr = *incSamUCStr;
            ++delUCStr;
            ++incSamUCStr;
        } /*Loop: get past RNEXT, PNEXT, & TLEN*/
    } /*If: I need to find the sequence entry*/

    else incSamUCStr = samST->seqCStr;

    /*Record new sequence start*/
    samST->seqCStr = delUCStr;

    /*skip the trim region at start*/
    incSamUCStr += lenStartTrimUInt;

    /*Find how long the sequence will be after triming*/
    samST->readLenUInt =
         samST->unTrimReadLenUInt
       - lenStartTrimUInt
       - lenEndTrimUInt;

    for(
       uIntSeq=0;
       uIntSeq < samST->readLenUInt;
       ++uIntSeq
    ){ /*Loop: shift bases back in sequence*/
        *delUCStr = *incSamUCStr;
        ++delUCStr;
        ++incSamUCStr;
    } /*Loop: shift bases back in sequence*/

    /*Move off trimed bases & tab*/
    incSamUCStr += lenEndTrimUInt + 1;

    *delUCStr = *(incSamUCStr - 1);    /*Save the tab*/
    ++delUCStr;                        /*Move past tab*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-01 Sec-04:
    ^  - Trim the q-score entry
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    if(*(samST->qCStr) != '*' || *(samST->qCStr+1) !='\t')
    { /*If their is a Q-score entry to trim*/
        samST->qCStr = delUCStr;  /*new start of q-score*/

        /*skip starting trim region*/
        incSamUCStr += lenStartTrimUInt;

        for(
            uIntSeq=0;
            uIntSeq < samST->readLenUInt;
            ++uIntSeq
        ) { /*Loop: shift bases back in q-score entry*/
            *delUCStr = *incSamUCStr;
            ++delUCStr;
            ++incSamUCStr;
        } /*Loop: shift bases back in q-score entry*/

        /*Move off end trim to tab*/
        incSamUCStr += lenEndTrimUInt;
    } /*If their is a Q-score entry to trim*/

    else samST->qCStr = delUCStr; /*new start of q-score*/

    /*>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>\
    ^ Fun-01 Sec-05:
    ^  - Shift characters for other parts of the cigar
    \<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<*/

    while(*incSamUCStr != '\0' && *incSamUCStr != '\n')
    { /*Loop: Till at end of sam entry*/
        *delUCStr = *incSamUCStr;
        ++delUCStr;
        ++incSamUCStr;
    } /*Loop: Till at end of sam entry*/

    *delUCStr = *incSamUCStr;

    /*Make sure null terminated*/
    if(*delUCStr == '\n') *(delUCStr + 1) = '\0';

    return 0;
} /*trimSamEntry*/

/*-------------------------------------------------------\
| Name: trimSamReads (Fun-02:)
| Use:
|  - Goes though sam file and calls trimSamEntry for each
|    entry
| Input:
|  - samFILE:
|    o Pointer to FILE. Sam file with entries to trim
|  - outFILE:
|    o File to write the trimmed sam file entries to
|  - keepUnmappedReadsBl:
|    o Also print out entries with unmapped reads
| Output:
|  - Prints:
|    o Trimmed sam entries with sequences to outFILE, but
|      ignores sam entries without sequences
\-------------------------------------------------------*/
#define trimSamReads(\
    samFILE,               /*Sam file to trim*/\
    outFILE,               /*File to store output*/\
    keepUnmappedReadsBl    /*1: keep unmapped reads*/\
){ /*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
   ' Fun-02 TOC: trimSamReads
   '  - Goes though sam file and calls trimSamEntry for
   '    each entry
   \~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/\
    \
    unsigned char errUC = 0;  /*Tells if memory error*/\
    struct samEntry samST;\
    \
    initSamEntry(&samST);\
    errUC = readSamLine(&samST, samFILE);\
    \
    while(errUC & 1)\
    { /*While there are lines in sam file to convert*/\
        \
        if(*(samST.samEntryCStr) == '@')\
        { /*If was a header*/\
            printSamEntry(&samST, outFILE);\
            blankSamEntry(&samST);\
            errUC = readSamLine(&samST, samFILE);\
            continue; /*header line, move to next line*/\
        } /*If was a header*/\
        \
        /*Convert & print out sam file entry*/\
        errUC = trimSamEntry(&samST);\
        \
        /*Print out the converted entry*/\
        if(!(errUC >> 2))\
            {printSamEntry(&samST, outFILE);}\
            /*header or sequence*/\
        else if(errUC & 4 && keepUnmappedReadsBl & 1)\
            {printSamEntry(&samST, outFILE);}\
            /*Else if printing umapped reads*/\
        \
        blankSamEntry(&samST);\
        errUC = readSamLine(&samST, samFILE);\
    } /*While there are lines in sam file to convert*/\
    \
    freeStackSamEntry(&samST);\
} /*trimSamReads*/

#endif
