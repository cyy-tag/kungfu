// SPDX-License-Identifier: Apache-2.0

//
// Created by Keren Dong on 2021/5/21.
//

#include <algorithm>
#include <cstdlib>
#include <kungfu/common.h>
#include <kungfu/yijinjing/common.h>
#include <regex>

namespace kungfu::yijinjing::data {

namespace fs = std::filesystem;
namespace es = longfist::enums;

// 存储根目录, 默认 /home/user/.config/kungfu/home
fs::path get_default_root() {
  char *kf_home = std::getenv("KF_HOME");
  if (kf_home != nullptr) {
    return fs::path{kf_home};
  }
#ifdef _WINDOWS
  auto appdata = std::getenv("APPDATA");
  auto root = fs::path(appdata);
#elif __APPLE__
  auto user_home = std::getenv("HOME");
  auto root = fs::path(user_home) / "Library" / "Application Support";
#elif __linux__
  auto user_home = std::getenv("HOME");
  auto root = fs::path(user_home) / ".config";
#endif // _WINDOWS
  return root / "kungfu" / "home";
}

// 运行时的目录 /home/user/.config/kungfu/home/runtime
std::string get_runtime_dir() {
  auto runtime_dir = std::getenv("KF_RUNTIME_DIR");
  if (runtime_dir != nullptr) {
    return runtime_dir;
  }
  return (get_default_root() / "runtime").string();
}

std::string get_root_dir(es::mode m, const std::vector<std::string> &tags) {
  static const std::unordered_map<es::mode, std::pair<std::string, std::string>> map_env = {
      {es::mode::LIVE, std::pair("KF_RUNTIME_DIR", "runtime")},
      {es::mode::BACKTEST, std::pair("KF_BACKTEST_DIR", "backtest")},
      {es::mode::DATA, std::pair("KF_DATASET_DIR", "dataset")},
      {es::mode::REPLAY, std::pair("KF_REPLAY_DIR", "replay")},
  };

  auto iter = map_env.find(m);
  if (iter == map_env.end()) {
    SPDLOG_WARN("unrecognized mode: {}, init root_ as mode::LIVE", m);
    return get_runtime_dir();
  } else {
    auto dir_path = std::getenv(iter->second.first.c_str());
    if (dir_path != nullptr) {
      return dir_path;
    }
    auto home_dir_path = get_default_root() / iter->second.second;
    home_dir_path /= std::accumulate(tags.begin(), tags.end(), fs::path{},
                                     [&](const fs::path &p, const std::string &tag) { return p / tag; });
    return home_dir_path.string();
  }
}

// 默认locator 实时模式
locator::locator() : root_(get_runtime_dir()), dir_mode_(es::mode::LIVE) {}

locator::locator(es::mode m, const std::vector<std::string> &tags) : dir_mode_(m) {
  root_ = get_root_dir(dir_mode_, tags);
}

bool locator::has_env(const std::string &name) const { return std::getenv(name.c_str()) != nullptr; }

std::string locator::get_env(const std::string &name) const { return std::getenv(name.c_str()); }

std::string locator::layout_dir(const location_ptr &location, es::layout layout) const {
  auto dir = root_ /                                     //
             es::get_category_name(location->category) / //
             location->group /                           //
             location->name /                            //
             es::get_layout_name(layout) /               //
             es::get_mode_name(location->mode);
  if (not fs::exists(dir)) {
    fs::create_directories(dir);
  }
  return dir.string();
}

std::string locator::layout_file(const location_ptr &location, es::layout layout, const std::string &name) const {
  auto path = fs::path(layout_dir(location, layout)) / fmt::format("{}.{}", name, es::get_layout_name(layout));
  return path.string();
}

[[maybe_unused]] std::string locator::default_to_system_db(const location_ptr &location,
                                                           const std::string &name) const {
  auto sqlite_layout = es::layout::SQLITE;
  auto db_file = layout_file(location, sqlite_layout, name);
  if (not fs::exists(db_file)) {
    auto system_db_file = root_ /                                     //
                          es::get_category_name(location->category) / //
                          location->group /                           //
                          location->name /                            //
                          es::get_layout_name(sqlite_layout) /        //
                          es::get_mode_name(location->mode);
    fs::copy(system_db_file, db_file);
  }
  return db_file;
}

std::vector<uint32_t> locator::list_page_id(const location_ptr &location, uint32_t dest_id) const {
  std::vector<uint32_t> result = {};
  auto dest_id_str = fmt::format("{:08x}", dest_id);
  auto dir = fs::path(layout_dir(location, es::layout::JOURNAL));
  for (auto &it : fs::recursive_directory_iterator(dir)) {
    auto basename = it.path().stem();
    if (it.is_regular_file() and it.path().extension() == ".journal" and basename.stem() == dest_id_str) {
      auto index = std::atoi(basename.extension().string().c_str() + 1);
      result.push_back(index);
    }
  }
  std::sort(result.begin(), result.end());
  return result;
}

static constexpr auto w = [](const std::string &pattern) { return pattern == "*" ? ".*" : pattern; };

static constexpr auto g = [](const std::string &pattern) { return fmt::format("({})", w(pattern)); };

std::vector<location_ptr> locator::list_locations(const std::string &category, const std::string &group,
                                                  const std::string &name, const std::string &mode) const {
  fs::path search_path = root_ / g(category) / g(group) / g(name) / "journal" / g(mode);
  std::string pattern = std::regex_replace(search_path.string(), std::regex("\\\\"), "\\\\");
  std::regex search_regex(pattern);
  std::vector<location_ptr> result = {};
  std::smatch match;
  for (auto &it : fs::recursive_directory_iterator(root_)) {
    auto path = it.path().string();
    if (it.is_directory() and std::regex_match(path, match, search_regex)) {
      auto l = location::make_shared(es::get_mode_by_name(match[4].str()),     //
                                     es::get_category_by_name(match[1].str()), //
                                     match[2].str(),                           //
                                     match[3].str(),                           //
                                     std::make_shared<locator>(root_.string()));
      result.push_back(l);
    }
  }
  return result;
}

std::vector<uint32_t> locator::list_location_dest(const location_ptr &location) const {
  std::unordered_set<uint32_t> set = {};
  auto dir = fs::path(layout_dir(location, es::layout::JOURNAL));
  for (auto &it : fs::recursive_directory_iterator(dir)) {
    auto basename = it.path().stem();
    if (it.is_regular_file() and it.path().extension() == ".journal") {
      set.emplace(std::stoul(basename.stem(), nullptr, 16));
    }
  }
  return std::vector<uint32_t>{set.begin(), set.end()};
}

std::vector<uint32_t> locator::list_location_dest_by_db(const location_ptr &location) const {
  std::unordered_set<uint32_t> set = {};
  auto dir = fs::path(layout_dir(location, es::layout::SQLITE));
  for (auto &it : fs::recursive_directory_iterator(dir)) {
    auto basename = it.path().stem();
    if (it.is_regular_file() and it.path().extension() == ".db") {
      set.emplace(std::stoul(basename.stem(), nullptr, 16));
    }
  }
  return std::vector<uint32_t>{set.begin(), set.end()};
}

bool locator::operator==(const locator &another) const {
  return dir_mode_ == another.dir_mode_ and root_.string() == another.root_.string();
}
} // namespace kungfu::yijinjing::data