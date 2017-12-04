
#ifndef staticDATABASE_DB_TUPLE_C
#define staticDATABASE_DB_TUPLE_C

#include "../../include/database/db_tuple.h"

typedef struct {
	union {
		bool b;
		int64_t i;
		double d;
		char * s;
		db_tuple t;
	};
	db_tupleTYPE type;
} db_tuple_var ;

struct struct_db_tuple{
	db_tuple_var * data;
	char ** keylist;
	int size;
	int memory;
};

typedef struct_db_tuple* db_tuple;

db_tuple db_tupleNew(int size)
{
    if(!size) return NULL;
	db_tuple t = (db_tuple)ads_calloc(1,sizeof(struct_db_tuple));
	if(!t)return t;
	t->data = (db_tuple_var*)ads_calloc(size,sizeof(db_tuple_var));
	if(!t->data)
	{
		ads_free(t);
		return NULL;
	}
	t->keylist = (char**)ads_calloc(size,sizeof(char*));
	if(!t->keylist)
	{
		ads_free(t);
		ads_free(t->data);
		return NULL;
	}
	t->memory = size;
	t->size = 0;
	return t;
}

int db_tupleLen(const db_tuple self)
{
    if(!self) return -1;
	return self->size;
}

int db_tupleMemory(const db_tuple self)
{
    if(!self) return -1;
	return self->memory;
}

bool db_tupleHasKey(const db_tuple self,int index)
{
    if(!self) return false;
    if(index < 0 || index > self->size-1 ) return false;
    if(self->keylist[index] != NULL ) return true;
    return false;
}

const char* db_tupleGetKey(const db_tuple self,int index)
{
    if(!self) return false;
    if(index < 0 || index > self->size-1 ) return false;
    return self->keylist[index];
}

int db_tupleGetIndex(const db_tuple self,const char * key)
{
    if(!self) return -1;
	if(!key)return -1;
	int i;
	for(i=0;i<self->size;i++)
	{
		if(self->keylist[i])
		{
			if(stricmp(key,self->keylist[i]) == 0)
			{
				return i;
			}
		}
	}
	return -1;
}

bool db_tupleSetKey(db_tuple self,const char * key,int index)
{
    if(!self) return 0;
	if(!key)return false;
	if(index < 0 || index > self->size-1 ) false;
	int i;
	for(i=0;i<self->size;i++)
	{
		if(self->keylist[i])
		{
			if(stricmp(key,self->keylist[i]) == 0)
			{
				return false;
			}
		}
	}
	self->keylist[index] = ads_cstrCopy(key);
	return true;
}

bool db_tupleDelKey(db_tuple self,const char * key,int index)
{
    if(!self) return 0;
	if(index < 0 || index > self->size-1 )
	{
		if(!key) return false;
		int i;
		for(i=0;i<self->size;i++)
		{
			if(stricmp(key,self->keylist[i]) == 0)
			{
				ads_free(self->keylist[i]);
				self->keylist[i] = NULL;
				return true;
			}
		}
	}
	ads_free(self->keylist[index]);
	self->keylist[index] = NULL;
	return true;
}

db_tupleTYPE db_tupleGet_Type(db_tuple self,const char * key)
{
    if(!self) return db_NULL;
    int index = db_tupleGetIndex(self,key);
	if(index >= self->size) return db_NULL;
	return self->data[index].type;
}

db_tupleTYPE db_tupleGetItem_Type(db_tuple self,int index)
{
    if(!self) return db_NULL;
	if(index >= self->size) return db_NULL;
	return self->data[index].type;
}

bool db_tupleAppend_Null(db_tuple self,const char * key)
{
    if(!self) return false;
	if(self->size == self->memory) return false;
	self->data[self->size].type = db_NULL;
	if(key)
	{
		db_tupleSetKey(self,key,self->size);
	}
	self->size++;
	return true;
}

bool db_tupleIsNull(db_tuple self,const char * key)
{
	if(!self) return false;
	int index = db_tupleGetIndex(self,key);
	if(index >= self->size) return false;
	if(self->data[index].type != db_NULL) return true;
	return false;
}

