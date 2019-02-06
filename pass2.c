#include<stdio.h>
#include<string.h>
#include<stdlib.h>

void main()
{
    FILE *f1,*f2,*f3,*f4;
	int i,len,k=-1,x,lcn,st_add,l,start,op_code,objcod;;
	char str1[10],str2[15],str3[10],mstr3[10],op[10],operand[10],obj[6],loc[6],str4[10],str5[10],str6[10];
	f1=fopen("code.txt","r");
	f2=fopen("symtab.txt","r");
	f3=fopen("optab.txt","r");
	f4=fopen("objcode.txt","w");
	fscanf(f1,"%s %s %s",str1,str2,str3);
	printf("\n%s\t%s\t%s\n",str1,str2,str3);
	fprintf(f4,"\n%s\t%s\t%s\n",str1,str2,str3);
	while(!feof(f1))
    {
        fscanf(f1,"%s %s",str1,str2);
        if(strcmp(str1,".")==0)
        {
            while(strcmp(str1,".")==0)
            {
                fscanf(f1, "%*[^\n]\n", NULL);
                fscanf(f1,"%s %s",str1,str2);
            }
        }
        fscanf(f1,"%s",str3);
        fscanf(f3,"%s",op);
        fscanf(f2,"%s",operand);
        fscanf(f2,"%s",loc);
        strcpy(mstr3,str3);                         //For modification of str3 during pass2 (for index addressing)
        while(!feof(f3))
        {
            if(strcmp(str2,"BYTE")==0)
            {
                len = strlen(str3);
                if(str3[0]=='C')
                {
                    for(i = 2; i <= len-2; i++)
                    {
                        k++;
                        sprintf(obj+k*2, "%X", str3[i]);      //Conversion of char to its respective ASCII values
                    }
                    k = -1;
                    break;
                }
                else if(str3[0]=='X')
                {
                    for(i = 2; i <= len-2; i++)
                    {
                        k++;
                        sprintf(obj+k, "%c", str3[i]);
                    }
                    k = -1;
                    break;
                }
            }
            else if(strcmp(str2,"WORD")==0)
            {
                strcpy(obj,"");
                len = strlen(str3);
                for(i = 0; i <= 5-len; i++)
                    strcat(obj,"0");
                sscanf(str3, "%d", &x);                 //Conversion of string to an int value
                sprintf(str3, "%X", x);                 //Conversion of the hex value to string
                strcat(obj,str3);
                break;
            }
            else if(strcmp(str2,op)==0)
            {
                fscanf(f3,"%s",op);
                strcpy(obj, op);
                break;
            }
            else
                fscanf(f3,"%s",op);
        }
        fseek(f3,0,SEEK_SET);
        len = strlen(str3);
        if(str3[len-2] == ',' && str3[len-1] == 'X')
        {
                for(i = 0; i <= len-3; i++)
                {
                    k++;
                    sprintf(str3+k, "%c", mstr3[i]);              //Cropping of str3 string to OPERAND only
                }
                k = -1;
        }
        while(!feof(f2))
        {
            if(strcmp(str2,"BYTE")==0 || strcmp(str2,"WORD")==0)
                break;
            else if(strcmp(str2,"RSUB")==0)
            {
                strcat(obj,"0000");
                break;
            }
            else if(strcmp(str3,operand)==0)
            {
                if(mstr3[len-2] == ',' && mstr3[len-1] == 'X')
                {
                    sscanf(loc, "%X", &x);                      //Conversion of loc string to int value
                    x += 32768;                                 //32768 is the hex value of 8000
                    sprintf(loc, "%X", x);                      //Conversion of modified int value to loc string
                }
                strcat(obj,loc);
                break;
            }
            else
            {
                fscanf(f2,"%s",loc);
                fscanf(f2,"%s",operand);
            }
        }
        if(feof(f2)){
            if((strcmp(str2,"BYTE")!=0)&&(strcmp(str2,"WORD")!=0)&&(strcmp(str2,"RESB")!=0)&&(strcmp(str2,"RESW")!=0)&&(strcmp(str2,"END")!=0))
            {
                printf("\n\nERROR detected in the code. The label %s is not a valid reference label.\n\n",str3);
                fprintf(f4,"\n\nERROR OCCURED DURING PASS2");

            }
        }
        fseek(f2,0,SEEK_SET);
        if(mstr3[len-2] == ',' && mstr3[len-1] == 'X')
        {
            printf("\n%s\t%s\t%s\t%s\n",str1,str2,mstr3,obj);
            fprintf(f4,"\n%s\t%s\t%s\t%s\n",str1,str2,mstr3,obj);
        }
        else
        {
            printf("\n%s\t%s\t%s\t\t%s\n",str1,str2,mstr3,obj);
            fprintf(f4,"\n%s\t%s\t%s\t\t%s\n",str1,str2,mstr3,obj);
        }
        strcpy(obj,"-");
    }
    fclose(f1);
    fclose(f2);
    fclose(f3);
    fclose(f4);
//OBJECT PROGRAM CODE
    f1=fopen("code-loc.txt","r");
    f2=fopen("objcode.txt","r");
    f3=fopen("com-code.txt","w");
    fscanf(f1,"%s %s %x",str1,str2,&start);
    fscanf(f2,"%s %s %s %X",str4,str5,str6,&objcod);
    if(strcmp(str2,"START")==0)
    {
        fprintf(f3,"\t%s\t%s\t%X\n",str1,str2,start);
    }
    while(!feof(f1))
    {
        fscanf(f1,"%X %s %s %s",&lcn,str1,str2,str3);
        l=strlen(str3);
        if((str3[l-2]==',')&&(str3[l-1]=='X'))
        {
        fprintf(f3,"%X\t%s\t%s\t%s\t",lcn,str1,str2,str3);
        fscanf(f2,"%s %s %s %s",str4,str5,str6,op);
        fprintf(f3,"%s\n",op);
        }
        else if(strcmp("END",str2)==0)
        {
            fprintf(f3,"%X\t%s\t%s\t%s\t",lcn,str1,str2,str3);
            fscanf(f2,"%s %s %s %s",str4,str5,str6,op);
            fprintf(f3,"%s\n",op);
            break;
        }
        else
        {
            fprintf(f3,"%X\t%s\t%s\t%s\t\t",lcn,str1,str2,str3);
            fscanf(f2,"%s %s %s %s",str4,str5,str6,op);
            fprintf(f3,"%s\n",op);
        }
    }
    fclose(f1);
    fclose(f2);
    fclose(f3);

    printf("\n\n\t\tOBJECT PROGRAM\n");
    f1=fopen("com-code.txt","r");
    f2=fopen("obj-prog.txt","w");
    fscanf(f1,"%s %s %X",str1,str2,&start);
    st_add=start;
    printf("H^%s  ^00%X^00107A\n",str1,start);
    fprintf(f2,"H %s   00%X 00107A\n",str1,start);
    printf("T^00%X",start);
    fprintf(f2,"T 00%X",start);
    while(!feof(f1))
    {
        fscanf(f1,"%X %s %s %s %s",&lcn,str1,str2,str3,str4);
        objcod=lcn-start;
        if((objcod<=27)&&(strcmp(str4,"-")!=0))
        {
            k=lcn-start;
            printf("^%s",str4);
            fprintf(f2," %s",str4);
        }
        else if(objcod==30)
        {
            start=lcn;
            printf("\n");
            fprintf(f2,"\n");
            printf("T^00%X^%s",start,str4);
            fprintf(f2,"T 00%X %s",start,str4);
        }
        else
        {
            if(strcmp(str4,"-")!=0)
            {
                start=lcn;
                printf("\n");
                fprintf(f2,"\n");
                printf("T^00%X^%s",start,str4);
                fprintf(f2,"T 00%X %s",start,str4);
            }
        }
    }
    printf("\nE^00%X\n",st_add);
    fprintf(f2,"\nE 00%X\n",st_add);
    fclose(f1);
    fclose(f2);
}
