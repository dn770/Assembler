/*MMN14 project - Assembler
Netanel Dahan 313565871
--------------------------
This program receives a list of files that represent programs written in Assembly language and translated into machine language.*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

enum {no, yes} ; /* for flags*/

#define INI_IC_VALUE 100
#define STR_LEN 83 /*len 80 ,1- check too long line, 1-\n, 1- \0 */
#define CODE_LEN 1000
#define OPER_NUM 16
#define  spaces {while (isspace(strline[i])) i++ ;} /*skip whitespaces in the lines of input files*/

/*struct for the arry of the operands*/
typedef struct oper { char *name; int opcode; int funct ; } oper ;
/*Define a label struct and define a list of labels*/
typedef struct label {char *name ; int value ; int type ; int ent_ext ; struct label *next ;} label ;
typedef struct labellist {label *head ;} labellist  ;
/*Define an external label struct and define a list of labels to output file ".ext"*/
typedef struct extlab {char *name ; int adress ; struct extlab *next ;} extlab  ;
typedef struct externlist {extlab *head ;} externlist  ;
/*Define a line that consists of an address and a binary encoding of the instruction or prompt*/
typedef struct line {int adress ; int word ;} line ;

/*Function statement*/
int firstscan (FILE *, label *, line *, line *, oper *, int *, int *);
int secondscan (FILE *,label *,extlab *, line *, oper *);
int printfiles (label *,extlab *, line *,line *, char *, int *,int *) ;

