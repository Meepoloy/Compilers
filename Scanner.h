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
* File name: Scanner.h
* Compiler: MS Visual Studio 2022
* Course: CST 8152 � Compilers, Lab Section: [011, 012]
* Assignment: A22, A32.
* Date: May 01 2022
* Purpose: This file is the main header for Scanner (.h)
* Function list: (...).
*************************************************************/

#ifndef COMPILERS_H_
#include "Compilers.h"
#endif

#ifndef READER_H_
#include "Reader.h"
#endif

#ifndef SCANNER_H_
#define SCANNER_H_

#ifndef NULL
#include <_null.h> /* NULL pointer constant is defined there */
#endif

/*#pragma warning(1:4001) */	/*to enforce C89 type comments  - to make //comments an warning */

/*#pragma warning(error:4001)*/	/* to enforce C89 comments - to make // comments an error */

/* Constants */
#define VID_LEN 20  /* variable identifier length */
#define ERR_LEN 40  /* error message length */
#define NUM_LEN 5   /* maximum number of digits for IL */

#define RTE_CODE 1  /* Value for run-time error */

/* TO_DO: Define the number of tokens */
#define NUM_TOKENS 13

/* TO_DO: Define Token codes - Create your token classes */
enum TOKENS {
	ERR_T,		/*  0: Error token */
	MNID_T,		/*  1: Method name identifier token (start: &) */
	INL_T,		/*  2: Integer literal token */
	STR_T,		/*  3: String literal token */
	LPR_T,		/*  4: Left parenthesis token */
	RPR_T,		/*  5: Right parenthesis token */
	FLT_T,		/*  6: Float literal token */
	//COL_T,		/*  7: Colon token */
	KW_T,		/*  8: Keyword token */
	EOS_T,		/*  9: End of function (colon) */
	RTE_T,		/* 10: Run-time error token */
	SEOF_T,		/* 11: Source end-of-file token */
	CMT_T		/* 12: Comment token */
};

/* TO_DO: Define the list of keywords */
static weaver_string tokenStrTable[NUM_TOKENS] = {
	"ERR_T",
	"MNID_T",
	"INL_T",
	"STR_T",
	"LPR_T",
	"RPR_T",
	"LBR_T",
	"RBR_T",
	"KW_T",
	"EOS_T",
	"RTE_T",
	"SEOF_T",
	"CMT_T"
};

/* TO_DO: Operators token attributes */
typedef enum ArithmeticOperators { OP_ADD, OP_SUB, OP_MUL, OP_DIV } AriOperator;
typedef enum RelationalOperators { OP_EQ, OP_NE, OP_GT, OP_LT } RelOperator;
typedef enum LogicalOperators { OP_AND, OP_OR, OP_NOT } LogOperator;
typedef enum SourceEndOfFile { SEOF_0, SEOF_255 } EofOperator;

/* TO_DO: Data structures for declaring the token and its attributes */
typedef union TokenAttribute {
	weaver_intg codeType;      /* integer attributes accessor */
	AriOperator arithmeticOperator;		/* arithmetic operator attribute code */
	RelOperator relationalOperator;		/* relational operator attribute code */
	LogOperator logicalOperator;		/* logical operator attribute code */
	EofOperator seofType;				/* source-end-of-file attribute code */
	weaver_intg intValue;				/* integer literal attribute (value) */
	weaver_intg keywordIndex;			/* keyword index in the keyword table */
	weaver_intg contentString;			/* string literal offset from the beginning of the string literal buffer (stringLiteralTable->content) */
	weaver_real floatValue;				/* floating-point literal attribute (value) */
	weaver_char idLexeme[VID_LEN + 1];	/* variable identifier token attribute */
	weaver_char errLexeme[ERR_LEN + 1];	/* error token attribite */
} TokenAttribute;

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct idAttibutes {
	weaver_byte flags;			/* Flags information */
	union {
		weaver_intg intValue;				/* Integer value */
		weaver_real floatValue;			/* Float value */
		weaver_string stringContent;		/* String value */
	} values;
} IdAttibutes;

