
#ifndef _SATATICFILES_DB_C
#define _SATATICFILES_DB_C

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

#include "../../include/database/files.h"


int ads_fileLength(FILE * fp)
{
    fseek(fp, 0L, SEEK_END);
    return ftell(fp);
}

bool ads_fileExists(const char * file)
{
    FILE * fp = fopen(file,"r");
    if(fp)
    {
        fclose(fp);
        return true;
    }
    return false;
}

sdb_sig sdb_signature = {"staticDB v1.0",13};

static char * GetSignature(FILE * fp)
{
    static char tmp[16];
    int i=0;
    while(i<sdb_signature.len)
    {
        tmp[i++] = fgetc(fp);
    }
    tmp[i] = '\0';
    return tmp;
}

void writeSignature(FILE*fp)
{
    fprintf(fp,sdb_signature.data);
}

bool isSignatureMatch(FILE * fp)
{
    fseek(fp,0,SEEK_END);
    if( ftell(fp) < (2*sdb_signature.len) ) return false;
    fseek(fp,0,SEEK_SET);
    if(strcmp(GetSignature(fp),sdb_signature.data) == 0 )
    {
        fseek(fp,-sdb_signature.len,SEEK_END);
        if(strcmp(GetSignature(fp),sdb_signature.data) == 0 ) return true;
    }
    return false;
}



#endif // _SATATICFILES_DB_C
