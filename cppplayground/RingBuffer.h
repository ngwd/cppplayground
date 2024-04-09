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
using EventType = enum { Type1, Type2 };
struct Event {
    void* content;
    EventType type;
    Event() :content(nullptr), type(Type1) {}
};
/*

 */
class RingBuffer {
public:
    RingBuffer(size_t _n): capacity(_n), size(0), pbuf(new Event[capacity]), head(nullptr), tail(pbuf), UpperBound(pbuf+capacity) { }
    ~RingBuffer() {
        delete[] pbuf;
    }
    void copy_event(Event *src, Event *dst) {}

    size_t length() { return size; }
    Event* pop() 
    {
        Event* prev = head;

        // handle head
        ++head;
        if (head == UpperBound) {
            head = pbuf;
        }
        if (size) --size;
        // no need to handle tail
        return prev;
    }

    void push(Event *e) 
    {
        Event* prev = tail;

        // hanle tail
        copy_event(e, tail++); // place e;
        if (tail == UpperBound) {
            tail = pbuf;
        }

        // handle head
        if (head == nullptr) {
           head = pbuf;
        }
        else if (head == prev) {
            ++head;
            if (head == UpperBound)
                head = pbuf;
        }
        if (size != capacity) ++size;
    }
private:
    size_t capacity; // last N events kept in buffer
    size_t size;
    Event* pbuf, * head, * tail, * UpperBound;
};

Event* generateRandomEvent() { return new Event(); }
void handleEvent(Event* e) { return; }

int main() {

    const int N = 16;
    RingBuffer rb(N);

    auto a = [&]()->void {
        while (true) {
            rb.push(generateRandomEvent());
        }
    };
    auto b = [&]()->void {
        while (rb.length()) {
            handleEvent(rb.pop());
        }
    };
    Process A = startProcess(a);
    Process B = startProcess(b);
}