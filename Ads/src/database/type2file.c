
#ifndef staticDATABASET2F_C
#define staticDATABASET2F_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "../../include/database/type2file.h"


typedef struct _storeStruct {
    union {
        bool b;
        int64_t i;
        double d;
    };
    int64_t key;
    db_tupleTYPE t;
}storeStruct;

bool verifyTupleFormat(db_tuple t , const db_tupleTYPE * format,int len)
{
    int i;
    if( db_tupleLen(t) < len) return false;
    for(i=0;i<len;i++)
    {
        if(format[i] != db_tupleGetItem_Type(t,i))return false;
    }
    return true;
}

int tupleSize(int size)
{
    return size/sizeof(storeStruct);
}

int ads_bufferSize(int len)
{
    return len*sizeof(storeStruct);
}

int ads_getBufferSize(db_tuple t)
{
    return db_tupleLen(t)*sizeof(storeStruct);
}

static void db_tupleToBuffer(staticDB *db,storeStruct *data,db_tuple t)
{
    if(db_tupleLen(t)<=0) return ;
    int i; // db_tuplePrint(t,1)
    for(i=0;i<db_tupleLen(t);i++)
    {
        data[i].t = db_tupleGetItem_Type(t,i);
        data[i].key = (db_tupleHasKey(t,i))?baseInsertCStr(db,db_tupleGetKey(t,i)):0;
        switch(data[i].t)
        {
            case db_INT: {
                data[i].i = db_tupleGetItem_Int(t,i);
                break;
            }
            case db_STRING: {
                data[i].i = baseInsertCStr(db,db_tupleGetItem_String(t,i));
                break;
            }
            case db_DOUBLE: {
                data[i].d = db_tupleGetItem_Double(t,i);
                break;
            }
            case db_NULL : {
                data[i].i = 0;
                break;
            }
            case db_BOOL : {
                data[i].b = db_tupleGetItem_Bool(t,i);
                break;
            }
            case db_TUPLE : {
                data[i].i = db_tupleToIndex(db,db_tupleGetItem_Tuple(t,i));
                break;
            }
        }
    }
}

int64_t db_tupleToIndex(staticDB *db,db_tuple t)
{
    storeStruct data[db_tupleLen(t)];
    memset(data,0,sizeof(data));
    db_tupleToBuffer(db,data,t);
    return baseInsert(db,data,sizeof(data));
}

bool updateBuffer(staticDB *db, int64_t index,db_tuple t)
{
    storeStruct data[db_tupleLen(t)];
    memset(data,0,sizeof(data));
    db_tupleToBuffer(db,data,t);
    return baseUpdate(db,index,data,sizeof(data));
}

bool extendBuffer(staticDB *db, int64_t index,db_tuple t)
{
    storeStruct data[db_tupleLen(t)];
    memset(data,0,sizeof(data));
    db_tupleToBuffer(db,data,t);
    return baseExtend(db,index,data,sizeof(data));
}

db_tuple indexToDB_tuple(staticDB *db, int64_t index )
{
    int i,len = tupleSize(baseNodeSize(db,index));
    db_tuple t = db_tupleNew(len);
    storeStruct * data = (storeStruct*)baseNodeData(db,index);
    for(i=0;i<len;i++)
    {
        storeStruct s = data[i] ;
        const char * key = baseNodeCStr(db,s.key);
        switch(s.t)
        {
            case db_INT: {
                db_tupleAppend_Int(t,key,s.i);
                break;
            }
            case db_STRING: {
                db_tupleAppend_String(t,key,baseNodeCStr(db,s.i));
                break;
            }
            case db_DOUBLE: {
                db_tupleAppend_Double(t,key,s.d);
                break;
            }
            case db_NULL : {
                db_tupleAppend_Null(t,key);
                break;
            }
            case db_BOOL : {
                db_tupleAppend_Bool(t,key,s.b);
                break;
            }
            case db_TUPLE : {
                db_tupleAppend_Tuple(t,key,indexToDB_tuple(db,s.i));
                break;
            }
        }
    }
    return t;
}

int deleteBuffer(staticDB *db,int64_t index)
{
    int i,len = tupleSize(baseNodeSize(db,index));
    storeStruct * data = (storeStruct*)baseNodeData(db,index);
    for(i=0;i<len;i++)
    {
        storeStruct s = data[i] ;
        baseDelete(db,s.key);
        switch(s.t)
        {
            case db_INT: {
                break;
            }
            case db_STRING: {
                baseDelete(db,s.i);
                break;
            }
            case db_DOUBLE: {
                break;
            }
            case db_NULL : {
                break;
            }
            case db_BOOL : {
                break;
            }
            case db_TUPLE : {
                deleteBuffer(db,s.i);
                break;
            }
        }
    }
    baseDelete(db,index);
    return true;
}

