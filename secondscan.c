#include "assembler.h"
#include "scan.h"
  /*This function scans the file again and handles all the tagged information words after they were defined during the first scan*/
int secondscan (FILE *fin, label *head, extlab *exthead,line *code,oper *operarr) {
 /*Declaration*/
 int i=0, j=0 ;
 int line; /*line of file*/
 int error= 0; /*flag*/
 char strline [STR_LEN]; /*Complete line from the file*/
 char auxstr [STR_LEN/2]; /*Auxiliary string*/
 char labstr [STR_LEN/2]; /*label to add to the list*/
 label *ptr;/*for loops*/
 extlab *newext ;/*for add to extlist*/
 int IC= 0 ; /*index of code arry*/
 int L= 0; /*number of more information words*/
 int labelf=0 ; /*label flag*/
 
 for (line=1 ; !feof(fin) ; line++,L=0,error=0,i=0,j=0,labelf=0) {
  fgets(strline, STR_LEN, fin) ;
  spaces;
  if (strline[i]==';'|| strline[i]=='\n')
   continue ; /*Ignores from a comment line and an empty line*/
  if (strline[i]!='.') { /*is letter- operand or label*/
   while (isalpha(strline[i])||isdigit(strline[i])) 
    labstr[j++]= strline [i++] ; /*read the label (or operand)*/
   labstr[j]='\0';
   if (strline[i]==':'){ /*label*/
    i++;/*skip ':'*/
    labelf=yes; /*label flag*/
    spaces ;
    while (isalpha(strline[i]))  /*read the next opernad*/
     auxstr[j++]= strline [i++] ;
    auxstr[j]='\0';
   } /*end of "if label" block*/
   else /*no label*/ 
    for (j=0 ; j<strlen(labstr) ; j++)
     auxstr[j] = labstr[j] ; /* go to operand's checks*/
  } /*end of not '.'  block*/
  if (strline[i]=='.') { /*DC or ent\ext*/
    auxstr[j++]= strline [i++] ; 
   while(isalpha(strline[i])) /*read the '.' operand*/
      auxstr[j++]= strline [i++] ;
   auxstr[j]='\0';

  if (strcmp(".entry",auxstr)==0) {
   spaces ;
   j=0;
   /*Reads the name of the label and looks for it in the list*/
   while (isalpha(strline[i])||isdigit(strline[i])) 
    auxstr[j++]= strline [i++] ;
   auxstr[j]='\0';
   for (ptr=head; ptr; ptr=(*ptr).next) /*look for the label*/
    if (strcmp((*ptr).name,auxstr)==0)
     break ;
   if (!ptr){ /*not used break*/
     fprintf(stdout,"\nline %d error : The label does not exist.",line);
     error=yes ;
     continue;
   }
    else /*found the label*/
     (*ptr).ent_ext = entry ;
   }/*end of entry block*/
   else /* DC */ 
    continue ; /*Treated in the previous scan*/
  } /*end '.' block*/

  else/*oprand*/
   if ((strcmp(operarr[0].name,auxstr)==0)||(strcmp(operarr[2].name,auxstr)==0)||(strcmp(operarr[3].name,auxstr)==0)){
    spaces ;
    if ((strline[i]== '#')|| isreg){
     if (strline[i]== '#') 
      L++; /*There is another word for this figure*/
    while (strline[i] != ',')
     i++ ;/*Treated in the previous scan*/
    }
    else{  /*label*/ 
     checklabsec ;
    }
    spaces;
    i++; /*Skip the comma*/
    spaces;
    /*2nd paremer*/
    if (!(isreg)){ 
     checklabsec ;
    }
   } /*end first group- continue at endline Going forward*/
  else if (strcmp(operarr[1].name ,auxstr)==0) {
   if ((strline[i]== '#')|| isreg){
    if (strline[i]== '#') 
      L++;
    while (strline[i] != ',')
     i++ ;
   }
   else {
    checklabsec ;
  }
   spaces;
   i++;
   spaces;
   /*2nd parameter*/
   if ((strline[i]== '#')|| (isreg)){
    if (strline[i]== '#') 
     L++;
   }
   else {
    checklabsec;
   }
  } /*end operand*/
  if (strcmp(operarr[4].name ,auxstr)==0) {
   spaces ;
   checklabsec ;
   spaces ;
   i++ ; /*skip comma*/
   spaces ;
   if (!isreg){
    checklabsec ;
   }
  }
  if ((strcmp(operarr[5].name,auxstr)==0)||(strcmp(operarr[6].name,auxstr)==0)||(strcmp(operarr[7].name,auxstr)==0)||(strcmp(operarr[8].name,auxstr)==0)||(strcmp (operarr[12].name,auxstr)==0)) {
   spaces ;
    if (!isreg) { /*label addmeth*/
     checklabsec;
   }
  }

  if ((strcmp(operarr[9].name,auxstr)==0) || (strcmp(operarr[10].name,auxstr)==0) || (strcmp(operarr [11].name,auxstr)==0)) {
   spaces  ;
   if (strline[i]=='&') { /*Relative addressing method*/
     i++ ; /*skip '&'*/
     while (((isalpha(strline[i]))) || (isdigit(strline[i])))  
      auxstr[j++]= strline [i++] ; /*read label parameter*/
     auxstr[j]='\0'; 
     /*look for the label and Keep the difference value at a distance between the lines*/ 
     for (ptr=head; ptr; ptr=(*ptr).next) 
      if(strcmp((*ptr).name,auxstr)==0){ 
         code[IC+(++L)].word |= ((IC+INI_IC_VALUE-(*ptr).value) << datacode) ; 
         break ; 
       } 
      if (!ptr) { /*not found label*/
        fprintf(stdout,"\nline %d : error : Undefined label.",line); 
        error=yes ; 
        continue ; 
     } 
    /*else*/ 
    if ((*ptr).ent_ext == external) {
        fprintf(stdout,"\nline %d : error : This addressing method does not support external labels.",line); 
        error=yes ; 
        continue ;
    } 
    else /*valid unextrenal label*/ 
     code[IC+L].word |= (yes << A) ; 
   }
   else {/*label address method*/
    checklabsec ;
   }
  }
  else if (strcmp(operarr[13].name,auxstr)==0) {
   spaces ;
   if ((strline[i]== '#')|| (isreg)) {
    if (strline[i]== '#') 
     L++;
   }
   else { /*label*/
    checklabsec ;
   } 
  }
  else if ((strcmp(operarr[14].name,auxstr)==0) || (strcmp(operarr[15].name,auxstr)==0))
   ; /*have no parameters*/

 /*line ending & update external list*/
 if (labelf) /*Check whether the line label itself is external and add it to the list*/
  for (ptr=head; ptr; ptr=(*ptr).next)
   if((strcmp((*ptr).name,labstr)==0) && ((*ptr).ent_ext = external)) { 
   (*newext).name =(*ptr).name ;
   (*newext).adress = IC+INI_IC_VALUE;
   (*newext).next = exthead ;
   exthead = newext ;
   }
  IC+=L;
  IC++;
 } /*end of file scan loop*/
 if (error)
  return 1 ;
return 0;
} /*end function*/
