#include "worm/util/string.h"

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <string>
#include <string_view>

#include "worm/parse/types.h"

namespace worm::util::string {
bool isspace(char ch) {
  auto uch {static_cast<unsigned char>(ch)};
  return std::isspace(uch);
}

void trim(std::string& s, bool (*should_strip)(char)) {
  std::size_t start {0};
  while (start < s.size() && should_strip(s[start])) {
    ++start;
  }

  if (start >= s.size()) {
    s.clear();
    return;
  }

  std::size_t end_incl {s.size()};
  while (end_incl-- > 0 && should_strip(s[end_incl])) {
    s.pop_back();
  }

  s.erase(0, start);
}

std::string_view trimmed_view(const std::string& s) {
  auto start {std::find_if(s.begin(), s.end(), [](auto ch) {
    return !isspace(ch);
  })};

  auto end_incl {std::find_if(s.rbegin(), s.rend(), [](auto ch) {
    return !isspace(ch);
  })};

  return std::string_view {start, end_incl.base()};
}

std::string_view trimmed_view(std::string_view sv) {
  auto start {std::find_if(sv.begin(), sv.end(), [](auto ch) {
    return !isspace(ch);
  })};

  auto end_incl {std::find_if(sv.rbegin(), sv.rend(), [](auto ch) {
    return !isspace(ch);
  })};

  return std::string_view {start, end_incl.base()};
}

std::string_view strip_circumfix(const std::string& s, char target) {
  if (s.empty() || s.front() != target || s.back() != target) {
    return s;
  }

  return {s.begin() + 1, s.end() - 1};
}

bool contains_keyword(
  std::string_view needle,
  std::string_view haystack,
  bool wrapping_spaces
) {
  if (!wrapping_spaces) {
    return find_not_quoted(needle, haystack) != std::string::npos;
  }

  if (haystack.size() < needle.size()) {
    return false;
  }

  if (
    std::string_view {haystack.begin(), haystack.begin() + needle.size()} == needle
    && (haystack.size() == needle.size() || std::isspace(haystack[needle.size()]))
  ) {
    return true;
  }

  bool in_str {false};
  std::size_t idx {1};

  while (idx + needle.size() < haystack.size()) {
    if (haystack[idx] == parse::types::str_delim) {
      in_str = !in_str;
    }

    if (
      in_str || !std::isspace(haystack[idx - 1])
      || !std::isspace(haystack[idx + needle.size()])
    ) {
      ++idx;
      continue;
    }

    std::string_view cmp {
      haystack.begin() + idx,
      haystack.begin() + idx + needle.size()
    };

    if (cmp == needle) {
      return true;
    }

    ++idx;
  }

  return needle == std::string_view {haystack.begin() + idx, haystack.end()};
}

std::size_t find_not_quoted(std::string_view needle, std::string_view haystack) {
  if (haystack.empty() || needle.size() > haystack.size()) {
    return std::string::npos;
  }

  if (needle.empty()) {
    return 0;
  }

  bool in_quotes {false};
  for (std::size_t i {0}; i < haystack.size(); ++i) {
    if (haystack[i] == parse::types::str_delim) {
      in_quotes = !in_quotes;
      continue;
    }

    if (in_quotes || haystack[i] != needle[0]) {
      continue;
    }

    bool early_break {false};
    std::size_t j {1};

    while (!early_break && j < needle.size()) {
      if (haystack[i + j] != needle[j]) {
        early_break = true;
      }

      ++j;
    }

    if (!early_break) {
      return i;
    }
  }

  return std::string::npos;
}

std::size_t indent_lvl(const std::string& s) {
  for (std::size_t i {0}; i < s.size(); ++i) {
    if (!isspace(s[i])) {
      return i;
    }
  }

  return s.size();
}

void unescape_str_delims(std::string& s) {
  std::size_t idx {1};
  while (idx < s.size()) {
    if (s[idx - 1] == '\\' && s[idx] == parse::types::str_delim) {
      s.erase(idx - 1, 1);
    } else {
      ++idx;
    }
  }
}
} // namespace worm::util::string
