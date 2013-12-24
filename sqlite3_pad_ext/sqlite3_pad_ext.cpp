/*
	Sqlite Left/Right Pad Extension Functions
	
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

#define TYPE_LPAD  1
#define TYPE_RPAD  2

void lpad_func(sqlite3_context *context, int argc, sqlite3_value **argv);
void rpad_func(sqlite3_context *context, int argc, sqlite3_value **argv);
void pad_str(sqlite3_context *context, int argc, sqlite3_value **argv, int pad_type);

sqlite3_ext_func_t sqlite3_ext_func_table[] = {
	{ "lpad", 3, SQLITE_UTF8, NULL, lpad_func, NULL, NULL, NULL },
	{ "rpad", 3, SQLITE_UTF8, NULL, rpad_func, NULL, NULL, NULL },
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

void lpad_func(sqlite3_context *context, int argc, sqlite3_value **argv)
{
	pad_str(context, argc, argv, TYPE_LPAD);
}

void rpad_func(sqlite3_context *context, int argc, sqlite3_value **argv)
{
	pad_str(context, argc, argv, TYPE_RPAD);
}

void pad_str(sqlite3_context *context, int argc, sqlite3_value **argv, int pad_type)
{
	const char *val_str = (const char *)sqlite3_value_text(argv[0]);
	const char *pad_str = (const char *)sqlite3_value_text(argv[2]);
	int result_length = sqlite3_value_int(argv[1]);

	if(val_str == NULL){
		sqlite3_result_null(context);
		return;
	}

	char *result = (char *)sqlite3_malloc(result_length + 1);
	if(result == NULL){
		sqlite3_result_error_nomem(context);
		return;
	}

	if(result_length != 0){
		size_t pad_len = (pad_str != NULL) ? strlen(pad_str) : 0;
		size_t val_len = strlen(val_str);
		if(pad_type == TYPE_RPAD){
			for(int i = 0; i < result_length; i++){
				result[i] = (i < val_len) ? val_str[i] : 
					pad_str[(val_len - i) % pad_len];
			}
		}
		else if(pad_type == TYPE_LPAD){
			int m = result_length - val_len;
			for(int i = 0; i < result_length; i++){
				result[i] = (i >= m) ? val_str[i - m] : 
					pad_str[i % pad_len];
			}
		}
	}
	result[result_length] = 0;
	
	sqlite3_result_text(context, (const char *)result, result_length, sqlite3_free);
}
