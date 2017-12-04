
#ifndef _SATATICTABLE_DB_C
#define _SATATICTABLE_DB_C


#include "../../include/database/table.h"

struct _sdbTable {
    int64_t rd;
    int64_t tablename;
    int64_t formatIndex;
    int rows;
    ads_list ptRs;
};

static sdbTable* fetchTable(staticDB * db,const char * tablename);
static sdbTable* fetchTable(staticDB * db,const char * tablename);
static sdbTable* GetTableDetails(staticDB * db,const char * tablename);
static void deleteTableData(staticDB * db,sdbTable* fi);
static void ads_tableCommit(staticDB * db,sdbTable * fi);

void ads_tableDestroy(staticDB * db)
{
    if(!db) return ;
    ads_avl ti = ads_dbGetTableTree(db);
    if(!ti) return ;
    ads_vector v = ads_avlTraversal( ti , ads_inOrder, ads_avlData);
    ads_foreach(t,ads_vectorIterator(v),sdbTable*)
    {
        //if( !ads_listEmpty(t->ptRs) ) deleteTableData(db,t);
        ads_deleteOne(t->ptRs);
        ads_deleteOne( t );
    }ads_next(t);
    ads_deleteOne(v);
    ads_deleteOne(ti);
}

bool ads_tableExist(staticDB * db,const char * tablename)
{
    if(!db)return false;
    if(!tablename)return false;
    bool b;
    ads_avl t = ads_dbGetTableTree(db);
    if(!t)return false;
    ads_avlContains( t, ads_cstrHash(tablename) , int64_t, b );
    return b;
}

int ads_tableCount(staticDB * db)
{
    return ads_avlLen( ads_dbGetTableTree(db) );
}

nosqlRT ads_tableNames(staticDB * db)
{
    if(!db)
    {
        sdb_setNoSqlMsg("Database Not Valid | Not Connected");
        return nosql_ERROR_MSG;
    }

    db_tuple t = db_tupleNew( (ads_tableCount(db))?ads_tableCount(db):1 );

    ads_vector v = ads_avlTraversal( ads_dbGetTableTree(db) , ads_inOrder, ads_avlData);
    ads_foreach( p , ads_vectorIterator(v) , sdbTable* )
    {
        db_tupleAppend_String(t,NULL,baseNodeCStr(db,p->tablename));
    }ads_next(p);
    ads_deleteOne(v);

    sdb_setNoSqlReturnTuple(t);
    return nosql_RETURN_TUPLE;
}


static sdbTable* GetTableDetails(staticDB * db,const char * tablename)
{
    if(!db)return NULL;
    if(ads_tableExist(db,tablename))
    {
        sdbTable* t ;
        ads_avlGet_Copy( ads_dbGetTableTree(db) , ads_cstrHash(tablename) , int64_t, sdbTable* , t );
        return t;
    }
    return NULL;
}

static void cacheTable(staticDB * db,sdbTable*ft)
{
    if(!ft) return ;
    if( ft->rd == 0 ) return ;
    if( !ads_listEmpty(ft->ptRs) || ft->rows == 0 ) return ;
    int64_t * data = (int64_t*)baseNodeData(db,ft->rd);
    int i;
    for(i=0;i<ft->rows;i++)
    {
        ads_listAppend(ft->ptRs,data[i],int64_t);
    }//ads_printf(" list = {list(d)} \n",ft->ptRs);
}

 void ads_tableCacheAll(staticDB * db)
{
    ads_avl ti = ads_dbGetTableTree(db);
    if(!ti) return ;
    ads_vector v = ads_avlTraversal( ti ,ads_inOrder, ads_avlData);
    ads_foreach(t,ads_vectorIterator(v),sdbTable*)
    {
        if( ads_listEmpty(t->ptRs) && t->rows != 0 ) cacheTable( db , t );
    }ads_next(t);
    ads_deleteOne(v);
}

static sdbTable* fetchTable(staticDB * db,const char * tablename)
{
    if(!db) return NULL;
    sdbTable * fi = GetTableDetails(db,tablename);
    if(!fi) return NULL;
    if( ads_listEmpty(fi->ptRs) && fi->rows != 0 )
    {
        cacheTable(db,fi);
    }
    return fi;
}

