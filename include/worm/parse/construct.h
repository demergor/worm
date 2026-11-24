#ifndef WORM_PARSE_CONSTRUCT_H
#define WORM_PARSE_CONSTRUCT_H

#include <string_view>
#include <utility>

namespace worm::parse::construct {
enum class Construct {
  Assignment,
  Condition,
  Expression,
  FuncDecl,
  Loop,
  Statement,
  ShellCmd,
};

/*
 * Reads the given line and determines the exact construct of the line
 *
 * Edge cases:
 * Should ignore empty lines
 *
 * @return: Returns an enum type that specifies the type of construct; it's value is
 * intended to be used at the callsite to choose how the parser should proceed
 */
[[nodiscard]] Construct determine_construct(std::string_view);
[[nodiscard]] std::pair<bool, std::string_view> is_statement(std::string_view);
} // namespace worm::parse::construct

#endif // WORM_PARSE_CONSTRUCT_H
