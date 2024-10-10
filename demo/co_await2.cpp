#include <coroutine>
#include <iostream>

struct Awaiter{
    bool await_ready(){
        // false 表示暂停
        return false; 
    }
    void await_suspend(std::coroutine_handle<> h){}
    void await_resume() {};
};

struct Promise{
    using promise_type = Promise;
    Promise get_return_object(){
        return { std::coroutine_handle<promise_type>::from_promise(*this)};
    }
    std::suspend_never initial_suspend() {return {};}
    std::suspend_always final_suspend() noexcept {return {};}
    void return_void() {};
    void unhandled_exception() {};

    std::coroutine_handle<promise_type> h_;
};

int main()
{
    // 协程函数，由返回值类型确定是否为协程
    auto lambda = []()->Promise {
        std::cout<< "lambda\n";
        co_await Awaiter();
        std::cout<< "complete\n";
    };
    auto result = lambda();
    std::cout<< "main\n";
    result.h_.resume();
}