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

template <typename T>
class s_ptr
{
private:
    int* counter;
    T* resource;
public:
    // contructor
    s_ptr(T* resc = nullptr) : counter(new int(1)), resource(resc) {}
    // copy contructor
    s_ptr(const s_ptr& rhs) : counter(rhs.counter), resource(rhs.resource) {
        (*counter)++;
    }
    // copy assign operator
    s_ptr& operator=(const s_ptr& rhs) {
        if (&rhs != this) {
            (*counter)--;
            if (*counter == 0) {
                delete counter;
                delete resource;
            }
            resource = rhs.resource;
            counter = rhs.counter;
            (*counter)++;
        }
        return *this;
    }
    ~s_ptr() {
        (*counter)--;
        if (*counter == 0) {
            delete counter;
            delete resource;
        }
    }
    int use_count() {
        return *counter;
    }
};