#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "25";
	static const char MONTH[] = "08";
	static const char YEAR[] = "2023";
	static const char UBUNTU_VERSION_STYLE[] =  "23.08";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 7;
	static const long BUILD  = 62;
	static const long REVISION  = 301;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1787;
	#define RC_FILEVERSION 1,7,62,301
	#define RC_FILEVERSION_STRING "1, 7, 62, 301\0"
	static const char FULLVERSION_STRING [] = "1.7.62.301";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 0;
	

}
#endif //VERSION_H
