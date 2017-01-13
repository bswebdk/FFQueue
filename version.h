#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "09";
	static const char MONTH[] = "01";
	static const char YEAR[] = "2017";
	static const char UBUNTU_VERSION_STYLE[] =  "17.01";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 7;
	static const long BUILD  = 48;
	static const long REVISION  = 247;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1696;
	#define RC_FILEVERSION 1,7,48,247
	#define RC_FILEVERSION_STRING "1, 7, 48, 247\0"
	static const char FULLVERSION_STRING [] = "1.7.48.247";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 0;
	

}
#endif //VERSION_H
