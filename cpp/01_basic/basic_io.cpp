#include <iostream>
#include <iomanip>
int main()
{

  int first;
  int second;

  auto is_ready = true;
  if(is_ready)
  {  
    std::cout << "First: ";
    std::cin >> first;
    
    std::cout << "Second: ";
    std::cin >> second;

    std::cout << is_ready << '\n';
    std::cout<< "Sum: " << first + second << '\n';
  }
  if(!is_ready)
  {
    std::cout << "not ready" << '\n';
  }


  int a = 3.14;
  int b(3.14);
  //int c{3.14};  // narrowing

  std::cout << a << '\n';
  std::cout << b << '\n';
  // std::cout << c << '\n';

  const double pi = 3.14159265;
  //pi = 3.14159;   // assignment of read-only variable ‘pi’
  std::cout << "pi : " << pi << '\n';
  std::cout << "pi : " << std::setprecision(10) << pi << '\n';
  // Set floating-point output precision
  


  return 0;
}