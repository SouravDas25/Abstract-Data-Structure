


#include "../include/marshal.h"

static nosqlRT connect(SDB* obj,const char * file,const char * passwrd)
{
    if(obj->db != NULL)
    {
        sdb_setNoSqlMsg("Database Not Initialized With 'new_SDB' |  Database Already Connected.");
        return nosql_ERROR_MSG;
    }
    return ads_connectDB(&obj->db,file,passwrd);
}

static nosqlRT execute(SDB* obj,const char * str)
{
    if(obj->db == NULL)
    {
        sdb_setNoSqlMsg("Database Not Connected");
        return nosql_ERROR_MSG;
    }
    return sdb_parse(obj->db,str);
}

static nosqlRT disconnect(SDB* obj)
{
    if(obj->db == NULL)
    {
        sdb_setNoSqlMsg("Database Not Connected");
        return nosql_ERROR_MSG;
    }
    ads_disconnectDB( obj->db );
    obj->db = NULL;
    sdb_setNoSqlMsg("Database Successfully Disconnected.");
    return nosql_SUCCESS_MSG;
}

const SDB new_SDB = {NULL,connect,execute,disconnect};

#define helpMsg \
"Each Statement Has The Same Format - (Command DataType Identifier : Arguments) \n" \
"Commands : \n"\
"+ CREATE - Takes A Tuple\n"\
"    - Key   ( CREATE KEY keyname : {1,'Dummy',3.235} ) \n"\
"    - Array ( CREATE ARRAY arrayname : int,string,int) \n"\
"+ GET - Array Index Or Key Field\n"\
"    - Key   ( GET KEY keyname.childname[index] ) \n"\
"    - Array ( GET ARRAY arrayname ) \n"\
"    - Array ( GET ARRAY arrayname[rowid] ) \n"\
"+ DELETE - No Arguments\n" \
"    - Key   ( DELETE KEY keyname ) \n"\
"    - Array ( DELETE ARRAY arrayname ) \n"\
"    - Array ( DELETE ARRAY arrayname : 0  ) \n"\
"+ INSERT - Takes A Tuple \n"\
"    - Key   ( INSERT KEY keyname : {1,'Dummy',3.235} ) \n"\
"    - Array ( INSERT ARRAY arrayname : {1,'Dummy',3.235} ) \n"\
"+ UPDATE - Takes A Integer And A Tuple \n"\
"    - Key   ( No Use ) \n"\
"    - Array ( UPDATE ARRAY arrayname : 0 -> {1,2,3} ) \n"\
"- COMMIT - Only Command\n"\
"- HELP   - Only Command\n"\
"- QUIT   - Only Command\n"\



nosqlRT sdb_execute(staticDB * db,proc p)
{
    sdb_setNoSqlMsg("NoSQL Statement Not Recognized.");
	nosqlRT r = nosql_ERROR_MSG;
	if(p.cmd == nosql_CREATE)
	{
		if(p.o == nosql_KEY)
		{
			db_tuple t = db_tupleLiteral(p.res);
			#ifdef debug
			db_tuplePrint(t,1);
			#endif // debug
			r = ads_pairCreate(db,p.key,t);
            db_tupleDelete(t);
		}
		else if(p.o == nosql_ARRAY)
		{
		    r = ads_createTable(db,p.key,p.res);
		}
	}
	else if(p.cmd == nosql_DELETE)
	{
		if(p.o == nosql_KEY)
		{
		    r = ads_pairDelete(db,p.key);
		}
		else if(p.o == nosql_ARRAY)
		{
		    r = ads_tableDelete(db,p.key);
		}
	}
	else if(p.cmd == nosql_UPDATE)
	{
		if(p.o == nosql_KEY)
		{

		}
		else if(p.o == nosql_ARRAY)
		{
		    int ty = -1;
		    sscanf(p.res,"%d",&ty);
		    db_tuple t = db_tupleLiteral(p.res);
		    r = ads_tableUpdateRow(db,p.key,ty,t);
		    db_tupleDelete(t);
		}
	}
	else if(p.cmd == nosql_INSERT)
	{
		if(p.o == nosql_KEY)
		{
		    db_tuple t = db_tupleLiteral(p.res);
		    r = pairInsert(db,p.key,t);
		    #ifdef debug
		    db_tuplePrint(t,1);
		    #endif // debug
		    db_tupleDelete(t);
		}
		else if(p.o == nosql_ARRAY)
		{
		    db_tuple t = db_tupleLiteral(p.res);
		    r = ads_tableInsetRow(db,p.key,t);
		    #ifdef debug
		    db_tuplePrint(t,1);
		    #endif // debug
		    db_tupleDelete(t);
		}
	}
	else if(p.cmd == nosql_GET)
	{
		if(p.o == nosql_KEY)
		{
		    r = ads_pairFetch(db,p.key);
		}
		else if(p.o == nosql_ARRAY)
		{
		    r = ads_tableFetch(db,p.key);
		}
		if(p.o == nosql_KEYLIST)
        {
            r = ads_pairKeys(db);

        }
        if(p.o == nosql_ARRAYLIST)
        {
            r = ads_tableNames(db);
        }
	}
	else if(p.cmd == nosql_COMMIT)
	{
        ads_dbCommit(db);
        sdb_setNoSqlMsg("Database Successfully Committed.");
        r = nosql_SUCCESS_MSG;
	}
	else if(p.cmd == nosql_QUIT)
	{
        sdb_setNoSqlMsg("Database Not Disconnected Should Be Disconnected Manually.");
        r = nosql_QUIT_MSG;
	}
	else if(p.cmd == nosql_HELP)
	{
        sdb_setNoSqlMsg(helpMsg);
        r = nosql_SUCCESS_MSG;
	}
	return r;
}

#undef helpMsg
