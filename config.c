#include "config.h"
#include "kmalloc.h"
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"

#include <memory.h>

#define CONFIG_SET_STRING(attr, key)                    \
do {                                                    \
    lua_getglobal(L, "getXTableElement");               \
    lua_pushstring(L, config_file);                     \
    lua_pushstring(L, (key));                           \
    lua_pcall( L, 2, 1, LUA_FUNCTION_RETURN_SUCCESS );  \
    buff = lua_tostring(L, LUA_STACK_TOP);              \
    buff_len = strlen(buff) + 1;                        \
    config->attr = (const char *)kmalloc(buff_len);     \
    memcpy((void *)config->attr, buff, buff_len);       \
    lua_pop(L, 1);                                      \
} while(0)

#define CONFIG_SET_INTEGER(attr, key)                   \
do {                                                    \
    lua_getglobal(L, "getXTableElement");               \
    lua_pushstring(L, config_file);                     \
    lua_pushstring(L, (key));                           \
    lua_pcall( L, 2, 1, LUA_FUNCTION_RETURN_SUCCESS );  \
    config->attr = lua_tointeger(L, LUA_STACK_TOP);     \
    lua_pop(L, 1);                                      \
} while(0)

void read_config(config_t * config, const char * config_file)
{
    if (config) {
        memset(config, 0, sizeof(config_t));

        lua_State * L = luaL_newstate();

        if (L) {
            luaL_openlibs(L);

            //加载配置解析脚本
            luaL_dofile(L, "./lua/func.luac");

            const char * buff;
            size_t buff_len;

            //读取配置文件
            CONFIG_SET_STRING(pem_file, "/pem_file");
            CONFIG_SET_STRING(pass_phrase, "/pass_phrase");
            CONFIG_SET_STRING(apns_push_host, "/apns_push_host");
            CONFIG_SET_STRING(test_devtoken, "/test_devtoken");
            CONFIG_SET_STRING(test_msg, "/test_msg");
            CONFIG_SET_INTEGER(apns_push_port, "/apns_push_port");
            CONFIG_SET_INTEGER(apns_connect_timeout, "/apns_connect_timeout");
            CONFIG_SET_INTEGER(apns_batch_send_count, "/apns_batch_send_count");
            CONFIG_SET_INTEGER(apns_batch_buffer, "/apns_batch_buffer");
            CONFIG_SET_INTEGER(apns_send_total, "/apns_send_total");

            lua_close(L);
        }
    }
}

void free_config(config_t * config)
{
    if (config) {
        kfree((void *)config->apns_push_host);
        kfree((void *)config->pass_phrase);
        kfree((void *)config->pem_file);
        kfree((void *)config->test_devtoken);
        kfree((void *)config->test_msg);

        config->apns_push_host = NULL;
        config->pass_phrase = NULL;
        config->pem_file = NULL;
    }
}