/* Token declaration */
typedef struct Token {
	weaver_intg code;				/* token code */
	TokenAttribute attribute;	/* token attribute */
	IdAttibutes   idAttribute;	/* not used in this scanner implementation - for further use */
} Token;

/* Scanner */
typedef struct scannerData {
	weaver_intg scanHistogram[NUM_TOKENS];	/* Statistics of chars */
} ScannerData, * pScanData;

///////////////////////////////////////////////////////////////////////////////////////////////////////

/* EOF definitions */
#define CHARSEOF0 '\0'
#define CHARSEOF255 0xFFFFFFFFFFFFFFCD

/*  Special case tokens processed separately one by one in the token-driven part of the scanner:
 *  LPR_T, RPR_T, LBR_T, RBR_T, EOS_T, SEOF_T and special chars used for tokenis include _, & and ' */

/* TO_DO: Define lexeme FIXED classes */
/* These constants will be used on nextClass */
#define CHRCOL2 '#'		//'_'
#define CHRCOL3	'^'		//'&'
#define CHRCOL4	'\''	//'\''
#define CHRCOL5	'\n'	//'#'
#define CHRCOL6	'\t'	//'#'
#define CHRCOL7	' '		//'#'
#define CHRCOL8	'.'	//'#'
#define CHRCOL9	'-'	//'#'
#define CHRCOL10	'_'	//'#'

/* These constants will be used on VID / MID function */
#define MNID_SUF '('
#define COMM_SYM '#'

/* TO_DO: Error states and illegal state */
#define ESNR	16		/* Error state with no retract */
#define ESWR	17		/* Error state with retract */
#define FS		18		/* Illegal state */

 /* TO_DO: State transition table definition */
#define NUM_STATES		18
#define CHAR_CLASSES	12

/* TO_DO: Transition table - type of states defined in separate table */
static weaver_intg transitionTable[NUM_STATES][CHAR_CLASSES] = {
/*    [A-z],[0-9],    #,    ^,   \',   \n,   \t,  ' ',    .,    -,     _,  other
	   L(0), D(1), H(2), C(3), Q(4), T(5), I(6), S(7), Z(8), N(9), U(10), O(11) */
	{     6,	9,    1, ESNR,   13,	0,	  0,	0, ESNR,    9,    7, ESNR},	// S0: NOAS
	{     1,    1,    1,    3,    1,    2,	  1,    1,    1,    1,    1,    1},	// S1: NOAS
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS},	// S2: ASNR (single-line comment)
	{     3,    3,    3,    4,    3,    3,    3,    3,    3,    3,    3,    3},	// S3: NOAS
	{  ESNR, ESNR,    5, ESNR, ESNR, ESWR, ESNR, ESNR, ESNR, ESNR, ESNR, ESNR},	// S4: NOAS
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS},	// S5: ASNR (multi-line comment)
	{     6,    7,    7,    7,    7,   15,	 15,   15,    7,    7,    7,    7},	// S6: NOAS 
	{     7,    7,    8,    8,    8,    8,    8,    8,    8,    8,    7,    8},	// S7: NOAS
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS},	// S8: ASWR (variable name or method name)
	{    10,    9,   10,   10,   10,   10,	 10,   10,   11,   10,   10,   10},	// S9: NOAS
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS},	// S10: ASWR (IS)
	{    12,   11,   12,   12,   12,   12,	 12,   12,   12,   12,   12,   12},	// S11: NOAS
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS},	// S12: ASWR (FL)
	{    13,   13,   13,   13,   14,   13,	 13,   13,   13,   13,   13,   13},	// S13: NOAS
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS},  // S14: ASWR (SL)
	{    FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS,   FS}  // S15: ASWR (key)
};

/* Define accepting states types */
#define NOFS	0		/* not accepting state */
#define FSNR	1		/* accepting state with no retract */
#define FSWR	2		/* accepting state with retract */

