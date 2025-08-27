#include "necessary_libs.hpp"
#include "neutralElem.hpp"

template<class Iter, typename T>
auto multithread_accumulate(Iter p, Iter q, T start_initial) {
    if(p > q) throw std::invalid_argument("Incorrect input");
    if(p == q) return start_initial;
    if(std::distance(p,q) < 1000) return std::accumulate(p,q, start_initial);

    T result = start_initial;
    const size_t hardware_threads_num = std::thread::hardware_concurrency() > 3 ? std::thread::hardware_concurrency() : 3;
    const size_t chunk = std::distance(p,q) / hardware_threads_num;

    std::vector<std::thread> thread_vec;
    std::vector<T> temp_res(hardware_threads_num);
    auto begin = p;

    for(size_t i = 0; i < hardware_threads_num; i++) {
        auto start = begin;

        if(i == hardware_threads_num - 1) begin = q;
        else std::advance(begin, chunk);

        thread_vec.emplace_back([&temp_res, start, begin, i]() {
            temp_res[i] = std::accumulate(start, begin, T{});
        });
    }
    
    for(auto& t : thread_vec) t.join();
    for(const auto& i : temp_res) result = std::move(result) + i;
    return result;
};

template<class Iter, typename T, typename functor>
auto multithread_accumulate(Iter p, Iter q, T start_initial, functor binaryOp) {
    if(p > q) throw std::invalid_argument("Incorrect input");
    if(p == q) return start_initial;
    if(std::distance(p,q) < 1000) return std::accumulate(p,q, start_initial, binaryOp);

    T result = start_initial;
    const size_t hardware_threads_num = std::thread::hardware_concurrency() > 3 ? std::thread::hardware_concurrency() : 3;
    const size_t chunk = std::distance(p,q) / hardware_threads_num;

    std::vector<std::thread> thread_vec;
    std::vector<T> temp_res(hardware_threads_num);
    auto begin = p;

    for(size_t i = 0; i < hardware_threads_num; i++) {
        auto start = begin;

        if(i == hardware_threads_num - 1) begin = q;
        else std::advance(begin, chunk);

        thread_vec.emplace_back([&temp_res, start, begin, i, &binaryOp]() {
            temp_res[i] = std::accumulate(start, begin, Neutral_Elem<binaryOp,T>(), binaryOp);
        });
    }
    
    for(auto& t : thread_vec) t.join();
    for(const auto& i : temp_res) result = binaryOp(std::move(result), i);
    return result;
};

template<class Iter, typename T>
auto async_accumulate(Iter p, Iter q, T start_initial) {
    if(p > q) throw std::invalid_argument("Incorrect input");
    if(p == q) return start_initial;
    std::vector<std::future<T>> temp_res;

    T result = start_initial;
    const size_t hardware_threads_num = std::thread::hardware_concurrency() > 3 ? std::thread::hardware_concurrency() : 3;
    const size_t chunk = std::distance(p,q) / hardware_threads_num;

    auto begin = p;

    for(size_t i = 0; i < hardware_threads_num; i++) {
        auto start = begin;

        if(i == hardware_threads_num - 1) begin = q;
        else std::advance(begin, chunk);

        temp_res.emplace_back(std::async(std::launch::async, [start, begin]() {
            return std::accumulate(start, begin, T{});
        }));
    }

    for(auto& i : temp_res) {
        result = std::move(result) + i.get();
    }
    return result;
};

template<class Iter, typename T, typename functor>
auto async_accumulate(Iter p, Iter q, T start_initial, functor binaryOp) {
    if(p > q) throw std::invalid_argument("Incorrect input");
    if(p == q) return start_initial;
    std::vector<std::future<T>> temp_res;

    T result = start_initial;
    const size_t hardware_threads_num = std::thread::hardware_concurrency() > 3 ? std::thread::hardware_concurrency() : 3;
    const size_t chunk = std::distance(p,q) / hardware_threads_num;

    auto begin = p;

    for(size_t i = 0; i < hardware_threads_num; i++) {
        auto start = begin;

        if(i == hardware_threads_num - 1) begin = q;
        else std::advance(begin, chunk);

        temp_res.emplace_back(std::async(std::launch::async, [start, begin, binaryOp] {
            return std::accumulate(start, begin, Neutral_Elem<binaryOp, T>(), binaryOp);
        }));
    }

    for(auto& i : temp_res) {
        result = binaryOp(std::move(result), i.get());
    }
    return result;
};