bool db_tupleItemIsNull(db_tuple self,int index)
{
	if(!self) return false;
	if(index >= self->size) return false;
	if(self->data[index].type != db_NULL) return true;
	return false;
}

bool db_tupleAppend_Bool(db_tuple self,const char * key,bool value)
{
    if(!self) return false;
	if(self->size == self->memory) return false;
	self->data[self->size].type = db_BOOL;
	self->data[self->size].b = value;
	if(key)
	{
		db_tupleSetKey(self,key,self->size);
	}
	self->size++;
	return true;
}

bool db_tupleGet_Bool(db_tuple self,const char * key)
{
    if(!self) return 0;
	int index = db_tupleGetIndex(self,key);
	if(index >= self->size) return 0;
	if(self->data[index].type != db_BOOL) return 0;
	return self->data[index].b;
}

bool db_tupleGetItem_Bool(db_tuple self,int index)
{
    if(!self) return 0;
	if(index >= self->size) return 0;
	if(self->data[index].type != db_BOOL) return 0;
	return self->data[index].b;
}

bool db_tupleAppend_Int(db_tuple self,const char * key,int64_t value)
{
    if(!self) return false;
	if(self->size == self->memory) return false;
	self->data[self->size].type = db_INT;
	self->data[self->size].i = value;
	if(key)
	{
		db_tupleSetKey(self,key,self->size);
	}
	self->size++;
	return true;
}

int64_t db_tupleGet_Int(db_tuple self,const char * key)
{
    if(!self) return 0;
	int index = db_tupleGetIndex(self,key);
	if(index >= self->size) return 0;
	if(self->data[index].type != db_INT) return 0;
	return self->data[index].i;
}

int64_t db_tupleGetItem_Int(db_tuple self,int index)
{
    if(!self) return 0;
	if(index >= self->size) return 0;
	if(self->data[index].type != db_INT) return 0;
	return self->data[index].i;
}

bool db_tupleAppend_Double(db_tuple self,const char * key,double value)
{
    if(!self) return false;
	if(self->size == self->memory) return false;
	self->data[self->size].type = db_DOUBLE;
	self->data[self->size].d = value;
	if(key)
	{
		db_tupleSetKey(self,key,self->size);
	}
	self->size++;
	return true;
}

double db_tupleGet_Double(db_tuple self,const char * key)
{
    if(!self) return 0;
	int index = db_tupleGetIndex(self,key);
	if(index >= self->size) return 0;
	if(self->data[index].type != db_DOUBLE) return 0;
	return self->data[index].d;
}

double db_tupleGetItem_Double(db_tuple self,int index)
{
    if(!self) return 0;
	if(index >= self->size) return 0;
	if(self->data[index].type != db_DOUBLE) return 0;
	return self->data[index].d;
}

bool db_tupleAppend_String(db_tuple self,const char * key,const char * value)
{
    if(!self) return false;
	if(self->size == self->memory) return false;
	self->data[self->size].type = db_STRING;
	self->data[self->size].s = ads_cstrCopy(value);
	if(key)
	{
		db_tupleSetKey(self,key,self->size);
	}
	self->size++;
	return true;
}

bool db_tupleAppend_Tuple(db_tuple self,const char * key,db_tuple value)
{
    if(!self) return false;
	if(self->size == self->memory) return false;
	self->data[self->size].type = db_TUPLE;
	self->data[self->size].t = value;
	if(key)
	{
		db_tupleSetKey(self,key,self->size);
	}
	self->size++;
	return true;
}

const char * db_tupleGet_String(db_tuple self,const char * key)
{
    if(!self) return NULL;
	int index = db_tupleGetIndex(self,key);
	if(index >= self->size) return NULL;
	if(self->data[index].type != db_STRING) return NULL;
	return self->data[index].s;
}

const char * db_tupleGetItem_String(db_tuple self,int index)
{
    if(!self) return NULL;
	if(index >= self->size) return NULL;
	if(self->data[index].type != db_STRING) return NULL;
	return self->data[index].s;
}

