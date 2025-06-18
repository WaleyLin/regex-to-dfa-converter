# 🧮 Regular Expression to DFA Converter (C++)

This is a C++ command-line tool that converts simplified regular expressions (REs) into deterministic finite automata (DFAs). It supports basic RE operators like:

- Kleene Star: *
- Union (OR): U
- Grouping: ()

The tool is interactive — enter a regular expression, and it will build and print the DFA transitions until you type STOP.

## Features

- Constructs a DFA directly from user-input regular expressions
- Handles 0 and 1 symbols
- Supports:
  - Kleene star: * (zero or more repetitions)
  - Union: (0U1) accepts 0 or 1
  - Group repetition: (01)*
- Automatically handles transitions to reject states
- Final and start states clearly marked in the output

## How to Run

1. Compile the code:
   g++ -std=c++11 -o re_to_dfa re_to_dfa.cpp

2. Run the program:
   ./re_to_dfa

3. Enter a regular expression when prompted. Type STOP to quit.

## Example Regular Expressions

- 01* → 0 followed by zero or more 1s
- (0U1) → accepts 0 or 1
- (0U1)* → any combination of 0s and 1s
- (0U1)1* → any combination followed by 1s
- (0U1)1(0U1) → match specific patterns

## Output Format

Each DFA transition is printed like this:

(current_state, input_symbol) -> next_state

Symbols:
- `qN`: state N
- `q0`: start state (marked with backtick `q0`)
- [qN]: final/accepting state
- reject: dead/reject state

Example output:

(`q0, 0) -> [q1]  
([q1], 1) -> [q1]  
([q1], 0) -> reject

## Author

Waley Lin  
Open an issue or contact me if you have suggestions or feedback!

## Notes

- This tool is designed for educational use and automata theory learning.
- It does not support full regular expression parsing beyond simple structures using 0, 1, *, and U.
