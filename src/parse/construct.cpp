#include "worm/parse/construct.h"

#include <string_view>
#include <utility>

#include "worm/parse/keywords.h"
#include "worm/util/string.h"

namespace worm::parse::construct {
// NOTE: The logic in this will need to be updated once one line can contain more than
// one construct
Construct determine_construct(std::string_view line) {
  struct Triplet {
    const std::string_view keyword;
    const Construct construct;
    const bool wrapping_spaces;
  };

  if (line.empty()) {
    return Construct::Statement;
  }

  static const Triplet map[] {
    {keywords::condition, Construct::Condition, true},
    {keywords::func_decl, Construct::FuncDecl, true},
    {keywords::assignment, Construct::Assignment, false},
  };

  for (const auto& [keyword, construct, wrapping_spaces] : map) {
    if (util::string::contains_keyword(keyword, line, wrapping_spaces)) {
      return construct;
    }
  }

  if (is_statement(line).first) {
    return Construct::Statement;
  }

  return Construct::Expression;
}

std::pair<std::string_view, std::string_view> parse_statement(std::string_view line) {
  // TODO: Implement
}
} // namespace worm::parse::construct
