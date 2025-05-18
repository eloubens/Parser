# Parser

Produces a parse tree representation of the tokenized `WLP4` code provided as input.


### Context:
These are the followings steps for the code compilation process (turning code into something the computer can run):
1) Lexical Analysis → Scanner (tokens)
2) Parsing → Syntax tree
3) Semantic Analysis → Type checks, scopes
4) Intermediate Code Generation → IR (Intermediate Representation)
5) Optimization → Makes IR better
6) Code Generation → Machine code (CPU instructions)
7) Linking & Execution → Final executable

`Parser` is responsible for step #2 in code compliation for the `WLP4` language. `WLP4` is simplified `C++`.


### Code to run:
```
g++ -g -std=c++17 -Wall wlp4parse.cc wlp4data.h -o l
./exec < input.txt > output.txt

wlp4parse < input.txt > expect.txt ## depreciated command
```

### Files

`wlp4data.h`: Is a C++ header file that provides four string constants:
- `WLP4_CFG` is a CFG (context free grammar) component for the augmented WLP4 grammar. It defines the grammar productions for the WLP4 language.
- `WLP4_TRANSITIONS` is a TRANSITIONS component for the WLP4 SLR(1) DFA. This is the DFA (deterministic finite automaton) representation of WLP4_CFG for the parser. Each line represents the current state, the token, and the next transition state for the particular token. This section is used to decide the next parser state when reading the next input token (for shifting). SLR(1) defines the type of DFA -> a simple left-to-right rightmost-derivation parser with 1 symbol lookahead.
- `WLP4_REDUCTIONS` is a REDUCTIONS component for the WLP4 SLR(1) DFA. It is used to decide which grammar rule to apply to reduce a sequence of symbols on the stack.
- `WLP4_COMBINED` is a combination of all the 3 sections  followed by a .END line.

`WLP4.txt`: Contains `WLP4` language specifications.

### Example:
Input:
```
INT int
WAIN wain
LPAREN (
INT int
ID a
COMMA ,
INT int
ID b
RPAREN )
LBRACE {
RETURN return
NUM 42
SEMI ;
RBRACE }
```

Output:
```
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
```

Output Explanation:
1. Notation:
`start BOF procedures EOF` represents the production rule `start -> BOF procedures EOF`.
This production rule is part of a CFG (context-free grammar). A CFG describes the structure of a programming language.

3. Order:
The next line shows the transitions for the children of the current transition.
Eg.
```
start BOF procedures EOF
Then the next lines would be the trasitions for BOF : This would look like `BOF -> BOF`
Then the next lines would be the trasitions for procedures : .........
Then the next lines would be the trasitions for EOF :  This would look like `EOF -> EOF`
```



### Parse Tree Construction:
- The value stored at each node in the tree is either the production rule used to expand the nonterminal at the node (if it is an internal node), or the token kind and lexeme (if it is a leaf node).
- If the parse is successful, the node values, one per line, using a left-to-right preorder traversal of the tree are printed.

### Parse Tree Rendering Algorithm:
*The result of this preorder traversal is a .wlp4i (WLP4 Intermediate) file.*
1. Prints the value at the root of the tree (that is, prints the production rule or the token kind and lexeme, depending on which kind of node).
2. Visits the children in order from left to right.
3. For each child subtree, recursively prints the values in the subtree using a left-to-right preorder traversal.

### Unsuccessful Parsing:
It's possible that the parse will be unsuccessful. This happens if there is no transition on the kind of the next token the parsing DFA tells you to shift. If this occurs, the code output a single line consisting of the string ERROR at k (terminated with a line feed) to standard error, where k is one greater than the number of WLP4 tokens that were successfully read from the input prior to the error. BOF and EOF are not part of the input, so they do not contribute to the value of k. "one token" means a kind and lexeme pair; so each line of the input contains one token.





