#include <iostream>
#include "LuaReader.h"

int main(int , char *[])
{
    RoTool::CLuaReader reader;

    if ( reader.Open("./test.lua") )
    {
        int x = 0;
        if ( reader.GetValue("x2", x) )
        {
            std::cout << __LINE__ << " "<< x << reader.GetDepth() << std::endl;
        }

        std::string str ;

        if ( reader.GetValue("count", str) )
        {
            std::cout << __LINE__ << " " << "Getstring ok : " << str << std::endl;
            str.clear();
        }

        if ( reader.EnterTable("user_table") )
        {
            if ( reader.GetValue("u", str) )
            {
                std::cout << __LINE__ << " " << "depth string : " << str << std::endl;
            }

            if ( reader.EnterTable("user2") )
            {
                if ( reader.GetValue("u2", x) )
                {
                    std::cout << __LINE__ << " " << "int : " << x << std::endl;
                }

                bool b = true;

                if ( reader.GetValue("u4", b) )
                {
                    std::cout << __LINE__ << " " << "bool : " << b << std::endl;
                }

                reader.ExitTable();
            }

            reader.ExitTable();
        }

        if ( reader.GetValue("count2", str) )
        {
            std::cout << __LINE__ << " " << "Getstring ok : " << str << std::endl;
        }
    }
    else
    {
        std::cout << __LINE__ << " " << "Open lua file error !" << std::endl;
    }

    return 0;
}