db_tuple getTupIndexAsTup(db_tuple t,int i)
{
    if(i == -1 )return NULL;
    db_tuple nt = db_tupleNew(1);
    switch(db_tupleGetItem_Type(t,i))
    {
        case db_INT: {
            db_tupleAppend_Int(nt,db_tupleGetKey(t,i),db_tupleGetItem_Int(t,i));
            break;
        }
        case db_STRING: {
            db_tupleAppend_String(nt,db_tupleGetKey(t,i),db_tupleGetItem_String(t,i));
            break;
        }
        case db_DOUBLE: {
            db_tupleAppend_Double(nt,db_tupleGetKey(t,i),db_tupleGetItem_Double(t,i));
            break;
        }
        case db_NULL : {
            db_tupleAppend_Null(nt,db_tupleGetKey(t,i));
            break;
        }
        case db_BOOL : {
            db_tupleAppend_Bool(nt,db_tupleGetKey(t,i),db_tupleGetItem_Bool(t,i));
            break;
        }
        case db_TUPLE : {
            db_tuple tmp = db_tupleCopy(db_tupleGetItem_Tuple(t,i));
            db_tupleAppend_Tuple(nt,db_tupleGetKey(t,i),tmp);
            break;
        }
    }
    return nt;
}



db_tuple splitKey(const char * key)
{
    ads_vector v = ads_cstrSplit(key,".[]");
    db_tuple t = db_tupleNew(ads_vectorLen(v));
    int i;
    for(i = 0 ; i < ads_vectorLen(v) ; i++)
    {
        ads_str tms;
        ads_vectorGetItem_NoCopy(v,i,ads_str,tms);
        if( ads_strIsInteger(tms) == true ) db_tupleAppend_Int(t,NULL,ads_strToInt64(tms,10));
        else db_tupleAppend_String(t,NULL,ads_strGetCstr(tms));
    }
    ads_deleteOne(v);
    return t;
}

db_tuple traverseTuple(db_tuple kt,db_tuple t,int start)
{
    if(db_tupleLen(kt) < start)
    {
        return NULL;
    }
    if( db_tupleLen(kt)  == start)
    {
        return db_tupleCopy(t);
    }
    db_tuple lt = t,plt;
    int i,ind = -1;
    for(i = start ; i < db_tupleLen(kt) ; i++)
    {
        if( db_tupleGetItem_Type(kt,i) == db_INT ) ind = db_tupleGetItem_Int(kt,i);
        else ind = db_tupleGetIndex(lt,db_tupleGetItem_String(kt,i));
        plt = lt;
        if(ind == -1 || ind > db_tupleLen(kt)) return NULL;
        if( db_tupleGetItem_Type(lt,ind) != db_TUPLE && i < db_tupleLen(kt) - 1 ) return NULL;
        lt = db_tupleGetItem_Tuple(lt,ind);//db_tuplePrint(plt,1)
    }
    return getTupIndexAsTup(plt,ind);;
}

