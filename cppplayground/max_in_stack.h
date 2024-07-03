#pragma once

#include <stack>
#include <iostream>

class stack_with_max1 {
public:
    int get_max() {
        if (mono_stk.size() > 0) {
            return mono_stk.top();
        }
        else {
            throw std::runtime_error("Stack is empty");
        }
    }
    int push(int t) {
        stk.push(t);
        if (mono_stk.size() == 0 || mono_stk.size() > 0 && t > mono_stk.top()) {
            mono_stk.push(t);
        }
    }
    void pop() {
        if (stk.size() == 0) {
            return;
        }
        if (stk.top() == mono_stk.top()) {
            mono_stk.pop();
        }
        stk.pop();
    }

    bool empty() { return stk.empty(); }
    int top() { return stk.top(); }

private:
    std::stack<int> stk;
    std::stack<int> mono_stk;
};


class stack_with_max2 : public std::stack<int> {
private:
    std::stack<int> mono_stk;

public:
    int get_max() {
        if (mono_stk.empty()) {
            throw std::runtime_error("Stack is empty");
        }
        return mono_stk.top();
    }

    void push(const int& t) {
        std::stack<int>::push(t);
        if (mono_stk.empty() || t >= mono_stk.top()) {
            mono_stk.push(t);
        }
    }

    void pop() {
        if (empty()) {
            return;
        }
        if (top() == mono_stk.top()) {
            mono_stk.pop();
        }
        std::stack<int>::pop();
    }
};


template <typename T>
class stack_with_max : public std::stack<T> {
private:
    std::stack<T> mono_stk;

public:
    T get_max() {
        if (!mono_stk.empty()) {
            return mono_stk.top();
        }
        throw std::runtime_error("Stack is empty");
    }

    void push(const T& value) {
        std::stack<T>::push(value);
        if (mono_stk.empty() || value >= mono_stk.top()) {
            mono_stk.push(value);
        }
    }

    void pop() {
        if (this->empty()) {
            return;
        }
        if (this->top() == mono_stk.top()) {
            mono_stk.pop();
        }
        // this->pop(); // this is not correct, it will cause recursive call
        std::stack<T>::pop();
    }
    //We use this->empty() and this->top() instead of just empty() 
    // and top() to avoid potential name lookup issues in templates.
};

void UT_stack_with_max() {
    stack_with_max<int> stk;
    stk.push(4);
    std::cout << stk.get_max() << std::endl;
    stk.push(3);
    std::cout << stk.get_max() << std::endl;
    stk.push(6);
    std::cout << stk.get_max() << std::endl;
    stk.push(2);
    std::cout << stk.get_max() << std::endl;
    stk.push(5);
    std::cout << stk.get_max() << std::endl;
    stk.push(7);
    std::cout << stk.get_max() << std::endl;
    stk.push(1);
    std::cout << stk.get_max() << std::endl;
    stk.pop();
    std::cout << stk.get_max() << std::endl;
    stk.pop();
    std::cout << stk.get_max() << std::endl;
    stk.pop();
    std::cout << stk.get_max() << std::endl;
    stk.pop();
    std::cout << stk.get_max() << std::endl;
    stk.pop();
    std::cout << stk.get_max() << std::endl;
    stk.pop();
    std::cout << stk.get_max() << std::endl;
}
