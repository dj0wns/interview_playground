#include <print>
#include <new>
#include <utility>
#include <unordered_map>
#include <list>

template <typename T>
class lru_cache {
  public:
    lru_cache(int capacity) : capacity(capacity) {}

    void clear() {
      count = 0;
      cache_list.clear();
      cache_map.clear();
    }

    T get(int key) {
      auto it = cache_map.find(key);
      if (it == cache_map.end()) return -1;
      // move element to the beginning with splice
      push_to_front(it->second);
      return it->second->second;
    }

    void put(int key, int value) {
      auto it = cache_map.find(key);
      if (it != cache_map.end()) {
        push_to_front(it->second);
        it->second->second = value;
      } else {
        ++count;
        cache_list.emplace_front(key, value);
        cache_map[key] = cache_list.begin();
        if (count > capacity) {
          --count;
          auto back = std::prev(cache_list.end());
          cache_map.erase(back->first);
          cache_list.erase(back);
        }
      }
    }

    void push_to_front(std::list<std::pair<int, T>>::iterator it) {
      cache_list.splice(cache_list.begin(), cache_list, it);
    }

    void print() {
      std::println("{{ ");
      for (auto it : cache_list) {
        std::println("([{}] -> {}),", it.first, it.second);
      }
      std::println(" }}");
    }

  private:

    int capacity;
    int count = 0;
    std::list<std::pair<int, T>> cache_list;
    std::unordered_map<int, typename std::list<std::pair<int, T>>::iterator> cache_map;
};

int main() {
  std::println("---------------Test 10 ints---------------");
  {
    lru_cache<int> test(10);
    for (auto i{0}; i < 10; ++i) {
      test.put(i, i);
    }
    test.print();
    std::println("---------------Test 5 ints---------------");
    for (auto i{10}; i < 15; ++i) {
      test.put(i, i);
    }
    test.print();
    std::println("---------------Get even ints---------------");
    for (auto i{0}; i < 15; i+=2) {
      auto val = test.get(i);
      std::println("GET ({}) -> {}", i, val);
    }
    test.print();
    std::println("---------------Test 5 ints---------------");
    for (auto i{15}; i < 20; ++i) {
      test.put(i, i);
    }
    test.print();
    std::println("---------------Test overwrite val---------------");
    for (auto i{15}; i < 20; ++i) {
      std::println("PUT ({}) -> {}", i, 20-i);
      test.put(i, 20-i);
    }
    test.print();
  }
}
