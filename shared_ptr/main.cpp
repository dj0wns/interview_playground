#include <print>

template <typename T>
class shared_ptr {
  public:
    shared_ptr() : data(nullptr), reference_count(nullptr) {}

    // constructor to take ownership of raw pointer
    shared_ptr(T* data) : data(data), reference_count(new int(1)) {}

    ~shared_ptr() {
      std::println("destructor called");
      remove_reference();
    }

    // Copy constructor - increment counter
    shared_ptr(const shared_ptr<T>& val) : data(val.data), reference_count(val.reference_count) {
      std::println("copy constructor called");
      increment_reference();
    }

    // Move constructor
    shared_ptr(shared_ptr<T>&& val) : data(val.data), reference_count(val.reference_count) {
      std::println("move constructor called");
      val.data = nullptr;
      val.reference_count = nullptr;
    }

    // Copy assign
    shared_ptr<T>& operator=(const shared_ptr<T>& val) {
      // Self assignment guard!
      if (this == &val) return *this;
      remove_reference();
      std::println("copy assign called");
      data = val.data;
      reference_count = val.reference_count;
      increment_reference();
      return *this;
    }
    // Move assign
    shared_ptr<T>& operator=(shared_ptr<T>&& val) {
      // Self assignment guard!
      if (this == &val) return *this;
      remove_reference();
      std::println("move assign called");
      data = val.data;
      reference_count = val.reference_count;
      val.data = nullptr;
      val.reference_count = nullptr;
      return *this;
    }

    // dereference operator
    T& operator*() {
      return *data;
    }

    // Helper, only works if the value is trivially printable!
    void print_val() {
      if (data) {
        std::println("Shared ptr: address = {}, value = {} - references = {}", int(data), *data, *reference_count);
      } else {
        std::println("Shared ptr: address = {} - references = {}", int(data), int(reference_count));
      }
    }
  private:
    void remove_reference() {
      if (data && *reference_count == 1) {
        delete data;
        delete reference_count;
      } else {
        data = nullptr;
        if  (reference_count) {
          --(*reference_count);
          reference_count = nullptr;
        }
      }
    }

    void increment_reference() {
      if (data) {
        ++(*reference_count);
      }
    }

    T* data;
    int* reference_count;
};

int main() {
  // Default constructed
  {
    std::println("----------Default constructor----------");
    shared_ptr<int> a;
    a.print_val();
  }

  {
    // constructed with new
    std::println("----------Constructor with new----------");
    shared_ptr<int> b(new int(25));
    b.print_val();

    {
      // copy constructed
      std::println("----------Copy constructed----------");
      shared_ptr<int> c(b);
      c.print_val();
      b.print_val();
      std::println("----------Modify value----------");
      *c = 26;
      c.print_val();
      b.print_val();
      std::println("----------Move constructed----------");
      shared_ptr<int> d(std::move(c));
      c.print_val();
      d.print_val();
      b.print_val();
      std::println("----------Copy assigned----------");
      shared_ptr<int> e;
      e = d;
      d.print_val();
      b.print_val();
      c.print_val();
      e.print_val();
      std::println("----------Move assigned----------");
      c = std::move(d);
      d.print_val();
      b.print_val();
      c.print_val();
      e.print_val();
    }
    // c goes out of scope
    std::println("----------Destructor----------");
    b.print_val();
  }

}
