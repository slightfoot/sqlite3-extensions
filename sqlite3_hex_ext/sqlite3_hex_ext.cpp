/*
	Sqlite Hex Extension Functions
	
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
#include <stdlib.h>
#include <stdio.h>


void tohex  (sqlite3_context *context, int argc, sqlite3_value **argv);
void fromhex(sqlite3_context *context, int argc, sqlite3_value **argv);
void fromhexstr(sqlite3_context *context, int argc, sqlite3_value **argv);

sqlite3_ext_func_t sqlite3_ext_func_table[] = {
	{ "tohex",      -1, SQLITE_UTF8, NULL, tohex,      NULL, NULL, NULL },
	{ "fromhex",     1, SQLITE_UTF8, NULL, fromhex,    NULL, NULL, NULL },
	{ "fromhexstr",  1, SQLITE_UTF8, NULL, fromhexstr, NULL, NULL, NULL },
	NULL
};


int sqlite3_ext_pre_init(sqlite3 *db)
{
	return SQLITE_OK;
}

int sqlite3_ext_post_init(sqlite3 *db)
{
	printf(SQLITE3_EXT_NAME_STR " loaded.\n");
	return SQLITE_OK;
}

void tohex(sqlite3_context *context, int argc, sqlite3_value **argv)
{
	char buf[24];
	if(sqlite3_value_type(argv[0]) == SQLITE_NULL){
		sqlite3_result_null(context);
		return;
	}
	sprintf(buf, ((argc > 1) ? sqlite3_value_int(argv[1]) : 0) ? 
		"%llX" : "%llx", sqlite3_value_int64(argv[0]));
	sqlite3_result_text(context, buf, -1, SQLITE_TRANSIENT);
}

void fromhex(sqlite3_context *context, int argc, sqlite3_value **argv)
{
	const char *val = (const char *)((argc > 0) ? 
		sqlite3_value_text(argv[0]) : NULL);
	if(val == NULL){
		sqlite3_result_null(context);
		return;
	}
	sqlite3_result_int64(context, strtoll(val, NULL, 16));
}

static char hexbyte(char hex)
{
	static char lookup[] = { "0123456789abcdefABCDEF" };
	for(int i = 0; i < sizeof(lookup); i++){
		if(hex == lookup[i])
			return (char)((i > 15) ? i - 6 : i);
	}
	return 0;
}

void fromhexstr(sqlite3_context *context, int argc, sqlite3_value **argv)
{
	const char *val = (const char *)((argc > 0) ? 
		sqlite3_value_text(argv[0]) : NULL);
	if(val == NULL){
		sqlite3_result_null(context);
		return;
	}
	const int len = strlen(val) / 2;
	char *result = (char *)sqlite3_malloc(len + 1);
	int i = 0;
	for(int j = 0; i < len; j+= 2){
		result[i++] = hexbyte(val[j + 0]) << 4 | hexbyte(val[j + 1]);
	}
	sqlite3_result_text(context, result, i, sqlite3_free);
}
