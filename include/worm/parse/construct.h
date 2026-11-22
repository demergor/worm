#ifndef WORM_PARSE_CONSTRUCT_H
#define WORM_PARSE_CONSTRUCT_H

enum class Construct {
  Assignment,
  Condition,
  FuncDecl,
  Loop,
  Statement,
  ShellCmd,
};

#endif // WORM_PARSE_CONSTRUCT_H
