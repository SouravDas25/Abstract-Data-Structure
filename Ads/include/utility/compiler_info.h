#ifndef COMPILER_INFO_H
#define SOMETHING_H

#ifndef __ADS_CSTDs__

#if defined(__ANSI__)
#   define __ADS_STDC_VERSION_GET__ 80L
#elif defined(__STDC__)
#	define __ADS_STDC_VERSION_GET__ 89L
# 	if defined(__STDC_VERSION__)
#		undef __ADS_STDC_VERSION_GET__
#		define __ADS_STDC_VERSION_GET__ 90L
#  		if (__STDC_VERSION__ >= 199409L)
#			undef __ADS_STDC_VERSION_GET__
#			define __ADS_STDC_VERSION_GET__ 94L
#  		endif
#  		if (__STDC_VERSION__ >= 199901L)
#			undef __ADS_STDC_VERSION_GET__
#			define __ADS_STDC_VERSION_GET__ 99L
#  		endif
# 	endif
#else
#	define __ADS_STDC_VERSION_GET__ 99L
#endif

	#if defined(__cplusplus)
	#	undef __ADS_STDC_VERSION_GET__
	#	define __ADS_STDC_VERSION_GET__ 2011L
	#endif

#endif

#if __ADS_STDC_VERSION_GET__ < 99L
#pragma message ( "Some Feature of This Libary is only compatible with C99 and above" )
#endif

#define __ADS_VERSION__ 1.2

#endif
