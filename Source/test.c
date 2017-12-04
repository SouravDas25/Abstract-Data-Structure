





#ifndef staticDATABASESTRING_C
#define staticDATABASESTRING_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <ads/avl.h>
#include <ads/str.h>

typedef int64_t filepos;



typedef struct _baseNode {
	int64_t addr;
	char * str;
	int len;
}baseNode;

typedef struct _stringBase {
	const char * file;
    ads_avlTree base;
    int cur_index;
}stringBase;

void cacheBase(stringBase *sb,baseNode * bn,FILE*fp);
void readBase(stringBase * sb);

stringBase* createBase(const char * file)
{
	stringBase * b = ads_new(ads_var,stringBase);
	b->base = ads_avlTreeNew(int64_t,ads_cmpInt64,baseNode*);
	b->file = file;
	readBase(b);
	ads_avlTreeGetMax(b->base,int64_t,b->cur_index);
	return b;
}

bool baseHasKey(stringBase * sb,int64_t index)
{
	bool b;
	ads_avlTreeContains(sb->base,index,int64_t,b);
	return b;
}

int64_t insertBase(stringBase * sb,const char * const str)
{
	if(!str)return -1 ;
	int64_t ret = sb->cur_index++;
	baseNode * bn = ads_new(ads_var,baseNode);
	bn->addr = 0;
	bn->len = strlen(str);
	bn->str = ads_new(ads_array,char,bn->len+1);
	strncpy(bn->str,str,bn->len);
	ads_avlTreeInsert(sb->base,ret,int64_t,bn,baseNode*);
	return ret;
}

void fetchBase(stringBase * sb,int64_t index, char * str)
{
	if(!sb)return;
	if(!str)return;
	if( index > sb->cur_index ) return ;
	if( baseHasKey(sb,index) == false )
	{
		printf("BASE INDEX NOT FOUND");
		return ;
	} 
	baseNode * bn ;
	ads_avlTreeGetItem(sb->base,index,int64_t,baseNode*,bn);
	//if(!bn)return;
	if(!bn->str)
	{
		FILE * fp = fopen(sb->file,"rb");
		cacheBase(sb,bn,fp);
		fclose(fp);
	}
	strncpy(str,bn->str,bn->len);
}

void deleteBase(stringBase * sb,int64_t index)
{
	if(!sb)return;
	if( index > sb->cur_index ) return ;
	if( baseHasKey(sb,index) == false )
	{
		printf("BASE INDEX NOT FOUND");
		return ;
	}
	baseNode * bn ;
	ads_avlTreeGetItem(sb->base,index,int64_t,baseNode*,bn);
	ads_deleteMany(2,bn->str,bn);
	ads_avlTreeDelete(sb->base,index,int64_t);
}

void commitAllBase(stringBase * sb,FILE *fp)
{
	if(!sb)return;
	ads_vector v = ads_avlTreeTraversal( sb->base , ads_inOrder, ads_avlTreeData);
    ads_foreach(bn,ads_vectorIterator(v),baseNode*)
    {
        bn->addr = ftell(fp);
        fwrite(&bn->len,sizeof(int),1,fp);
        fwrite(bn->str,sizeof(char),bn->len,fp);
    }ads_next(bn);
    ads_deleteOne(v);
}

void cacheBase(stringBase *sb,baseNode * bn,FILE*fp)
{
	if(!sb)return;
	if(!bn)return;
	if(bn->str) return;
	if(!bn->addr) return;
	if( fseek(fp,bn->addr,SEEK_SET) )
	{
		return;
	}
	fread(&bn->len,sizeof(int),1,fp);
	bn->str = ads_new(ads_array,char,bn->len+1);
	fread(bn->str,sizeof(char),bn->len,fp);
}

void cacheAllBase(stringBase * sb,FILE *fp)
{
	if(!sb)return;
	ads_vector v = ads_avlTreeTraversal( sb->base , ads_inOrder, ads_avlTreeData);
    ads_foreach(bn,ads_vectorIterator(v),baseNode*)
    {
        if(!bn->str)cacheBase(sb,bn,fp);
    }ads_next(bn);
    ads_deleteOne(v);
}

