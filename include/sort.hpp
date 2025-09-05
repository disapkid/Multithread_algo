#include "necessary_libs.hpp"

template <class Iter>
void multithread_sort(Iter begin, Iter end, int thread_num = -1) {
    static_assert(!std::is_same<typename std::iterator_traits<Iter>::value_type, void>::value);

    if(thread_num == -1) {
        size_t threads = std::thread::hardware_concurrency() < 3 ? 3 : std::thread::hardware_concurrency();
        thread_num = threads;
    }

    size_t length = std::distance(begin, end);
    if(length <= 1) return;
    if(length < 1 << 16) {
        std::sort(begin, end);
        return;
    }

    size_t mid = length / 2;
    auto middleIter = begin;
    std::advance(middleIter, mid);

    if(thread_num > 0) {
        std::thread t([=] () {
            multithread_sort(begin, middleIter, thread_num - 1);
        });
        multithread_sort(middleIter, end, thread_num - 1);
        t.join();
    } else {
        multithread_sort(begin, middleIter, 0);
        multithread_sort(middleIter, end, 0);
    }

    std::inplace_merge(begin, middleIter, end);
};

template <class Iter, typename func>
void multithread_sort(Iter begin, Iter end, func Compare, int thread_num = -1) {
    static_assert(!std::is_same<typename std::iterator_traits<Iter>::value_type, void>::value);

    if(thread_num == -1) {
        size_t threads = std::thread::hardware_concurrency() < 3 ? 3 : std::thread::hardware_concurrency();
        thread_num = threads;
    }

    size_t length = std::distance(begin, end);
    if(length <= 1) return;
    if(length < 1 << 16) {
        std::sort(begin, end);
        return;
    }

    size_t mid = length / 2;
    auto middleIter = begin;
    std::advance(middleIter, mid);

    if(thread_num > 0) {
        std::thread t([=] () {
            multithread_sort(begin, middleIter, Compare, thread_num - 1);
        });
        multithread_sort(middleIter, end, Compare ,thread_num - 1);
        t.join();
    } else {
        multithread_sort(begin, middleIter, Compare, 0);
        multithread_sort(middleIter, end, Compare, 0);
    }

    std::inplace_merge(begin, middleIter, end, Compare);
};

template<class Iter>
void async_sort(Iter begin, Iter end, int thread_num = -1) {
    static_assert(!std::is_same<typename std::iterator_traits<Iter>::value_type, void>::value);

    if(thread_num == -1) {
        size_t threads = std::thread::hardware_concurrency() < 3 ? 3 : std::thread::hardware_concurrency();
        thread_num = threads;
    }

    size_t length = std::distance(begin, end);
    if(length <= 1) return;
    if(length < 1 << 16) {
        std::sort(begin, end);
        return;
    }
    size_t mid = length / 2;
    auto middleIter = begin;
    std::advance(middleIter, mid);

    if(thread_num > 0) {
        auto future = std::async([=] () {
            async_sort(begin, middleIter, thread_num - 1);
        });
        async_sort(middleIter, end, thread_num - 1);
        future.get();
    } else {
        async_sort(begin, middleIter, thread_num - 1);
        async_sort(middleIter, end, thread_num - 1);
    }

    std::inplace_merge(begin, middleIter, end);   
};

template<class Iter, typename func>
void async_sort(Iter begin, Iter end, func Compare, int thread_num = -1) {
    static_assert(!std::is_same<typename std::iterator_traits<Iter>::value_type, void>::value);

    if(thread_num == -1) {
        size_t threads = std::thread::hardware_concurrency() < 3 ? 3 : std::thread::hardware_concurrency();
        thread_num = threads;
    }

    size_t length = std::distance(begin, end);
    if(length <= 1) return;
     if(length < 1 << 16) {
        std::sort(begin, end);
        return;
    }

    size_t mid = length / 2;
    auto middleIter = begin;
    std::advance(middleIter, mid);

    if(thread_num > 0) {
        auto future = std::async([=] () {
            async_sort(begin, middleIter, Compare, thread_num - 1);
        });
        async_sort(middleIter, end, Compare ,thread_num - 1);
        future.get();
    } else {
        async_sort(begin, middleIter, Compare, 0);
        async_sort(middleIter, end, Compare, 0);
    }

    std::inplace_merge(begin, middleIter, end, Compare);
};
