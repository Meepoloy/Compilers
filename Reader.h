/*
************************************************************
* COMPILERS COURSE - Algonquin College
* Code version: Fall, 2023
* Author: TO_DO
* Professors: Paulo Sousa
************************************************************
=---------------------------------------=
|  COMPILERS - ALGONQUIN COLLEGE (F23)  |
=---------------------------------------=
|              ....                     |
|          ........::.::::::.           |
|        .:........::.:^^^~~~:          |
|        :^^::::::^^^::^!7??7~^.        |
|       .:^~~^!77777~~7?YY?7??7^.       |
|       :.^~!??!^::::^^~!?5PY??!~.      |
|       ~!!7J~.:::^^^^~!!~~?G5J?~       |
|       :^~?!~7?Y57^^?PP5YJ!J5Y?:       |
|       .~!!.:^!7!:.:7JYYJ7~7Y7^        |
|       .~77..    . .~^:^^^~7?:         |
|       .^!^~:::.:^!7?~^~!77J:          |
|        ^^!Y~^^^^~?YJ77??7JJ^          |
|       .^7J?~^~~^~7??7??7JY?~:         |
|        ::^^~^7?!^~~!7???J?J7~:.       |
|         ^~~!.^7YPPPP5Y?7J7777~.       |
|        ..:~..:^!JPP5YJ?!777!^.        |
| .~?JJJJJJJJJJYYYYYPPPPPPPPPPPP5PPYY~  |
|  :!Y5GGG.___ YYYYYY__._.PPGGGGGG5!.   |
|   :!Y5G / __| ___ / _(_)__ _ PGP5.    |
|    :~75 \__ \/ _ \  _| / _` | 5?.     |
|     7~7 |___/\___/_| |_\__,_| Y5?.    |
|    .^~!~.....................P5YY7.   |
|   .:::::::::::::?JJJJYYYYYYYYYJJJJ7.  |
|                                       |
=---------------------------------------=
*/

/*
************************************************************
* File name: Reader.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A12.
* Date: May 01 2023
* Professor: Paulo Sousa
* Purpose: This file is the main header for Reader (.h)
************************************************************
*/

/*
 *.............................................................................
 * MAIN ADVICE:
 * Please check the "TODO" labels to develop your activity.
 *.............................................................................
 */

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#define READER_H_

/* TIP: Do not change pragmas, unless necessary .......................................*/
/*#pragma warning(1:4001) *//*to enforce C89 type comments  - to make //comments an warning */
/*#pragma warning(error:4001)*//* to enforce C89 comments - to make // comments an error */

/* standard header files */
#include <stdio.h>  /* standard input/output */
#include <malloc.h> /* for dynamic memory allocation*/
#include <limits.h> /* implementation-defined data type ranges and limits */

/* CONSTANTS DEFINITION: GENERAL (NOT LANGUAGE DEPENDENT) .................................. */

/* Modes (used to create buffer reader) */
enum READER_MODE {
	MODE_FIXED = 'f', /* Fixed mode (constant size) */
	MODE_ADDIT = 'a', /* Additive mode (constant increment to be added) */
	MODE_MULTI = 'm'  /* Multiplicative mode (constant increment to be multiplied) */
};

/* Constants about controls (not need to change) */
#define READER_ERROR		(-1)						/* General error message */
#define READER_TERMINATOR	'\0'							/* General EOF */

/* CONSTANTS DEFINITION: PREFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* You should add your own constant definitions here */
#define READER_MAX_SIZE	INT_MAX-1	/* maximum capacity */ 

#define READER_DEFAULT_SIZE			250		/* default initial buffer reader capacity */
#define READER_DEFAULT_INCREMENT	10		/* default increment factor */

/* Add your bit-masks constant definitions here - Defined for BOA */
/* BITS                                (7654.3210) */
#define READER_DEFAULT_FLAG 0x00 	/* (0000.0000)_2 */
/* TO_DO: BIT 3: END = End of buffer flag */
#define READER_END_FLAG 0x08 /*0000.1000*/
/* TO_DO: BIT 2: REL = Rellocation memory flag */
#define READER_RELOCATION_FLAG 0x04 /*0000.0100*/
/* TO_DO: BIT 1: EMP = Buffer empty flag */
#define READER_EMPTY_FLAG 0x02 /*0000.0010*/
/* TO_DO: BIT 0: FUL = Buffer full flag */
#define READER_FULL_FLAG 0x01	/*0000.0001*/

#define NCHAR				128			/* Chars from 0 to 127 */

#define CHARSEOF			(-1)		/* EOF Code for Reader */

/* STRUCTURES DEFINITION: SUFIXED BY LANGUAGE NAME (SOFIA) .................................. */

/* TODO: Adjust datatypes */

/* Offset declaration */
typedef struct position {
	weaver_intg mark;			/* the offset to the mark position (in chars) */
	weaver_intg read;			/* the offset to the get a char position (in chars) */
	weaver_intg wrte;			/* the offset to the add chars (in chars) */
} Position;

/* Buffer structure */
typedef struct bufferReader {
	weaver_string	content;			/* pointer to the beginning of character array (character buffer) */
	weaver_intg		size;				/* current dynamic memory size (in bytes) allocated to character buffer */
	weaver_intg		increment;			/* character array increment factor */
	weaver_intg		mode;				/* operational mode indicator */
	weaver_byte		flags;				/* contains character array reallocation flag and end-of-buffer flag */
	Position		position;				/* Offset / position field */
	weaver_intg		histogram[NCHAR];	/* Statistics of chars */
	weaver_intg		numReaderErrors;	/* Number of errors from Reader */
} Buffer, *BufferPointer;

/* FUNCTIONS DECLARATION:  .................................. */
/* General Operations */
BufferPointer	readerCreate		(weaver_intg, weaver_intg, weaver_intg);
BufferPointer	readerAddChar		(BufferPointer const, weaver_char);
weaver_boln		readerClear		    (BufferPointer const);
weaver_boln		readerFree		    (BufferPointer const);
weaver_boln		readerIsFull		(BufferPointer const);
weaver_boln		readerIsEmpty		(BufferPointer const);
weaver_boln		readerSetMark		(BufferPointer const, weaver_intg);
weaver_intg		readerPrint		    (BufferPointer const);
weaver_intg		readerLoad			(BufferPointer const, FILE* const);
weaver_boln		readerRecover		(BufferPointer const);
weaver_boln		readerRetract		(BufferPointer const);
weaver_boln		readerRestore		(BufferPointer const);
/* Getters */
weaver_char		readerGetChar		(BufferPointer const);
weaver_string	readerGetContent	(BufferPointer const, weaver_intg);
weaver_intg		readerGetPosRead	(BufferPointer const);
weaver_intg		readerGetPosWrte	(BufferPointer const);
weaver_intg		readerGetPosMark	(BufferPointer const);
weaver_intg		readerGetSize		(BufferPointer const);
weaver_intg		readerGetInc		(BufferPointer const);
weaver_intg		readerGetMode		(BufferPointer const);
weaver_byte		readerGetFlags		(BufferPointer const);
weaver_void		readerPrintStat		(BufferPointer const);
weaver_intg		readerNumErrors		(BufferPointer const);

#endif
