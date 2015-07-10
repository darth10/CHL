#include "cgi.h"
#include "../error/error.h"
#include "../types.h"

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define GET_DATA_LIM (QUERY_NAME_LIM + QUERY_VALUE_LIM) * GET_LIM
#define POST_DATA_LIM (QUERY_NAME_LIM + QUERY_VALUE_LIM) * POST_LIM
#define METHOD_POST 1
#define METHOD_GET 0

Host _host;
Request _request;

QUERY_ITEM POST[POST_LIM];
QUERY_ITEM GET[POST_LIM];

static t_INDEX post_index = 0;
static t_INDEX get_index = 0;

static void init_host();
static void init_request();
static char * get_env(char * var);
static void init_get();
static void init_post();
static void set_query_item(char * str, short method);

void cgi_init() {
	init_host();
	init_request();
	init_get();
	init_post();
}

static void init_host() {
	_host.SERVER = get_env("SERVER_SOFTWARE");
	_host.PORT = get_env("SERVER_PORT");
	_host.NAME = get_env("SERVER_NAME");
	_host.EMAIL = get_env("SERVER_ADMIN");
	_host.SCRIPT_PATH = get_env("SCRIPT_NAME");
	_host.SCRIPT_REL_PATH = get_env("SCRIPT_FILENAME");
	_host.SYS_PATH = get_env("PATH");
	_host.ROOT = get_env("DOCUMENT_ROOT");
}

static void init_request() {
	_request.HOST = get_env("HTTP_HOST");
	_request.REFERER = get_env("HTTP_REFERER");
	_request.USER_AGENT = get_env("HTTP_USER_AGENT");
	_request.HTTPS = get_env("HTTPS");
	_request.IP = get_env("REMOTE_ADDR");
	_request.PORT = get_env("REMOTE_PORT");
	_request.USERNAME = get_env("REMOTE_USER");
	_request.METHOD = get_env("REQUEST_METHOD");
	_request.URI = get_env("REQUEST_URI");
}

static void init_get() {
	char get_data[GET_DATA_LIM] = {0};
	char * get;
	char * token;

	// Needed for strtok_r to suppress error
	get = get_data;

	strncpy(get_data, get_env("QUERY_STRING"), GET_DATA_LIM);
	((token = strtok_r(get, "&", &get)) == NULL) ? (token = get) : 0;

	do {
		set_query_item(token, METHOD_GET);
		token = strtok_r(NULL, "&", &get);
	}
	while(token != NULL);
}

// Initialize post array
static void init_post() {
	char post_data[POST_DATA_LIM] = {0};
	char * post;
	char * token;

	// Needed for strtok_r to suppress error
	post = post_data;

	fgets(post_data, POST_DATA_LIM, stdin);
	((token = strtok_r(post, "&", &post)) == NULL) ? (token = post) : 0;

	do {
		set_query_item(token, METHOD_POST);
		token = strtok_r(NULL, "&", &post);
	}
	while(token != NULL);
}

// Add element to array of method
static void set_query_item(char * str, short method) {
	char * name;
	char * value;

	name = strtok(str, "=");
	if((value = strtok(NULL, "=")) == NULL)
		value = "\0";

	if(strlen(str) == 0)
		return;

	if(get_index > GET_LIM) {
		set_errno(ERRNO_GET_OVERFLOW, NULL);
		return;
	}

	if(post_index > POST_LIM) {
		set_errno(ERRNO_POST_OVERFLOW, NULL);
		return;
	}

	switch(method) {
		case METHOD_GET:
			strncpy(GET[get_index].name, name, QUERY_NAME_LIM);
			strncpy(GET[get_index++].value, value, QUERY_VALUE_LIM);

			break;

		case METHOD_POST:
			strncpy(POST[post_index].name, name, QUERY_NAME_LIM);
			strncpy(POST[post_index++].value, value, QUERY_VALUE_LIM);

			break;
	}
}

// Get environment variable
static char * get_env(char * var) {
	char * val;

	if((val = getenv(var)) != NULL)
		return val;

	val = "\0";
	return val;
}
