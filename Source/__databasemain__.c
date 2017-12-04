

#include "dataBase.h"

int main()
{
    int a =0,b=0,i,ty;
    staticDB * db = ConnectDatabase("testdb.sdb");
    //printDB(db);
    char val[512];
    char key[50];
    memset(val,0,30);
    char format[] = "%s[30]";


    //CreateTable(db,"lol",format);
    do
    {
        printf("\nIS READ(0) OR WRITE(1) : ");scanf("%d",&ty);
        if(ty == 1)
        {
            strcpy(val,"Sourav Das");fflush(stdin);
            printf("\nEnter A Key : ");gets(key);
            printf("\nEnter A value : ");gets(val);
            insertPair(db,key,format,val);
            printf("\nA Pair IS Written into DB : \n");
        }
        else if(ty == 2)
        {
            printf("\Deleting key from DB : \n\n");fflush(stdin);
            printAllKeys(db,stdout);
            printf("\nEnter A Key : ");gets(key);
            deletePair(db,key);
        }
        else if(ty == 0)
        {

            printf("\nprinting DB : \n\n");fflush(stdin);
            printAllKeys(db,stdout);
            printf("\nEnter A Key : ");gets(key);
            fetchPair(db,key,val);
            printf("'%s' : '%s'",key,val);
            printf("\n\nEND Printing \n");
        }
    }while(ty!=-1);


    Commit(db);
    return 0;
}

