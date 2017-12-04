

#ifndef _SATATICPAIRS_DB_C
#define _SATATICPAIRS_DB_C

#include "../../include/database/pairs.h"

struct sdb_pair
{
    int64_t keyname;
    int64_t data;
};

void ads_pairDestroy(staticDB * db)
{
    ads_avl pt = ads_dbGetPairTree(db);
    ads_vector v = ads_avlTraversal( pt , ads_inOrder, ads_avlData);
    ads_foreach(p,ads_vectorIterator(v),sdb_pair*)
    {
        ads_free(p);
    }ads_next(p);
    ads_deleteOne(v);
    ads_deleteOne(pt);
}

int ads_pairCount(staticDB * db)
{
    return ads_avlLen( ads_dbGetPairTree(db) );
}

nosqlRT ads_pairKeys(staticDB * db)
{
    if(!db)
    {
        sdb_setNoSqlMsg("Database Not Valid | Not Connected");
        return nosql_ERROR_MSG;
    }

    db_tuple t = db_tupleNew( ads_pairCount(db) );

    ads_vector v = ads_avlTraversal( ads_dbGetPairTree(db) , ads_inOrder, ads_avlData);
    ads_foreach( p , ads_vectorIterator(v) , sdb_pair* )
    {
        db_tupleAppend_String(t,NULL,baseNodeCStr(db,p->keyname));
    }ads_next(p);
    ads_deleteOne(v);

    sdb_setNoSqlReturnTuple(t);
    return nosql_RETURN_TUPLE;
}

bool ads_pairExists(staticDB * db,const char * keyname)
{
    if(!db)return false;
    bool b;
    ads_avl t = ads_dbGetPairTree(db);
    if(!t)return false;
    ads_avlContains( t, ads_cstrHash(keyname) , int64_t, b );
    return b;
}

static sdb_pair* getPairNode(staticDB * db,const char * key)
{
    if(!db)return false;
    if(ads_pairExists(db,key))
    {
        sdb_pair * p ;
        ads_avlGet_Copy( ads_dbGetPairTree(db) , ads_cstrHash(key) , int64_t, sdb_pair* , p );
        return p;
    }
    return NULL;
}

nosqlRT pairInsert(staticDB * db,const char * key,db_tuple t)
{
    if(!db)
    {
        sdb_setNoSqlMsg("Database Not Valid | Not Connected");
        return nosql_ERROR_MSG;
    }
    sdb_pair * p = getPairNode(db,key);
    if(!p)
    {
        sdb_setNoSqlMsg("Key Not Present In Database.");
        return nosql_ERROR_MSG;
    }
    extendBuffer(db,p->data,t);
    sdb_setNoSqlMsg("Key Value Inserted.");
    return nosql_SUCCESS_MSG;
}

nosqlRT ads_pairCreate(staticDB * db,const char * key,db_tuple t)
{
    if( identifierValidity(key,strlen(key)) == false )
    {
        sdb_setNoSqlMsg("Key Name Not Valid.");
        return nosql_ERROR_MSG;
    }
    if(!db)
    {
        sdb_setNoSqlMsg("Database Not Valid | Not Connected");
        return nosql_ERROR_MSG;
    }
    int64_t data = db_tupleToIndex(db,t);

    sdb_pair * p = getPairNode(db,key);
    if(p == NULL)
    {
        p = (sdb_pair*)ads_calloc(1,sizeof(sdb_pair));
        p->keyname = baseInsertCStr(db,key);
        ads_avlInsert( ads_dbGetPairTree(db) , ads_cstrHash(key) ,int64_t,p,sdb_pair*);
    }
    p->data = data;
    sdb_setNoSqlMsg("New Key Created.");
    return nosql_SUCCESS_MSG;
}

/*
static void cachePair(sdb_pair * p,FILE*fp)
{
    if(!p) return ;
    if( p->recordstart == 0 ) return ;
    if( fseek(fp,p->recordstart,SEEK_SET) )
    {
        printf("\n\nINVALID DATABASE FILE CANNOT READ PAIR.");
        exit(0);
    }
    void * data = ads_calloc(1,p->buffersize);
    fread(data,1,p->buffersize,fp);
    p->data = data;
}*/