void masterBaseWrite(stringBase * sb,FILE *fp,filepos * pos)
{
	if(!sb)return;
	*pos = ftell(fp);
	ads_vector v = ads_avlTreeTraversal( sb->base , ads_inOrder, ads_avlTreeKey);
    ads_foreach(index,ads_vectorIterator(v),int64_t)
    {
        baseNode*bn;
        ads_avlTreeGetItem(sb->base,index,int64_t,baseNode*,bn);
        fwrite(&index,sizeof(int64_t),1,fp);
        fwrite(&bn->addr,sizeof(int64_t),1,fp); // bn->str
    }ads_next(index);
    ads_deleteOne(v);
}

void masterBaseRead(stringBase * sb,FILE *fp,filepos * pos,int len)
{
	if(!sb)return;
	if(*pos == 0) return ;
	if( fseek(fp,*pos,SEEK_SET))return ;
	int i;int64_t index = -1 ;baseNode * bn=NULL;
	for(i =0;i<len;i++)
	{
		bn = (baseNode*)ads_calloc( 1,sizeof(baseNode) );
		fread(&index,sizeof(int64_t),1,fp);
        fread(&bn->addr,sizeof(filepos),1,fp);
        ads_avlTreeInsert(sb->base,index,int64_t,bn,baseNode*);
	}
}

void readBase(stringBase * sb)
{
	if(!sb)return;
	FILE * fp = fopen(sb->file,"rb");
	if(fp == NULL)return;
	if( fseek(fp,4,SEEK_SET) )
	{
		fclose(fp);
		return ;
	}
	filepos st = 0;int len = 0;
	if( fseek(fp,-(4+(int)sizeof(filepos)+(int)sizeof(int)),SEEK_END) )
	{
		fclose(fp);
		return ;
	}
	fread(&st,sizeof(filepos),1,fp);
	fread(&len,sizeof(int),1,fp);
	masterBaseRead(sb,fp,&st,len);
	fclose(fp);
}

void commitBase(stringBase * sb)
{
	if(!sb)return;
	FILE * fp = fopen(sb->file,"rb");
	cacheAllBase(sb,fp);
	fclose(fp);
	fp = fopen(sb->file,"wb");
	fprintf(fp,"SIGN");
	commitAllBase(sb,fp);
	filepos st = 0;int len = ads_avlTreeLen(sb->base);
	masterBaseWrite(sb,fp,&st);
	fwrite(&st,sizeof(filepos),1,fp);
	fwrite(&len,sizeof(int),1,fp);
	fprintf(fp,"SIGN");
	fclose(fp);
}


int main()
{
	int ty = 0,index = -1;
	stringBase* b = createBase("test.sbase");
	char val[255];
	while(ty != -1)
    {
        printf("\nIS READ(0) OR WRITE(1) : ");scanf("%d",&ty);
        if(ty == 1)
        {
            strcpy(val,"Sourav Das");fflush(stdin);
            printf("\nEnter A value : ");gets(val);
            index = insertBase(b,val);
            printf("\nA base IS Written into %d index : \n",index);
        }
        else if(ty == 2)
        {
            printf("\nDeleting key from DB : \n\n");fflush(stdin);
            //printAllKeys(db,stdout);
            printf("\nEnter A Index : ");scanf("%d",&index);
            deleteBase(b,index);
        }
        else if(ty == 0)
        {

            printf("\nprinting DB : \n\n");fflush(stdin);
            //printAllKeys(db,stdout);
            printf("\nEnter A Index : ");scanf("%d",&index);
            fetchBase(b,index,val);
            printf("'%d' : '%s'",index,val);
            printf("\n\nEND Printing \n");
    	}
    	else if(ty == 3)
    	{
    		commitBase(b);
		}
    }
}




#endif // staticDATABASESTRING_C


