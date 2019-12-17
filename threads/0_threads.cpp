// mutex example
#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex

std::mutex mtx;           // mutex for critical section

void print_block (int n, char c) {
  for (int i=0; i<n; ++i) { std::cout << c; }
  std::cout << '\n';
}

// critical section (exclusive access to std::cout signaled by locking mtx):
void print_block_separate (int n, char c) {
  mtx.lock();
  for (int i=0; i<n; ++i) { std::cout << c; }
  std::cout << '\n';
  mtx.unlock();
}

// thread construction is slow, fewer items and th1 will be done before th2 starts up
int N = 10000;

int main ()
{
    // this will mix output
  std::thread th1 (print_block,N,'*');
  std::thread th2 (print_block,N,'$');

  th1.join();
  th2.join();

    // this will seperate output
  std::thread th3 (print_block_separate,N,'*');
  std::thread th4 (print_block_separate,N,'$');

  th3.join();
  th4.join();

  return 0;
}