/* TO_DO: Define list of acceptable states */
static weaver_intg stateType[NUM_STATES] = {
	NOFS, /* 00 */
	NOFS, /* 01 */
	FSNR, /* 02 */
	NOFS, /* 03 */
	NOFS, /* 04 */
	FSNR, /* 05 */
	NOFS, /* 06 */
	NOFS, /* 07 */
	FSWR, /* 08 */
	NOFS, /* 09 */
	FSWR, /* 10 */
	NOFS, /* 11 */
	FSWR, /* 12 */
	NOFS, /* 13 */
	FSWR, /* 14 */
	FSWR, /* 15 comment addition*/
	FSNR, /* 16 (Err1 - no retract) */
	FSWR  /* 17 (Err2 - retract) */
};

/*
-------------------------------------------------
TO_DO: Adjust your functions'definitions
-------------------------------------------------
*/

/* Static (local) function  prototypes */
weaver_intg			startScanner(BufferPointer psc_buf);
static weaver_intg	nextClass(weaver_char c);					/* character class function */
static weaver_intg	nextState(weaver_intg, weaver_char);		/* state machine function */
weaver_void			printScannerData(ScannerData scData);
Token				tokenizer(weaver_void);

/*
-------------------------------------------------
Automata definitions
-------------------------------------------------
*/

/* TO_DO: Pointer to function (of one char * argument) returning Token */
typedef Token(*PTR_ACCFUN)(weaver_string lexeme);

/* Declare accepting states functions */
Token funcSL	(weaver_string lexeme);
Token funcIL	(weaver_string lexeme);
Token funcID	(weaver_string lexeme);
Token funcCMT   (weaver_string lexeme);
Token funcKEY	(weaver_string lexeme);
Token funcErr	(weaver_string lexeme);
Token funcFL	(weaver_string lexeme);

/* 
 * Accepting function (action) callback table (array) definition 
 * If you do not want to use the typedef, the equvalent declaration is:
 */

/* TO_DO: Define final state table */
static PTR_ACCFUN finalStateTable[NUM_STATES] = {
	NULL,		/* -			[00] */
	NULL,		/* -			[01] */
	funcCMT,	/* COM-SL		[02] */
	NULL,		/* -			[03] */
	NULL,		/* -			[04] */
	funcCMT,	/* COM-SL		[05] */
	NULL,	/* KEY			[06] */
	NULL,		/* -			[07] */
	funcID,		/* MNID/VARID	[08] */
	NULL,		/* -			[09] */
	funcIL,		/* IL			[10] */
	NULL,		/* -			[11] */
	funcIL,		/* FL			[12] */
	NULL,		/* -			[13] */
	funcSL,		/* SL			[14] */
	funcKEY,	/* KEY			[15] */
	funcErr,	/* ERR1 [06] */
	funcErr		/* ERR2 [07] */
};

/*
-------------------------------------------------
Language keywords
-------------------------------------------------
*/

/* TO_DO: Define the number of Keywords from the language */
#define KWT_SIZE 10

/* TO_DO: Define the list of keywords */
static weaver_string keywordTable[KWT_SIZE] = {
	"def",		/* KW00 */
	"data",		/* KW01 */
	"int",		/* KW02 */
	"float",	/* KW03 */
	"string",	/* KW04 */
	"if",		/* KW05 */
	"then",		/* KW06 */
	"else",		/* KW07 */
	"while",	/* KW08 */
	"do"		/* KW09 */
};

/* NEW SECTION: About indentation */

/*
 * Scanner attributes to be used (ex: including: intendation data
 */

#define INDENT '\t'  /* Tabulation */

/* TO_DO: Should be used if no symbol table is implemented */
typedef struct languageAttributes {
	weaver_char indentationCharType;
	weaver_intg indentationCurrentPos;
	/* TO_DO: Include any extra attribute to be used in your scanner (OPTIONAL and FREE) */
} LanguageAttributes;

/* Number of errors */
weaver_intg numScannerErrors;

/* Scanner data */
ScannerData scData;

#endif
