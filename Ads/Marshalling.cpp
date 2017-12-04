/** @cond ADS_INTERNAL */


#ifndef muster_H
#define muster_H

#include "Error.cpp"
#include "String.cpp"
#include "Dict.cpp"

typedef struct  {
	string filename;
	dict lines;
}structmuster;

typedef structmuster* muster;

dict musterload(FILE* fp);

muster newmuster(const char * filename)
{
	muster obj = (muster)mycalloc(1,sizeof(structmuster));
	FILE* fp =  fopen(filename,"wb+");
	obj->lines = musterload(fp);
	obj->filename = newstring(filename);
	fclose(fp);
	return obj;
}

void muster_add(muster self,char * key,void * object,string (musterfunction)(void*))
{
	string s = musterfunction(object);
	dict_setitem(self->lines,key,s);
	freestring(s);
}

void getmusterkey(char *c,char*ibuf)
{
	int i =-1,cou = 0;
	while(c[++i]!=NULL)
	{
		if(c[i] == '\x06')
		{
			while(c[++i]!='\x1c')
			{
				ibuf[cou++] = c[i];
			}
			ibuf[cou++] = NULL;
			return ;
		}
	}
}

void getmusterdata(char *c,char*ibuf)
{
	int i =-1,cou = 0;
	while(c[++i]!=NULL)
	{
		if(c[i] == '\x1c')
		{
			while(c[++i]!='\x06')
			{
				ibuf[cou++] = c[i];
			}
			ibuf[cou++] = NULL;
			return ;
		}
	}
}

bool detectrecordend(char*c)
{
	if(c[0] == NULL && c[1] == '\x1e' && c[2] == NULL) return true;
	return false;
}

bool detectrecordstart(char*c)
{
	if(c[0] == NULL && c[1] == '\x1d' && c[2] == NULL) return true;
	return false;
}

string getrecord(FILE*fp)
{
	char ibuf[64];int cou=0;bool cpy = false;
	memset(ibuf,0,63);
	string buffer = newstring("");
	while(!feof(fp))
	{
		if(detectrecordstart(ibuf) == true)
		{
			cpy= true;
			cou = 0;
		}
		if(cpy == true)
		{
			if(detectrecordend(&ibuf[cou-3])==true)
			{
				ibuf[cou] = NULL;
				string_concat(buffer,ibuf);
				return buffer;
			}
		}
		ibuf[cou++] = fgetc(fp);
		if(cou == 63)
		{
			ibuf[cou] = NULL;
			string_concat(buffer,ibuf);
			cou=0;
		}
	}
	freestring(buffer);
	return NULL;
}

dict musterload(FILE* fp)
{
	char ibuf[20],keybuf[BUFSIZ],data[BUFSIZ];
	string s;int len;
	fgets(ibuf,20,fp);
	fread(&len,sizeof(int),1,fp);
	dict d  = newdict(string,len+1,copystring,freestring);
	while(!feof(fp))
	{
		s = getrecord(fp);
		if(s == NULL)
		{
			return d;
		}
		getmusterkey(getcstring(s),keybuf);
		getmusterdata(getcstring(s),data);
		string_set(s,data);
		dict_setitem(d,keybuf,s);
		freestring(s);
	}
	return d;
}

void* unmuster(muster self,char * key,void*(unmusterfunction)(char*))
{
	string s = (string)dict_popitem(self->lines,key);
	void * b= unmusterfunction(getcstring(s));
	freestring(s);
	return b;
}

list muster_keys(muster self)
{
	return dict_keys(self->lines);
}

void muster_flush(muster self)
{
	dict_clear(self->lines);
}

void muster_sync(muster self)
{
	FILE *fp = fopen(getcstring(self->filename),"wb+");
	char obuf[10];
	list l = dict_keys(self->lines);
	int len = list_len(l);
	fwrite("mustring v1.00\x00\x1d\x00\n",sizeof(char),18,fp);
	fwrite(&len,sizeof(int),1,fp);
	foreach(st,list_iterator(l))
	{
		string key = (string)st;
		string s = (string)dict_getitem(self->lines,key);
		fwrite("\x00\x1d\x00\x06",sizeof(char),4,fp);
		fputs(getcstring(key),fp);
		fputs("\x1c",fp);
		fputs(getcstring(s),fp);
		//NULL x1d NULL x06 key x1c data x06 NULL x1e NULL
		fwrite("\x06\x00\x1e\x00",sizeof(char),4,fp);
		freestring(s);
	}
	fclose(fp);
	freelist(l);
}

void freemuster(muster self)
{
	freedict(self->lines);
	freestring(self->filename);
	myfree(self);
}

string musterint(void*a)
{
	return newstring("I\x2%d\x3",refitem(a,int));
}

void* unmusterint(char*buffer)
{
	int * i = (int*)mycalloc(1,sizeof(int));
	sscanf(buffer,"I\x2%d\x3",i);
	return i;
}

string musterlong(void*a)
{
	return newstring("L\x2%ld\x3",refitem(a,long long));
}

void* unmusterlong(char*buffer)
{
	long * i = (long*)mycalloc(1,sizeof(long long));
	sscanf(buffer,"L\x2%ld\x3",i);
	return i;
}

string musterfloat(void*a)
{
	return newstring("F\x2%f\x3",refitem(a,float));
}

void* unmusterfloat(char*buffer)
{
	float * i = (float*)mycalloc(1,sizeof(float));
	sscanf(buffer,"F\x2%f\x3",i);
	return i;
}

string musterdouble(void*a)
{
	return newstring("D\x2%lf\x3",refitem(a,double));
}

void* unmusterdouble(char*buffer)
{
	double * i = (double*)mycalloc(1,sizeof(double));
	sscanf(buffer,"D\x2%lf\x3",i);
	return i;
}

string musterstring(void* _self)
{
	string self = (string)_self;
	assert(self->type == "string");
	return newstring("S\x2%s\x3",getcstring(self));
}

void* unmusterstring(char*buffer)
{
	char str[BUFSIZ];int i=2,cou=0;
	while(buffer[i] != 3)
	{
		str[cou++] = buffer[i];
		i++;
	}
	str[cou++] = NULL;
	return newstring(str);
}


#endif
/** @endcond */
