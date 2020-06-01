#pragma once

#include <list>
#include <string>
#include <utility>
#include <fstream>
#include <iostream>
#include <boost/filesystem.hpp>
#include "pcout.h"

namespace fs = boost::filesystem;

namespace mapping
{
    std::pair<bool,std::list<std::string>> mapping( const std::string& file_name,const size_t min,const size_t offset, size_t lenght)
    {
        std::list<std::string> result;
        std::ifstream fread (file_name);
        if (!fread.is_open())
        {
            std::cout << "ERR open file\n";
            return std::make_pair(0,result);
        }

        fread.seekg(offset,std::ios::beg);
        char temp;
        size_t count = min;
        std::string str;
        std::vector<bool> max_lenght;
        for(size_t i = 0; i < lenght; ++i)
        {
            fread.get(temp);
            if(temp == '\n') 
            {
                result.emplace_back(str);
                if(count) max_lenght.emplace_back(true);
                else max_lenght.emplace_back(false);
                count = min;
                str.erase();
            }
            else
            {
                if(count > 0)
                {
                    str+=temp;
                    --count;
                }
            }
        }
        if(!str.empty())
        {
            result.emplace_back(str);
                if(count) max_lenght.emplace_back(true);
                else max_lenght.emplace_back(false);
        }
        result.sort();

        bool all_string = true;
        for(const auto& i : max_lenght)
        {
            if(!i) all_string = false;
        }

        return std::make_pair(all_string,result);
    }


    std::vector<std::pair<size_t,size_t>> getBorders(const std::string& name_file,const size_t m)
    {
        std::vector<size_t> result;
        size_t file_size = fs::file_size(fs::path(name_file));
        size_t part_size = file_size / m;
        
        std::ifstream fread (name_file,std::ios::in);
        if (fread.is_open())
        {
            const char delim = '\n';
            result.push_back(0);
            for(size_t i = 1;i < m;++i)
            {
                fread.seekg(part_size*i,std::ios::beg);
                std::size_t lenght = fread.tellg();
                char buff;
                for(size_t l = lenght;l >0 ;--l)
                {
                    fread.seekg(-1,std::ios::cur);
                    fread.get(buff);
                    if(buff == delim) break;
                    fread.seekg(-1,std::ios::cur);
                }
                size_t left_border = fread.tellg();

                while (fread.get(buff))
                {
                    if(buff == delim) break;
                }
                size_t right_border = fread.tellg();

                fread.clear();

                if(part_size*i - left_border < right_border - part_size*i)
                {
                    result.push_back(left_border);
                }
                else
                {
                    result.push_back(right_border);
                }        
            }
        }
        fread.close();
        result.push_back(file_size);
        std::vector<std::pair<size_t,size_t>> offset_size;
        std::sort(std::begin(result),std::end(result));
        result.resize(std::unique(std::begin(result),std::end(result))-std::begin(result));
        for (size_t i = 0; i < result.size()-1;++i)
        {
        offset_size.emplace_back(std::make_pair(result.at(i),result.at(i+1) - result.at(i)));
        }

        return offset_size;
    }
}