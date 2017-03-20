//testlua.c
#include <stdio.h>
#include <lua.hpp>
#include <time.h>
#include <unistd.h> 


class LuaPlugin{
    public:
        LuaPlugin(){}

    public:
        register_func(const std::string& file_path, const std::string& func_name)
        {

        }

    public:
        ;

};


lua_State* L;
int add(lua_State* L);

int add(lua_State* L)
{
    int x = luaL_checkint(L,1);
    int y = luaL_checkint(L,2);
    printf("result:%d\n",x+y);
    return 1;
}


int main()
{
    L = luaL_newstate();
    luaL_openlibs(L);
    lua_pushcfunction(L, add);
    lua_setglobal(L, "ADD"); 
    if (luaL_loadfile(L, "mylua.lua"))
    {
        printf("error\n");
    }
    while(true){
        lua_pcall(L,0,0,0);
        lua_getglobal(L, "mylua"); 
        lua_pcall(L,0,0,0);

        lua_getglobal(L, "feed");
        lua_pushstring(L, "asdf");
        lua_pcall(L, 1, 1, 0);
        printf("%s\n", lua_tostring(L, -1));
        lua_pop(L, 1);
        sleep(1);
    }
    return 0;
}


