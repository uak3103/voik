#ifndef VERSION_H
#define VERSION_H

namespace AutoVersion{
	
	//Date Version Types
	static const char DATE[] = "04";
	static const char MONTH[] = "02";
	static const char YEAR[] = "2013";
	static const char UBUNTU_VERSION_STYLE[] = "13.02";
	
	//Software Status
	static const char STATUS[] = "Beta";
	static const char STATUS_SHORT[] = "b";
	
	//Standard Version Type
	static const long MAJOR = 2;
	static const long MINOR = 6;
	static const long BUILD = 1763;
	static const long REVISION = 9543;
	
	//Miscellaneous Version Types
	static const long BUILDS_COUNT = 2768;
	#define RC_FILEVERSION 2,6,1763,9543
	#define RC_FILEVERSION_STRING "2, 6, 1763, 9543\0"
	static const char FULLVERSION_STRING[] = "2.6.1763.9543";
	
	//These values are to keep track of your versioning state, don't modify them.
	static const long BUILD_HISTORY = 63;
	

}
#endif //VERSION_H
