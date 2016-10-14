#pragma once

#include <string>

namespace RoTool
{

class CLuaIndexHelper;

class CLuaReader
{
public:
    CLuaReader();
    ~CLuaReader();

    /**
     * @brief Open 打开并加载一个lua文件 [需要IsOpen() == false]
     * @param strFileName 文件名
     * @return true -> 加载成功, false -> 加载失败
     */
    bool Open(const std::string& strFileName);

    /**
     * @brief IsOpen 判断是否打开某个lua文件
     * @return true -> 已打开, false -> 已关闭
     */
    bool IsOpen() const;

    /**
     * @brief Close 关闭当前打开的lua文件
     */
    void Close();

    /**
     * @brief GetValue 获取某个字段的值, 取决于你现在所处于的位置 [需要IsOpen() == true]
     * @param xIndex 字段索引, 支持int和string
     * @param var 储存字段值
     * @return true -> 获取成功, false -> 获取失败
     */
    bool GetValue(const CLuaIndexHelper& xIndex, int    &iVarStore);
    bool GetValue(const CLuaIndexHelper& xIndex, bool   &bVarStore);
    bool GetValue(const CLuaIndexHelper& xIndex, double &dVarStore);
    bool GetValue(const CLuaIndexHelper& xIndex, std::string &strVarStore);

    /**
     * @brief EnterTable 尝试进入一个table [需要IsOpen() == true]
     * @param szTableName table的名字
     * @return true -> 进入成功, false -> 进入失败
     */
    bool EnterTable(const CLuaIndexHelper& xIndex);

    /**
     * @brief ExitTable 从table中退出来 [需要IsOpen() == true && GetDepth() > 0 ]
     */
    void ExitTable();

    /**
     * @brief GetDepth 查询当前进入几层table [需要IsOpen() == true]
     * @return 进入了table的层数 ; 如果没有进入table, 则返回0 [返回值 >= 0 ]
     */
    int  GetDepth();
private:
    /**
      * 不允许被复制
     */
    CLuaReader& operator= (const CLuaReader&) { return *this; }
    CLuaReader(const CLuaReader&) {}
private:
    friend class CLuaIndexHelper;
    class _implCLuaReader;
    _implCLuaReader *m_pImpl;
};

/**
 * @brief The CLuaIndexHelper class 在使用int为index的时候需要注意, 目前状态 GetDepth > 0
 */
class CLuaIndexHelper
{
public :
    CLuaIndexHelper(const char *szFieldName);
    CLuaIndexHelper(const std::string& strFieldName);
    CLuaIndexHelper(int iFiledIndex);
    ~CLuaIndexHelper();
private:
    /**
      * 不允许被复制
     */
    CLuaIndexHelper& operator= (const CLuaIndexHelper&) { return *this; }
    CLuaIndexHelper(const CLuaIndexHelper&) {}
private:
    friend class CLuaReader;
    class _implCLuaIndexHelper;
    _implCLuaIndexHelper *m_pImpl;
};

#define LUAREADER_GETVALUE_DEFAULT(r, idx, store, def) \
    if ( r.GetValue(idx, store) == false ) \
        store = def;
}
