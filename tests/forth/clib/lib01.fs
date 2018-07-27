C-LIB libhello
\C #include <stdio.h>
\C int hello1(int a) { printf("H1: %d+%d\n", a, a); return a+a; }
\C void hello2(int a, int b) { printf("H2: %d %d\n", a, b); }
\C void hello3(int a, int b, int c) { printf("H2: %d %d %d\n", a, b, c); }
\C int hello4(int a, int b, int c) { printf("H2: %d %d %d\n", a, b, c); return a+b+c;}
C-FUNCTION HELLO1 hello1 n -- n
C-FUNCTION HELLO2 hello2 n n
C-FUNCTION HELLO3 hello3 n n n
C-FUNCTION HELLO4 hello4 n n n -- n
END-C-LIB
66 HELLO1 . CR CR
42 10 HELLO2 .S CR
11 12 13 HELLO3 .S CR
11 12 13 HELLO4 . .S CR
