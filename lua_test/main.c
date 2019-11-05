

#include "iterate_table.h"

#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>

#include <stdio.h>

#define uchar(c)	((unsigned char)(c))

int lupper(lua_State*);

static int lupper(lua_State* L) {
	size_t l;
	size_t i;
	luaL_Buffer b;
	const char* s = luaL_checklstring(L, 1, &l);
	char* p = luaL_buffinitsize(L, &b, l);
	for (i = 0; i < l; i++)
		p[i] = toupper(uchar(s[i]));
	luaL_pushresultsize(&b, l);
	return 1;
}

int new_panic(lua_State* L) {
	const char* out = luaL_checklstring(L, -1, NULL);
	printf("error: %s", out);
	exit(-1);
}

static void test_lua_buffer() {
	lua_State* L = luaL_newstate();
	lua_atpanic(L, new_panic);
	luaL_checkversion(L);
	lua_Number* version = lua_version(L);
	lua_register(L, "to_upper", lupper);
	int res = luaL_dostring(L, "params = {...} \r\n upper_name = to_upper(\"name\") \r\n name = \"liliang\" \r\n map = {\"name\"}");
	if (res == LUA_OK) {
		lua_getglobal(L, "upper_name");
		const char* out = luaL_checklstring(L, -1, NULL);
		printf("out = %s\n", out);
		lua_getglobal(L, "map");
		int t = lua_gettop(L);
		if (lua_istable(L, -1)) {
			iterate_table_and_print(L, -1);
		}
		else if (lua_isstring(L,-1)) {
			printf("name = %s", luaL_checklstring(L, -1, NULL));
		}
	}
	printf("res = %d\n", res);
	lua_close(L);
}

int main(int argc, char* argv[]) {
	test_lua_buffer();
	return 0;
}