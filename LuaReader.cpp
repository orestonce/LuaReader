#include "LuaReader.h"
#include <lua.hpp>
#include <assert.h>

namespace RoTool
{

enum ELuaIndexType
{
    ELIT_INT,       // 整数类型的index
    ELIT_STRING,    // 字符串类型的index
};

class CLuaIndexHelper::_implCLuaIndexHelper
{
public :
    ELuaIndexType m_xIndexType;
    std::string   m_strIndex;
    int           m_iIndex;
public :
    _implCLuaIndexHelper(ELuaIndexType xIndexType, const std::string& strIndex, int iIndex)
    {
        m_xIndexType = xIndexType;
        m_strIndex = strIndex;
        m_iIndex = iIndex;
    }
};

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

    void Begin(int iIndex)
    {
        int cnt = lua_gettop(m_pLuaState);
        assert(cnt > 0 );

        lua_rawgeti(m_pLuaState, -1, iIndex);
    }

    void Begin(CLuaIndexHelper::_implCLuaIndexHelper *pIndex)
    {
        if ( pIndex->m_xIndexType == ELIT_STRING )
        {
            Begin(pIndex->m_strIndex.c_str());
        }
        else
        {
            Begin(pIndex->m_iIndex);
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

bool CLuaReader::Open(const std::string &strFileName)
{
    assert( m_pImpl == NULL );

    m_pImpl = new _implCLuaReader;

    if ( !m_pImpl->Open( strFileName.c_str() ) )
    {
        Close();
        return false;
    }


    return true;
}

bool CLuaReader::IsOpen() const
{
    return m_pImpl != NULL;
}

void CLuaReader::Close()
{
    if ( m_pImpl )
    {
        delete m_pImpl;
        m_pImpl = NULL;
    }
}

bool CLuaReader::GetValue(const CLuaIndexHelper &xIndex, int &iVarStore)
{
    assert(m_pImpl);

    m_pImpl->Begin( xIndex.m_pImpl );
    _implCLuaReader::CLuaEnder ender(m_pImpl);

    if ( !lua_isinteger(m_pImpl->m_pLuaState, -1) )
    {
        return false;
    }

    iVarStore = lua_tointeger(m_pImpl->m_pLuaState, -1);

    return true;
}

bool CLuaReader::GetValue(const CLuaIndexHelper &xIndex, bool &bVarStore)
{
    assert( m_pImpl );

    m_pImpl->Begin(xIndex.m_pImpl);

    _implCLuaReader::CLuaEnder ender(m_pImpl);

    if ( !lua_isboolean(m_pImpl->m_pLuaState, -1) )
    {
        return false;
    }

    bVarStore = lua_toboolean(m_pImpl->m_pLuaState, -1);

    return true;
}

bool CLuaReader::GetValue(const CLuaIndexHelper &xIndex, double &dVarStore)
{
    assert( m_pImpl );

    m_pImpl->Begin(xIndex.m_pImpl);

    _implCLuaReader::CLuaEnder ender(m_pImpl);

    if ( !lua_isnumber(m_pImpl->m_pLuaState, -1) )
    {
        return false;
    }

    dVarStore = lua_tonumber(m_pImpl->m_pLuaState, -1);

    return true;
}

bool CLuaReader::GetValue(const CLuaIndexHelper &xIndex, std::string &strVarStore)
{
    assert( m_pImpl );

    m_pImpl->Begin(xIndex.m_pImpl);
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

bool CLuaReader::EnterTable(const CLuaIndexHelper &xIndex)
{
    assert( m_pImpl );

    m_pImpl->Begin(xIndex.m_pImpl);

    if ( !lua_istable(m_pImpl->m_pLuaState, -1) )
    {
        m_pImpl->End();
        return false;
    }

    return true;
}

void CLuaReader::ExitTable()
{
    assert(m_pImpl);
    m_pImpl->End();
}

int CLuaReader::GetDepth()
{
    assert(m_pImpl);
    return lua_gettop(m_pImpl->m_pLuaState);
}

CLuaIndexHelper::CLuaIndexHelper(const char *szFieldName)
{
    assert(szFieldName);
    m_pImpl = new _implCLuaIndexHelper(ELIT_STRING, szFieldName, 0);
}

CLuaIndexHelper::CLuaIndexHelper(const std::string &strFieldName)
{
    m_pImpl = new _implCLuaIndexHelper(ELIT_STRING, strFieldName, 0);
}

CLuaIndexHelper::CLuaIndexHelper(int iFiledIndex)
{
    m_pImpl = new _implCLuaIndexHelper(ELIT_INT, "", iFiledIndex);
}

CLuaIndexHelper::~CLuaIndexHelper()
{
    delete m_pImpl;
}

}
