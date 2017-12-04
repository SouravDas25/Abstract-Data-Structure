#ifndef VERSION_H
#define VERSION_H

	//Date Version Types
	static const char DATE[] = "04";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2017";
	static const char UBUNTU_VERSION_STYLE[] =  "17.12";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 41;
	static const long BUILD  = 134;
	static const long REVISION  = 740;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 270;
	#define RC_FILEVERSION 1,41,134,740
	#define RC_FILEVERSION_STRING "1, 41, 134, 740\0"
	static const char FULLVERSION_STRING [] = "1.41.134.740";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 92;
	

#endif //VERSION_H
