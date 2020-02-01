# Bison&Flex calculator
Calculator using Flex & Bison, with additional functions,loops, and conditions, with the ability to create variables and functions.

## Functionality
  * Support of C-style (single-line, multi-line) and Unix-style (single-line) comments
  * Hexadecimal (ex. `0x9A`), Octal (ex. `0o77`), Binary(ex. `ob1011`) and Decimal(ex. `9581`) numeral systems
  * Floating point numbers and calculations (ex. `49.21`, `0x9A.AA`, `0o57.72`, `0b10.01`, `95.03 + 0x7F.42`...)
  * Arithmetic following BODMAS rules
  * Build-in functions (`sin`, `cos`, `mod`, `pow`, `sqrt`, `abs`, `floor`, `ceil`...)
  * Constant values (`pi()`, `e()`)
  * Creation (`var a = 10;`) and usage (`a+20*a;`) of own variables
  * Creation (`let add_and_print(a, b)=var a= a+b; print(a); end;`) and usage (`add_and_print(10, 20);`) of own functions
  * `if then end`, `if then else end` and `while do end` constructions

## Example
Files `test1.calc` and `test2.calc` consist of example usages of this program.

## Requirements
```
bison>=3.3.2
flex>=2.5.4
gcc>=8.3.0
```

## Compile and Run
```
make clean
make all
./calc
```

## TODO
  * [ ] Reduce RAM usage
  * [ ] Rewrite AST structure
  * [ ] Add `while do` loop
  * [ ] Add `and`, `or`, `not` boolean operators
  * [ ] Add *strings* support

