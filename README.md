# Brainfuck transcompiler
A brainfuck transpiler (brainfuck to C) written in C. Clang or GCC required

Set the amount of available memory for brainfuck programs when compiling the transpiler with -DAVAILABLE_MEMORY="..." (default is 1024)
Or just modify the source file

Usage:

./bcc \<input file\> \<output name (optional)\>

(assuming the transpiler was compiled to "bcc")
