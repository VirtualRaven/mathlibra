#ifndef VERSION_H_INCLUDED 
#define VERSION_H_INCLUDED
#include "git_version.h"
namespace version
{
	const char * wrapper_git_version = GIT_SHA_COMMIT;
}

#endif