/*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\
' samRef SOF: Start Of File
'   - structure functions to get references lengths from
'     a sam file
'   - keeping this separate from samEntry avoids usage
'     of strAry in other programs. This can save 4kb in
'     compile size
'   o header:
'     - forward declarations and guards
'   o .h st01: refs_samRef
'     - holds reference length & name from sam file header
'   o fun01: blank_refs_samRef
'     - blanks a refs_samRef struct
'   o fun02: init_refs_samRef
'     - initializes a refs_samRef struct
'   o fun03: freeStack_refs_samRef
'     - frees variables in a refs_samRef struct
'   o fun04: freeHeap_refs_samRef
'     - frees a refs_samRef struct
'   o fun05: setup_refs_samRef
'     - allocates memory for a refs_samRef struct
'   o fun06: realloc_refs_samRef
'     - reallocates memory for a refs_samRef struct
'   o fun07: getRefLen_samEntry
'     - gets reference ids & length from a sam file header
'   o fun08: findRef_refs_samRef
'     - finds a reference id in a refs_samRef struct
'   o fun09: addRef_samRef
'     - adds reference information to array in refStack
'   o license:
'     - licensing for this code (public domain / mit)
\~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

/*-------------------------------------------------------\
| Header:
|   - forward declarations and guards
\-------------------------------------------------------*/

#ifndef SAM_FILE_GET_REFERENCES_H
#define SAM_FILE_GET_REFERENCES_H

struct samEntry;

/*-------------------------------------------------------\
| ST01: refs_samRef
|   - holds reference length and name from sam file header
\-------------------------------------------------------*/
typedef struct refs_samRef
{
   unsigned int *lenAryUI; /*reference lengths*/
   signed char *idAryStr;  /*reference names (ids)*/
   unsigned int numRefUI;  /*number of references*/
   unsigned int arySizeUI; /*length of length/id arrays*/
}refs_samRef;


/*-------------------------------------------------------\
| Fun01: blank_refs_samRef
|   - blanks a refs_samRef struct
| Input:
|   - refsSTPtr:
|     o pointer to refs_samRef struct to blank
| Output:
|   - Modifies:
|     o refsSTPtr to be blanked
\-------------------------------------------------------*/
void
blank_refs_samRef(
   struct refs_samRef *refSTPtr
);

/*-------------------------------------------------------\
| Fun02: init_refs_samRef
|   - initializes a refs_samRef struct
| Input:
|   - refsSTPtr:
|     o pointer to refs_samRef struct to initialize
| Output:
|   - Modifies:
|     o refsSTPtr to be initialize (everything 0)
\-------------------------------------------------------*/
void
init_refs_samRef(
   struct refs_samRef *refSTPtr
);

/*-------------------------------------------------------\
| Fun03: freeStack_refs_samRef
|   - frees variables in a refs_samRef struct
| Input:
|   - refsSTPtr:
|     o pointer to refs_samRef with variables to free
| Output:
|   - Frees:
|     o lenAryUI and refIdStr arrays in refsSTPtr
\-------------------------------------------------------*/
void
freeStack_refs_samRef(
   struct refs_samRef *refSTPtr
);

/*-------------------------------------------------------\
| Fun04: freeHeap_refs_samRef
|   - frees a refs_samRef struct
| Input:
|   - refsSTPtr:
|     o pointer to refs_samRef struct to free
| Output:
|   - Frees:
|     o refSTPtr (you must set to 0)
\-------------------------------------------------------*/
void
freeHeap_refs_samRef(
   struct refs_samRef *refSTPtr
);

/*-------------------------------------------------------\
| Fun05: setup_refs_samRef
|   - allocates memory for a refs_samRef struct
| Input:
|   - refsSTPtr:
|     o pointer to refs_samRef struct to get memory for
|   - numRefsUI:
|     o number of refs to start out with (0 goes to 16)
| Output:
|   - Mofidifies:
|     o lenAryUI and idAryUI in refSTPtr to have memory
|   - Returns:
|     o 0 for no errors
|     o def_memErr_samEntry for memory errors
\-------------------------------------------------------*/
signed char
setup_refs_samRef(
   struct refs_samRef *refSTPtr,
   unsigned int numRefsUI
);

/*-------------------------------------------------------\
| Fun06: realloc_refs_samRef
|   - reallocates memory for a refs_samRef struct
| Input:
|   - refsSTPtr:
|     o pointer to refs_samRef struct to reallocate
|   - numRefsUI:
|     o new number of refs
| Output:
|   - Mofidifies:
|     o lenAryUI and idAryUI in refSTPtr to be resized
|     o sizeAryUI to be numRefsUI
|     o calls blank_refs_samRef
|   - Returns:
|     o 0 for no errors
|     o def_memErr_samEntry for memory errors
\-------------------------------------------------------*/
signed char
realloc_refs_samRef(
   struct refs_samRef *refSTPtr,
   unsigned int numRefsUI
);