nosqlRT ads_createTable(staticDB * db,const char * tablename,const char * types)
{
    if(!db)
    {
        sdb_setNoSqlMsg("Database Not Valid | Not Connected");
        return nosql_ERROR_MSG;
    }
    if( identifierValidity(tablename,strlen(tablename)) == false)
    {
        sdb_setNoSqlMsg("Array Name Not Valid.");
        return nosql_ERROR_MSG;
    }
    if( ads_tableExist(db,tablename) == true )
    {
        sdb_setNoSqlMsg("Array Name Already Exists.");
        return nosql_ERROR_MSG;
    }
    if(!types)
    {
        sdb_setNoSqlMsg("Array Container Type Not Defined");
        return nosql_ERROR_MSG;
    }

    ads_avl t = ads_dbGetTableTree(db);

    sdbTable* ti = (sdbTable*)ads_calloc(1,sizeof(sdbTable));
    int len = 0;
    db_tupleTYPE * format = generate_TypeFormat(types,strlen(types),&len);
    ti->formatIndex = baseInsert(db,format,len*sizeof(db_tupleTYPE));
    ads_free(format);

    ti->tablename = baseInsertCStr(db,tablename);
    ti->rd = 0;
    ti->rows = 0;
    ti->ptRs = ads_listNew(int64_t);

    ads_avlInsert(t,ads_cstrHash(tablename),int64_t,ti,sdbTable*);
    sdb_setNoSqlMsg("New Array Created.");
    return nosql_SUCCESS_MSG;
}

nosqlRT ads_tableClear(staticDB * db,const char * tablename)
{
    if(!db)
    {
        sdb_setNoSqlMsg("Database Not Valid | Not Connected");
        return nosql_ERROR_MSG;
    }
    //if( ads_tableExist(db,tablename) == false ) return;
    sdbTable * fi = GetTableDetails(db,tablename);
    if( !fi )
    {
        sdb_setNoSqlMsg("Array Not Present In Database.");
        return nosql_ERROR_MSG;
    }
    deleteTableData(db,fi);
    sdb_setNoSqlMsg("All Rows Have Been Deleted.");
    return nosql_SUCCESS_MSG;
}

static void deleteTableData(staticDB * db,sdbTable* fi)
{
    if( ads_listEmpty(fi->ptRs) || fi->rows == 0 ) return ;
    int i;
    for(i=0;i<fi->rows;i++)
    {
        int64_t v;
        ads_listGetItem_NoCopy(fi->ptRs,i,int64_t,v);
        deleteBuffer(db,v);
    }
    ads_listClear(fi->ptRs);
    baseDelete(db,fi->rd);
    fi->rows = 0;
}

static void tableDeleteEntire(staticDB * db,const char * tablename )
{
    sdbTable* ti;
    int64_t hh = ads_cstrHash(tablename);
    ads_avlGet_Copy( ads_dbGetTableTree(db) , hh , int64_t, sdbTable* , ti );
    deleteTableData(db,ti);
    baseDelete(db,ti->rd);
    baseDelete(db,ti->tablename);
    baseDelete(db,ti->formatIndex);
    ads_deleteOne(ti->ptRs);
    ads_free(ti);
    ads_avlRemove( ads_dbGetTableTree(db) , hh ,int64_t);
}

nosqlRT ads_tableDelete(staticDB * db,const char * tablename)
{
    if(!db)
    {
        sdb_setNoSqlMsg("Database Not Valid | Not Connected");
        return nosql_ERROR_MSG;
    }
    db_tuple kt = splitKey(tablename);
    if(db_tupleLen(kt)<1)
    {
        db_tupleDelete(kt);
        sdb_setNoSqlMsg("Key Not Present In Database.");
        return nosql_ERROR_MSG;
    }
    if( ads_tableExist(db,db_tupleGetItem_String(kt,0)) == false )
    {
        db_tupleDelete(kt);
        sdb_setNoSqlMsg("Array Not Present In Database.");
        return nosql_ERROR_MSG;
    }
    if( db_tupleLen(kt) == 1 )
    {
        tableDeleteEntire(db,db_tupleGetItem_String(kt,0));
    }
    else if( db_tupleLen(kt) == 2 && db_tupleGetItem_Type(kt,1) == db_INT )
    {
        if( ads_tableDeleteRow(db,db_tupleGetItem_String(kt,0),db_tupleGetItem_Int(kt,1)) == nosql_ERROR_MSG)
        {
            db_tupleDelete(kt);
            return nosql_ERROR_MSG;
        }
    }
    else
    {
        db_tupleDelete(kt);
        sdb_setNoSqlMsg("Array Modifier Not Recognized.");
        return nosql_ERROR_MSG;
    }
    db_tupleDelete(kt);
    sdb_setNoSqlMsg("Array Deleted From Database.");
    return nosql_SUCCESS_MSG;
}


int ads_tableRowCount(staticDB * db,const char * tablename)
{
    sdbTable * fi = fetchTable(db,tablename);
    return fi->rows;
}

