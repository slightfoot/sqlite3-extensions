/*
	Sqlite Base64 Extension Functions
	
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
#include <stdio.h>
#include "polarssl/base64.h"

typedef int (*polarssl_base64_func)(unsigned char *dst, size_t *dlen, const unsigned char *src, size_t slen);
void base64_ext_func(sqlite3_context *context, int argc, sqlite3_value **argv);
const char *base64_errstr(int code);

sqlite3_ext_func_t sqlite3_ext_func_table[] = {
	{ "base64_encode", 1, SQLITE_UTF8, (void *)base64_encode, base64_ext_func, NULL, NULL, NULL },
	{ "base64_decode", 1, SQLITE_UTF8, (void *)base64_decode, base64_ext_func, NULL, NULL, NULL },
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

void base64_ext_func(sqlite3_context *context, int argc, sqlite3_value **argv)
{
	const unsigned char *src;
	unsigned char *dst;
	size_t dlen = 0;
	size_t slen = 0;
	int errcode;

	polarssl_base64_func func = (polarssl_base64_func)sqlite3_user_data(context);

	src = (const unsigned char *)sqlite3_value_text(argv[0]);
	if(src == NULL){
		sqlite3_result_null(context);
		return;
	}
	slen = strlen((const char *)src);

	if((errcode = func(NULL, &dlen, src, slen)) != POLARSSL_ERR_BASE64_BUFFER_TOO_SMALL){
		if(errcode == 0){
			sqlite3_result_null(context);
			return;
		}
		sqlite3_result_error(context, base64_errstr(errcode), -1);
		return;
	}

	dst = (unsigned char *)sqlite3_malloc((int)dlen);
	if(dst == NULL){
		sqlite3_result_error_nomem(context);
		return;
	}

	if((errcode = func(dst, &dlen, src, slen)) != 0){
		sqlite3_result_error(context, base64_errstr(errcode), -1);
		sqlite3_free(dst);
		return;
	}

	sqlite3_result_text(context, (const char *)dst, (int)dlen, sqlite3_free);
}

const char *base64_errstr(int code)
{
	if(code == POLARSSL_ERR_BASE64_BUFFER_TOO_SMALL){
		return "Buffer too small.";
	}
	else if(code == POLARSSL_ERR_BASE64_INVALID_CHARACTER){
		return "Invalid character.";
	}
	else{
		return "Failed";
	}
}
