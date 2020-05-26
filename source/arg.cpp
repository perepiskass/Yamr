#include "version_lib.h"
#include <iostream>
#include <string>
#include <utility>
#include <boost/filesystem.hpp>

namespace fs = boost::filesystem;
/**
 * @brief Функция для получения введенных аргументов программы порта и размера bulk.
 */
auto getArg(int argc,char** argv)
{
    if ( argc > 1)
    {
        std::string version = "-version";
        if (argv[1] == version)
        {
            std::cout << "version: " << version_major()<< '.'<< version_minor() << '.' << version_patch() << std::endl;
            exit(0);
        }
        else
        {
            std::pair<std::string,std::pair<size_t,size_t>> result;
            result.first = argv[1];
            result.second.first = atoi(argv[2]);
            result.second.second = atoi(argv[3]);
            if (result.second.first > 0 && result.second.second > 0 && fs::exists(fs::path(result.first))) 
            {
                return result;
            } 
            else  
            {

                std::cout << "Enter correct file name or m/r size"<< std::endl;
                exit(0);
            }
        }
    }
    else 
    {
      std::cout << "Usage: yamr <file_name> <m count> <r count> or -version" << std::endl;
      exit(0);
    }
    
}