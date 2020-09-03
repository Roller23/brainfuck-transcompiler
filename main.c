#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define OUTPUT_NAME "output.c"
#define LUT_SIZE 0xFF
#define AVAILABLE_MEMORY "1024"

#if defined(__clang__)
  #define COMPILER "clang"
#else
  #define COMPILER "gcc"
#endif

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Not enough arguments\n");
    return 1;
  }
  char *input_name = argv[1];
  char *commands[LUT_SIZE];
  memset(commands, 0, LUT_SIZE * sizeof(char *));
  commands['>'] = "++p;";
  commands['<'] = "--p;";
  commands['+'] = "++*p;";
  commands['-'] = "--*p;";
  commands['.'] = "putchar(*p);";
  commands[','] = "*p=getchar();";
  commands['['] = "while(*p){";
  commands[']'] = "}";
  FILE *output = fopen(OUTPUT_NAME, "w");
  FILE *input = fopen(input_name, "r");
  if (output == NULL || input == NULL) {
    printf("File error\n");
    return 1;
  }
  fseek(input, 0, SEEK_END);
  int size = ftell(input);
  fseek(input, 0, SEEK_SET);
  char *brainfuck_code = calloc(size + 1, sizeof(char));
  fread(brainfuck_code, 1, size, input);
  fclose(input);
  char program_template[] = "#include <stdio.h>\n"
                            "char m[" AVAILABLE_MEMORY "];\n"
                            "char *p = m;\n"
                            "int main(void) {%s return 0;}";
  char *code = NULL;
  int code_size = 0;
  int i = 0;
  while (brainfuck_code[i]) {
    if (commands[brainfuck_code[i]] == NULL) {
      i++;
      continue;
    }
    int length = strlen(commands[brainfuck_code[i]]);
    code = realloc(code, code_size + length + 1);
    strcpy(code + code_size, commands[brainfuck_code[i]]);
    code_size += length;
    i++;
  }
  int output_length = strlen(code) + strlen(program_template);
  char *final_output = calloc(output_length + 1, sizeof(char));
  sprintf(final_output, program_template, code);
  fputs(final_output, output);
  fclose(output);
  free(code);
  free(final_output);
  char template[] = COMPILER " -o %s " OUTPUT_NAME;
  char *selected_name = argc > 2 ? argv[2] : "brainfart";
  int mem = strlen(template) + strlen(selected_name) + 1;
  char *system_command = calloc(mem, sizeof(char));
  sprintf(system_command, template, selected_name);
  system(system_command);
  free(system_command);
  remove(OUTPUT_NAME);
  printf("Done\n");
  return 0;
}