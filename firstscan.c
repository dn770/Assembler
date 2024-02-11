#include "assembler.h"
#include "scan.h"
/*This function receives the initialized data structures, scans the file by lines, reads and analyzes the data and instructions, and handles all data that is independent of labels that may have been defined later in the file.*/
int firstscan (FILE *fin, label *head, line *code, line *data, oper *operarr, int *ICF, int *DCF) {
 /*Declaration*/
 int i=0, j=0 ;
 int line; /*line of file*/
 int error =0 , comma=0 ; /*flags*/
 char strline [STR_LEN]; /*Complete line from the file*/
 char auxstr [STR_LEN/2]; /*Auxiliary string*/
 char labstr [STR_LEN/2]; /*label to add to the list*/
 label *ptr ; /*for loops*/
 label *new ; /*for add to list*/
 int DC=0; /*index of data arry*/
 int IC= 0 ; /*index of code arry*/
 int L= 0; /*number of more information words*/
 int labelf=0 ; /*label flag*/

 for (line=1;!feof(fin);line++, L=0,error=0,i=0,j=0,comma=0,labelf=0) {
  fgets(strline, STR_LEN, fin) ;
  if (strline[STR_LEN-2]!='\n' && strline[STR_LEN-2]!='\0') { /*line too long*/
    fprintf(stdout,"\nline %d error: line too long.",line);
    error = yes ;
    continue ;
  }
  /*Beginning of line analysis*/
  spaces ;
  if ((strline[i]==';')|| (strline[i]=='\n')) 
    continue ; /*Ignores from a comment line and an empty line*/

  if (!isalpha(strline[i])&& strline[i]!='.') {
    fprintf(stdout,"\nline %d error : unvalid operand / label.",line);
    error= yes ;
    continue;
   }
  /**/
  if (strline[i]!='.'){ /*is letter- operand or label*/ 
     while (isalpha(strline[i])||isdigit(strline[i])) 
       labstr[j++]= strline [i++] ; /*read the label (or operand)*/
     labstr[j]='\0';
   }

  if (strline[i]==':'){ /*have label*/
     i++ ; /*skip ':'*/
     for( j=0 ;j<OPER_NUM;j++) /*Checks that the label is not a reserved word*/
        if (strcmp(operarr[j].name,labstr)==0){
          fprintf(stdout,"line %d error: The label is a reserved word",line);
          error=yes ;
          break ;
        }
     if (error) /*error in the loop*/
       continue ; /*to the next line*/
     /*Checks that the label is not double*/
     for( ptr=head; ptr!=NULL && (!error) ;ptr= (*ptr).next)
        if (strcmp((*ptr).name,labstr)==0){
          fprintf(stdout,"line %d error: double label",line);
          error= yes ;
        }
     if (error) 
        continue ;
     labelf = yes ; /*label flag*/
     spaces;
     if (strline[i]== '\n') {
        fprintf(stdout,"line %d : null label",line);
        error=yes ;
        continue;
     }
  else /*no label*/
  for (j=0 ; j<strlen(labstr) ; j++)
   auxstr[j] = labstr[j] ; /* go to operand's checks*/
  } /*end of label block*/

  if (strline[i]=='.') { /*Guidance line*/
     for (j=0;isalpha(strline[i]);)
        auxstr[j++]= strline [i++] ;/*read the '.' operand*/
     auxstr[j]='\0';
     if (strcmp(".data",auxstr)==0){
       j=0;
       spaces ;
       if (strline[i]=='\n'){ 
        fprintf(stdout,"\nline %d error : not found numbers in data line.",line);
        error=yes ;
        continue;
        }
       while ((strline[i]!= '\n')&&(!error)){
         if ((strline[i]=='+')||(strline[i]=='-')) /*sign number*/
           auxstr[j++]= strline [i++] ;
         if (!isdigit(strline[i])) { 
           fprintf(stdout,"\nline %d error : Invalid number.",line);
           error=yes ;
           continue ;
          }
        for (j=0; isdigit(strline[i]); j++,i++)
          auxstr[j]= strline[i] ; /*read number*/
        auxstr[j]='\0' ;
        if (!isspace(strline[i])&&(strline[i]!=',')&&(strline[i]!='\n')){
         if (strline[i]=='.')
            fprintf(stdout,"\nline %d error : Support for integers only.",line);
         else 
            fprintf(stdout,"\nline %d : Invalid number.",line);
          error=yes ;
          continue ;
         } /*end of error block*/
        data[DC+L].word |=  atoi(auxstr) ; 
        data[DC+L].adress = DC+L ;
        L++;

        spaces ;
        if (strline[i]=='\n')
          break ; /*End of data numbers*/
        /*else- comma check*/
          if (strline[i]==',')
             comma=yes ;
         spaces ;
      if (strline[i]=='\n'&&comma){
         fprintf(stdout,"\nline %d  error: Unnecessary comma.",line);
         error=yes;
       }
      if ((strline[i]!='\n')&&!comma){
        fprintf(stdout,"\nline %d  error: missing comma.",line);
        error=yes ; 
      }
     }
     if (error) {/*Reset all code words of this prompt*/
     for (j=0;j<=L;j++) {
       data[DC+j].word &= 0 ;
       data[DC+j].adress = 0 ;
     }
    continue ;
    }/*end of error*/
   else { /*No errors*/
     /*Add the label to the list and update the data counter*/
     if (labelf) {
   (*new).name =labstr ;
   (*new).value = DC ;
   (*new).type = (types+4) ; /*types=data*/
   (*new).ent_ext=null ;
   (*new).next = head ;
   head = new ;
     }
    DC+=L;
    DC++;
    L=0 ;
    continue ;
  } /**end of no errors*/
 } /*End of ".data"*/
  else if (strcmp(".string",auxstr)==0) {
    spaces ;
    if (strline[i]!='\"') {
      fprintf(stdout,"\nline %d error : string must beging with \" . ",line);
    for (j=0;strline[i]!='\n'|| !isspace(strline[i]);j++)
      auxstr[j++]= strline [i++] ;/*read string*/
    auxstr[j]='\0';
    if (auxstr[j-1]!='\"'){
      j-- ; /*skip '\n'*/
      while (isspace(auxstr[--j])) ; /*skip whitespaces back*/
      if (auxstr[j]!='\"'){
       fprintf(stdout,"\nline %d error: string must ending with \" .",line);
       error=yes ;
       continue ;
      }
    } /*valid string*/
    spaces ;
    if (strline[i]!='\n') {
      if (strline[i]==',')
       fprintf(stdout,"\nline %d : error- extra comma.",line);
      else
       fprintf(stdout,"\nline %d : error- too many parameters.",line);
      error=yes ;
      continue ;
    }
    /*legal string*/
    if (labelf) {/*Add the label to the list*/
     (*new).name =labstr ;
     (*new).value = DC ;
     (*new).type = (types+4) ; /*data*/
     (*new).ent_ext=null ;
     (*new).next = head ;
      head = new ;
    }
    /*Inserting the string characters into the data arry*/
    for (j=1; j<strlen(auxstr)-1 ; j++, DC++){
      data[DC].word |= auxstr[j] ; 
      data[DC].adress = DC ;
    } 
    /*end string*/ 
    data[DC].word |= '\0'; 
    data[DC].adress = DC ;
    DC++ ;
    continue;
   } /*end of string block*/

  if (strcmp(".entry",auxstr)==0) {
    if (labelf)
      fprintf(stdout,"line %d error : label not needed.\n",line);
    continue ; /*Will be treated in the second scan*/
   }
  if (strcmp(".extern",auxstr)==0) {
   if (labelf)
    fprintf(stdout,"line %d error : label no needed.\n",line);
   spaces;
   if (!isalpha(strline[i])) {
    fprintf(stdout,"line %d error : unvalid label\n",line);
    error=yes;
    continue;
   }
   j=0;
   while (isalpha(strline[i])||isdigit(strline[i])) 
    auxstr[j++]= strline [i++] ; /*read the label parameter*/
   auxstr[j]='\0';
   spaces ;
   if (strline[i]!='\n') {
    fprintf(stdout,"\nline %d error : Too many parameters.",line);
    error=yes ;
    continue ;
   }
   for (ptr=head; (*ptr).next;ptr=(*ptr).next)
     /*Check that this is not a duplicate label defined in this file (but if external is not an error)*/
     if ((strcmp((*ptr).name,auxstr)==0)&& ((*ptr).ent_ext != external)){ 
       fprintf(stdout,"\nline %d error : double label",line);
       break;
     }
   if ((*ptr).next){ /* do break*/
     error=yes ;
     continue ;
   }
   /*Set a new label at the end of the label list*/
   (*new).name =labstr ;
   (*new).value = IC+INI_IC_VALUE ;
   (*new).type = null ; /*types=code*/
   (*new).ent_ext= external ;
   (*new).next = head ;
   head = new ;
   continue ;
   } /*End of "extern" block*/ 
   else /*begin at '.' but invalid directive*/
   fprintf(stdout,"\nline %d error : invalid directive operand.",line);
   error=yes ;
   continue ;
   }
  }/*End of directive line*/

  /*else- operand*/
  /*Operand coding*/
  code[IC].word |= (yes << A) ;
  for (j=0; j<OPER_NUM ; j++)
   if (strcmp(operarr[j].name,auxstr)==0) {/*Encoding the operand number*/
    code[IC].word |= (operarr[j].opcode << opcode) ;
    code[IC].word |= (operarr[j].funct << funct) ;
    break ; 
   }
   if (j==OPER_NUM){ /*No compatible operand found*/
    fprintf(stdout,"\nline %d error: illegal operand.",line);
    error=yes ;
    continue ;
   }
  else if ((strcmp(operarr[0].name,auxstr)==0)||(strcmp(operarr[2].name,auxstr)==0)||(strcmp(operarr[3].name,auxstr)==0)){
   /*Deciphering the instruction line*/
   spaces ;
   if (strline[i]=='#') {
    immcheck ;  /*define - scan.h*/
    L++ ; /*extra word for number*/
    /*suraddmeth imm = 0*/
    code[IC+L].word |=((atoi(auxstr))<<datacode) ;
    code[IC+L].word |= (yes << A) ;
    }
   else if (strline[i]=='&') { 
    fprintf(stdout,"\nline %d error: Unsuitable addressing method.",line);
    error= yes ;
    code[IC].word &= 0 ;
    continue ;
    }
   /*suraddmeth reg*/
   else if(isreg) {
    code[IC].word |= (reg << suraddmeth) ;
    (code[IC].word) |= ((atoi (&strline[i+1])) << surreg) ; /*between 0-7*/
    i=(i+1)+1; /*Jump to the space or comma after the parameter*/
   }
   else {/* addmeth label*/ 
    checkunlab;
    code[IC].word |= (dire << suraddmeth) ;
    L++; /*Extra word for label*/
   }
   spaces ;
   checkcomma ;
   spaces;  
  /*second parameter*/
  if (strline[i]=='#'||strline[i]=='&'){
    fprintf(stdout,"line %d : error",line);
    error= yes ;
    code[IC].word &= 0 ;
    if(L)
     code[IC+L].word &= 0 ;
    continue ;
  }
  /*reg*/
  else if(isreg) {
   code[IC].word |= (reg << destaddmeth) ;
   code[IC].word |= ((atoi (&strline[i+1]) << destreg)) ;
   i= (i+1)+1 ;
  }
  else {/*label*/ 
   checkunlab ; 
   code[IC].word |= (dire << destaddmeth) ;
   L++;
  }
 }
  else if (strcmp(operarr[1].name ,auxstr)==0) {
   spaces ;
   if (strline[i]=='#'){
    immcheck;
    L++ ;
    code[IC+L].word |= ((atoi (auxstr)) << datacode) ;
    code[IC+L].word |= (yes << A) ;
   }
   else if (strline[i]=='&') { 
    fprintf(stdout,"\nline %d error: Unsuitable addressing method.",line);
    error= yes ;
    code[IC].word &= 0 ;
    continue ;
   }
   /*3 - reg*/
   else if(isreg) {
    code[IC].word |= (reg << suraddmeth) ;
    code[IC].word |=(atoi (&strline[i+1]) << surreg) ; /*between 0-7*/
    i=(i+1)+1; /*Jump to the space or comma after the parameter*/
   }
   else {/*label*/ 
    checkunlab ;
    L++;
    code[IC].word |= (dire << suraddmeth) ;
   }
   spaces ;
   checkcomma ;
   spaces ;
  /*second parameter*/
  if (strline[i]=='#'){
   immcheck ;
   L++ ;
   code[IC+L].word |=(atoi (auxstr) << datacode) ;
   code[IC+L].word |= (yes << A) ;
  }
  /*3 - reg*/
  else if(isreg) {
   code[IC].word |= (reg << destaddmeth) ;
   code[IC].word |= (atoi (&strline[i+1]) << destreg) ;
   i=(i+1)+1;
  }
  else {/*label*/ 
   checkunlab ;
   L++;
   code[IC+L].word |= (dire << destaddmeth) ;
  }  
 }
 else if (strcmp(operarr[4].name,auxstr)==0) {
  spaces ;
  if (strline[i]=='#'||strline[i]=='&'||isreg){
   fprintf(stdout,"\nline %d error: Unsuitable addressing method.",line);
   error= yes ;
   code[IC].word &= 0 ;
   continue ;
  }
  else {/*label*/ 
   checkunlab ; 
   L++;
   code[IC].word |= (dire << suraddmeth) ;
  }
  spaces ;
  checkcomma ;
  spaces ;
  /*second parameter*/
  if (strline[i]=='#'||strline[i]=='&'){
   fprintf(stdout,"\nline %d error: Unsuitable addressing method.",line);
   error= yes ;
   code[IC].word &= 0 ;
   if(L)
    code[IC+L].word &= 0 ;
   continue ;
  }
  /*3 - reg*/
  else if(isreg) {
   code[IC].word |= (reg << destaddmeth) ;
   code[IC].word |=(atoi (&strline[i]) << destreg) ;
   i=(i+1)+1 ;
  }
  else {/*label*/ 
   checkunlab ;
   L++; 
   code[IC].word |= (dire << destaddmeth) ;
  }
 }
  else if ((strcmp(operarr[5].name,auxstr)==0)||(strcmp(operarr[6].name,auxstr)==0)||(strcmp(operarr[7].name,auxstr)==0)||(strcmp(operarr[8].name,auxstr)==0)||(strcmp (operarr[12].name,auxstr)==0)) {
   spaces ;
   if (strline[i]=='#'||strline[i]=='&'){
    fprintf(stdout,"\nline %d error : Unsuitable addressing method.",line);
    error= yes ;
    code[IC].word &= 0 ;
    continue ;
   }
   /*3 - reg*/
   else if (isreg) {
    code[IC].word |= (reg << suraddmeth) ;
    code[IC].word |=((atoi (&strline[i+1])) << destreg ) ;
    i=(i+1)+1 ; /*Jump to the space or comma after the parameter*/
   }
   else {/* label*/ 
    checkunlab ;
    code[IC].word |= (dire << suraddmeth) ;
    L++;
   }
  }
  else if ((strcmp(operarr[9].name,auxstr)==0) || (strcmp(operarr[10].name,auxstr)==0) || (strcmp(operarr [11].name,auxstr)==0)) {
   spaces ;
   if (strline[i]== '&') {/*addmeth rel*/
     i++ ;
     if (!isalpha(strline[i])) { /*unvalid label parameter*/
       if (strline[i]=='\n') 
        fprintf(stdout,"\nline %d error : miising label parametr.",line);
       else
        fprintf(stdout,"\nline %d error: unvalid parameter.",line);
       error=yes ;
       code[IC].word &= 0 ;
       continue ;
     }
     /*else- is alpha*/
     while (isalpha(strline[i])||isdigit(strline[i])) 
      i++; /*The label will be treated in the second*/
     if ((!isspace(strline[i])) && strline[i]!='\n' && strline[i]!=',') {
       fprintf(stdout,"\nline %d error: Unvalid label.",line);
       error=yes ;
       code[IC].word &= 0 ;
       continue ;
      } 
      /*else- valid label*/
      L++; /*extra info word*/
      code[IC].word |= (rel << suraddmeth) ;
    }
    else{ /*addmeth label*/
     checkunlab;
     code[IC].word |= (dire << suraddmeth) ;
     L++;
    }
   }
   else if (strcmp(operarr[13].name,auxstr)==0) {
    spaces ;
    if (strline[i]=='#'){
     immcheck ;
     L++ ;
     code[IC+L].word |=(atoi(auxstr) << datacode) ;
     code[IC+L].word |= (yes << A) ;
    }
   /*3 - reg*/
   else if (isreg){
    code[IC].word |= (reg << suraddmeth) ;
    code[IC].word |= ((atoi (&strline[i])) << surreg) ;
    i=(i+1)+1 ;
   }
   else {/*label*/ 
    checkunlab ; 
    L++;
    code[IC].word |= (dire << suraddmeth) ;
   }
  }
  else if ((strcmp(operarr[14].name,auxstr)==0) || (strcmp(operarr[15].name,auxstr)==0))
   ; /*paramless opernands*/

 /*line ending & adresses coding*/
 spaces;
  if (strline[i]!='\n'){ /*Checks line ending*/
    if (isdigit(strline[i])||isalpha(strline[i]))
     fprintf(stdout,"\nline %d error: Too many parameters",line);
    else if (strline[i] ==',')
     fprintf(stdout,"\nline %d error: Too many commas.",line);
    else 
    fprintf(stdout,"\nline %d error: Illegal line ending.",line);
    error=yes ;
    for (j=0 ; j<=L; j++)
    code[IC+j].word &= 0 ;/*Reset all the information words related to the wrong line*/
    continue ;
  }
  /*Address encoding*/
  for (j=0 ; j<=L; j++)
   code[IC+j].adress = IC + j + INI_IC_VALUE;
  /*Create the label */
  if (labelf) {
   (*new).name =labstr ;
   (*new).value = IC+INI_IC_VALUE ;
   (*new).type = (types+3) ; /*types=code*/
   (*new).ent_ext=null ;
   (*new).next = head ;
   head = new ;
  }
  IC+=L;
  IC++;
  
 } /*end of file loop*/

 if (error)
  return  1 ;  
/*Saving the values of the number of lines of instructions and the final lines of instructions*/
 (*ICF) = IC;
 (*DCF)= DC;
/*Promote the addresses in the arrays to address 100 and beyond and arrange the data words after the instruction words*/
 for (i=0 ; i<IC; i++) 
  code[i].adress += INI_IC_VALUE ;
 for (i=0 ; i<DC; i++) 
  data[i].adress = (*ICF) + INI_IC_VALUE ;

 return 0;
} /*end of function*/

