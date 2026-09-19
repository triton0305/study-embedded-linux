#include <iostream>

void to300(int& ref);

int main()
{

  int num = 0;
  int &ref  = num;

  std::cout << "num: " << num << '\n';
  std::cout << "ref: " << ref << '\n';

  ref = 3;

  std::cout << "num: " << num << '\n';
  std::cout << "ref: " << ref << '\n';

  std::cout << "&num: " << &num << '\n';
  std::cout << "&ref: " << &ref << '\n';

  to300(num);
  std::cout << "after num: " << num << '\n';
  std::cout << "after ref: " << ref << '\n';

  return 0;
}
//void to300(const int& ref) assignment of read-only reference
void to300(int& ref)
{
  ref = 300;
}