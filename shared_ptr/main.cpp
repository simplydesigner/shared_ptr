#include <iostream>

template <typename T>
class shared_ptr {
private:
    struct block
    {
        block(T * ptr) : ptr_(ptr), refs_(1)
        {
            ;
        }
        ~block()
        {
            delete ptr_;
        }
        
        T * ptr_;
        std::size_t refs_;
    };
public:
    explicit shared_ptr(T * ptr) : block_(new block(ptr))
    {
        ;
    }
    
    ~ shared_ptr()
    {
        decrement();
    }
    
    shared_ptr(shared_ptr const & other) noexcept : block_(other.block_)
    {
        increment();
    }
    
    auto operator =(shared_ptr const & other) -> shared_ptr &
    {
        if ( block_ != other.block_ ) {
            decrement();
            block_ = other.block_;
            increment();
        }
        
        return *this;
    }
    
    shared_ptr(shared_ptr && other) noexcept : block_(other.block_)
    {
        other.block_ = nullptr;
    }
    
    auto operator =(shared_ptr && other) -> shared_ptr &
    {
        if ( block_ != other.block_ ) {
            decrement();
            block_ = other.block_;
            other.block_ = nullptr;
        }
        
        return *this;
    }
    
    auto operator ->() const -> T *
    {
        return block_->ptr_;
    }
    
    auto operator *() const -> T *
    {
        return *(block_->ptr_);
    }
    
private:
    block * block_;
    
    auto increment() noexcept -> void
    {
        ++(block_->refs_);
    }
    
    auto decrement() -> void
    {
        if ( block_ && --(block_->refs_) == 0 ) {
            delete block_;
            block_ = nullptr;
        }
    }
};

class logable
{
    
#define self logable
public:
    self()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }

    self(self const &)
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
    
    ~self()
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
    
    auto hello() -> void
    {
        std::cout << __PRETTY_FUNCTION__ << std::endl;
    }
};


int main(int argc, const char * argv[]) {
    
    shared_ptr<logable> ptr_1 { new logable };
    
    shared_ptr<logable> ptr_2 { ptr_1 };
    
    shared_ptr<logable> ptr_3 { new logable };
    
    ptr_2 = ptr_3;
    
    ptr_2 = std::move(ptr_1);
    
    ptr_2->hello();
    
    
    return 0;
}
