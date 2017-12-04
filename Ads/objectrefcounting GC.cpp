/** @cond ADS_INTERNAL */




#ifndef object_H
#define object_H

#include "Error.cpp"

typedef struct {
	void * ref;
	unsigned short refcount;
	void* (*copyfunction)(void*) ;
    void (*deletefunction)(void*) ;
} structobject;

typedef structobject* object;

object newobject(void* obj,void * (*copyfunction)(void*) = NULL,void(*deletefunction)(void*) = myfree)
{
	object newobj = (object)mycalloc(1,sizeof(structobject));
	newobj->ref = obj;
	newobj->refcount = 1;
	return newobj;
}

object copyobject(object self)
{
	if(self->refcount == 0)
	{
		MemoryError("Cannot Copy NULL object in copyobjectfunction");
	}
	self->refcount++;
	return self;
}

object deepcopyobject(object self)
{
	void * n = self->copyfunction(self->ref);
	object o = newobject(n,self->copyfunction,self->deletefunction);
	return o;
}

void freeobject(object self)
{
	self->refcount--;
	if(self->refcount == 0)
	{
		self->deletefunction(self->ref);
	}
	myfree(self);
}

#endif
/** @endcond */
