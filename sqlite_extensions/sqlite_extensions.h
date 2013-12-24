/*
	Sqlite Extensions Helper
	
	Copyright 2013 Simon Lightfoot
	
	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at
	
		http://www.apache.org/licenses/LICENSE-2.0
	
	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/
#include <sqlite3ext.h>

SQLITE_EXTENSION_INIT3

#ifndef SQLITE3_EXTENSIONS
#define SQLITE3_EXTENSIONS

#define _SQLITE3_EXT_EXPAND_STR(s) #s
#define _SQLITE3_EXT_EXPAND_XSTR(s) _SQLITE3_EXT_EXPAND_STR(s)

#define SQLITE3_EXT_NAME_STR _SQLITE3_EXT_EXPAND_XSTR(SQLITE3_EXT_NAME)

typedef struct sqlite3_ext_func_t {
	const char *zFunctionName;
	int nArg;
	int eTextRep;
	void *pApp;
	void (*xFunc)(sqlite3_context*,int,sqlite3_value**);
	void (*xStep)(sqlite3_context*,int,sqlite3_value**);
	void (*xFinal)(sqlite3_context*);
	void(*xDestroy)(void*);
} sqlite3_ext_func_t;

#endif // SQLITE3_EXTENSIONS

