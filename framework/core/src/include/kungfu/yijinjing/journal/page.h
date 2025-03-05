// SPDX-License-Identifier: Apache-2.0

#ifndef YIJINJING_PAGE_H
#define YIJINJING_PAGE_H

#include <kungfu/yijinjing/journal/common.h>
#include <kungfu/yijinjing/journal/frame.h>

namespace kungfu::yijinjing::journal {

// KF_DEFINE_PACK_TYPE(                          //
//     page_header, 1, PK(version), PERPETUAL(), //
//     (uint32_t, version),                      //
//     (uint32_t, page_header_length),           //
//     (uint32_t, page_size),                    //
//     (uint32_t, frame_header_length),          //
//     (uint64_t, last_frame_position)           //
//);

// page 可以存放若干个frame对象，从一开始就固定了大小
// 行情数据为：128MB, 交易数据/策略数据为：16MB, 由静态函数find_page_size决定
// 采用mmap映射的方式, 这样在对内存进行更新时，操作系统就会自动进行持久化

// page 的数据结构
// |                           page_header                                                | frame_header + body | frame_header + body | ... |
// | version | page_header_length | page_size | frame_header_length | last_frame_position |

class page {
public:
  // 释放mmap 映射的page文件
  ~page();

  [[nodiscard]] uint32_t get_page_size() const { return header_->page_size; }

  [[nodiscard]] data::location_ptr get_location() const { return location_; }

  [[nodiscard]] uint32_t get_dest_id() const { return dest_id_; }

  [[nodiscard]] uint32_t get_page_id() const { return page_id_; }

  // 返回page第一个frame的产生时间戳
  [[nodiscard]] int64_t begin_time() const {
    return reinterpret_cast<longfist::types::frame_header *>(first_frame_address())->gen_time;
  }
  // 返回page中最后一个frame的产生时间戳
  [[nodiscard]] int64_t end_time() const {
    return reinterpret_cast<longfist::types::frame_header *>(last_frame_address())->gen_time;
  }

  // 返回page_header的地址
  [[nodiscard]] uintptr_t address() const { return reinterpret_cast<uintptr_t>(header_); }

  // 返回 body 的边界, 用于判断当前page是否已满
  [[nodiscard]] uintptr_t address_border() const {
    return address() + header_->page_size - sizeof(longfist::types::frame_header);
  }

  // 返回page中第一个frame的地址
  [[nodiscard]] uintptr_t first_frame_address() const { return address() + header_->page_header_length; }

  // 返回page中最后一个frame的地址
  [[nodiscard]] uintptr_t last_frame_address() const { return address() + header_->last_frame_position; }

  // 判断page是否已满, 当剩余大小不能存放下一个frame的大小时, 表示page已满
  [[nodiscard]] bool is_full() const {
    return last_frame_address() + reinterpret_cast<longfist::types::frame_header *>(last_frame_address())->length >
           address_border();
  }

  // 根据location和dest_id, page_id 加载新的page
  static page_ptr load(const data::location_ptr &location, uint32_t dest_id, uint32_t page_id, bool is_writing,
                       bool lazy);
  
  // 获取page的对应mmap的文件路径
  static std::string get_page_path(const data::location_ptr &location, uint32_t dest_id, uint32_t page_id);

  static uint32_t find_page_id(const data::location_ptr &location, uint32_t dest_id, int64_t time);

private:
  const data::location_ptr location_;
  const uint32_t dest_id_;
  const uint32_t page_id_;
  const bool lazy_;
  const size_t size_;
  const longfist::types::page_header *header_;

  page(data::location_ptr location, uint32_t dest_id, uint32_t page_id, size_t size, bool lazy, uintptr_t address);

  /**
   * update page header when new frame added
   */
  void set_last_frame_position(uint64_t position);

  friend class journal;

  friend class writer;

  friend class reader;
};

// 返回对应类型的page的大小
inline static uint32_t find_page_size(const data::location_ptr &location, uint32_t dest_id) {
  if (location->category == longfist::enums::category::MD && dest_id != 1) {
    return 128 * MB;
  }
  if ((location->category == longfist::enums::category::TD ||
       location->category == longfist::enums::category::STRATEGY) &&
      dest_id != 0) {
    return 16 * MB;
  }
  return MB;
}
} // namespace kungfu::yijinjing::journal

#endif // YIJINJING_PAGE_H
