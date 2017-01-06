#include "any_small.hpp"
#include <cassert>
#include <iostream>
#include <vector>

int main()
{
  {
    // test construct from trivial type

    int value = 13;
    any_small a1(value);

    assert(a1.type() == typeid(value));
    assert(any_cast<int&>(a1) == value);
  }

  {
    // test copy construct from non-trivial type

    std::vector<int> value(10, 13);
    any_small a1(value);

    assert(a1.type() == typeid(value));
    assert(any_cast<std::vector<int>&>(a1) == value);
  }

  {
    // test move construct from non-trivial type

    std::vector<int> value(10, 13);
    auto reference = value;

    any_small a1(std::move(value));
    
    assert(a1.type() == typeid(value));
    assert(any_cast<std::vector<int>&>(a1) == reference);
    assert(value.empty());
  }

  {
    // test copy construct from trivial any_small

    int value = 13;
    any_small a1(value);
    any_small a2 = a1;

    assert(a2.type() == a2.type());
    assert(a2.type() == typeid(int));

    assert(any_cast<int&>(a1) == value);
  }

  {
    // test copy construct from non-trivial any_small

    std::vector<int> value(10, 13);
    any_small a1(value);
    any_small a2 = a1;

    assert(a2.type() == a2.type());
    assert(a2.type() == typeid(std::vector<int>));

    assert(any_cast<std::vector<int>&>(a1) == value);
    assert(any_cast<std::vector<int>&>(a2) == value);
  }

  {
    // test move construct from non-trivial any_small

    std::vector<int> value(10, 13);
    any_small a1(value);
    any_small a2 = std::move(a1);

    assert(a2.type() == a1.type());
    assert(a2.type() == typeid(std::vector<int>));

    assert(any_cast<std::vector<int>&>(a1).empty());
    assert(any_cast<std::vector<int>&>(a2) == value);
  }

  {
    // test copy assign trivial -> trivial

    int value1 = 13;
    any_small a1(value1);

    int value2 = 7;
    any_small a2(value2);

    a2 = a1;

    assert(any_cast<int>(a1) == value1);
    assert(any_cast<int>(a2) == value1);
  }

  {
    // test copy assign trivial -> non-trivial

    int value1 = 13;
    any_small a1(value1);

    std::vector<int> value2(10, 13);
    any_small a2(value2);

    a2 = a1;

    assert(any_cast<int>(a1) == value1);
    assert(any_cast<int>(a2) == value1);
  }

  {
    // test copy assign non-trivial -> trivial

    std::vector<int> value1(10, 13);
    any_small a1(value1);

    int value2 = 7;
    any_small a2(value2);

    a2 = a1;

    assert(any_cast<std::vector<int>>(a1) == value1);
    assert(any_cast<std::vector<int>>(a2) == value1);
  }

  {
    // test copy assign non-trivial -> non-trivial

    std::vector<int> value1(10, 13);
    any_small a1(value1);

    std::vector<int> value2(10, 7);
    any_small a2(value2);

    a2 = a1;

    assert(any_cast<std::vector<int>>(a1) == value1);
    assert(any_cast<std::vector<int>>(a2) == value1);
  }

  {
    // test move assign trivial -> trivial

    int value1 = 13;
    any_small a1(value1);

    int value2 = 7;
    any_small a2(value2);

    a2 = std::move(a1);

    assert(any_cast<int>(a1) == value1);
    assert(any_cast<int>(a2) == value1);
  }

  {
    // test move assign trivial -> non-trivial

    int value1 = 13;
    any_small a1(value1);

    std::vector<int> value2(10, 7);
    any_small a2(value2);

    a2 = std::move(a1);

    assert(any_cast<int>(a1) == value1);
    assert(any_cast<int>(a2) == value1);
  }

  {
    // test move assign non-trivial -> non-trivial

    std::vector<int> value1(10, 13);
    any_small a1(value1);

    std::vector<int> value2(10, 7);
    any_small a2(value2);

    a2 = std::move(a1);

    assert(any_cast<std::vector<int>>(a1).empty());
    assert(any_cast<std::vector<int>>(a2) == value1);
  }

  {
    // test move assign non-trivial -> non-trivial

    std::vector<int> value1(10, 13);
    any_small a1(value1);

    std::vector<int> value2(10, 7);
    any_small a2(value2);

    a2 = std::move(a1);

    assert(any_cast<std::vector<int>>(a1).empty());
    assert(any_cast<std::vector<int>>(a2) == value1);
  }

  std::cout << "OK" << std::endl;

  return 0;
}

