#include "LuaReader.h"
#include <lua.hpp>
#include <assert.h>

namespace RoTool
{

class CLuaReader::_implCLuaReader
{
public :
    lua_State *m_pLuaState;
public :
    _implCLuaReader()
    {
        m_pLuaState = luaL_newstate();
        luaL_openlibs(m_pLuaState);
        assert( m_pLuaState );
    }
    ~_implCLuaReader()
    {
        lua_close( m_pLuaState );
    }

    bool Open(const char *szFileName)
    {
        int ret = luaL_loadfile(m_pLuaState, szFileName);

        if ( ret != 0 )
        {
            return false;
        }

        int ret2 = lua_pcall(m_pLuaState, 0, 0, 0 );

        return ret2 == 0;
    }

    void Begin(const char *szVarName)
    {
        int cnt = lua_gettop(m_pLuaState);
        if ( cnt == 0 )
        {
            lua_getglobal(m_pLuaState, szVarName);
        }
        else
        {
            lua_getfield(m_pLuaState, -1, szVarName);
        }
    }

    void End()
    {
        if ( lua_gettop(m_pLuaState) != 0 )
        {
            lua_remove(m_pLuaState, -1);
        }
    }

    class CLuaEnder
    {
    public :
        CLuaEnder(_implCLuaReader *pReader)
        {
            m_pImplReader = pReader;
        }

        ~CLuaEnder()
        {
            m_pImplReader->End();
        }

    private:
        _implCLuaReader *m_pImplReader;
    };
};

CLuaReader::CLuaReader()
{
    m_pImpl = NULL;
}

CLuaReader::~CLuaReader()
{
    Close();
}

bool CLuaReader::Open(const char *szFileName)
{
    assert( m_pImpl == NULL );

    m_pImpl = new _implCLuaReader;

    if ( !m_pImpl->Open( szFileName ) )
    {
        Close();
        return false;
    }


    return true;
}

void CLuaReader::Close()
{
    if ( m_pImpl )
    {
        delete m_pImpl;
        m_pImpl = NULL;
    }
}

bool CLuaReader::GetValue(const char *szVarName, bool &bVarStore)
{
    assert( m_pImpl );

    m_pImpl->Begin(szVarName);

    _implCLuaReader::CLuaEnder ender(m_pImpl);

    if ( !lua_isboolean(m_pImpl->m_pLuaState, -1) )
    {
        return false;
    }

    bVarStore = lua_toboolean(m_pImpl->m_pLuaState, -1);

    return true;
}

bool CLuaReader::GetValue(const char *szVarName, int &bVarStore)
{
    assert( m_pImpl );

    m_pImpl->Begin(szVarName);

    _implCLuaReader::CLuaEnder ender(m_pImpl);

    if ( !lua_isinteger(m_pImpl->m_pLuaState, -1) )
    {
        return false;
    }

    bVarStore = lua_tointeger(m_pImpl->m_pLuaState, -1);

    return true;
}

bool CLuaReader::GetValue(const char *szVarName, double &dVarStore)
{
    assert( m_pImpl );

    m_pImpl->Begin(szVarName);

    _implCLuaReader::CLuaEnder ender(m_pImpl);

    if ( !lua_isnumber(m_pImpl->m_pLuaState, -1) )
    {
        return false;
    }

    dVarStore = lua_tonumber(m_pImpl->m_pLuaState, -1);

    return true;
}

bool CLuaReader::GetValue(const char *szVarName, std::string &strVarStore)
{
    assert( m_pImpl );

    m_pImpl->Begin(szVarName);
    _implCLuaReader::CLuaEnder ender(m_pImpl);

    if ( !lua_isstring(m_pImpl->m_pLuaState, -1) )
    {
        return false;
    }

    size_t len = -1;

    const char *xStart = lua_tolstring(m_pImpl->m_pLuaState, -1, &len);

    strVarStore.assign(xStart, xStart + len);

    return true;
}

bool CLuaReader::EnterTable(const char *szTableName)
{
    assert( m_pImpl );

    m_pImpl->Begin(szTableName);

    if ( !lua_istable(m_pImpl->m_pLuaState, -1) )
    {
        m_pImpl->End();
        return false;
    }

    return true;
}

void CLuaReader::ExitTable()
{
    m_pImpl->End();
}

int CLuaReader::GetDepth()
{
    return lua_gettop(m_pImpl->m_pLuaState);
}

}
