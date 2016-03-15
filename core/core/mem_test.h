/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */

namespace debug
{
	void check_tree_mem_leak(); /**< Uses util::coubtable to detect memory leaks. It Checks if the total number of allocated mathnode objects corresponds to the total deletion count. If check fails it frows an exception.**/
        bool mem_error_occured() noexcept;
}

