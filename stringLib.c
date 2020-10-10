
#include "stringLib.h"

string stringJoin(string a, string b) { //eqivalent to a + b in some other languages;
	string c;
	c.len = a.len + b.len;
	c.data = malloc(c.len + 1);
	if (!c.data) {
		c.len = 1;
		return c;
	};
	memcpy(c.data, a.data, a.len);
	memcpy(c.data + a.len, b.data, b.len);
	c.data[c.len] = '\0';
	return c;
};

int stringExtend(string *a, string b) {
	string c;
	c.len = a->len + b.len;
	c.data = realloc(a->data, c.len + 1);
	if (!c.data)
		return 1;
	memcpy(c.data + a->len, b.data, b.len);
	c.data[c.len] = '\0';
	*a = c;
	return 0;
};

string stringCopy(string str) {
	string out;
	out.data = malloc(str.len + 1);
	if (!out.data) return (string) { .data = NULL, .len = 1 };
	out.len = str.len;
	memcpy(out.data, str.data, out.len);
	out.data[out.len] = '\0';
	return out;
};

void stringDel(string str) {
	free(str.data);
};

string stringFromBuff(char *buff) {
	string str;
	str.data = buff;
	str.len = strlen(buff);
	return str;
};

string stringFromBuffcspn(char *buff, const char *ctrl) {
	string str;
	str.data = buff;
	str.len = strcspn(buff, ctrl);
	return str;
};

string stringFromBuffspn(char *buff, const char *ctrl) {
	string str;
	str.data = buff;
	str.len = strspn(buff, ctrl);
	return str;
};

//hash function
/* from python:
def __hash__(self):
	value = 0x345678
	for item in self:
		value = c_mul(1000003, value) ^ hash(item)
	value = value ^ len(self)
	if value == -1:
		value = -2
	return value
*/

hash_t stringHash(string *str) {
	hash_t value = 0x345678;
	for (char *c = str->data, *end = str->data + str->len; c != end; c++) {
		value = (1000003 * value) ^ *c;
	};
	value ^= str->len;
	return value;
};

int stringEq(string *a, string *b) {
	if (a == b)
		return 1;
	if (a->len != b->len)
		return 0;
	if (!a->len) return 1; //ensure we don't pass (NULL, 0) strings to memcmp
	return !memcmp(a->data, b->data, a->len);
};

int stringMatch(string *str, string pat) {
	char *s, *e;
	s = e = pat.data;
	size_t sLen = pat.len;
	size_t subLen;
	int ind = 0;
	while (sLen) {
		e = memchr(s, '|', sLen);
		if (e)
			subLen = e - s;
		else
			subLen = sLen++;
		if (str->len >= subLen && memcmp(str->data, s, subLen) == 0) {
			str->data += subLen;
			str->len -= subLen;
			return ind;
		};
		ind++;
		subLen++;
		s += subLen;
		sLen -= subLen;
	};
	return -1;
};

int stringIndex(string str, char chr) {
	char *temp = memchr(str.data, chr, str.len);
	if (!temp)
		return -1;
	return temp - str.data;
};

int stringContains(string str, char chr) {
	char *temp = memchr(str.data, chr, str.len);
	if (!temp)
		return 0;
	return 1;
};

uint stringCount(string str, char chr) {
	uint cnt = 0;
	for (uint i = 0; i < str.len; i++) if (str.data[i] == chr) cnt++;
	return cnt;
};

string stringSkipPast(string *str, char *control) {
	size_t len = strspn(str->data, control);
	if (len > str->len) {
		len = str->len;
	};
	string out = { .data = str->data,.len = len };
	str->data += len;
	str->len -= len;
	return out;
};

string stringSkipTo(string *str, char *control) {
	size_t len = strcspn(str->data, control);
	if (len > str->len) {
		len = str->len;
	};
	string out = { .data = str->data,.len = len };
	str->data += len;
	str->len -= len;
	return out;
};