nosqlRT ads_pairFetch(staticDB * db,const char * key)
{
    if(!db)
    {
        sdb_setNoSqlMsg("Database Not Valid | Not Connected");
        return nosql_ERROR_MSG;
    }
    db_tuple kt = splitKey(key);
    if(db_tupleLen(kt)<1)
    {
        db_tupleDelete(kt);
        sdb_setNoSqlMsg("Key Not Present In Database.");
        return nosql_ERROR_MSG;
    }
    sdb_pair * p = getPairNode(db,db_tupleGetItem_String(kt,0));
    if(!p)
    {
        db_tupleDelete(kt);
        sdb_setNoSqlMsg("Key Not Present In Database.");
        return nosql_ERROR_MSG;
    }
    db_tuple t = indexToDB_tuple(db,p->data);
    db_tuple rt = traverseTuple(kt,t,1);
    db_tupleDelete(kt);
    db_tupleDelete(t);
    if(!rt)
    {
        sdb_setNoSqlMsg("Key Not Present In Database.");
        return nosql_ERROR_MSG;
    }
    sdb_setNoSqlReturnTuple(rt);
    return nosql_RETURN_TUPLE;
}

nosqlRT ads_pairDelete(staticDB * db,const char * key)
{
    if(!db)
    {
        sdb_setNoSqlMsg("Database Not Valid | Not Connected");
        return nosql_ERROR_MSG;
    }
    sdb_pair * p = getPairNode(db,key);
    if(!p)
    {
        sdb_setNoSqlMsg("Key Not Present In Database.");
        return nosql_ERROR_MSG;
    }
    ads_avlRemove( ads_dbGetPairTree(db) , ads_cstrHash(key),int64_t);
    if(p->data)
    {
        deleteBuffer(db,p->data);
    }
    baseDelete(db,p->keyname);
    ads_free(p);

    sdb_setNoSqlMsg("Key Deleted From Database.");
	return nosql_SUCCESS_MSG;
}

/*
void ads_pairCacheAll(staticDB * db,FILE*fp)
{
    if(!db) return ;
    ads_vector v = ads_avlTraversal( ads_dbGetPairTree(db) , ads_inOrder, ads_avlData);
    ads_foreach(p,ads_vectorIterator(v),sdb_pair*)
    {
        if(!p->data)cachePair( p , fp);
    }ads_next(p);
    ads_deleteOne(v);
}*/

/*
void ads_pairCommitAll(staticDB * db,FILE *fp)
{
    if(!db) return ;
    ads_vector v = ads_avlTraversal( ads_dbGetPairTree(db) , ads_inOrder, ads_avlData);
    ads_foreach(p,ads_vectorIterator(v),sdb_pair*)
    {
        p->recordstart = ftell(fp);
        fwrite(p->data,1,p->buffersize,fp);
    }ads_next(p);
    ads_deleteOne(v);
}*/

void ads_pairRead(staticDB * db,FILE *fp,filepos * pos,int no_of_pairs)
{
    if(!db) return ;
    if(*pos == 0)return;
    if( fseek(fp,*pos,SEEK_SET) )
    {
        fclose(fp);
        printf("\n GIVEN DATABASE FILE IS INVALID .");
        exit(0);
    }
    ads_avl t = ads_dbGetPairTree(db);
    int i;
    for(i=0;i<no_of_pairs;i++)
    {
        sdb_pair * p = (sdb_pair*)ads_calloc(1,sizeof(sdb_pair));
        fread(&p->keyname,1,sizeof(int64_t),fp);
        fread(&p->data,1,sizeof(int64_t),fp);
        ads_avlInsert(t,ads_cstrHash(baseNodeCStr(db,p->keyname)),int64_t,p,sdb_pair*);
    }
}

void ads_pairWrite(staticDB * db,FILE *fp,filepos * pos)
{
    ads_avl t = ads_dbGetPairTree(db);
    if( ads_avlLen(t) <= 0 )return ;
    *pos = ftell(fp);
    ads_vector v = ads_avlTraversal( ads_dbGetPairTree(db) , ads_inOrder, ads_avlData);
    ads_foreach(p,ads_vectorIterator(v),sdb_pair*)
    {
        fwrite(&p->keyname,1,sizeof(int64_t),fp);
        fwrite(&p->data,1,sizeof(int64_t),fp);
    }ads_next(p);
    ads_deleteOne(v);
}




#endif
