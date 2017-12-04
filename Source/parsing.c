

#include "collection\list.h"
#include "collection\istring.h"

int main()
{
	FILE*wfp = fopen("minfied.c","w");
	FILE*rfp  = fopen("__mainstring__.c","r");
	int c,p;
	bool ins_str =false,sl_cmnt = false ;
	while( ( c = getc(rfp) ) != EOF)
	{
		if(c == '"') ins_str = !ins_str;
		if(ins_str && c != '"') goto writebuffer ;
		if(c == '/' && p == '/' && !ins_str )
		{
			sl_cmnt = true;
			
		} 
		if( sl_cmnt && c == '\n' )sl_cmnt = false;
		if(sl_cmnt) continue;
		if(c == p )
		{
			if( c == ' ' )continue ;
			if( c == '\n')continue ;
		}
		if( c == '\t') continue ;
		if( p == ';' )
		{
			if( !isalnum(c) && !ispunct(c) && c != EOF ) continue;
		}
		if(p == '{' || p=='}' )if( !isalnum(c) && !ispunct(c) && c != EOF ) continue;
		if(c == EOF ) break ;
		writebuffer : putc(c,wfp);
		p=c;
	}
	fclose(rfp);
	fclose(wfp);
	return 0;
}
