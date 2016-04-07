/* Copyright (C) 2016 Lukas Rahmn - All Rights Reserved
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/. */
#ifndef TYPE_ENUM_H_INCLUDED
#define TYPE_ENUM_H_INCLUDED

enum storage_types
{
	T_UNKN, /**< Storage type unknowns */
	T_DOUBLE, /**< stores doubles */
	T_CHAR, /**< Stores chars */
	T_TYPE, /**< The type stores more type objects*/
	T_FUNC /**< The type is a lamda function */
};

#endif
