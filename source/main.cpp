#include <iostream>
#include <utility>
#include <fstream>
#include "threadpool.h"
#include "mapping.h"
#include "reduce.h"
#include <future>
#include <algorithm>


std::pair<std::string,std::pair<size_t,size_t>> getArg(int argc,char** argv);

// std::string test(size_t i);
// std::string test2(std::string file_name,size_t offset,size_t lenght);
// std::string test3(std::string file_name,size_t offset,size_t lenght);

int main(int argc, char *argv[]) 
{
  try
  {
    auto args = getArg(argc,argv);
    std::string file_name{args.first};
    size_t m = args.second.first;
    size_t r = args.second.second;
    
    size_t min = 1;
    bool cicle = true;
    while (cicle)
    {
      cicle = false;
    std::cout << "Mapping\n";
      // Выполнение map функции в потоках------------------------------
      auto map_borders = mapping::getBorders(file_name,m);

      ThreadPool thread_m{map_borders.size()};
      std::vector<std::future<std::list<std::string>>> res;
      for(size_t i = 0;i < map_borders.size();++i)
      {
        res.emplace_back(thread_m.addTask(mapping::mapping,file_name,min,map_borders[i].first,map_borders[i].second));
      }
      std::cout << "after map\n";
      // Объединение всех результатов работы map функций---------------
      std::list<std::string> union_list;
      for (size_t i = 0; i < res.size();++i)
      {
        auto temp = res[i].get();
        // std::cout << "size " << i << temp.size() << std::endl;
        // for(auto& it : temp) std::cout <<"str -"<<it.size() << ' '<< it << std::endl;
        // std::cout << std::endl;
        union_list.merge(temp);
      }
      std::cout << "Size " << union_list.size() << std::endl;
      for(auto& i : union_list) std::cout << i << ' ';
      std::cout << std::endl;

      // Выполнение reduce функции в потоках---------------------------
      auto reduce_borders = reduce::getBorders(union_list,r);

      for(auto& rb : reduce_borders) std::cout << rb.first << ' ' << rb.second << std::endl;


      ThreadPool thread_r{reduce_borders.size()};
      std::vector<std::future<bool>> red_result;
      for(size_t i = 0;i < reduce_borders.size();++i)
      {
        red_result.emplace_back(thread_r.addTask(reduce::reduce,union_list,reduce_borders[i].first,reduce_borders[i].second));
      }
      std::cout << "RESULT:\n";
      // Получение и проверка результатов
      for(auto& i : red_result)
      {
        auto temp_result = i.get();
        std::cout << temp_result << ' ';
        if(temp_result) continue;
        else
        {
          cicle = true;
          ++min;
          break;
        }
      }
    }

    std::cout << "Minimum - " << min << std::endl;
  }
  
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }

    return 0;
}
