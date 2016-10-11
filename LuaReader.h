#pragma once

#include <string>

namespace RoTool
{

class CLuaReader
{
public:
    CLuaReader();
    ~CLuaReader();

    /**
     * @brief Open 打开并加载一个lua文件
     * @param szFileName 文件名
     * @return true -> 加载成功, false -> 加载失败
     */
    bool Open(const char *szFileName);

    /**
     * @brief Close 关闭当前打开的lua文件
     */
    void Close();

    /**
     * @brief GetValue 获取某个字段的值, 取决于你现在所处于的位置
     * @param varName 字段名
     * @param var 储存字段
     * @return true -> 获取成功, false -> 获取失败
     */
    bool GetValue(const char *szVarName, int    &iVarStore);
    bool GetValue(const char *szVarName, bool   &bVarStore);
    bool GetValue(const char *szVarName, double &dVarStore);
    bool GetValue(const char *szVarName, std::string &strVarStore);

    /**
     * @brief EnterTable 尝试进入一个table
     * @param szTableName table的名字
     * @return true -> 进入成功, false -> 进入失败
     */
    bool EnterTable(const char *szTableName);

    /**
     * @brief ExitTable 从table中退出来
     */
    void ExitTable();

    /**
     * @brief GetDepth 查询当前进入几层table
     * @return 进入了table的层数; 如果没有进入table, 则返回0
     */
    int  GetDepth();
private:
    /**
      * 不允许被复制
     */
    CLuaReader& operator= (const CLuaReader&) { return *this; }
    CLuaReader(const CLuaReader&) {}
private:
    class _implCLuaReader;
    _implCLuaReader *m_pImpl;
};

}
