#include <coroutine>
#include <iostream>
#include <chrono>
#include <thread>
#include <cstdlib>
#include <unistd.h>

struct promise{
    struct promise_type{
        promise get_return_object(){
            return { std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        std::suspend_never initial_suspend() noexcept {return {};}
        std::suspend_always final_suspend() noexcept {return {};}
        void return_void() {};
        void unhandled_exception() {};
    };
    std::coroutine_handle<promise_type> h_;
};
struct Input{
    bool await_ready(){
        // false 表示暂停
        return false; 
    }
    void await_suspend(std::coroutine_handle<> h){
        using namespace std::chrono_literals;
        while (1){
            std::cout<< "await_suspend\n";
            std::this_thread::sleep_for(std::chrono::seconds(1s));
        }
    }
    void await_resume() {};
};
int main()
{
    read(0, nullptr, 0);
    auto lambda = []()->promise {
        Input t;
        std::cout<< "lambda\n";
        co_await t;
        std::cout<< "complete\n";
    };
    promise result = lambda();
    std::cout<< "main\n";
    result.h_.resume();
}