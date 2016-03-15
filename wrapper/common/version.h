/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef VERSION_H_INCLUDED 
#define VERSION_H_INCLUDED
#include "git_version.h"
namespace version
{
	const char * wrapper_git_version = GIT_SHA_COMMIT;
}

#endif
