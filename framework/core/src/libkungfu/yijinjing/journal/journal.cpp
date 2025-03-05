// SPDX-License-Identifier: Apache-2.0

#include <kungfu/common.h>
#include <kungfu/longfist/longfist.h>
#include <kungfu/yijinjing/journal/journal.h>
#include <kungfu/yijinjing/journal/page.h>
#include <kungfu/yijinjing/time.h>

namespace kungfu::yijinjing::journal {

journal::~journal() {
  if (page_.get() != nullptr) {
    page_.reset();
  }
}

// 当前页中frame切换到下一个frame
void journal::next() {
  assert(page_.get() != nullptr);
  if (frame_->msg_type() == longfist::types::PageEnd::tag) {
    load_next_page();
  } else {
    frame_->move_to_next();
    page_frame_nb_++;
  }
}

void journal::seek_to_time(int64_t nanotime) {
  int page_id = page::find_page_id(location_, dest_id_, nanotime); // 从page_ids中找到离nanotime最近的page_id
  load_page(page_id); //加载对应页，并将指针定位到page_header_length这一位置
  // 找到对应的page
  while (page_->is_full() && page_->end_time() <= nanotime) {
    load_next_page();
  }
  // 在page中找到要写入的frame的位置
  while (frame_->has_data() && frame_->gen_time() <= nanotime) {
    next();
  }
}

void journal::load_page(int page_id) {
  if (page_.get() == nullptr or page_->get_page_id() != page_id) {
    page_ = page::load(location_, dest_id_, page_id, is_writing_, lazy_);
  }
  frame_->set_address(page_->first_frame_address());
  page_frame_nb_ = 0u;
}

void journal::load_next_page() { load_page(page_->get_page_id() + 1); }
} // namespace kungfu::yijinjing::journal