// https://tree-sitter.github.io/tree-sitter/using-parsers
// https://github.com/tree-sitter/tree-sitter/issues/1469 # CST Traversal speed
// https://github.com/tree-sitter/tree-sitter/issues/1277 # Tree-sitter on large files

#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <tree_sitter/api.h>
#include <stdlib.h>

// subprojects/tree-sitter-nix/src/tree-sitter-nix/src/node-symbols-enum.h
#include "node-symbols-enum.h"

// Declare the `tree_sitter_nix` function, which is
// implemented by the `tree-sitter-nix` library.
// subprojects/tree-sitter-nix/src/tree-sitter-nix/src/tree_sitter/parser.h:struct TSLanguage {
// subprojects/tree-sitter-nix/src/tree-sitter-nix/src/parser.c:extern const TSLanguage *tree_sitter_nix(void) {
TSLanguage *tree_sitter_nix();

void print_indent(int depth) {
  for (int i = 0; i < depth; i++) {
    printf("  ");
  }
}

int main(int argc, const char** argv) {
  if (argc != 3) {
    printf("usage: %s -f input-file.nix\n", argv[0]);
    return 1;
  }
  if (strcmp(argv[1], "-f") != 0) {
    printf("usage: %s -f input-file.nix\n", argv[0]);
    return 1;
  }

  const char* source_file_path = argv[2];

  // https://github.com/tree-sitter/tree-sitter/issues/1277
  // Copy the file into BUFFER.
  FILE *file = fopen(source_file_path, "rb");
  fseek(file, 0, SEEK_END);
  long length = ftell (file);
  fseek(file, 0, SEEK_SET);
  char *buffer = malloc (length);
  // FIXME warning: ignoring return value of ‘fread’
  long actual_length = fread(buffer, 1, length, file);
  if (actual_length != length) {
    fprintf(stderr, "warning: fread returned %li of %li bytes\n", actual_length, length);
  }
  fclose (file);

  // Create a parser.
  TSParser *parser = ts_parser_new();

  // Set the parser's language (nix in this case).
  ts_parser_set_language(parser, tree_sitter_nix());

  // Build a syntax tree based on source code stored in a string.
  //const char *source_code = "1\n+\n1\n"; // 1+1
  //const char *source_code = argv[1];
  const char *source_code = buffer;

  TSTree *tree = ts_parser_parse_string(
    parser,
    NULL,
    source_code,
    strlen(source_code)
  );

  // Get the root node of the syntax tree.
  TSNode root_node = ts_tree_root_node(tree);

  TSTreeCursor cursor = ts_tree_cursor_new(root_node);

  // inorder cursor traversal
  // https://github.com/tree-sitter/py-tree-sitter/issues/33#issuecomment-864557166

  bool reached_root = false;
  int depth = 0;
  bool ascending = false;
  TSSymbol parent_symbols[1024*1024]; // TODO dynamic size to support "infinite" stack size

  while (reached_root == false) {
    TSNode node = ts_tree_cursor_current_node(&cursor);
    const char* node_type = ts_node_type(node);
    // typedef uint16_t TSSymbol;
    TSSymbol node_symbol = ts_node_symbol(node);
    char* node_string = ts_node_string(node); // s-expression of node
    TSPoint node_start_point = ts_node_start_point(node);

    // handle current node

    printf("\n"); // empty line before node

    // codegen: macro with source location
    // row is zero-based, line is one-based
    //printf("# %i \"%s\"\n", node_start_point.row, source_file_path);
    //print_indent(depth);
    //printf("#line %i \"%s\"\n", (node_start_point.row + 1), source_file_path);

    // codegen: comment with s-expression
    print_indent(depth);
    printf("// open: %i = %s: %s\n", node_symbol, node_type, node_string);

    // codegen: C code
    // TODO non-recursive fast term-rewriting -> functional language?
    // guile scheme, chicken scheme, cakelisp, ...
    // TODO writing a compiler in C
    // https://www.cs.hmc.edu/~fleck/envision/compiler/steps7-9-c.html # Compiler Steps 7' to 9': C code generation
    // https://duckduckgo.com/?q=compiler+code+generation+in+c
    // codegen: open node
    //printf("// opening node symbol %i\n", node_symbol);
    switch (node_symbol) {
      case sym_source_code:
        //print_indent(depth); // depth is zero
        printf("#include <assert.h>\n");
        printf("#include <string.h>\n");
        printf("#include <stdio.h>\n");
        printf("#include <stdlib.h>\n");

        print_indent(depth);
        printf("#line %i \"%s\"\n", (node_start_point.row + 1), source_file_path);
        //print_indent(depth);
        printf("void *eval(void *input) {\n");

        print_indent(depth);
        printf("#line %i \"%s\"\n", (node_start_point.row + 1), source_file_path);
        //print_indent(depth);
        printf("  void *result = NULL;\n");

        break;
    }

    // go to next node

    if (ts_tree_cursor_goto_first_child(&cursor)) {
      // store symbol for later -> codegen: close node
      parent_symbols[depth] = node_symbol;
      depth++;
      continue;
    }
    if (ts_tree_cursor_goto_next_sibling(&cursor)) {
      continue;
    }
    ascending = true;
    while (ascending) {
      if (ts_tree_cursor_goto_parent(&cursor)) {
        // codegen: close node
        print_indent(depth);
        depth--;
        //printf("// closing node symbol %i\n", parent_symbols[depth]);
        printf("// close: %i\n", parent_symbols[depth]);
        switch (parent_symbols[depth]) {
          case sym_source_code:
            print_indent(depth);
            printf("  return result;\n");
            print_indent(depth);
            printf("}\n");
            break;
        }
        //printf("\n"); // empty line after node
      }
      else {
        ascending = false;
        reached_root = true;
      }
      if (ts_tree_cursor_goto_next_sibling(&cursor)) {
        ascending = false;
        //printf("\n"); // empty line after node
      }
    }
  }

/*
bool ts_tree_cursor_goto_first_child(TSTreeCursor *);
bool ts_tree_cursor_goto_next_sibling(TSTreeCursor *);
bool ts_tree_cursor_goto_parent(TSTreeCursor *);

TSNode ts_tree_cursor_current_node(const TSTreeCursor *);
const char *ts_tree_cursor_current_field_name(const TSTreeCursor *);
TSFieldId ts_tree_cursor_current_field_id(const TSTreeCursor *);
*/

  // Get some child nodes.
/*
  TSNode array_node = ts_node_named_child(root_node, 0);
  TSNode number_node = ts_node_named_child(array_node, 0);

  // Check that the nodes have the expected types.
  assert(strcmp(ts_node_type(root_node), "document") == 0);
  assert(strcmp(ts_node_type(array_node), "array") == 0);
  assert(strcmp(ts_node_type(number_node), "number") == 0);

  // Check that the nodes have the expected child counts.
  assert(ts_node_child_count(root_node) == 1);
  assert(ts_node_child_count(array_node) == 5);
  assert(ts_node_named_child_count(array_node) == 2);
  assert(ts_node_child_count(number_node) == 0);
*/

  /*
  // Print the syntax tree as an S-expression.
  char *string = ts_node_string(root_node);
  printf("Syntax tree: %s\n", string);

  // Free all of the heap-allocated memory.
  free(string);
  */
  free(buffer);
  ts_tree_delete(tree);
  ts_parser_delete(parser);
  return 0;
}
