
#ifndef _SATATICDATABSE_DB_C
#define _SATATICDATABSE_DB_C


#include "../../include/database/dataBase.h"

struct _staticDB {
    char * file;
    char * password;
    ads_avl tables;
    ads_avl pairs;
    stringBase * sb;
};


struct _sdbMasterDB {
    filepos tableArr;
    filepos pairArr;
    filepos baseArr;
    int64_t base_nos;
    int64_t pairs_nos;
    int64_t no_of_tables;
    char passwrd[37];
};

static bool checkPassword(const char * s1, char * enc);
static nosqlRT readDB(FILE * fp,sdbMasterDB * mdb);

nosqlRT ads_connectDB(staticDB** addr_db,const char * file,const char * password)
{
    if(strlen(password) > 36 )
    {
        sdb_setNoSqlMsg("Security Issue : Password Have To Be Between (0-31) Characters.");
        return nosql_ERROR_MSG;
    }
    if(ads_fileExists(file))
    {
        FILE * fp = fopen(file,"rb");
        sdbMasterDB mdb;
        memset(&mdb,0,sizeof(mdb));
        if( readDB(fp,&mdb) == nosql_ERROR_MSG )
        {
            return nosql_ERROR_MSG;
        }
        if(checkPassword(password,mdb.passwrd) == false)
        {
            sdb_setNoSqlMsg("Security Issue : Incorrect Password.");
            return nosql_ERROR_MSG;
        }
        *addr_db = (staticDB*)ads_calloc(1,sizeof(staticDB));
        staticDB * db = *addr_db;
        db->file = ads_cstrCopy(file);
        db->password = encrypt(ads_cstrCopy(password),123);
        db->tables = ads_avlNew(int64_t,ads_cmpInt64,sdbTable*);
        db->pairs = ads_avlNew(int64_t,ads_cmpInt64,sdb_pair*);
        db->sb = ads_baseCreate();
        if(mdb.baseArr)  ads_baseRead(db->sb,fp,&mdb.baseArr,mdb.base_nos);
        if(mdb.tableArr) ads_tableRead(db,fp,mdb.tableArr,mdb.no_of_tables);
        if(mdb.pairArr)  ads_pairRead(db,fp,&mdb.pairArr,mdb.pairs_nos);
        fclose(fp);
    }
    else
    {
        *addr_db = (staticDB*)ads_calloc(1,sizeof(staticDB));
        staticDB * db = *addr_db;
        db->file = ads_cstrCopy(file);
        db->password = encrypt(ads_cstrCopy(password),123);
        db->tables = ads_avlNew(int64_t,ads_cmpInt64,sdbTable*);
        db->pairs = ads_avlNew(int64_t,ads_cmpInt64,sdb_pair*);
        db->sb = ads_baseCreate();
    }
    sdb_setNoSqlMsg("Database Successfully Connected.");
    return nosql_SUCCESS_MSG;
}

void ads_disconnectDB(staticDB*db)
{
    ads_tableDestroy(db);
    ads_pairDestroy(db);
    ads_baseDestroy(db->sb);
    ads_free(db->file);
    ads_free(db->password);
    ads_free(db);
    sdb_freeNoSqlReturnTuple();
}

stringBase* ads_dbGetBaseTree(staticDB * db)
{
    return db->sb;
}

ads_avl ads_dbGetPairTree(staticDB * db)
{
    return db->pairs;
}

ads_avl ads_dbGetTableTree(staticDB * db)
{
    return db->tables ;
}

const char * ads_dbFileName(staticDB * db)
{
    return db->file;
}

static void ads_dbReadyForWrite(staticDB * db)
{
    //ads_tableCacheAll(db);
    FILE *fp = fopen(db->file,"rb");
    ads_baseCacheAll(db->sb,fp);
    fclose(fp);
    ads_tableCommitAll(db);
}

static bool checkPassword(const char * s1, char * enc)
{
    decrypt(enc,123);
    if(strcmp(s1,enc) == 0) return true;
    return false;
}

