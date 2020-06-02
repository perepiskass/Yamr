
#include "threadpool.h"
#include "mapping.h"
#include "reduce.h"

std::pair<std::string,std::pair<size_t,size_t>> getArg(int argc,char** argv);

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
    bool duplicates = false;

    while (cicle)
    {
      cicle = false;
      /// Получение границ разделения файла
      auto map_borders = mapping::getBorders(file_name,m);

      /// Создание потоков по количеству границ  и выполнение map функции в потоках
      ThreadPool thread_m{map_borders.size()};
      std::vector<std::future<std::pair<bool,std::list<std::string>>>> res;
      for(size_t i = 0;i < map_borders.size();++i)
      {
        res.emplace_back(thread_m.addTask(mapping::mapping,file_name,min,map_borders[i].first,map_borders[i].second));
      }

      /// Объединение всех результатов работы map функции
      std::list<std::string> union_list;
      std::vector<bool> max_lenght;
      for (size_t i = 0; i < res.size();++i)
      {
        auto temp = res[i].get();
        union_list.merge(temp.second);
        max_lenght.emplace_back(temp.first);
      }

      /// Проверка на случай если была считана самая длинная строка из исходного файла(на случай проверки, если строки в файле были задублированы)
      bool all_strings = true;
      for(const auto& ml : max_lenght)
      {
        if(!ml) all_strings = false; 
      }

      /// Получение границ разделения для функции reduce (одинаковые элементы должны попасть для подсчета в один поток)
      auto reduce_borders = reduce::getBorders(union_list,r);

      /// Создание потоков по количеству границ  и выполнение reduce функции в потоках
      ThreadPool thread_r{reduce_borders.size()};
      std::vector<std::future<bool>> red_result;
      for(size_t i = 0;i < reduce_borders.size();++i)
      {
        red_result.emplace_back(thread_r.addTask(reduce::reduce,union_list,all_strings,i,reduce_borders[i].first,reduce_borders[i].second));
      }

      /// Получение и проверка результатов
      for(auto& i : red_result)
      {
        auto temp_result = i.get();
        if(!temp_result) continue;
        else
        {
          cicle = true;
          ++min;
          break;
        }
      }

      /// Если во всех потоках работы функции map была считана самая длинная строка значит различия между строк не найдено, есть дубликаты в исходном списке, проверяем это
      auto it = std::find(std::begin(max_lenght),std::end(max_lenght),false);
      if(it == max_lenght.end())
      {
        if(!duplicates) duplicates = true;
        else 
        {
          std::cout << "List have duplicates\n";
          cicle = false;
          min -=3;
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