db_tuple db_tupleGet_Tuple(db_tuple self,const char * key)
{
    if(!self) return NULL;
    int index = db_tupleGetIndex(self,key);
	if(index >= self->size) return NULL;
	if(self->data[index].type != db_TUPLE) return NULL;
	return self->data[index].t;
}

db_tuple db_tupleGetItem_Tuple(db_tuple self,int index)
{
    if(!self) return NULL;
	if(index >= self->size) return NULL;
	if(self->data[index].type != db_TUPLE) return NULL;
	return self->data[index].t;
}

bool db_tuplePrint(db_tuple self,bool key)
{
    if(!self) return false;
	int i;
	printf("( ");
	for(i=0;i<self->size;i++)
	{
		if(key)if(self->keylist[i]) printf("%s : ",self->keylist[i]);
		if(self->data[i].type == db_INT) printf("%I64d",self->data[i].i);
		if(self->data[i].type == db_BOOL) (self->data[i].b)?printf("True"):printf("False");
		else if(self->data[i].type == db_DOUBLE) printf("%g",self->data[i].d);
		else if(self->data[i].type == db_STRING) printf("\"%s\"",self->data[i].s);
		else if(self->data[i].type == db_TUPLE) db_tuplePrint(self->data[i].t,key);
		else if(self->data[i].type == db_NULL) printf("NULL");
		if(i<self->size-1)printf(" , ");
	}
	printf(" )");
	return true;
}

db_tuple db_tupleCopy(db_tuple t)
{
	int i;
	db_tuple tc = db_tupleNew(db_tupleLen(t));
	for( i = 0 ; i < db_tupleLen(t) ; i++ )
	{
		tc->data[i] = t->data[i];
		if(tc->data[i].type == db_STRING)
		{
			tc->data[i].s = ads_cstrCopy(t->data[i].s);
		}
		if(tc->data[i].type == db_TUPLE)
		{
			tc->data[i].t = db_tupleCopy(t->data[i].t);
		}
		if(t->keylist[i])tc->keylist[i] = ads_cstrCopy(t->keylist[i]);
	}
	tc->size = t->size;
	return tc;
}

bool db_tupleDelete(db_tuple self)
{
    if(!self) return false;
	int i;
	for(i=0;i<self->size;i++)
	{
		if(self->data[i].type == db_STRING)
		{
			ads_free(self->data[i].s);
		}
		if(self->data[i].type == db_TUPLE)
		{
			db_tupleDelete(self->data[i].t);
		}
		if(self->keylist[i])ads_free(self->keylist[i]);
	}
	ads_free(self->data);
	ads_free(self->keylist);
	ads_free(self);
	return true;
}

static int comma_count(const char * cmd,int size)
{
	int i,clevel=0,cnt = 0;bool qou1 = false,qou2 = false;
	for(i=0;i<=size;i++)
	{
		if(ads_isStartBracket(cmd[i]) == true ) clevel++;
		else if(ads_isEndBracket(cmd[i]) == true ) clevel--;
		if( cmd[i]  == '\'') qou1 = !qou1;
		if( cmd[i]  == '"') qou2 = !qou2;
		if( clevel == 0 && qou1 == false && qou2 == false)
		{
			if(cmd[i] == ',' ) cnt++;
		}
	}
	return cnt;
}

static db_tupleTYPE recognize_type(const char * str,int len)
{
	int i;
	for(i=0;i<len;i++)
	{
		if( str[i] == '\'' || str[i] == '"') return db_STRING;
		if( str[i] == '.')return db_DOUBLE;
		if( str[i] == '{')return db_TUPLE;
		if( strnicmp(&str[i],"null",4) == 0 ) return db_NULL;
		if( strnicmp(&str[i],"true",4) == 0 || strnicmp(&str[i],"false",5) == 0 ) return db_BOOL;
	}
	return db_INT;
}

static void remove_space(char * str, int * len)
{
	int i,cnt =0;bool qou1 = false,qou2 = false;
	for(i=0;i<*len;i++)
	{
		if( str[i]  == '\'') qou1 = !qou1;
		if( str[i]  == '"') qou2 = !qou2;
		if( str[i] != ' ' && isprint(str[i]) ) str[cnt++] = str[i];
		else if( str[i] == ' ' && (qou1 == true || qou2 == true) ) str[cnt++] = str[i];
	}
	str[cnt] = '\0';
	*len = cnt;
}

