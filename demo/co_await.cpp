#include <coroutine>
#include <iostream>

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
    void await_suspend(std::coroutine_handle<> h){}
    void await_resume() {};
};
int main()
{
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