/*-------------------------------------------------------\
| Fun07: getRefLen_samEntry
|   - gets reference ids and length from a sam file header
| Input:
|   - refSTPtr:
|     o pointer to refs_samRef struct to hold id/length
|     o is sorted by reference id (use functions in
|       ../genLib/strAry to access ids)
|   - samSTPtr:
|     o pointer to samEntry struct to hold line after
|       last reference (used for reading sam file)
|   - samFILE:
|     o FILE pointer to sam file header
|   - outFILE:
|     o FILE pointer to print all headers to (0 no print)
|   - headStrPtr:
|     o pointer to c-string to hold non-reference headers
|   - lenHeadULPtr:
|     o unsigned long with headStrPtr length
| Output:
|   - Modifies:
|     o refSTPtr to have reference length and ids
|     o samSTPtr to have first read
|     o samFILE to point to first read after header
|     o outFILE to have header (or not use if 0)
|     o headStrPtr to have non-reference headers
|     o lenHeadULPtr have new headStrPtr size (if resized)
|   - Returns:
|     o 0 for no errors
|     o def_memErr_samEntry for memory errors
|     o def_fileErr_samEntry for file errors
\-------------------------------------------------------*/
signed char
getRefLen_samEntry(
   struct refs_samRef *refSTPtr,/*holds ref lengths*/
   struct samEntry *samSTPtr,    /*for reading sam*/
   void *samFILE,                /*sam file with lengths*/
   void *outFILE,                /*print headers to*/
   signed char **headStrPtr,     /*holds non-ref header*/
   unsigned long *lenHeadULPtr   /*length of headStrPtr*/
);

/*-------------------------------------------------------\
| Fun08: findRef_refs_samRef
|   - finds a reference id in a refs_samRef struct
| Input:
|   - idStr:
|     o c-string with reference id to find
|   - refSTPtr:
|     o pointer to refs_samRef struct with references
| Output:
|   - Returns:
|     o index of reference id if found
|     o < 0 if reference id not in list
\-------------------------------------------------------*/
signed long
findRef_refs_samRef(
   signed char *idStr,            /*id to find*/
   struct refs_samRef *refSTPtr /*holds ref lengths*/
);

/*-------------------------------------------------------\
| Fun09: addRef_samRef
|   - adds reference information to array in refStack
| Input:
|   - idStr:
|     o c-string with id to add
|   - lenUI:
|     o length of reference sequence
|   - refsPtr:
|     o pointer to refs_samRef struct to add ref to
|   - errSCPtr:
|     o pointer to signed char to hold errors
| Output:
|   - Modifies:
|     o idAryStr in refsPtr to have idStr
|     o lenAryUI in refsPtr to have lenUI
|     o numRefUI in refsPtr to be resized if realloc used
|     o arrySizeUI in refsPtr to be incurmented by 1
|     o errSCPtr to be
|       * 0 for no error
|       * def_expand_samEntry if needed to realloc
|       * def_memErr_samEntry for memory error
|   - Returns
|     o index of reference
|     o -1 for errors
\-------------------------------------------------------*/
signed long
addRef_samRef(
   signed char *idStr,
   unsigned int lenUI,
   struct refs_samRef *refsPtr,
   signed char *errSCPtr
);

#endif

/*=======================================================\
: License:
: 
: This code is under the unlicense (public domain).
:   However, for cases were the public domain is not
:   suitable, such as countries that do not respect the
:   public domain or were working with the public domain
:   is inconvient / not possible, this code is under the
:   MIT license.
: 
: Public domain:
: 
: This is free and unencumbered software released into the
:   public domain.
: 
: Anyone is free to copy, modify, publish, use, compile,
:   sell, or distribute this software, either in source
:   code form or as a compiled binary, for any purpose,
:   commercial or non-commercial, and by any means.
: 
: In jurisdictions that recognize copyright laws, the
:   author or authors of this software dedicate any and
:   all copyright interest in the software to the public
:   domain. We make this dedication for the benefit of the
:   public at large and to the detriment of our heirs and
:   successors. We intend this dedication to be an overt
:   act of relinquishment in perpetuity of all present and
:   future rights to this software under copyright law.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO
:   EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM,
:   DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
:   CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR
:   IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
:   DEALINGS IN THE SOFTWARE.
: 
: For more information, please refer to
:   <https://unlicense.org>
: 
: MIT License:
: 
: Copyright (c) 2024 jeremyButtler
: 
: Permission is hereby granted, free of charge, to any
:   person obtaining a copy of this software and
:   associated documentation files (the "Software"), to
:   deal in the Software without restriction, including
:   without limitation the rights to use, copy, modify,
:   merge, publish, distribute, sublicense, and/or sell
:   copies of the Software, and to permit persons to whom
:   the Software is furnished to do so, subject to the
:   following conditions:
: 
: The above copyright notice and this permission notice
:   shall be included in all copies or substantial
:   portions of the Software.
: 
: THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF
:   ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT
:   LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
:   FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO
:   EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE
:   FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN
:   AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
:   FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
:   USE OR OTHER DEALINGS IN THE SOFTWARE.
\=======================================================*/
