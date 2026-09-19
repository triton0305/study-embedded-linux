#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <iomanip>
#include <algorithm>

struct Player
{
  int hp;
  std::string name;
};

int main()
{
  std::pair<int, std::string> point = {1, "triton"};

  std::cout << point.first <<'\n';
  std::cout << point.second << '\n';

  std::vector<Player> players;

  players.push_back({100, "knight"});
  players.push_back({80, "archer"});
  players.push_back({120, "tank"});

  for(const Player& player : players)
  {
    std::cout << std::left << std::setw(8) << player.name << " : " << player.hp << '\n';
  }

  std::vector<int> nums = {50, 20, 30, 10, 40};
  std::sort(nums.begin() + 2, nums.end());
  // end() one past the last element

  for(auto it = nums.begin() ; it != nums.end() ; it++)
  {
    std::cout << *it << '\n';
  }
  

  return 0;
}