#include<stdio.h>
#include<string.h>
#include<stdlib.h>
void main()
{
	FILE *f1,*f2,*f3,*f4;
	int loc,start_ad,l,start,length;
	char str1[10],str2[15],str3[10],op[10];
	f1=fopen("code.txt","r");
	f3=fopen("symtab.txt","w");
	f4=fopen("code-loc.txt","w");
	fscanf(f1,"%s %s %x",str1,str2,&start);
	if(strcmp(str2,"START")==0)
	{
		start_ad=start;
		loc=start_ad;
	 	printf("\t%s\t%s\t%X\n",str1,str2,start);
	 	fprintf(f4,"\t%s\t%s\t%X\n",str1,str2,start);
 	}
 	else
 		loc=0;
	fscanf(f1,"%s %s",str1,str2);
	f2=fopen("optab.txt","r");
	while(!feof(f1))
	{
 		fscanf(f1,"%s",str3);
 		printf("\n%X\t%s\t%s\t%s\n",loc,str1,str2,str3);
 		fprintf(f4,"\n%X\t%s\t%s\t%s\n",loc,str1,str2,str3);
 		if(strcmp(str1,"-")!=0)
 			fprintf(f3,"\n%X\t%s\n",loc,str1);
 	fscanf(f2,"%s",op);
 	while(!feof(f2))
 	{
  		if(strcmp(str2,op)==0)
  		{
            if(strcmp(str2,"BYTE")==0)
                break;
            else{
                loc=loc+3;
                break;
            }
  		}
  		fscanf(f2,"%s",op);
  	}
  	if(strcmp(str2,"WORD")==0)
    {
        loc=loc+3;
   	}
   	else if(strcmp(str2,"RESW")==0)
   	{
        start=atoi(str3);
        loc=loc+(3*start);
    }
    	else if(strcmp(str2,"BYTE")==0)
    	{
    		if(str3[0]=='X')
      		loc=loc+1;
      		else
      		{
      			length=strlen(str3)-3;
      			loc=loc+length;
            }
    	}
    	else if(strcmp(str2,"RESB")==0)
    	{
     		start=atoi(str3);
     		loc=loc+start;
     	}
     	if(feof(f2))
        {
            if((strcmp(str2,"BYTE")!=0)&&(strcmp(str2,"WORD")!=0)&&(strcmp(str2,"RESB")!=0)&&(strcmp(str2,"RESW")!=0)&&(strcmp(str2,"END")!=0))
            {
                printf("\n\nERROR detected in the code. The mnemonic %s is not valid.\n\n",str2);
                fprintf(f3,"\n\nERROR OCCURED DURING PASS1");
               // return 0;
            }
        }
        fscanf(f1,"%s%s",str1,str2);
        fseek(f2,0,SEEK_SET);
        if(strcmp(str1,".")==0)
        {
            while(strcmp(str1,".")==0)
            {
                fscanf(f1, "%*[^\n]\n", NULL);
                fscanf(f1,"%s%s",str1,str2);
            }
        }
    }
	fclose(f2);
    if(strcmp(str2,"END")==0)
    {
        printf("\n");
        printf("\nProgram length =%X",loc-start_ad);
    }
	printf("\n\nsymtab generated in new text file\n\nPress 'Enter' to continue.");
    fclose(f1);
    fclose(f3);
    fclose(f4);
    getchar();
}