/*
static void printFileTill(FILE*fp,int till,char * msg)
{
    printf("\n\nStart Printing : %s \n'",msg);
    int i = 0;
    fseek(fp,0,SEEK_SET);
    while(i<till)
    {
        printf("%c",fgetc(fp));
        i++;
    }
    fseek(fp,till,SEEK_SET);
    printf("'\n");
}*/

static nosqlRT databaseIO(bool RoW,sdbMasterDB * mdb,FILE *fp,filepos * origin)
{
    if(RoW == WRITE)
    {
        *origin = ftell(fp);
        fwrite(mdb,1,sizeof(sdbMasterDB),fp);
    }
    else
    {
        if( fseek(fp,*origin,SEEK_SET) )
        {
            sdb_setNoSqlMsg("Not A Valid Database File, Database Origin Did Not Match.");
            return nosql_ERROR_MSG;
        }
        fread(mdb,1,sizeof(sdbMasterDB),fp);
    }
    return nosql_SUCCESS_MSG;
}

static nosqlRT finishIO(bool RoW,filepos * origin,FILE *fp)
{
    if(RoW == WRITE)
    {
        fseek(fp,0,SEEK_END);
        fwrite(origin,1,sizeof(filepos),fp);
    }
    else
    {
        if( fseek(fp,-(sizeof(filepos)+sdb_signature.len),SEEK_END) )
        {
            sdb_setNoSqlMsg("Not A Valid Database File, Database Origin Did Not Match.");
            return nosql_ERROR_MSG;
        }
        fread(origin,1,sizeof(filepos),fp);
        if(*origin == 0)
        {
            sdb_setNoSqlMsg("Not A Valid Database File, Database Origin Did Not Match.");
            return nosql_ERROR_MSG;
        }
    }
    return nosql_SUCCESS_MSG;
}

static nosqlRT isAuthenticatedDB(FILE * fp,filepos *origin)
{
    if( isSignatureMatch(fp) == false)
    {
        sdb_setNoSqlMsg("Not A Valid Database File, Database Signature Did Not Match.");
        return nosql_ERROR_MSG;
    }
    return finishIO(READ,origin,fp);
}

static nosqlRT readDB(FILE * fp,sdbMasterDB * mdb)
{
    memset(mdb,0,sizeof(sdbMasterDB));
    filepos origin = 0;
    if( isAuthenticatedDB(fp,&origin) == nosql_ERROR_MSG )
    {
        return nosql_ERROR_MSG;
    }
    return databaseIO(READ,mdb,fp,&origin);
}


void ads_dbCommit(staticDB * db)
{
    if(!db) return ;

    ads_dbReadyForWrite(db);

    FILE *fp = fopen(db->file,"wb");

    writeSignature(fp);
    ads_baseCommitAll(db->sb,fp);

    filepos arrst = 0, origin = 0,pairst = 0,basest =0;

    ads_tableWrite(db,fp,&arrst);

    //printFileTill(fp,ftell(fp),"master table");

    ads_pairWrite(db,fp,&pairst);
    ads_baseWrite(db->sb,fp,&basest);

    //printFileTill(fp,ftell(fp),"master pair");

    //printf("\nfilearr = %I64d",arrst);

    sdbMasterDB mdb ;
    memset(&mdb,0,sizeof(mdb));
    mdb.tableArr = arrst;
    mdb.pairArr = pairst;
    mdb.baseArr = basest;
    mdb.base_nos = baseCount(db->sb);
    mdb.pairs_nos = ads_pairCount(db);
    mdb.no_of_tables = ads_tableCount(db);
    strcpy(mdb.passwrd,db->password);
    databaseIO(WRITE,&mdb,fp,&origin);

    //printFileTill(fp,ftell(fp),"master dB");

    //printf("\norigin = %I64d",origin);

    finishIO(WRITE,&origin,fp);

    //printFileTill(fp,ftell(fp),"finish dB");
    writeSignature(fp);

    fclose(fp);
}

#endif // _SATATICDATABSE_DB_C
