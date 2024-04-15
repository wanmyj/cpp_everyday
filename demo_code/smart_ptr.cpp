template <typename T>
class u_ptr {
private:
    T* ptr;
public:
    u_ptr(T* p) : ptr(p) {}
    u_ptr(const u_ptr&) = delete;
    u_ptr(u_ptr&& rhs) noexcept : ptr(rhs.ptr) {
        rhs.ptr = nullptr;
    }
    u_ptr& operator=(const u_ptr&) = delete;
    u_ptr& operator=(u_ptr&& rhs) noexcept {
        if (this != &rhs) {
            delete ptr;
            ptr = rhs.ptr;
            rhs.ptr = nullptr;
        }
        return *this;
    }
    ~u_ptr() {
        delete ptr;
    }
    T* operator->() const noexcept {
        return ptr;
    }
    T& operator*() const noexcept {
        return *ptr;
    }
    explicit operator bool() const noexcept {
        return ptr != nullptr;
    }
};

// typedef unsigned int uint;
using uint = unsigned int;
template <typename T>
class s_ptr
{
private:
    uint* counter;
    T* resource;
public:
    // contructor
    s_ptr() : counter(new uint()), resource(nullptr) {}
    s_ptr(T* resc = nullptr) : counter(new uint(1)), resource(resc) {}
    // copy contructor
    s_ptr(const s_ptr& rhs) : counter(rhs.counter), resource(rhs.resource) {
        if (resource != nullptr)
            (*counter)++;
    }
    // copy assign operator
    s_ptr& operator=(const s_ptr& rhs) {
        if (&rhs != this) {
            __cleanup__();
            resource = rhs.resource;
            counter = rhs.counter;
            if (resource != nullptr)
                (*counter)++;
        }
        return *this;
    }
    // move contructor
    s_ptr(s_ptr&& rhs) noexcept : counter(rhs.counter), resource(rhs.resource) {
        rhs.counter = nullptr;
        rhs.resource = nullptr;
    }
    // move assign operator
    s_ptr& operator=(s_ptr&& rhs) noexcept {
        if (&rhs != this) {
            __cleanup__();
            resource = rhs.resource;
            counter = rhs.counter;
            rhs.counter = nullptr;
            rhs.resource = nullptr;
        }
        return *this;
    }
    ~s_ptr() {
        __cleanup__();
    }
    uint use_count() {
        return *counter;
    }
    T* get() {
        return resource;
    }
    T& operator*() {
        return *resource;
    }
    T* operator->() {
        return resource;
    }
private:
    void __cleanup__() {
        (*counter)--;
        if (*counter == 0) {
            delete counter;
            if (resource != nullptr)
                delete resource;
        }
    }
};