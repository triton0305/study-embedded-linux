#include <iostream>
#include <string>

void printString(const std::string& str);

int main()
{
  std::string name = "triton";

  std::cout << "   ID   : " << name << '\n';
  std::cout << "  size  : " << name.size() << '\n';
  std::cout << " address: " << &name << '\n';

  name += "0305";

  printString(name);

  return 0;
}

void printString(const std::string& name)
{
  std::cout << "   ID   : " << name << '\n';
  std::cout << "  size  : " << name.size() << '\n';
  std::cout << " address: " << &name << '\n';
}