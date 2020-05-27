#pragma once

#include <list>
#include <string>
#include <utility>
#include <fstream>
#include <iostream>


std::list<std::string> mapping(size_t min, size_t offset, size_t lenght, const std::string& file_name)
{
    std::list<std::string> result;
    std::ifstream fread (file_name);
    if (!fread.is_open())
    {
        std::cout << "ERR open file\n";
        return result;
    }

    char temp[lenght+1];
    fread.seekg(offset,std::ios::beg);
    auto start = fread.tellg();
    auto cur = start;
    while (cur - start <= lenght)
    {    
        fread.getline(temp,lenght);
        auto cur = fread.tellg();
        result.push_back(std::string(temp,min));
        cur = fread.tellg();
    }

    return result;
}