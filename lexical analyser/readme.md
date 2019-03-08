# Lexical analyser in C using automata

This program was done as part of an assignment of Compiler Design

## Problem

Write a C program to implement lexical analyser which takes a C program
as input and outputs the following categories of tokens and the line number
in which they occur in the input source code. 

```
1. Relational operator : RELOP
2. Identifier (includes variable name, array name, struct name etc): ID
3. Keyword: KEY
4. Integer Constant: INT_CONST
5. Any arithmetic operator like +,-,/,* : ARITH_OP
```
Use finite automata logic for performing lexical analysis.

The output must be in the form of *Line number <token, lexeme>*

Example:

```
Line 4 <ID, total>
Line 6 <RELOP,”>”>
Line 8 <INT_CONST,67>
Line 10 <KEY, for>
Line 10 <ID,i>
Line 10 <ARITH_OP,->
Line 10 <INT_CONST, 10>
Line 12 <ARITH_OP,+>
Line 15 <KEY,while>
```