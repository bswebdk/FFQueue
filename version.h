#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "24";
	static const char MONTH[] = "04";
	static const char YEAR[] = "2022";
	static const char UBUNTU_VERSION_STYLE[] =  "22.04";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 7;
	static const long BUILD  = 59;
	static const long REVISION  = 284;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1786;
	#define RC_FILEVERSION 1,7,59,284
	#define RC_FILEVERSION_STRING "1, 7, 59, 284\0"
	static const char FULLVERSION_STRING [] = "1.7.59.284";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 0;
	

}
#endif //VERSION_H
