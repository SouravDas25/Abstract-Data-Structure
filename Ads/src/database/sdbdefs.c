#ifndef SDB_DEFS_C
#define SDB_DEFS_C

#include "../../include/database/sdbdefs.h"

static db_tuple NoSqlReturnTuple = NULL ;

void sdb_setNoSqlReturnTuple(db_tuple t)
{
    if(NoSqlReturnTuple)db_tupleDelete(NoSqlReturnTuple);
    NoSqlReturnTuple = t;
    sdb_setNoSqlMsg("Tuple Returned.");
}

void sdb_freeNoSqlReturnTuple()
{
    if(NoSqlReturnTuple)db_tupleDelete(NoSqlReturnTuple);
    NoSqlReturnTuple = NULL;
}

db_tuple sdb_getNoSqlReturnTuple()
{
    return NoSqlReturnTuple;
}

static const char * nosqlcpmsg;

void sdb_setNoSqlMsg(const char * msg)
{
    nosqlcpmsg = msg;
}

const char * sdb_getNoSqlMsg()
{
    return nosqlcpmsg;
}

void sdb_resetNoSqlRT()
{
    sdb_freeNoSqlReturnTuple();
    sdb_setNoSqlMsg(NULL);
}

bool identifierValidity(const char * str,int len)
{
    int i;bool n = true;
    for(i=0;i<len;i++)
    {
        if(n)
        {
            if( isdigit(str[i]) != false) return false;
        }
        if( isalpha(str[i]) != false)
        {
            n = false;
            continue;
        }
        if( isdigit(str[i]) != false)
        {
            continue;
        }
        if(str[i] == '_')continue;
        if(str[i] == '$')continue;
        return false;
    }
    return true;
}

char* encrypt(char * password,int key)
{
    if(!password)return password;
    unsigned int i,len = strlen(password) ;
    for(i=0;i<len;++i)
    {
        password[i] = password[i] - key;
    }
    return password;
}

char* decrypt(char * password,int key)
{
    if(!password)return password;
    unsigned int i,len = strlen(password) ;
    for(i=0;i<len;++i)
    {
        password[i] = password[i] + key;
    }
    return password;
}

uint64_t checksum(void *buffer, size_t len, uint64_t seed)
{
      unsigned char *buf = (unsigned char *)buffer;
      size_t i;

      for (i = 0; i < len; ++i)
            seed += (unsigned int)(*buf++);
      return seed;
}

#endif // SDB_DEFS_C
