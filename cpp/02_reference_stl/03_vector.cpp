#include <iostream>
#include <vector>

int main()
{

  std::vector<int> v = {10, 20 ,30};

  auto it = v.begin();

  std::cout << *it << '\n';

  ++it;

  std::cout << *it << '\n';

  return 0;
}