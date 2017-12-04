
#ifndef SDB_PARSER_C
#define SDB_PARSER_C

#include "../../include/database/parser.h"


int comma_count(const char * cmd,int size)
{
	int i,cnt = 0;bool qou1 = false,qou2 = false;
	for(i=0;i<=size;i++)
	{
		if( cmd[i]  == '\'') qou1 = !qou1;
		if( cmd[i]  == '"') qou2 = !qou2;
		if( qou1 == false && qou2 == false)
		{
			if(cmd[i] == ',' ) cnt++;
		}
	}
	return cnt;
}

db_tupleTYPE sdb_ParseType(const char * str,int len)
{
    int i;
	for(i = 0;i<len;i++)
	{
		if( isalnum(*str) != false )  break;
		str++;
	}
	if( strnicmp(str,"int",3) == 0)
	{
		return db_INT;
	}
	if( strnicmp(str,"double",6) == 0)
	{
		return db_DOUBLE;
	}
	if( strnicmp(str,"string",6) == 0)
	{
		return db_STRING;
	}
	if( strnicmp(str,"str",3) == 0)
	{
		return db_STRING;
	}
	if( strnicmp(str,"float",5) == 0)
	{
		return db_DOUBLE;
	}
	return db_NULL;
}

db_tupleTYPE* generate_TypeFormat(const char * str, int len,int * cnt1)
{
	int size = comma_count(str,len) + 1 ,cnt = 0;
	bool qou1 = false,qou2 = false;
	db_tupleTYPE * t = (db_tupleTYPE*)ads_calloc(size,sizeof(db_tupleTYPE));
	int i,p = 0;
	for(i=0;i<len+1;i++)
	{
		if( str[i]  == '\'') qou1 = !qou1;
		if( str[i]  == '"') qou2 = !qou2;
		if( qou1 == false && qou2 == false)
		{
			if(str[i] == ',' || str[i] == 0 )
			{
				t[cnt++] = sdb_ParseType(&str[p],i-p);
				p = i + 1;
			}
		}
	}
	*cnt1 = cnt;
	return t;
}

static ads_vector Split(const char* str,const char* sep)
{
	int i = 0,clevel=0,prev = -1,len = strlen(str);
	bool qou1 = false,qou2 = false;
	ads_vector  l1 = ads_vectorNew(ads_str,4);
	ads_vectorSetTD(l1,ads_strTD);
	ads_str s  ;
	char tmp[2] = {0,0};
	for(i=0;i<len+1;i++)
	{
		tmp[0] = str[i];
		if(ads_isStartBracket(str[i]) == true ) clevel++;
		else if(ads_isEndBracket(str[i]) == true ) clevel--;
		if( str[i]  == '\'') qou1 = !qou1;
		if( str[i]  == '"') qou2 = !qou2;
		if( clevel == 0 && qou1 == false && qou2 == false)
        {
            if(ads_cstrContains(sep,tmp) == true ||  str[i] == ads_endOfStr)
            {
                if(i-prev > 1)
                {
                    if(prev != 0) prev++;
                    s = ads_cstrGetSlice(str,prev,i-1);//ads_printf("%'str'",s)
                    ads_vectorAppend(l1,s,ads_str);
                    ads_strDelete(s);
                }
                prev = i;
            }
        }
	}
	ads_vectorShrink(l1);
	return l1;
}

void remove_space(char * str, int * len)
{
	int i,cnt =0;bool qou1 = false,qou2 = false;
	for(i=0;i<*len;i++)
	{
		if(isprint(str[i]) == false )continue;
		if( str[i]  == '\'') qou1 = !qou1;
		if( str[i]  == '"') qou2 = !qou2;
		if( str[i] != ' ' ) str[cnt++] = str[i];
		else if( str[i] == ' ' && (qou1 == true || qou2 == true) ) str[cnt++] = str[i];
		else if( str[i] == ' ' &&  str[i+1] != ' ' && str[i-1] != ' '  ) str[cnt++] = str[i];
	}
	str[cnt] = '\0';
	*len = cnt;
}

char * remove_space_completely(char * str)
{
	int i,len = strlen(str),cnt =0;bool qou1 = false,qou2 = false;
	for(i=0;i<len;i++)
	{
		if(isprint(str[i]) == false )continue;
		if( str[i]  == '\'') qou1 = !qou1;
		if( str[i]  == '"') qou2 = !qou2;
		if( str[i] != ' ' ) str[cnt++] = str[i];
		else if( str[i] == ' ' && (qou1 == true || qou2 == true) ) str[cnt++] = str[i];
		//else if( str[i] == ' ' &&  str[i+1] != ' ' && str[i-1] != ' '  ) str[cnt++] = str[i];
	}
	str[cnt] = '\0';
	len = cnt;
	return str;
}

