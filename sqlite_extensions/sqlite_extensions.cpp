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
#include <sqlite_extensions.h>
#include <string.h>

SQLITE_EXTENSION_INIT1

#ifndef SQLITE3_EXT_NAME
#error "You must specify a extension name in SQLITE3_EXT_NAME!"
#endif

#ifdef SQLITE3_EXT_BUILD_STATIC_LIB
#define SQLITE3_EXT_EXPORT SQLITE3_EXT_NAME
#else
#define SQLITE3_EXT_EXPORT sqlite3_extension_init
#endif

extern sqlite3_ext_func_t sqlite3_ext_func_table[];
extern int sqlite3_ext_pre_init(sqlite3 *db);
extern int sqlite3_ext_post_init(sqlite3 *db);

#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
__declspec(dllexport)
#endif

int SQLITE3_EXT_EXPORT(sqlite3 *db, char **pzErrMsg, const sqlite3_api_routines *pApi)
{
	int rc = SQLITE_OK;
	SQLITE_EXTENSION_INIT2(pApi);
	rc = sqlite3_ext_pre_init(db);
	if(rc == SQLITE_OK){
		sqlite3_ext_func_t *func = sqlite3_ext_func_table;
		while(func->zFunctionName != NULL && rc == SQLITE_OK){
			rc = sqlite3_create_function_v2(db, 
				func->zFunctionName, func->nArg, func->eTextRep, func->pApp, 
				func->xFunc, func->xStep, func->xFinal, func->xDestroy);
			func++;
		}
	}
	if(rc == SQLITE_OK){
		rc = sqlite3_ext_post_init(db);
	}
	if(rc != SQLITE_OK){
		*pzErrMsg = sqlite3_mprintf("%s", pApi->errmsg(db));
	}
	return rc;
}

#ifdef __cplusplus
}
#endif
