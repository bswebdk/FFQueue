#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "13";
	static const char MONTH[] = "12";
	static const char YEAR[] = "2023";
	static const char UBUNTU_VERSION_STYLE[] =  "23.12";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 7;
	static const long BUILD  = 64;
	static const long REVISION  = 310;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1792;
	#define RC_FILEVERSION 1,7,64,310
	#define RC_FILEVERSION_STRING "1, 7, 64, 310\0"
	static const char FULLVERSION_STRING [] = "1.7.64.310";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 0;
	

}
#endif //VERSION_H
