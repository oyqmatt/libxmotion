/**
* @file nc_hbox.cpp
* @date 1/29/23
* @brief
* 
* @copyright Copyright (c) 2023 Ruixiang Du (rdu)
*/

#include "ncview/nc_hbox.hpp"

#include <cassert>

namespace robosw {
namespace swviz {
void NcHbox::OnResize(int rows, int cols, int y, int x) {
  int div = elements_.size();
  assert(div != 0 && "Vbox divider cannot be zero!");
  int col_width = cols / div;
  for (int i = 0; i < elements_.size(); ++i) {
    elements_[i]->OnResize(rows, col_width, y, x + col_width * i);
  }
}
}
} // robosw