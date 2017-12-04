

#include "collection\IString.c"
#include "collection\Marshalling.cpp"

int main()
{
	string s = newstring("Hi How R u\\n \\t \\x25");
	string s1 = newstring("Sourav Das");
	
	muster f = newmuster("buffers\\out.in");
	
	muster_add(f,"s",s,musterstring);
	
	muster_sync(f);
	freemuster(f);
	
	freestring(s);
	freestring(s1);
	
	Garbagecheck();
}
