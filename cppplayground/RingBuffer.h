/*
Suppose we have 2 tasks or processes,
task / process A generates events and task / process B to process them.

///  Task A has higher priority than task B,

if task A generates too many events exceeding certain limit,
the system should start dropping old events
and keep only last N events available for B to process.

Your task is to implement the data structure needed to fulfill
the requirements and write sample application code for both tasks A& B.
*/
#include <iostream>
#include <random>
// #include <memory>

#include <thread>
#include <condition_variable>
#include <mutex>

#include <chrono>

using EventType = enum { Type1, Type2 };
struct Event {
    static std::random_device rd;
    static std::mt19937 gen;
    static size_t min_value;
    static size_t max_value;
    static std::uniform_int_distribution<size_t> distribution;

    size_t id;
    EventType type;
    Event() :id(distribution(gen)), type(Type1) {}
};

std::random_device Event::rd;
std::mt19937 Event::gen(Event::rd());
size_t Event::min_value = 0;
// size_t Event::max_value = std::numeric_limits<size_t>::max();
size_t Event::max_value = 100;
std::uniform_int_distribution<size_t> Event::distribution(Event::min_value, Event::max_value);

/*

 */
class RingBuffer {
public:
    RingBuffer(size_t _n) : capacity(_n), size(0), pbuf(new Event[capacity]),
        head(nullptr), tail(pbuf), UpperBound(pbuf + capacity) {

    push_cnt = pop_cnt = drop_cnt = 0;
    }
    ~RingBuffer() { delete[] pbuf; }

    void copy_event(Event* src, Event* dst) { std::swap(*src, *dst); }
    size_t length() { return size; }

    Event* pop() // lower priority, consumer
    {
        std::unique_lock<std::mutex> lck(mtx);
        while (!length()) {
            cv.wait(lck);
        }
        Event* prev = head;

        // handle head
        ++head;
        if (head == UpperBound) {
            head = pbuf;
        }
        if (size) --size;
        // no need to handle tail

        std::cout << "Consumed:: event id " << prev->id << ", type " << prev->type 
            <<", Push " << push_cnt << ", Pop " << pop_cnt << ", Drop " << drop_cnt
            << std::endl;
        ++pop_cnt;
        return prev;
    }

    void push(Event* e) // higher priority, producer
    {
        std::unique_lock<std::mutex> lck(mtx);
        {
            std::cout << "Produced:: event id " << e->id << ", type " << e->type 
                <<", Push " << push_cnt << ", Pop " << pop_cnt << ", Drop " << drop_cnt
                << std::endl;
            Event* prev = tail;

            // hanle tail
            copy_event(e, tail++); // place e;
            if (tail == UpperBound) {
                tail = pbuf;
            }

            // handle head
            if (head == prev) { // drop
                ++drop_cnt;
                ++head;
                if (head == UpperBound) { 
                    head = pbuf;
                }
            }
            else if (head == nullptr) {
                head = pbuf;
            }
            if (size != capacity) ++size;
            ++push_cnt;
        }

        // std::this_thread::sleep_for(std::chrono::milliseconds(100));
        cv.notify_one();
    }
private:
    size_t capacity; // last N events kept in buffer
    size_t size;
    Event* pbuf, * head, * tail, * UpperBound;
    std::mutex mtx;
    std::condition_variable cv;

    size_t push_cnt; 
    size_t pop_cnt;
    size_t drop_cnt;
};

Event* generateRandomEvent() { return new Event(); }
void handleEvent(Event* e) 
{
    // adjust it for simulating the consumer time
    std::this_thread::sleep_for(std::chrono::milliseconds(4));
    return;
}

void UT_ring_buffer() {
    const int N = 32;
    RingBuffer rb(N);

    // producer, higher priority
    auto a = [&rb]()->void {
        auto startTime = std::chrono::steady_clock::now();
        Event* tmp;
        while (true) {
            tmp = generateRandomEvent();
            rb.push(tmp);
            delete tmp;

            // let the thread runs 10s
            std::this_thread::sleep_for(std::chrono::seconds(1));
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
            if (elapsedTime >= 10) {
                break;
            }
        }
    };

    // consumer, lower priority
    auto b = [&rb]()->void {
        auto startTime = std::chrono::steady_clock::now();
        while (true) {
            if (rb.length())
                handleEvent(rb.pop());

            // let the thread runs 10s
            std::this_thread::sleep_for(std::chrono::seconds(1));
            auto currentTime = std::chrono::steady_clock::now();
            auto elapsedTime = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime).count();
            if (elapsedTime >= 10) {
                break;
            }
        }
    };

    std::thread A(a);
    std::thread B(b);
    B.join();
    A.join();
    return;
}
