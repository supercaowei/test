#include <iostream>
#include <deque>

void iterInc(std::deque<int>::iterator& iter) {
    printf("before iter inc, %d\n", *iter);
    iter++;
    printf("after iter inc, %d\n", *iter);
}

bool iterEnd(const std::deque<int>::iterator& iter, const std::deque<int>& deq) {
    if (iter != deq.end()) {
        printf("iter not end, %d\n", *iter);
    } else {
        printf("iter end\n");
    }
    return iter != deq.end();
}

int main ()
{
  std::deque<int> mydeque;

  // set some values (from 1 to 10)
  for (int i=1; i<=11; i++) mydeque.push_back(i);

//   // erase the 6th element
//   mydeque.erase(mydeque.begin()+5);

//   // erase the first 3 elements:
//   mydeque.erase (mydeque.begin(),mydeque.begin()+3);

  for (auto iter = mydeque.begin(); iterEnd(iter, mydeque);) {
      printf("before iter erase, %d\n", *iter);
      if (*iter % 2 == 0) {
          iter = mydeque.erase(iter);
          printf("after iter erase, %d\n", *iter);
      } else {
        printf("after iter not erase, %d\n", *iter);
        iterInc(iter);
      }
  }

  std::cout << "mydeque contains:";
  for (std::deque<int>::iterator it = mydeque.begin(); it!=mydeque.end(); ++it)
    std::cout << ' ' << *it;
  std::cout << '\n';

  return 0;
}