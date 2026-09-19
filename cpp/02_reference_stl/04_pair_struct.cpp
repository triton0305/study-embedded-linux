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
  std::cout << '\n';

//=============================================================================

  std::vector<Player> players;
  bool descending = true;

  players.push_back({100, "knight"});
  players.push_back({80, "archer"});
  players.push_back({120, "tank"});

  for(const Player& player : players)
  {
    std::cout << std::left << std::setw(8) << player.name << " : " << player.hp << '\n';
  }

  std::sort(players.begin(), players.end(),
            [descending](const Player& a, const Player& b)
            {
              return descending ? a.hp > b.hp : a.hp < b.hp;
            });

  for(const Player& player : players)
  {
    std::cout << std::left << std::setw(8) << player.name << " : " << player.hp << '\n';
  }


  std::cout << '\n';
  
//==================================================================

  std::vector<int> nums = {50, 20, 30, 10, 40};
  std::sort(nums.begin() + 2, nums.end());
  // end() is one past the last element

  for(auto it = nums.begin() ; it != nums.end() ; ++it)
  {
    std::cout << *it << '\n';
  }
  


  //======================================================
  bool enabled = true;
  
  auto valueCapture = [enabled]()
  {
    std::cout << "valueCapture : " << enabled << '\n';
  };

  auto referenceCapture = [&enabled]()
  {
    std::cout <<"referenceCapture : " << enabled << '\n';
  };

  enabled = false;

  valueCapture();
  referenceCapture();

  return 0;
}
