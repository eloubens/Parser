##Parser

This code produces a representation of a parse tree as output.

The input format is simply the output format of the WLP4 scanner.

The output format of the parser is also different. You should construct a parse tree for the program, in which the value stored at each node in the tree is either the production rule used to expand the nonterminal at the node (if it is an internal node), or the token kind and lexeme (if it is a leaf node). If the parse is successful, the print the node values, one per line, using a left-to-right preorder traversal of the tree:

Print the value at the root of the tree (that is, print the production rule or the token kind and lexeme, depending on which kind of node).
Visit the children in order from left to right. For each child subtree, recursively print the values in the subtree using a left-to-right preorder traversal.
The result of this preorder traversal is a .wlp4i (WLP4 Intermediate) file.

In this problem, it is possible that the parse will be unsuccessful. This happens if there is no transition on the kind of the next token when the parsing DFA tells you to shift. If this occurs, output a single line consisting of the string ERROR at k (terminated with a line feed) to standard error, where k is one greater than the number of WLP4 tokens that were successfully read from the input prior to the error.

BOF and EOF are not part of the input, so they do not contribute to the value of k. Also, "one token" means a kind and lexeme pair; so each line of the input contains one token.

```
wlp4data.h is a C++ header file that provides four string constants:
WLP4_CFG is a CFG component for the augmented WLP4 grammar.
WLP4_TRANSITIONS is a TRANSITIONS component for the WLP4 SLR(1) DFA.
WLP4_REDUCTIONS is a REDUCTIONS component for the WLP4 SLR(1) DFA.
WLP4_COMBINED is a single string containing the above three strings followed by a .END line.
```


Code to run:
```
g++ -g -std=c++17 -Wall wlp4parse.cc wlp4data.h -o l
./l < input.txt > output.txt         // my version
wlp4parse < input.txt > expect.txt   // correct version
```



Output explained:
```
output:
start BOF procedures EOF
BOF BOF
procedures main
main INT WAIN LPAREN dcl COMMA dcl RPAREN LBRACE dcls statements RETURN expr SEMI RBRACE
INT int
WAIN wain
LPAREN (
dcl type ID
type INT
INT int
ID foo
COMMA ,
dcl type ID
type INT
INT int
ID bar
RPAREN )
LBRACE {
dcls .EMPTY
statements .EMPTY
RETURN return
expr term
term factor
factor NUM
NUM 42
SEMI ;
RBRACE }
EOF EOF


Explanation:
1. Notation:
start BOF procedures EOF
means
start -> BOF procedures EOF

2. Order:
The next line would the transitions for the children of the current transition.
Eg.
start BOF procedures EOF  =  start -> BOF procedures EOF
Then the next lines would be the trasitions for BOF : BOF -> BOF
Then the next lines would be the trasitions for procedures : there are a lot.
Then the next lines would be the trasitions for EOF :  EOF -> EOF

```
