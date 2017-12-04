#ifndef MARSHAL_H_INCLUDED
#define MARSHAL_H_INCLUDED


#include "database/dataBase.h"
#include "database/parser.h"

typedef struct _SDB SDB;
typedef struct _SDB StaticDB;

struct _SDB {
    staticDB * db;
    nosqlRT (*connect)(SDB* obj,const char * file,const char * passwrd);
    nosqlRT (*execute)(SDB* obj,const char * str);
    nosqlRT (*disconnect)(SDB* obj);
};

extern const SDB new_SDB;

#endif // MARSHAL_H_INCLUDED
