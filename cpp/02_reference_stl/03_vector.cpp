#include <iostream>
#include <vector>
#include <string>

int main()
{

  std::vector<int> nums;
  nums.reserve(5);
  nums.push_back(105);
  nums.push_back(205);
  nums.push_back(305);

  for(int num : nums)
  {
    num += 10;
  }

  for(std::size_t i = 0 ; i < nums.size() ; i++ )
  {
    std::cout << "nums[" << i << "] :" << nums[i] << '\n';
  }

  for(int& num : nums)
  {
    num += 60;
  }

  for(int num : nums)
  {
    std::cout << num << '\n';
  }

  for(const int& num : nums)
  {
    std::cout <<"const"<< num << '\n';
  }


  std::vector<std::string> names;
  names.resize(5);

  std::cout << names.size() << '\n';
  std::cout << names.capacity() << '\n';
  std::cout << names[0] << '\n';

  std::vector<std::string> basket = {"apple", "banana", "melon"};
  basket.push_back("tangerine");
  if(!basket.empty())
  {
    std::string jeju = basket.back();
    basket.pop_back();
    for(const std::string& fruit : basket)
    {
      std::cout << fruit << '\n';
    }
    basket.push_back(jeju);
    std::cout << basket.front() << '\n';
    std::cout << basket.back() << '\n';
  }
  //std::cout << jeju << '\n';
  //jeju is only in scope inside the if block.
  
  return 0;
}