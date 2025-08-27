#include "necessary_libs.hpp"

template<class Iter, typename func>
void multithread_foreach(Iter begin, Iter end, func UnarFunc) {
    static_assert(!std::is_same<typename std::iterator_traits<Iter>::value_type, void>::value);

    size_t distance = std::distance(begin, end);

    if(distance < 1000) {
        std::for_each(begin, end, UnarFunc);
        return;
    }

    size_t thread_num = std::thread::hardware_concurrency() > 3 ? std::thread::hardware_concurrency() : 3;
    size_t chunk = distance / thread_num;

    auto temp = begin;
    std::vector<std::thread> threads(thread_num);
    for(size_t i = 0; i < thread_num; i++) {
        auto start = temp;
        std::advance(temp, i < thread_num - 1 ? chunk : distance - chunk * i);
        threads[i] = std::thread([start, temp, UnarFunc] () mutable {
            for( ; start != temp; start++) {
                *start =UnarFunc(*start);
            }
        });
    }

    for(auto& t : threads) t.join();
};

template<class Iter, typename func>
void async_foreach(Iter begin, Iter end, func UnarFunc) {
    static_assert(!std::is_same<typename std::iterator_traits<Iter>::value_type, void>::value);

    size_t distance = std::distance(begin, end);

    if(distance < 1000) {
        std::for_each(begin, end, UnarFunc);
        return;
    }

    size_t thread_num = std::thread::hardware_concurrency() > 3 ? std::thread::hardware_concurrency() : 3;
    size_t chunk = distance / thread_num;

    auto temp = begin;
    std::vector<std::future<void>> threads(thread_num);
    for(size_t i = 0; i < thread_num; i++) {
        auto start = temp;
        std::advance(temp, i < thread_num - 1 ? chunk : distance - chunk * i);
        threads[i] = std::async(std::launch::async, [start, temp, UnarFunc] () {
            for(; start != temp; start++) {
                *start = UnarFunc(*start);
            }
        });
    }

    for(auto& t : threads) t.get();   
};