db_tupleTYPE recognize_type(const char * str,int len)
{
	int i;
	for(i=0;i<len;i++)
	{
		if(str[i] == '\'' || str[i] == '"') return db_STRING;
		if(str[i] == '.')return db_DOUBLE;
		if(str[i] == 'N' && str[i+1] == 'U' && str[i+2] == 'L') return db_NULL;
	}
	return db_INT;
}

/*
static char * GivenKey(char * str )
{
	return remove_space_completely(str);
}*/

nosqlcmd GetCommand(const char * str)
{
    if(!str)return nosql_NOCMD;
	while(*str != '\0')
	{
		if( isalnum(*str) != false )  break;
		str++;
	}
	if( stricmp(str,"create") == 0)
	{
		return nosql_CREATE;
	}
	if( stricmp(str,"delete") == 0)
	{
		return nosql_DELETE;
	}
	if( stricmp(str,"get") == 0)
	{
		return nosql_GET;
	}
	if( stricmp(str,"update") == 0)
	{
		return nosql_UPDATE;
	}
	if( stricmp(str,"insert") == 0)
	{
		return nosql_INSERT;
	}
	if( stricmp(str,"commit") == 0)
	{
		return nosql_COMMIT;
	}
	if( stricmp(str,"quit") == 0)
	{
		return nosql_QUIT;
	}
	if( stricmp(str,"help") == 0)
	{
		return nosql_HELP;
	}
	return nosql_NOCMD;
}

nosqOT GetOperationType(const char * str)
{
    if(!str)return nosql_NOOT;
	while(*str != '\0')
	{
		if( isalnum(*str) != false )  break;
		str++;
	}
	if( stricmp(str,"key") == 0)
	{
		return nosql_KEY;
	}
	if( stricmp(str,"array") == 0)
	{
		return nosql_ARRAY;
	}
	if( stricmp(str,"keylist") == 0)
	{
		return nosql_KEYLIST;
	}
	if( stricmp(str,"arraylist") == 0)
	{
		return nosql_ARRAYLIST;
	}
	return nosql_NOOT;
}

//#define debug

nosqlRT sdb_execute(staticDB * db,proc p);

nosqlRT sdb_parse(staticDB * db,const char * str)
{
    sdb_resetNoSqlRT();
    ads_vector v = Split(str,":");
    ads_str tms ;

    #ifdef debug
    ads_printf("v = %['str'] \n",v);
    #endif // debug

    if(ads_vectorLen(v)<1)
    {
        sdb_setNoSqlMsg("NoSQL Syntax Error");
        ads_deleteOne(v);
        return nosql_ERROR_MSG;
    }

    ads_vectorGetItem_NoCopy(v,0,ads_str,tms);
    const char *c = ads_strGetCstr(tms);

    const char * res;

    if(ads_vectorLen(v)==1) res = NULL;
    else
    {
        ads_vectorGetItem_NoCopy(v,1,ads_str,tms);
        res = ads_strToCstr(tms,NULL);
    }

    ads_vector v1 = ads_cstrSplit(c," ");

    #ifdef debug
    ads_printf("v1 = %['str'] %s\n",v1,res);
    #endif // debug

    if(ads_vectorLen(v1)<1)
    {
        sdb_setNoSqlMsg("NoSQL Syntax Error USE(COMMAND - TYPE - KEYNAME : ARGs)");
        ads_deleteMany(3,v,v1,res);
        return nosql_ERROR_MSG;
    }

	ads_vectorGetItem_NoCopy(v1,0,ads_str,tms);
	char cmd[ads_strLen(tms)+1];
	ads_strToCstr(tms,cmd);

	char * OT;

	if(ads_vectorLen(v1) < 2) OT = NULL;
    else
    {
        ads_vectorGetItem_NoCopy(v1,1,ads_str,tms);
        OT = ads_strToCstr(tms,NULL);
    }

	char * key;

    if(ads_vectorLen(v1) < 3) key = NULL;
    else
    {
        ads_vectorGetItem_NoCopy(v1,2,ads_str,tms);
        key = ads_strToCstr(tms,NULL);
    }

    ads_deleteMany(2,v,v1);

	nosqlcmd cm = GetCommand(cmd);
	nosqOT dt = GetOperationType(OT);

	#ifdef debug
	ads_printf("key = %'s' \n",key);
	#endif // debug

	proc p = {cm,dt,key,res};
	nosqlRT r = sdb_execute(db,p);
    ads_deleteMany(3,res,key,OT);
    return r;
}

/*
int main()
{
	parser("create key name : { 1 , 2 , 3 , 6 } ");
	parser("delete key name");
	parser("get key name");
	return true;
}*/


#endif // SDB_PARSER_C
