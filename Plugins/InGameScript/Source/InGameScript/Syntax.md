#In-Game Script Syntax

This document includes the syntax of in-game script.

## Accidence

```
- digit     -> [0-9]
- digits    -> digital+
- letter    -> [A-Za-z]
- id        -> letter(letter|digit)*
- if        -> if
- else      -> else
- for       -> for
- while     -> while
- break     -> break
- continue  -> continue
- binaryOp  -> +|-|*|/|%|=|==|>|<|>=|<=|!=
- singleOp  -> -|!
```

## Operator Priority
```
- ()
- - !
- * / %
- + -
- == != > < >= <=
- =
```

## Syntax

```
- program       -> block
- scope         -> '{' block '}'
- block         -> stmt [stmt]
- stmt          -> if (expr) block | for (expr;expr;expr) block | while (expr) block | stmt expr | expr
- expr          -> expr + term | expr - term | term
- term          -> term * factor | term / factor | term % factor | factor
- singleOpExp   -> singleOp atomicexp
- atomicexp     -> id | (expr)
- index         -> '[' expr ']'
- suffixexp     -> atomicexp { '[' index ']' | '::' atomicexp funcargs | funcargs }
```

```
- expr -> expr + term | term
->
- expr -> term rest 
- rest -> + term rest
```

##Example

- Basic Statement
```
// End with ;
a = 1;
```
- Binary operation statement
```
// expr -> (preexpr | unop expr) [binop expr]
a + b
-a + b
```
- Prefix operation
```
// preexpr -> singleop atomic
!a
-a
```
- Suffix expression
```
// suffixexpr -> atomexpr {'.' varname funcargs | '[' expr ']'}
// funcargs -> '(' {atomexpr','} ')'
a.Add(i)
a[i]
```
- Atomic expression
```
// atomexpr -> varname | (expr)
```
- Statement block
```
// block -> '{'{stat}'}'
{
    a = 1;
    b = 2;
}
```
- Variable definition
```
// vardef -> type name [assignsuffix]';'
// assignsuffix -> '=' expr
int a;
int b = 1;
```
- Assignment
```
// assign -> name assignsuffix';'
a = 1;
```
- If statement
```
// ifstat -> if (expr) block [else block]
if (a > 1)
{
    a = a + 1;
}

// ifstat -> if (expr) block [else block]
if (a > 1)
{
    a = a + 1;
}
else
{
    a = a - 1;
}
```
- for loop
```
// forstat -> for (expr;cond;expr) block
// Stop when cond value equals false
for (int i = 0; i < 4; i = i + 1)
{
    a = a + i;
}

// forstat -> for (var in expr) block
for (int i in Range)
{
    a = a + 1;
}
```
- while loop
```
// while (cond) block
// Stop when cond value equals false
while (i < 4)
{
    a = a + i;
}
```
- function definition
```
// funcdef -> type name funcparms block
// funcparms -> '(' {vardef','} ')'
int Add(int a, int b)
{
    return a + b;
}
```
