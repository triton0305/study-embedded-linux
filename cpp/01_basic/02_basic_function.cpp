#include <iostream>


int add(int a, int b = 10);
double add(double a, double b = 3.14);

int main()
{
  std::cout <<add(3.14, 5.6) << '\n'; 
  std::cout << add(3.14) << '\n'; 
  std::cout << static_cast<int>(add(3.14)) << '\n';

  int *ptr = nullptr;

  if(ptr==nullptr)
  {
    std::cout << "ptr is null\n";
  }

  return 0;
}

int add(int a, int b)
{
  return a + b;
}

double add(double a, double b)
{
  return a + b;
}