#include "simd_types.hpp"
#include <iostream>
int main()
{
  F32 floats[5];
  for(auto &f: floats)
  {
    std::cout << "Enter a float: ";
    std::cin >> f;
  }

  F32_4x simd(floats[0], floats[1], floats[2], floats[3]);
  auto mask = simd >= floats[4];

  if(mask.all())
  {
    std::cout
        << "I see that all lanes are greater than or equal to the compared "
           "value!"
           "That means that you don't have to mix!\n";
    simd *= 2;
  } else if(mask.any())
  {
    std::cout
        << "I see that some lanes are greater than or equal to the compared "
           "value!"
           "That means that you have to do something with *all the lanes* and "
           "mix that with the existing stuff!\n";
    auto tmp = simd * 2;
    simd = F32_4x::select(mask, tmp, simd);
  } else
  {
    std::cout << "I see that no lanes are greater than or equal to the "
                 "compared value! We can skip doing any work in this case!\n";
  }

  for(auto f: simd)
  {
    std::cout << "f: " << f << '\n';
  }
}
