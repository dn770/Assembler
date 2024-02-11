enum {null, entry, external, code, data} types;
enum {opcode= 18, suraddmeth= 16, surreg=13, destaddmeth=11, destreg=8, funct=3, datacode=3, A=2, R=1, E=0} binary ;
enum {imm, dire ,rel, reg} addmaths ;

/*Checking the correctness of the parameter according to the immediate addressing method*/
#define immcheck  {  i++;  /*skip '#'*/ \
    if (strline[i]== '-' || strline[i]== '+')  /*sign number*/ \
      auxstr[j++]= strline[i++] ; \
    while (isdigit (strline[i]) ) /*Reading the number*/\
      auxstr[j++]= strline[i++] ; \
    auxstr[j]= '\0'; \
    spaces ; \
    if (strline[i]!='\n'&& strline[i]!= ',') {/*unvalid number*/ \
      if (strline[i]== '.') \
       fprintf(stdout,"\nline %d error : Support for integers only.",line); \
      else \
       fprintf(stdout,"\nline %d error : Unvalid number.",line); \
       error= yes ; \
      code[IC].word &= 0 ; \
       if (L) \
       code[IC+L].word &= 0 ; \
      continue ; \
    } }
/*Checking the correctness of the label without referring to its contents and context*/
#define checkunlab { if (!isalpha(strline[i])){ \
       if (strline[i]=='\n') \
        fprintf(stdout,"\nline %d error : Missing parameters.",line); \
       else \
        fprintf(stdout,"\nline %d  error : Unvalid label- must start with a letter.",line); \
       error=yes ; \
       code[IC].word &= 0 ; \
       if (L) \
       code[IC+L].word &= 0 ; \
      continue ; \
       continue ; \
     } /*end of error block*/\
     while ((isalpha(strline[i]))||(isdigit(strline[i]))) \
       i++ ; /*checksthe contents at the  second scan*/ \
     if (!(isspace(strline[i])|| strline[i]=='\n' || strline[i] == ',')) { \
      fprintf(stdout,"\nline %d error : Unvalid label- only letters and numbers.",line); \
      error=yes ; \
      code[IC].word &= 0 ; \
       if (L) \
       code[IC+L].word &= 0 ; \
      continue ; \
     } }  
/*Check whether this is a register addressing method (in the number between 0-7)*/
#define isreg  ((strline[i]== 'r') && (strline[i+1]>='0' && strline[i+1]<='7') \
           && (isspace (strline[(i+1)+1]) || strline [(i+1)+1] == ',')) /*Make sure it is not a prefix of a label*/ 

/*Make sure that between every two parameters there is at least one comma and exactly one comma*/
#define checkcomma {  if (strline[i]==',') { \
     i++; \
    comma=yes ; \
    } \
  spaces ; \
  if (strline[i]=='\n'){ \
    fprintf(stdout,"\nline %d error : missing parameter.",line); \
    error=yes ; \
    code[IC].word &= 0 ; \
    continue ; \
  } /*else- not end line*/\
  if (strline[i]==','){ \
     if (comma==yes){  /*double comma*/ \
        fprintf(stdout,"\nline %d - Multiple consecutive commas.",line); \
        error=yes ;\
        code[IC].word &= 0 ;\
        if (L) \
          code[IC+L].word &= 0 ; \
        continue ;\
        } \
     comma=yes ; /*if this the first comma*/ \
    }\
    while (isspace(strline[i])) i++ ; \
      if (!comma) { \
        fprintf(stdout,"line %d - Between every two parameters there is at least one comma",line); \
        error=yes ; \
        code[IC].word &= 0 ; \
        if (L) \
          code[IC+L].word &= 0 ; \
        continue ; \
        } \
        if (strline[i]=='\n') {  /*endline*/\
        fprintf(stdout,"line %d - Missing parameter.",line); \
        error=yes ; \
        code[IC].word &= 0 ; \
        if (L) \
          code[IC+L].word &= 0 ; \
        continue ; \
        } }  

/*Check in the second scan that the label is set. If so, if it is an external label we will add the address to the external list*/
#define checklabsec { while ((isalpha(strline[i])) || (isdigit(strline[i]))) \
     auxstr[j++]= strline [i++] ;  /*reads the label*/ \
    auxstr[j]='\0'; \
    /*Encoding the label address for the additional information word*/ \
    for (ptr=head; ptr; ptr=(*ptr).next) \
      if(strcmp((*ptr).name,auxstr)==0){ \
         code[IC+(++L)].word |= ((*ptr).value << datacode) ; \
         break ; \
       } \
      if (!ptr) {  /*unset label- error*/ \
        fprintf(stdout,"\nline %d error : Undefined label.",line); \
        error=yes ; \
        continue ; \
     } \
    /*if it is an external label we will add the address to the external list.*/ \
    if ((*ptr).ent_ext = external) { \
     code[IC+L].word |= (yes << E) ; \
   (*newext).name = (*ptr).name ; \
   (*newext).adress = IC+INI_IC_VALUE; \
   (*newext).next = exthead ; \
   exthead = newext ; \
   } \
   else /*not extern*/ \
    code[IC+L].word |= (yes << A) ; } 

