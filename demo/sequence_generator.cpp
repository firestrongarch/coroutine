#include <iostream>
#include <coroutine>
#include <utility>

struct Generator{
    struct promise_type{
                // 开始执行时不挂起，执行到第一个挂起点
        std::suspend_never initial_suspend() { return {}; };

        // // 执行结束后不需要挂起
        // std::suspend_never final_suspend() noexcept { return {}; }

        // 总是挂起，让 Generator 来销毁
        std::suspend_always final_suspend() noexcept { return {}; }

        // 为了简单，我们认为序列生成器当中不会抛出异常，这里不做任何处理
        void unhandled_exception() { }

        // 构造协程的返回值类型
        Generator get_return_object() {
            return Generator{std::coroutine_handle<promise_type>::from_promise(*this)};
        }
        
        // 传值的同时要挂起，值存入 value 当中
        std::suspend_always await_transform(int value) {
            this->value_ = value;
            return {};
        }
        int value_;

        // 没有返回值
        void return_void() { }
    };

    std::coroutine_handle<promise_type> h_;

    int next() {
        h_.resume();
        return h_.promise().value_;
    }

    explicit Generator(std::coroutine_handle<promise_type> handle) noexcept
        : h_(handle) {}
    Generator(Generator &&generator) noexcept
        : h_(std::exchange(generator.h_, {})) {}
    // 避免copy导致临时量被销毁连带h被销毁
    Generator(Generator &) = delete;
    Generator &operator=(Generator &) = delete;

    ~Generator() {
        // 销毁协程
        h_.destroy();
    }
};

Generator sequence() {
    int i = 0;
    while (true) {
        co_await i++; 
    }
}

int main()
{
    auto gen = sequence();
    for (int i = 0; i < 5; ++i) {
        std::cout << gen.next() << std::endl;
    }
}