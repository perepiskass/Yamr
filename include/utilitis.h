#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <boost/filesystem.hpp>
#include <fstream>
#include <utility>
#include <set>
#include <memory>


namespace fs = boost::filesystem; 
// std::vector<size_t> result;

std::vector<std::pair<size_t,size_t>> getBorders(const std::string& name_file,size_t m)
{
    std::vector<size_t> result;
    // result.reserve(m*2);
        size_t file_size = fs::file_size(fs::path(name_file));
        std::cout << "file size - " << file_size << std::endl;
        size_t part_size = file_size / m;
        std::cout << "part_size - " << part_size << std::endl;
       
    std::ifstream fread (name_file,std::ios::in);
    if (fread.is_open())
    {
        const char delim = '\n';
        result.push_back(0);
        for(size_t i = 1;i < m;++i)
        {
            std::cout << "CICLE - " << i << std::endl;
            char* buffer = new char(file_size+1);

            fread.seekg(part_size*i,std::ios::beg);
            std::size_t lenght = fread.tellg();
            std::cout << lenght << std::endl;
            char buff;
            for(;lenght >0 ;--lenght)
            {
                fread.seekg(-1,std::ios::cur);
                fread.get(buff);
                if(buff == delim) break;
                fread.seekg(-1,std::ios::cur);
                // std::cout << buff << ' ';
            }
            size_t first_border = fread.tellg();

            char b;
            fread.get(b);
            std::cout << "first - "<< b << ' ' << first_border << std::endl;
            fread.seekg(-1,std::ios::cur);

            fread.getline(buffer,file_size);
            size_t second_border = fread.tellg();

            if(second_border > file_size)
            {
                delete buffer;
                result.push_back(first_border);
                break;
            }

            fread.seekg(second_border,std::ios::beg);
            fread.get(b);
            std::cout << "second - " << b << ' ' << second_border << std::endl;
            fread.clear();

            if(part_size*i - first_border < second_border - part_size*i)
            {
                result.push_back(first_border);
                std::cout << "first to vector" << std::endl;
            }
            else
            {
                result.push_back(second_border);
                std::cout<< "second to vector" << std::endl;
            }        

            delete buffer;
        }
    }
    std::cout << "EXIT FROM CICLE\n";
    result.push_back(file_size);
    std::cout << "After push back\n";
    std::vector<std::pair<size_t,size_t>> offset_size;
    // offset_size.reserve(m+1);
    // offset_size.reserve(m);
    for (size_t i = 0;i<result.capacity()-1;++i)
    {
      offset_size.emplace_back(std::make_pair(result.at(i),result.at(i+1) - result.at(i)));
    //   offset_size.emplace_back(std::make_pair(1,2));
    }

    return offset_size;
}

