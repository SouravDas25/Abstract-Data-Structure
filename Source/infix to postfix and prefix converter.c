#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<stdlib.h>
#include<conio.h>

#include "Ads\stack.h"

bool isoperator(int c)
{
    switch(c)
    {
        case '+':
        case '-':
        case '*':
        case '\\':
        case '/':
        case '^':
        return true;
        default :
        return false;
    }
}

int priority(int c)
{
	switch(c)
    {
        case '+':
        	return 1;
        case '-':
        	return 0;
        case '*':
        	return 2;
        case '\\':
        	return 3;
        case '/':
        	return 3;
        case '^':
        	return 4;
        default:
        	return 5;
    }
}

bool ishigherprioritypresent(stack self,int c)
{
	foreach(ch,stack_iterator(self),char)
	{
		if(isoperator(ch) == true)
		{
			if(priority(ch) > priority(c))
			{
				return true;
			}
		}
		else if(ads_isstartbracket(ch) == true)
		{
			break;
		}
	}next(trav)
	return false;
}

void infix2postfix(const char *buff,char * cpf)
{
    stack s = newstack(char);
    int i,len= strlen(buff),con=0;
    for(i=0;i<len;i++)
    {
	    if(ads_isstartbracket(buff[i]) == true)
	    {
	    	stack_push(s,buff[i],char);
	    }
	    else if(isoperator(buff[i]) == true)
	    {
	    	while(ishigherprioritypresent(s,buff[i]) == true)
	    	{
	    		char b = stack_pop(s,char);
	        	cpf[con++] = b;
			}
			stack_push(s,buff[i],char);
		}
	    else if(ads_isendbracket(buff[i]) == true)
	    {
	        char opp = ads_opbrac(buff[i]),b=NULL;
	        while(b != opp)
	        {
	        	b = stack_pop(s,char);
	        	if(isoperator(b) == true)
	        	{
	        		cpf[con++] = b;
				}
			}
	    }
	    else
	    {
	    	cpf[con++]= buff[i];
		}
		printf("\n current charaacter('%c') computed string = %-10s ",buff[i],cpf);
		printstack(s,printchar);
		printf("\n");
    }
	while( !isstack_Empty(s)) 
	{
	    int b = stack_pop(s,char);
		if(isoperator(b) == true)
    	{
    		cpf[con++] = b;
		}
		printf("\n computed string = %-10s ",cpf);
		printstack(s,printchar);
		printf("\n");
	}
    cpf[con++] = '\0';
    freestack(s);
}

void infix2prefix(const char *buff,char * cpf)
{
	int len = strlen(buff);
	char bufcpy[len+1];
	strcpy(bufcpy,buff);
	strrev(bufcpy);
	int i;
	for(i=0;i<len;i++)
	{
		if(ads_isstartbracket(bufcpy[i]) || ads_isendbracket(bufcpy[i]))
		{
			bufcpy[i] = ads_opbrac(bufcpy[i]);
		}
	}
	infix2postfix(bufcpy,cpf);
	strrev(cpf);
}

int main()
{
	//(a+b/g)*d^2/c
    char f[64] = "(a+b/g)*d^2/c" ,g[64];
    char h[64];
    printf("Enter an expression : ");
    //gets(f);
    printf("%s",f);
    memset(h,0,64);
    infix2postfix(f,h);
    //infix2prefix(f,g);
    printf("\nits postfix is = %s\n",h);
    //printf("\nits prefix is = %s\n\n\n",g);
    getch();
    system("pause");
    return 0;
}
