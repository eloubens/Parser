# Parser
Write a scanner for WLP4. The scanner should read a WLP4 program from standard input and tokenize it, producing a sequence of lines, each representing one token of the program. Each line should be terminated by a line feed (ASCII 0x0A) and should contain the token kind (the all-caps name indicating the type of the token, such as ID, NUM, LPAREN, etc. as defined in the WLP4 Specification), followed by a single space, followed by the token lexeme (the actual string represented by the token).

Whitespace and comments are not considered tokens and should not be part of the output. However, tokens in the input might be separated by whitespace or comments.

If the input cannot be scanned into a sequence of valid tokens as per the requirements in the WLP4 Specification, your program must print an error message containing the string ERROR in all caps to standard error and exit normally.

For programs with a valid tokenization, your output should be identical to that of the wlp4scan tool.

Clarifications
- If a NUM token's numeric value is out of range (that is, it represents a number strictly greater than 231 - 1 = 2147483647) you should report an error.
- If you see something like 2147483647999, you should treat it as one NUM token, and report an error since it is out of range, rather than treating it as two in-range NUM tokens (2147483647 and 999).
- In the earlier Simplified Maximal Munch scanner problem, you were provided a scanning DFA, followed by a string to scan using the DFA. For this problem, the input to your program is simply WLP4 source code. That is, the input will not include a DFA for WLP4 tokens. You will likely have to create such a DFA yourself and embed it in your program somehow.
- If you want to reuse your DFA reading code from the earlier problems, you could embed it as a multi-line string literal (these are called raw strings in C++ and here strings in Racket) and then read it from a stringstream (C++) or using open-input-string (Racket).


Code to run:
./wlp4scan < input.txt > output.txt // my version
wlp4scan < input.txt > expect.txt   // correct version