static char * GivenKey(const char * str , int len , int * p)
{
	int i,clevel=0;
	static char * key;
	bool qou1 = false,qou2 = false;
	key = NULL;
	for(i=0;i<len;i++)
	{
		if(ads_isStartBracket(str[i]) == true ) clevel++;
		else if(ads_isEndBracket(str[i]) == true ) clevel--;
		if( str[i]  == '\'') qou1 = !qou1;
		if( str[i]  == '"') qou2 = !qou2;
		if( clevel == 0 && qou1 == false && qou2 == false)
		{
			if( str[i] == ':')
			{
				key = ads_calloc(i+1,sizeof(char));
				strncpy(key,str,i);
				*p = *p + i+1;
				return key;
			}

		}
	}
	return NULL;
}

static db_tuple db_tupleParser(const char * str, int len);

static db_tuple generate_db_tuple(const char * str, int len)
{
	int size = comma_count(str,len) + 1 ;bool qou1 = false,qou2 = false;
	db_tuple t = db_tupleNew(size);
	int i,clevel=0,p = 0;
	for(i=0;i<len;i++)
	{
		if(ads_isStartBracket(str[i]) == true ) clevel++;
		else if(ads_isEndBracket(str[i]) == true ) clevel--;
		if( str[i]  == '\'') qou1 = !qou1;
		if( str[i]  == '"') qou2 = !qou2;
		if( clevel == 0 && qou1 == false && qou2 == false)
		{
			if(str[i] == ',' || str[i] == 0 )
			{
				char * key = GivenKey(&str[p],i-p,&p);
				switch(recognize_type(&str[p],i-p))
				{
					case db_INT : {
						int64_t value ;
						sscanf(&str[p],"%I64i,",&value);
						db_tupleAppend_Int(t,key,value);
						break;
					}
					case db_DOUBLE : {
						double value ;
						sscanf(&str[p],"%lf,",&value);
						db_tupleAppend_Double(t,key,value);
						break;
					}
					case db_STRING : {
						char value[i-p+1] ;
						memset(value,0,((i-p+1)*sizeof(char)));
						strncpy(value,&str[p+1],i-p-2);
						db_tupleAppend_String(t,key,value);
						break;
					}
					case db_NULL : {
						db_tupleAppend_Null(t,key);
						break;
					}
					case db_TUPLE : {
						db_tuple value = db_tupleParser(&str[p],i-p);
						db_tupleAppend_Tuple(t,key,value);
						break;
					}
					case db_BOOL : {
						bool value = ( toupper(str[p]) == 'T' )? true : false ;
						db_tupleAppend_Bool(t,key,value);
						break;
					}
				}
				p = i + 1;
				if(key)ads_free(key);
			}
		}
	}
	return t;
}

static db_tuple db_tupleParser(const char * str, int len)
{
	char st[len+1];
	strncpy(st,str,len);
	remove_space(st,&len);
	int i;
	for(i = 0;i<len;i++)
	{
		if(st[i] == '{' || st[i] == '[' || st[i] == '(')
        {
            st[len-1] = 0;
            return generate_db_tuple(&st[i+1],len-1);
        }
	}
	int size;
	sscanf(st,"%d",&size);
	return db_tupleNew(size);
}

db_tuple db_tupleLiteral(const char * n,...)
{
    if(!n) return NULL;
	char tmp[BUFSIZ];
	va_list v;
	va_start(v,n);
	vsprintf(tmp,n,v);
	return db_tupleParser(tmp,strlen(tmp));
	va_end(v);
}

/*
int main()
{
	db_tuple t = db_tupleLiteral(" 0 -> { name }");
	db_tuplePrint(t,1);
	printf("\n s = %s",db_tupleGet_String(t,"String"));
	db_tupleDelete(t);
	return 0;
}*/

#endif // staticDATABASE_DB_TUPLE_C

