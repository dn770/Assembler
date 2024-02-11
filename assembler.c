#include "assembler.h"
/*This function receives a list of files, defines the data structures and reads in a loop the transition functions and the print function*/
int main (int argc, char *argv []) {
/*declaration*/
 oper operarr [] = {{"mov",0,0}, {"camp",1,0}, {"add",2,1}, {"sub",2,2}, {"lea",4,0}, {"clr",5,1}, {"not",5,2}, {"inc",5,3}, {"dec",5,4}, {"jmp",9,1}, {"bne",9,2}, {"jsr",9,3}, {"red",12,0}, {"prn",13,0},   {"rts",14,0},{"stop",15,0}} ;
 labellist lablist ;
 externlist extlist ;
 line code [CODE_LEN] ; /*Arry of sentences for instructions*/
 line data [CODE_LEN] ; /*Arry of sentences for guidance*/
 int i;
 FILE *fin ; /*ptr to input file*/
 int *ICF ; /*Final number of instructions lines in the file*/
 int *DCF ; /*Final number of guidance lines in the file*/
 char *fname ; /*name of input file*/
 
 if (argc==1){ /*No found files to open*/
   fprintf(stdout,"\nThere havn't input files.\n");
   return 0;
  }

  for (i=1; i<argc ; i++) { /*loop for input files */
    fname = argv[i] ;
    if (!(fin = fopen (strcat(fname, ".as"),"r"))){/*open file, if not- error*/
       fprintf(stdout, "\nCan't open the  input file\n");
       continue;
    }
    if (firstscan (fin,lablist.head, code, data, operarr, ICF, DCF)){ /*call to function to first scan*/
        fprintf(stdout, "\n Found errors in the file %s \n", strcat( fname, ".as"));
        continue;
    }
    /*Move the cursor to the top of the file for the second scan.*/
    rewind (fin);

    if (secondscan (fin, lablist.head, extlist.head, code, operarr)) {/*call to function to first scan*/
      fprintf(stdout, "\n Found errors in the file %s \n", strcat( fname, ".as"));
      continue;
    }   
    /*Preparation of output files according to the data structures built in the scans*/
    printfiles (lablist.head, extlist.head, code, data, ICF, DCF, fname);

  }
return 0 ;
}