nosqlRT ads_tableDeleteRow(staticDB * db,const char * tablename,int rowid)
{
    sdbTable * fi = fetchTable(db,tablename);
    if( rowid >= fi->rows  || rowid <  0)
    {
        sdb_setNoSqlMsg("Row Index Is Invalid | Out Of Range.");
        return nosql_ERROR_MSG;
    }
    int64_t data1 ;
    ads_listGetItem_NoCopy(fi->ptRs,rowid,int64_t,data1);
    if(!data1)
    {
        sdb_setNoSqlMsg("Array Data Has Been Corrupted.");
        return nosql_ERROR_MSG;
    }
    deleteBuffer(db,data1);
    ads_listDelItem(fi->ptRs,rowid);
    fi->rows--;

    sdb_setNoSqlMsg("Array Row Has Been Deleted From Database.");
	return nosql_SUCCESS_MSG;
}

static db_tuple tableFetchEntire(staticDB * db,sdbTable * fi)
{
    int64_t data1 ;
    int ty;
    db_tuple tt = db_tupleNew(fi->rows);
    for(ty = 0; ty < fi->rows ;ty++)
    {
        ads_listGetItem_NoCopy(fi->ptRs,ty,int64_t,data1);
        db_tuple t = indexToDB_tuple( db , data1 );
        db_tupleAppend_Tuple(tt,NULL,t);
    }
    return tt;
}

static db_tuple ads_tableFetchRow(staticDB * db,sdbTable * fi,int rowid)
{
    int64_t data1 ;
    ads_listGetItem_NoCopy(fi->ptRs,rowid,int64_t,data1);
    if(!data1)
    {
        return NULL;
    }
    return indexToDB_tuple(db,data1);
}

nosqlRT ads_tableFetch(staticDB * db,const char * tablename)
{
    if(!db)
    {
        sdb_setNoSqlMsg("Database Not Valid | Not Connected");
        return nosql_ERROR_MSG;
    }
    db_tuple kt = splitKey(tablename);
    if(db_tupleLen(kt)<1)
    {
        db_tupleDelete(kt);
        sdb_setNoSqlMsg("Array Not Present In Database.");
        return nosql_ERROR_MSG;
    }
    sdbTable * fi = fetchTable(db,db_tupleGetItem_String(kt,0));
    if(!fi)
    {
        db_tupleDelete(kt);
        sdb_setNoSqlMsg("Array Not Present In Database.");
        return nosql_ERROR_MSG;
    }
    if( db_tupleLen(kt) == 1)
    {
        db_tupleDelete(kt);
        sdb_setNoSqlReturnTuple( tableFetchEntire(db,fi) );
        return nosql_RETURN_TUPLE;
    }
    if( db_tupleGetItem_Type(kt,1) != db_INT )
    {
        db_tupleDelete(kt);
        sdb_setNoSqlMsg("Array Should Be Followed By A Row Index.");
        return nosql_ERROR_MSG;
    }
    int rowid = db_tupleGetItem_Int(kt,1);
    if( rowid >= fi->rows  || rowid <  0)
    {
        db_tupleDelete(kt);
        sdb_setNoSqlMsg("Row Index Is Invalid | Out Of Range.");
        return nosql_ERROR_MSG;
    }
    db_tuple t = ads_tableFetchRow(db,fi,rowid);
    if(t == NULL)
    {
        db_tupleDelete(kt);
        sdb_setNoSqlMsg("Array Data Has Been Corrupted.");
        return nosql_ERROR_MSG;
    }
    db_tuple rt = traverseTuple(kt,t,2);
    if(rt == NULL)
    {
        db_tupleDelete(kt);
        db_tupleDelete(t);
        sdb_setNoSqlMsg("Array Modifiers Not Found.");
        return nosql_ERROR_MSG;
    }
    db_tupleDelete(kt);
    db_tupleDelete(t);
    sdb_setNoSqlReturnTuple(rt);
    return nosql_RETURN_TUPLE;
}

nosqlRT ads_tableInsetRow(staticDB * db,const char * tablename,db_tuple t)
{
    if(!db)
    {
        sdb_setNoSqlMsg("Database Not Valid | Not Connected");
        return nosql_ERROR_MSG;
    }
    sdbTable * fi = fetchTable(db,tablename);
    if(!fi)
    {
        sdb_setNoSqlMsg("Array Not Present In Database.");
        return nosql_ERROR_MSG;
    }
    if( verifyTupleFormat(t,baseNodeData(db,fi->formatIndex),baseNodeSize(db,fi->formatIndex)/sizeof(db_tupleTYPE)) == false )
    {
        sdb_setNoSqlMsg("Given Tuple Domain Does not Match The Array Domain.");
        return nosql_ERROR_MSG;
    }
    int64_t data = db_tupleToIndex(db,t);

    fi->rows++;
    ads_listAppend(fi->ptRs,data,int64_t);

    sdb_setNoSqlMsg("New Array Row Inserted.");
    return nosql_SUCCESS_MSG;
}

