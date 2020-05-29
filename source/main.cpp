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
    
    size_t min = 3;
    // bool cicle = true;
    // while (cicle)
    // {
    //   cicle = false;
      // Выполнение map функции в потоках------------------------------
      auto map_borders = mapping::getBorders(file_name,m);
      std::cout << "TUT1_____________";

      ThreadPool thread_m{map_borders.size()};
      std::vector<std::future<std::list<std::string>>> res;
      for(size_t i = 0;i < map_borders.size();++i)
      {
        res.emplace_back(thread_m.addTask(mapping::mapping,file_name,min,map_borders[i].first,map_borders[i].second));
      }
      std::cout << "TUT2_____________";

      // Объединение всех результатов работы map функций---------------
      std::list<std::string> union_list;
      for (size_t i = 0; i < res.size();++i)
      {
        union_list.merge(res[i].get());
      }
      std::cout << "TUT3_____________";
      // Выполнение reduce функции в потоках---------------------------
      auto reduce_borders = reduce::getBorders(union_list,r);
      ThreadPool thread_r{reduce_borders.size()};
      std::vector<std::future<bool>> red_result;
      for(size_t i = 0;i < reduce_borders.size();++i)
      {
        red_result.emplace_back(thread_r.addTask(reduce::reduce,union_list,reduce_borders[i].first,reduce_borders[i].second));
      }

      // Получение и проверка результатов
      for(auto& i : red_result)
      {
        std::cout << i.get() << ' ';
        // if(i.get()) continue;
        // else
        // {
        //   // cicle = true;
        //   ++min;
        //   // break;
        // }
      }
    // }

    std::cout << "Minimum - " << min << std::endl;




    // for(auto& b : borders)
    // {
    //   std::cout << b.first << ' ' << b.second << std::endl;
    // }
    // for(auto& i : union_list)
    // {
    //   std::cout << i << ' ';
    // }
    // std::cout << std::endl;
    // for(size_t i = 0; i < borders.size(); ++i)
    // {
    //   std::list<std::string>::iterator it = union_list.begin();
    //   std::advance(it,borders[i].first);
    //   for(size_t z = 0;z < borders[i].second;++z)
    //   {
    //     std::cout << *it << ' ';
    //     ++it;
    //   }
    //   std::cout << std::endl;
    // }
  }
  
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }

    return 0;
}
