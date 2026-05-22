#include <print>
#include <new>
#include <utility>

template <typename T>
class vector {
  public:
    // default constructor is fine
    vector() {}

    // Default construct elements of size
    vector(int size) {
      buffer = allocate_new_buffer(size);
      for (auto i{0}; i < size; ++i){
        init_at_index(i);
      }
    }

    ~vector() {
      std::println("Destructor triggered");
      for (auto i{0}; i < vector_size; ++i) {
        std::println("Deleting element at [{}]", i);
        buffer[i].~T();
      }
      std::println("Freeing memory");
      ::operator delete(buffer);
    }

    void reserve(int size) {
      if (size <= buffer_size) {
        // nothing to do
        return;
      }
      auto new_buffer = allocate_new_buffer(size);
      move_from_old_to_new(new_buffer, size);
    }


    void push_back(const T& object) {
      std::println("Inserting element at [{}]", vector_size);
      if (vector_size >= buffer_size) {
        // allocate new buffer
        auto new_size = calculate_new_size();
        auto new_buffer = allocate_new_buffer(new_size);
        move_from_old_to_new(new_buffer, new_size);
      }
      init_at_index(vector_size++, object);
    }

    T& operator[] (int index) {
      return buffer[index];
    }

    void print() {
      for (auto i{0}; i < vector_size; ++i) {
        std::println("[{}] = {}", i, buffer[i]);
      }
    }

  private:
    void move_from_old_to_new(T* new_buffer, int new_size) {
      std::println("Copying to new buffer: {}", int(new_buffer));
      for (auto i{0}; i < vector_size; ++i) {
        new (&new_buffer[i]) T(std::move_if_noexcept(buffer[i]));
      }
      if (buffer) {
        ::operator delete(buffer);
      }
      buffer = new_buffer;
      buffer_size = new_size;
    }

    T* allocate_new_buffer(int count) {
      std::println("Allocating new buffer of size: {}", count);
      T* new_buffer = ::operator new(sizeof(T) * count);
      return new_buffer;
    }

    void init_at_index(int index, auto... args) {
      std::println("Initializing new element at [{}], addr: {}", index, int(&buffer[index]));
      new (&buffer[index]) T(args...);
    }

    int calculate_new_size() {
      return (buffer_size == 0 ? 5 : buffer_size) * 2;
    }

    T* buffer = nullptr;
    int buffer_size = 0;
    int vector_size = 0;
};

int main() {
  std::println("---------------Test 10 ints---------------");
  {
    vector<int> test;
    for (auto i{0}; i < 10; ++i) {
      test.push_back(i);
    }
    test.print();
    std::println("---------------Test deleting---------------");
  }
  std::println("---------------Test 100 ints---------------");
  {
    vector<int> test;
    for (auto i{0}; i < 100; ++i) {
      test.push_back(i);
    }
    test.print();
    std::println("---------------Test deleting---------------");
  }
}