nosqlRT ads_tableUpdateRow(staticDB * db,const char * tablename,int rowid,db_tuple t)
{
    if(!db)
    {
        sdb_setNoSqlMsg("Database Not Valid | Not Connected");
        return nosql_ERROR_MSG;
    }
    sdbTable * fi = fetchTable(db,tablename);
    if(!fi)
    {
        sdb_setNoSqlMsg("Array Not Present In Database.");
        return nosql_ERROR_MSG;
    }
    if( rowid >= fi->rows  || rowid <  0)
    {
        sdb_setNoSqlMsg("Row Index Is Invalid | Out Of Range.");
        return nosql_ERROR_MSG;
    }
    if( verifyTupleFormat(t,baseNodeData(db,fi->formatIndex),baseNodeSize(db,fi->formatIndex)/sizeof(db_tupleTYPE)) == false )
    {
        sdb_setNoSqlMsg("Given Tuple Domain Does not Match The Array Domain.");
        return nosql_ERROR_MSG;
    }
    int64_t data1 = 0 ;//ads_printf(" list = {list(d)} \n",fi->ptRs);
    ads_listGetItem_NoCopy(fi->ptRs,rowid,int64_t,data1);
    if(data1 <= 0)
    {
        sdb_setNoSqlMsg("Array Data Has Been Corrupted.");
        return nosql_ERROR_MSG;
    }
    updateBuffer(db,data1,t);
    sdb_setNoSqlMsg("New Array Row Updated.");
    return nosql_SUCCESS_MSG;
}

void ads_tableCommit(staticDB * db,sdbTable * fi)
{
    if(!fi)return ;
    int64_t data[fi->rows];
    int i;
    for(i=0;i<fi->rows;i++)
    {
        ads_listGetItem_NoCopy(fi->ptRs,i,int64_t,data[i]);
    }
    if(fi->rd) baseUpdate(db,fi->rd,data,sizeof(int64_t)*fi->rows);
    else fi->rd = baseInsert(db,data,sizeof(int64_t)*fi->rows);
}

void ads_tableCommitAll(staticDB * db)
{
    if(!db) return ;
    ads_avl ti = ads_dbGetTableTree(db);
    if(!ti) return ;
    ads_vector v = ads_avlTraversal( ti , ads_inOrder, ads_avlData);
    ads_foreach(t,ads_vectorIterator(v),sdbTable*)
    {
        if( t->rows && !ads_listEmpty(t->ptRs) )ads_tableCommit( db, t );
    }ads_next(t);
    ads_deleteOne(v);
}


void ads_tableRead(staticDB * db,FILE *fp,filepos pos,int no_of_tables)
{
    if(pos == 0)return;
    if( fseek(fp,pos,SEEK_SET) )
    {
        fclose(fp);
        printf("\nGIVEN DATABASE FILE IS INVALID .");
        exit(0);
    }
    ads_avl ti = ads_dbGetTableTree(db);
    int i;
    for(i=0;i<no_of_tables;i++)
    {
        sdbTable* t = (sdbTable*)ads_calloc(1,sizeof(sdbTable));
        fread(&t->rd,1,sizeof(int64_t),fp);
        fread(&t->tablename,1,sizeof(int64_t),fp);
        fread(&t->rows,1,sizeof(int),fp);
        fread(&t->formatIndex,1,sizeof(int64_t),fp);
        t->ptRs = ads_listNew(int64_t);
        ads_avlInsert(ti,ads_cstrHash(baseNodeCStr(db,t->tablename)),int64_t,t,sdbTable*);
    }
}

void ads_tableWrite(staticDB * db,FILE *fp,filepos * pos)
{
    ads_avl ti = ads_dbGetTableTree(db);
    if( ads_avlLen(ti) <= 0 )return ;
    *pos = ftell(fp);
    ads_vector v = ads_avlTraversal( ti ,ads_inOrder, ads_avlData);
    ads_foreach(t,ads_vectorIterator(v),sdbTable*)
    {
        fwrite(&t->rd,1,sizeof(int64_t),fp);
        fwrite(&t->tablename,1,sizeof(int64_t),fp);
        fwrite(&t->rows,1,sizeof(int),fp);
        fwrite(&t->formatIndex,1,sizeof(int64_t),fp);
    }ads_next(t);
    ads_deleteOne(v);
}

#endif // _SATATICTABLE_DB_C
