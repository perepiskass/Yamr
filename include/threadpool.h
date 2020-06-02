#pragma once

#include <vector>
#include <thread>
#include <future>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>


/**
 * @brief Класс обеспичивающий создание очереди потоков, и добавления в эти потоки задачь для выполнения
 */
class ThreadPool
{
    public:
        ThreadPool(const size_t thread_count);

        template<class F, class... Args>
        auto addTask(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;  /// Добавляем задачу и сразу получаем на нее объект future

        ~ThreadPool();

    private:
        std::vector<std::thread> pool;
        std::queue<std::function<void()>> tasks;
        std::mutex queue_mutex;
        std::condition_variable cv;
        bool stop;
};

ThreadPool::ThreadPool(const size_t thread_count):stop(false)
{
    for(size_t i = 0;i < thread_count;++i)
        pool.emplace_back([this](){
                // while(true)      Каждый процесс получает по одной задаче, если оставляем цикл то возможно один процесс будет отрабатывать больше чем один раз
                // {
                    std::function<void()> task;

                    {
                        std::unique_lock<std::mutex> lock(this->queue_mutex);
                        this->cv.wait(lock,[this]{ return this->stop || !tasks.empty(); });
                        if(this->stop && this->tasks.empty()) return;
                        task = std::move(this->tasks.front());
                        tasks.pop();
                    }
                    task();
                // }
            });
}

template<class F, class... Args>
auto ThreadPool::addTask(F&& f, Args&&... args)-> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;

    auto task = std::make_shared< std::packaged_task<return_type()> >(std::bind(std::forward<F>(f), std::forward<Args>(args)...)); /// создаем указатель на объект packaged_task с типом который возвращает наша переданая функция f и связываем данный объект с данной функцией.
    std::future<return_type> res = task->get_future();  /// получаем объект future (будущий результат) из указателя на задачу

    std::unique_lock<std::mutex> lock(queue_mutex);
    if(stop) throw std::runtime_error("add task to stopped threadpool");
    tasks.emplace([task](){ (*task)(); });  /// добавляем лямбду функцию в очередь задачь с вызовом нашей сформированной задачи(task)
    lock.unlock();

    cv.notify_one();
    
    return res;
}

ThreadPool::~ThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    cv.notify_all();
    for(auto &worker: pool)
    {
        worker.join();
    }


}