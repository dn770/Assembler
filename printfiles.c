#include "assembler.h"
#include "scan.h"
/*This function receives the data structures after the scans and builds the output files according to them - an image of instructions and instructions, a list of entry labels and an external label list*/
int printfiles (label *head, extlab *exthead, line *code,line *data,char *fname, int *ICF, int *DCF) {
 /**/
 int i ;
 int ent = no ; /*flag for entry label*/
 int ext = no ; /*flag for external label*/
 label *ptr ;
 extlab *pext ; /*ptr to extlab*/
 FILE *fob ;/*ptr to output ".ob" file*/
 FILE *fext ;/*ptr to output ".ext" file*/
 FILE *fent ;/*ptr to output ".ent" file*/

  /*Open and write file of instructions and guidance lines,Respectively and in order.*/
  if (!(fob = fopen (strcat(fname, ".ob"),"w"))){/*open file, if not- error*/
     fprintf(stdout, "\ncan't open \"%s.ob\" output file.",fname);
     return 1;
  }
  fprintf (fob, "%d %d", (*ICF), (*DCF)); /*"Title" of file*/
  for (i=0 ;i<(*ICF) ;i++)  /*instructions lines*/
   fprintf (fob, "\n%07d %06x",code[i].adress, code[i].word) ;
  for ( ;i<(*DCF) ;i++)  /*guidance lines*/
   fprintf (fob, "\n%07d %06x",data[i].adress, data[i].word) ;
  fclose (fob);

 /*Go through the list of labels and mark the flags for the presence of external and entery labels*/
 for (ptr= head; ptr && (!ext || !ent)  ; ptr= (*ptr).next) {
   if ((*ptr).ent_ext == external)
      ext= yes ;
   else if ((*ptr).ent_ext == entry)
     ent= yes ;
 }
 if (ent) { /*Open and write the file of the list of labels*/
   if (!(fent = fopen (strcat(fname, ".ent"),"w"))){/*open file, if not- error*/
      fprintf(stdout, "\ncan't open \"%s.ent\" output file.",fname);
      return 1;
   }
  for (ptr= head; ptr ; ptr= (*ptr).next) 
    if ((*ptr).ent_ext == entry) 
     fprintf (fent, "\n%s %07d",(*ptr).name , (*ptr).value);
  fclose (fent);
 }
 if (ext) {/*Open and write the file of the list of lines of code that use external labels*/
   if (!(fext = fopen (strcat(fname, ".ext"),"w"))){/*open file, if not- error*/
     fprintf(stdout, "\ncan't open \"%s.ext\" output file.",fname);
     return 1;
   }
   for (pext= exthead; pext ; pext= (*pext).next) 
    fprintf (fext, "\n%s %07d",(*pext).name, (*pext).adress);
   fclose (fext);
  }
 return 0 ;
}
