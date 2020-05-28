#pragma once

#include <list>
#include <string>
#include <utility>
#include <fstream>
#include <iostream>


std::list<std::string> mapping( const std::string& file_name,size_t min, size_t offset, size_t lenght)
{
    std::list<std::string> result;
    std::ifstream fread (file_name);
    if (!fread.is_open())
    {
        std::cout << "ERR open file\n";
        return result;
    }

    // char temp[lenght+1];
    fread.seekg(offset,std::ios::beg);
    size_t start = fread.tellg();
    size_t cur = start;
    while (cur - start < lenght)
    {    
        char* temp = new char[lenght+1];
        fread.read(temp,lenght);
        auto cur = fread.tellg();
        // std::string str{temp,min};
        std::cout << temp << std::endl;
        result.push_back(std::string(temp,min));
        cur = fread.tellg();
        delete[] temp;
    }

    result.sort();

    return result;
}