/*
#undef MIN_IORow


#define MIN_IORow(type,buffer,writef,readf,bufferoffset,va_l,tuple)\
                    if(RoW == READ)\
                    {\
                        type * s = va_arg(va_l,type*);\
                        *s = (type)readf(buffer);\
                    }\
                    else if(RoW == WRITE) \
                    {\
                        type* s = va_arg(va_l,type*);\
                        writef(buffer,*s);\
                    }\
                    else if(RoW == TUPLE)\
                    {\
                        type s ;\
                        s = (type)readf(buffer);\
                        ads_tupleAppend(tuple,s,type)\
                    }\
                    buffer += bufferoffset;

ads_tuple ads_valistToBuffer_WriteByRef(int RoW,staticDB *db, void * data ,const char * format,int len , va_list* v)
{
    int i,formattype,p=0;
    ads_tuple t = (RoW == TUPLE)?ads_tupleNew(getFormatCount(format,len)):NULL;
    const char * cmd = NULL;
    for(i=0;i<len;i++)
    {
        formattype = ads_isValidFormatting(format,&i);
        if(formattype)
        {
            p = i;
            if(formattype == 1) cmd = ads_getCMD(format,&i);
			else cmd = ads_getOldCMD(format,&i);

			ads_typeCode tc = ads_typeCodeOne(cmd,i-p);

            switch(tc)
            {
                case CHAR_TC: {
                    MIN_IORow(int,data,writeChar,readChar,sDB_CharSize,*v,t)
                    break;
                }
                case INT_TC: {
                    MIN_IORow(int,data,writeInt,readInt,sDB_IntSize,*v,t)
                    break;
                }
                case INT32_TC: {
                    MIN_IORow(int32_t,data,writeInt,readInt,sDB_IntSize,*v,t)
                    break;
                }
                case INT64_TC: {
                    MIN_IORow(int64_t,data,writeInt,readInt,sDB_IntSize,*v,t)
                    break;
                }
                case FLOAT_TC:
                case DOUBLE_TC: {
                    MIN_IORow(float64_t,data,writeDouble,readDouble,sDB_FloatSize,*v,t)
                    break;
                }
                case STRING_TC : {
                    int cn = getFormatVarCount(cmd,i-p);
                    if(RoW == READ)
                    {
                        char * s = va_arg(*v,char*);
                        readString(db,data,s,cn);
                    }
                    else if(RoW == WRITE)
                    {
                        char** s = va_arg(*v,char**);
                        writeString(db,data,*s,cn);
                    }
                    else if(RoW == DELETE)
                    {
                        deleteString(db,data);
                    }
                    else if(RoW == TUPLE)
                    {
                        const char * c = readStringForTuple(db,data);
                        ads_tupleAppendCStr(t,c);
                    }
                    data += sDB_IntSize;
                    break;
                }
                case ZBUFFER_TC : {
                    int cn = getFormatVarCount(cmd,i-p);
                    if(RoW == READ)
                    {
                        void * s = va_arg(*v,void*);
                        readBlob(data,s,cn);
                    }
                    else if(RoW == WRITE)
                    {
                        void * s = va_arg(*v,void*);
                        writeBlob(data,s,cn);
                    }
                    else if(RoW == TUPLE)
                    {
                        ads_tupleAppend_ByRef(t,data,cn);
                    }
                    data += cn;
                    break;
                }
                default :
                    printf("\nFormat not Found %c",format[i+1]);
                    break;
            }
        }
    }
    return t;
}

#undef MIN_IORow

#define SwitchCaseTypeIO(type,count,wf,rf,buffer,bufferinc) \
				if(once) \
				{ \
	        		*value += sizeof(type)*count; \
	        		once = false; \
				}\
	            if(RoW == WRITE) \
				{\
					for(p=0;p<count;p++) \
					{\
		            	wf(buffer,*(type*)val);\
		            	val += sizeof(type);\
		            	buffer += bufferinc; \
					}\
				}\
				else \
				{ \
					for(p=0;p<count;p++) \
					{\
						*(type*)val = rf(buffer); \
						val += sizeof(type);\
						buffer += bufferinc; \
					}\
				}


static void recognizeAndIO(void * data,char * format,int len,void ** value,bool RoW)
{
    int i,p;void * val = *value;
	bool once = true;
    long l = getFormatVarCount(format,len);
    for(i=0;i<len;i++)
    {
        switch(format[i])
        {
        	case '*' :
			{
        		if(once)
				{
	        		*value = ((char*)*value)+sizeof(void*);
	        		once = false;
				}
	            val = *(void**)val;
				break;
			}
        	case 'd' :
        	{
        		SwitchCaseTypeIO(int,l,writeInt,readInt,data,sDB_IntSize)
        		break;
			}
			case 'c' :
        	{
        		SwitchCaseTypeIO(char,l,writeChar,readChar,data,sDB_CharSize)
        		break;
			}
			case 'f' :
        	{
        		SwitchCaseTypeIO(double,l,writeDouble,readDouble,data,sDB_FloatSize)
        		break;
			}
        }
    }
}

#undef SwitchCaseTypeIO

void writeStruct(void * data,void * value,char * format)
{
    int i, len = strlen(format),p = 0;
    for(i=0;i<=len;i++)
    {
        if(format[i] == ',' || format[i] == '\0')
        {
            recognizeAndIO(data,&format[p],i-p,&value,WRITE);
            p = i+1;
        }
    }
}

void readStruct(void * data,void * value,char * format)
{
	int i, len = strlen(format),p = 0;
    for(i=0;i<=len;i++)
    {
        if(format[i] == ',' || format[i] == '\0')
        {
            recognizeAndIO(data,&format[p],i-p,&value,READ);
            p = i+1;
        }
    }
}
*/
/*

typedef struct {
	char c[30];
}std;

int main()
{

	FILE * fp;
	std s ;
	char * format = "%c[30]";
	int ty;


	strcpy(s.c,"Sourav Das");

	int size = ads_getFormatSize(format,strlen(format));
	char buf[size];
	memset(buf,0,size);

	printf("IS READ(0) OR WRITE(1) : ");scanf("%d",&ty);

	if(ty)
	{
		fp = fopen("test.txt","wb");
		writeStruct(buf,&s,format);
		fwrite(buf,1,size,fp);
		fclose(fp);
	}
	else
	{
		strcpy(s.c,"Changed");
		fp = fopen("test.txt","rb");
		fread(buf,1,size,fp);
		readStruct(buf,&s,format);
		fclose(fp);
	}

	printf("\n %s,%d",s.c,size);

}
*/


#endif // staticDATABASET2F_C
