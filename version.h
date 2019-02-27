#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "24";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2019";
	static const char UBUNTU_VERSION_STYLE[] =  "19.02";
	
	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";
	
	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 7;
	static const long BUILD  = 51;
	static const long REVISION  = 259;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1729;
	#define RC_FILEVERSION 1,7,51,259
	#define RC_FILEVERSION_STRING "1, 7, 51, 259\0"
	static const char FULLVERSION_STRING [] = "1.7.51.259";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 0;
	

}
#endif //VERSION_H
