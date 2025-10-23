#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{

	//Date Version Types
	static const char DATE[] = "23";
	static const char MONTH[] = "10";
	static const char YEAR[] = "2025";
	static const char UBUNTU_VERSION_STYLE[] =  "25.10";

	//Software Status
	static const char STATUS[] =  "Alpha";
	static const char STATUS_SHORT[] =  "a";

	//Standard Version Type
	static const long MAJOR  = 1;
	static const long MINOR  = 7;
	static const long BUILD  = 70;
	static const long REVISION  = 345;

	//Miscellaneous Version Types
	static const long BUILDS_COUNT  = 1850;
	#define RC_FILEVERSION 1,7,70,345
	#define RC_FILEVERSION_STRING "1, 7, 70, 345\0"
	static const char FULLVERSION_STRING [] = "1.7.70.345";

	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY  = 0;

}
#endif //VERSION_H
