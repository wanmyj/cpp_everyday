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

