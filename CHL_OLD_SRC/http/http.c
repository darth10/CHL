#include "http.h"
#include "../core/cgi.h"
#include "../core/types.h"
#include "../error/error.h"

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>

typedef struct {
	char header[HEADER_HEADER_LIM];
	char value[HEADER_VALUE_LIM];
} Header;

static Header HEADERS[HEADERS_LIM];
static t_INDEX headers_index = 0;
static t_STATE headers_state = 0;

// Output headers
void headers() {
	if(headers_state)
		return;

	int i;

	for(i = 0; i < headers_index; i++) {
		if(i == (headers_index - 1)) {
			printf("%s: %s\n\n", HEADERS[i].header, HEADERS[i].value);
			break;
		}

		printf("%s: %s\n", HEADERS[i].header, HEADERS[i].value);
	}

	headers_state = 1;
}

// Initialize standard headers
void headers_init() {
	set_header("Content-Type", "text/html");
	set_header("X-Powered-By", "CHL v.01 PRE ALPHA");
}

// Get header value
char * get_header(char * header) {
	int i;
	for(i = 0; (i < headers_index) && strncmp(header, HEADERS[i].header, HEADER_HEADER_LIM); i++);

	if(i == headers_index)
		return NULL;

	return HEADERS[i].value;

}

// Set header value
void set_header(char * header, char * value) {
	int i;

	if(headers_index > HEADERS_LIM) {
		SET_ERROR("Headers buffer overflow.");
		return;
	}

	for(i = 0; i < headers_index; i++) {
		if(! strncmp(header, HEADERS[i].header, HEADER_HEADER_LIM)) {
			strncpy(HEADERS[i].value, value, HEADER_VALUE_LIM);
			break;
		}
	}

	if(i == headers_index) {
		strncpy(HEADERS[headers_index].header, header, HEADER_HEADER_LIM);
		strncpy(HEADERS[headers_index++].value, value, HEADER_VALUE_LIM);
	}
}

// Set header, create a new one if already exists
void set_header_new(char * header, char * value) {
	if(headers_index > HEADERS_LIM) {
		SET_ERROR("Headers buffer overflow.");
		return;
	}

	strncpy(HEADERS[headers_index].header, header, HEADER_HEADER_LIM);
	strncpy(HEADERS[headers_index++].value, value, HEADER_VALUE_LIM);
}

// Get POST data
char * post(char * name) {
	if(! post_state)
		init_post();

	int i;

	for(i = 0; (i < post_index) && strncmp(name, POST[i].name, QUERY_NAME_LIM); i++);

	if(i == post_index)
		return NULL;

	return POST[i].value;
}

// Get GET data
char * get(char * name) {
	if(! get_state)
		init_get();

	int i;

	for(i = 0; (i < get_index) && strncmp(name, GET[i].name, QUERY_NAME_LIM); i++);

	if(i == get_index)
		return NULL;

	return GET[i].value;
}

// Get size of GET array
int get_size() {
	return get_index;
}

// Get size of POST array
int post_size() {
	return post_index;
}

// Decode URL
void url_decode(char * str) {
	char * decoded;
	decoded = str;

	if(*str == '\0')
		return;

	while(*str != '\0') {
		if((*str == '%') && (isxdigit(str[1]) && isxdigit(str[2]))) {
			{
				if(str[1] >= 'a')
					str[1] -= ('a' - 'A');
				else if(str[1] >= 'A')
					str[1] -= ('A' - 10);
				else
					str[1] -= '0';
			}

			{
				if(str[2] >= 'a')
					str[2] -= ('a' - 'A');
				else if(str[2] >= 'A')
					str[2] -= ('A' - 10);
				else
					str[2] -= '0';
			}

			*decoded++ = (16 * str[1]) + str[2];
			str += 3;
		}

		else
			*decoded++ = *str++;
	}

	*decoded++ = '\0';
}

// Convert string to hex form
int strtohex(char * str) {
	char * pt;
	return (int) strtol(str, &pt, 16);
}


