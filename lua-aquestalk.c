#ifdef WIN32
#define LUA_BUILD_AS_DLL
#define LUA_LIB
#endif

#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif
#include <lauxlib.h>
#include <lua.h>
#include <AquesTalk.h>
#ifdef __cplusplus
}
#endif

#if LUA_VERSION_NUM >= 502
#define lua_setfenv         lua_setuservalue
#define lua_getfenv         lua_getuservalue
#endif

int lua_aquestalk_synth(lua_State* L) {
    const char *text = luaL_checkstring(L, 1);

    int bas = (int)luaL_optinteger(L, 2, F1E);
    int spd = (int)luaL_optinteger(L, 3, 100);
    int vol = (int)luaL_optinteger(L, 4, 100);
    int pit = (int)luaL_optinteger(L, 5, 100);
    int acc = (int)luaL_optinteger(L, 6, 100);
    int lmd = (int)luaL_optinteger(L, 7, 100);
    int fsc = (int)luaL_optinteger(L, 8, 100);

    AQTK_VOICE voice = {
        bas = bas,
        spd = spd,
        vol = vol,
        pit = pit,
        acc = acc,
        lmd = lmd,
        fsc = fsc
    };

    int size;
    char *wav = (char*)AquesTalk_Synthe_Utf8(&voice, text, &size);
    if (wav == 0) {
        lua_pushnil(L);
        lua_pushinteger(L, size);
        return 2;
    }

    lua_pushlstring(L, wav, size);
    AquesTalk_FreeWave((unsigned char*)wav);
    return 1;
}

int lua_aquestalk_set_dev_key(lua_State* L) {
    const char *key = luaL_checkstring(L, 1);

    int result = AquesTalk_SetDevKey(key);
    lua_pushinteger(L, result);
    return 1;
}

int lua_aquestalk_set_usr_key(lua_State* L) {
    const char *key = luaL_checkstring(L, 1);

    int result = AquesTalk_SetUsrKey(key);
    lua_pushinteger(L, result);
    return 1;
}

static luaL_Reg lua_lib_aquestalk[] = {
    { "set_dev_key", lua_aquestalk_set_dev_key },
    { "set_usr_key", lua_aquestalk_set_usr_key },
    { "synth", lua_aquestalk_synth },
    { NULL, NULL }
};

#ifdef __cplusplus
extern "C" {
#endif
    LUALIB_API int luaopen_lua_aquestalk(lua_State* L) {
        lua_newtable(L);
        luaL_register(L, NULL, lua_lib_aquestalk);

        lua_newtable(L);
        lua_pushinteger(L, F1E);
        lua_setfield(L, -2, "F1E");
        lua_pushinteger(L, F2E);
        lua_setfield(L, -2, "F2E");
        lua_pushinteger(L, M1E);
        lua_setfield(L, -2, "M1E");
        lua_setfield(L, -2, "VoiceBase");

        return 1;
    }
#ifdef __cplusplus
}
#endif
