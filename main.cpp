#include <iostream>
#include "LuaReader.h"

void Output(bool b, int i, double d, std::string const& str)
{
    std::cout << std::boolalpha << b << std::endl;
    std::cout << i << std::endl;
    std::cout << d << std::endl;
    std::cout << str << std::endl;
}

void ReadByIntIndex(RoTool::CLuaReader& reader)
{
    bool        b   = false;
    int         i   = 0;
    double      d   = 0;
    std::string str = "";

    reader.GetValue(1, b);
    reader.GetValue(2, i);
    reader.GetValue(3, d);
    reader.GetValue(4, str);

    Output(b, i, d, str);
}

void TestDefault(RoTool::CLuaReader& reader)
{
    bool        b   = false;
    int         i   = 0;
    double      d   = 0;
    std::string str = "";

    LUAREADER_GETVALUE_DEFAULT(reader, "default_test", b, true);
    LUAREADER_GETVALUE_DEFAULT(reader, "default_test", i, 10);
    LUAREADER_GETVALUE_DEFAULT(reader, "default_test", d, 10.1);
    LUAREADER_GETVALUE_DEFAULT(reader, "default_test", str, "default string");

    Output(b, i, d, str);
}

void ReadByStringIndex(RoTool::CLuaReader& reader)
{
    bool        b   = false;
    int         i   = 0;
    double      d   = 0;
    std::string str = "";

    reader.GetValue("bool", b);
    reader.GetValue("int", i);
    reader.GetValue("double", d);
    reader.GetValue("string", str);

    Output(b, i, d, str);
}

int main(int , char *[])
{
    RoTool::CLuaReader reader;

    if ( reader.Open("E:/test.lua") )
    {
        std::cout << "Open lua : " << std::boolalpha << reader.IsOpen() << std::endl;
        TestDefault(reader);
        ReadByStringIndex(reader);

        if ( reader.EnterTable("table") )
        {
            TestDefault(reader);
            ReadByStringIndex(reader);
            ReadByIntIndex(reader);

            if ( reader.EnterTable(std::string("table2") ) )
            {
                TestDefault(reader);
                ReadByIntIndex(reader);
                ReadByStringIndex(reader);

                reader.ExitTable();
            }

            reader.ExitTable();
        }

        if ( reader.EnterTable("table3") )
        {
            TestDefault(reader);
            if ( reader.EnterTable(1) )
            {
                TestDefault(reader);
                ReadByIntIndex(reader);
                ReadByStringIndex(reader);

                reader.ExitTable();
            }

            if ( reader.EnterTable(2) )
            {
                TestDefault(reader);
                ReadByIntIndex(reader);
                ReadByStringIndex(reader);

                reader.ExitTable();
            }

            reader.ExitTable();
        }

        TestDefault(reader);

        reader.Close();
    }
    else
    {
        std::cout << "Open lua : " << std::boolalpha << reader.IsOpen() << std::endl;
    }

    return 0;
}
