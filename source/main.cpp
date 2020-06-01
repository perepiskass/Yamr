#include <iostream>
#include <utility>
#include <fstream>
#include "threadpool.h"
#include "mapping.h"
#include "reduce.h"
#include <future>
#include <algorithm>
#include <memory>


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
    std::vector<bool> max_lenght;
    bool duplicates = false;

    while (cicle)
    {
      max_lenght.clear();
      cicle = false;
    std::cout << "Get board\n";
      // Получение границ разделения файла------------------------------
      auto map_borders = mapping::getBorders(file_name,m);

    std::cout << "Get mapping\n";

      // for(auto& mb : map_borders) std::cout << mb.first << ' ' << mb.second << std::endl;

      // Выполнение map функции в потоках------------------------------
      std::unique_ptr<ThreadPool> thread_m (new ThreadPool{map_borders.size()});
      std::vector<std::future<std::pair<bool,std::list<std::string>>>> res;
      for(size_t i = 0;i < map_borders.size();++i)
      {
        res.emplace_back(thread_m->addTask(mapping::mapping,file_name,min,map_borders[i].first,map_borders[i].second));
      }

      std::cout << "Merge List\n";
      // Объединение всех результатов работы map функций---------------
      std::list<std::string> union_list;
      for (size_t i = 0; i < res.size();++i)
      {
        auto temp = res[i].get();
        // std::cout << "size " << i << temp.size() << std::endl;
        // for(auto& it : temp) std::cout <<"str -"<<it.size() << ' '<< it << std::endl;
        // std::cout << std::endl;
        union_list.merge(temp.second);
        max_lenght.emplace_back(temp.first);
      }
      bool all_strings = true;
      for(const auto& ml : max_lenght)
      {
        if(!ml) all_strings = false; 
      }
      // std::cout << "Size " << union_list.size() << std::endl;
      // for(auto& i : union_list) std::cout << i << ' ';
      // std::cout << std::endl;
    std::cout << "Get board List\n";
      // Выполнение reduce функции в потоках---------------------------
      auto reduce_borders = reduce::getBorders(union_list,r);

      // for(auto& rb : reduce_borders) std::cout << rb.first << ' ' << rb.second << std::endl;

    std::cout << "Get reduce\n";
      std::unique_ptr<ThreadPool> thread_r(new ThreadPool{reduce_borders.size()});
      std::vector<std::future<bool>> red_result;
      for(size_t i = 0;i < reduce_borders.size();++i)
      {
        red_result.emplace_back(thread_r->addTask(reduce::reduce,union_list,all_strings,i,reduce_borders[i].first,reduce_borders[i].second));
      }
      std::cout << "RESULT:\n";
      // Получение и проверка результатов
      for(auto& i : red_result)
      {
        auto temp_result = i.get();
        // std::cout << temp_result << ' ';
        if(!temp_result) continue;
        else
        {
          cicle = true;
          ++min;
          break;
        }
      }
      auto it = std::find(std::begin(max_lenght),std::end(max_lenght),false);
      for(const auto& b : max_lenght) std::cout << b << ' ';
      std::cout << std::endl;
      if(it == max_lenght.end())
      {
        if(!duplicates) duplicates = true;
        else 
        {
          std::cout << "List have duplicates\n";
          cicle = false;
          min -=2;
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
