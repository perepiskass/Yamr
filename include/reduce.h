#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <algorithm>
#include <map>

namespace reduce
{   
    /**
     * @brief Функция подсчета одинаковых экземпляров в заданном списке результат записывается в файл(при условии что нет дубликатов строк в исходном файле)
     * @return Возвращает true - если есть копии в списке и false если копий в обрабатываемом диапазоне нет
     */
    bool reduce(const std::list<std::string>& input,const bool all_string,const size_t number_thread,const size_t offset,const size_t lenght)
    {
        bool copies = false;
        auto it = input.begin();
        std::advance(it,offset);
        std::map<std::string,size_t> result;
        /// Считаем колличество одинаковых строк в заданном диапазоне
        for(size_t i = 0; i < lenght; ++i)
        {
            if(result.find(*it)!=result.end())
            {
                copies = true;
                ++result[*it];
            }
            else result[*it] = 1;
            ++it;
        }

        /// Записываем данные в файл, при условии, что копий не найдено или если найдены то они являються задублированными строками в исходнике
        if((copies == true && all_string == true) ||  copies == false)
        {
            std::string filename = "reduce " + std::to_string(number_thread);
            std::fstream file(filename,std::ios::out | std::ios::trunc);
            if(!file.is_open())
            {
                std::cout << "error open file " << filename << std::endl;
            }
            else
            {
                for(const auto& str : result)
                {
                    file << str.first << ": " << str.second << std::endl;
                }
                file.close();
            }
        }
        return copies;
    }
    /**
     * @brief Функция получения границ для функции подсчета, учитывает то, что в одинаковые элементы должны попасть в один поток для обработки
     * @return Возвращает вектор пар: first - начальная граница, second - кол-во символов для обработки в блоке
     */
    std::vector<std::pair<size_t,size_t>> getBorders(const std::list<std::string>& source,size_t r)
    {
        std::vector<std::pair<size_t,size_t>> result;
        if (r > source.size()) r = source.size();
        if(r == 1)
        {
            result.emplace_back(std::make_pair(0,source.size()));
            return result;
        }
        
        size_t part_size = source.size() / r;
        std::vector<size_t> offsets;
        offsets.emplace_back(0);
        auto it = source.begin();

        for(size_t i=1 ; i < r ; ++i)
        {
            std::advance(it, part_size);
            auto cur = it;
            size_t offset_left = 0;
            while(it != source.begin())
            {   
                if (*(--it) != *cur)
                 {
                     break;
                 }
                else
                {
                    ++offset_left;
                }
            }
            it = cur;
            size_t offset_right = 0;
            while(it != source.end())
            {
                if (*(++it) != *cur)
                {
                    ++offset_right;
                    break;
                } 
                else
                {
                    ++offset_right;
                }
            }

            if(offset_left < offset_right) offsets.emplace_back(part_size*i - offset_left);
            else offsets.emplace_back(part_size*i + offset_right);
            it = cur;
        }
        offsets.emplace_back(source.size());
        std::sort(std::begin(offsets),std::end(offsets));
        offsets.resize(std::unique(std::begin(offsets),std::end(offsets))-std::begin(offsets));

        for (size_t i = 0; i < offsets.size()-1;++i)
        {
            result.push_back(std::make_pair(offsets.at(i),offsets.at(i+1) - offsets.at(i)));
        }

        return result;
    }
}