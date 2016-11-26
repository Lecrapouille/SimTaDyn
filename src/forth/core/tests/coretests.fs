\ Inspired from https://github.com/gerryjackson/forth2012-test-suite
\ Adapted for SimForth

INCLUDE ../core/tester.fs

HEX

T{ -> }T
T{ 1 2 -> 1 2 }T

T{ 0 0 AND -> 0 }T
T{ 0 1 AND -> 0 }T
T{ 1 0 AND -> 0 }T
T{ 1 1 AND -> 1 }T

T{ 0 0= -> TRUE }T
T{ 1 0= -> FALSE }T
T{ 2 0= -> FALSE }T
T{ -1 0= -> FALSE }T

T{ 0 0 = -> TRUE }T
T{ 1 1 = -> TRUE }T
T{ -1 -1 = -> TRUE }T
T{ 1 0 = -> FALSE }T
T{ -1 0 = -> FALSE }T
T{ 0 1 = -> FALSE }T
T{ 0 -1 = -> FALSE }T

T{ 0 1 < -> TRUE }T
T{ 1 2 < -> TRUE }T
T{ -1 0 < -> TRUE }T
T{ -1 1 < -> TRUE }T
T{ 0 0 < -> FALSE }T
T{ 1 1 < -> FALSE }T
T{ 1 0 < -> FALSE }T
T{ 2 1 < -> FALSE }T
T{ 0 -1 < -> FALSE }T
T{ 1 -1 < -> FALSE }T

T{ 0 1 > -> FALSE }T
T{ 1 2 > -> FALSE }T
T{ -1 0 > -> FALSE }T
T{ -1 1 > -> FALSE }T
T{ 0 0 > -> FALSE }T
T{ 1 1 > -> FALSE }T
T{ 1 0 > -> TRUE }T
T{ 2 1 > -> TRUE }T
T{ 0 -1 > -> TRUE }T
T{ 1 -1 > -> TRUE }T

T{ 0 1 MIN -> 0 }T
T{ 1 2 MIN -> 1 }T
T{ -1 0 MIN -> -1 }T
T{ -1 1 MIN -> -1 }T
T{ 0 0 MIN -> 0 }T
T{ 1 1 MIN -> 1 }T
T{ 1 0 MIN -> 0 }T
T{ 2 1 MIN -> 1 }T
T{ 0 -1 MIN -> -1 }T
T{ 1 -1 MIN -> -1 }T

T{ 0 1 MAX -> 1 }T
T{ 1 2 MAX -> 2 }T
T{ -1 0 MAX -> 0 }T
T{ -1 1 MAX -> 1 }T
T{ 0 0 MAX -> 0 }T
T{ 1 1 MAX -> 1 }T
T{ 1 0 MAX -> 1 }T
T{ 2 1 MAX -> 2 }T
T{ 0 -1 MAX -> 0 }T
T{ 1 -1 MAX -> 1 }T

( T{ 0 INVERT 1 AND -> 1 }T
T{ 1 INVERT 1 AND -> 0 }T )

T{ 1 0 LSHIFT -> 1 }T
T{ 1 1 LSHIFT -> 2 }T
T{ 1 2 LSHIFT -> 4 }T
T{ 1 F LSHIFT -> 8000 }T         \ BIGGEST GUARANTEED SHIFT
\ T{ 1S 1 LSHIFT 1 XOR -> 1S }T

T{ 1 0 RSHIFT -> 1 }T
T{ 1 1 RSHIFT -> 0 }T
T{ 2 1 RSHIFT -> 1 }T
T{ 4 2 RSHIFT -> 1 }T
T{ 8000 F RSHIFT -> 1 }T \ BIGGEST

\ T{ 0 INVERT -> TRUE }T
T{ FALSE -> 0 }T

T{ 0 0 <> -> FALSE }T
T{ 1 1 <> -> FALSE }T
T{ -1 -1 <> -> FALSE }T
T{ 1 0 <> -> TRUE }T
T{ -1 0 <> -> TRUE }T
T{ 0 1 <> -> TRUE }T
T{ 0 -1 <> -> TRUE }T

T{ 0 0<> -> FALSE }T
T{ 1 0<> -> TRUE }T
T{ 2 0<> -> TRUE }T
T{ -1 0<> -> TRUE }T

T{ 0 0> -> FALSE }T
\ ko T{ -1 0> -> FALSE }T
T{ 1 0> -> TRUE }T

T{ 1 2 NIP -> 2 }T
T{ 1 2 3 NIP -> 1 3 }T

T{ 1 2 TUCK -> 2 1 2 }T
T{ 1 2 3 TUCK -> 1 3 2 3 }T

T{ : RO5 100 200 300 400 500 ; -> }T
\ T{ RO5 3 ROLL -> 100 300 400 500 200 }T
\ ko T{ RO5 2 ROLL -> RO5 ROT }T
\ T{ RO5 1 ROLL -> RO5 SWAP }T
\ T{ RO5 0 ROLL -> RO5 }T

T{ RO5 2 PICK -> 100 200 300 400 500 300 }T
T{ RO5 1 PICK -> RO5 OVER }T
T{ RO5 0 PICK -> RO5 DUP }T

\ T{ 0 0 0 WITHIN -> FALSE }T

( T{ : AG0 701 BEGIN DUP 7 MOD 0= IF EXIT THEN 1+ AGAIN ; -> }T
T{ AG0 -> 707 }T )

T{ 1 2 2DROP -> }T
T{ 1 2 2DUP -> 1 2 1 2 }T
T{ 1 2 3 4 2OVER -> 1 2 3 4 1 2 }T
T{ 1 2 3 4 2SWAP -> 3 4 1 2 }T
T{ 0 ?DUP -> 0 }T
T{ 1 ?DUP -> 1 1 }T
T{ -1 ?DUP -> -1 -1 }T
T{ DEPTH -> 0 }T
T{ 0 DEPTH -> 0 1 }T
T{ 0 1 DEPTH -> 0 1 2 }T
T{ 0 DROP -> }T
T{ 1 2 DROP -> 1 }T
T{ 1 DUP -> 1 1 }T
T{ 1 2 OVER -> 1 2 1 }T
T{ 1 2 3 ROT -> 2 3 1 }T
T{ 1 2 SWAP -> 2 1 }T

T{ : GR1 >R R> ; -> }T
\ T{ : GR2 >R R@ R> DROP ; -> }T
T{ 123 GR1 -> 123 }T
\ T{ 123 GR2 -> 123 }T
T{ 1 GR1 -> 1 }T

T{ 0 5 + -> 5 }T
T{ 5 0 + -> 5 }T
T{ 0 -5 + -> -5 }T
T{ -5 0 + -> -5 }T
T{ 1 2 + -> 3 }T
T{ 1 -2 + -> -1 }T
T{ -1 2 + -> 1 }T
T{ -1 -2 + -> -3 }T
T{ -1 1 + -> 0 }T

T{ 0 5 - -> -5 }T
T{ 5 0 - -> 5 }T
T{ 0 -5 - -> 5 }T
T{ -5 0 - -> -5 }T
T{ 1 2 - -> -1 }T
T{ 1 -2 - -> 3 }T
T{ -1 2 - -> -3 }T
T{ -1 -2 - -> 1 }T
T{ 0 1 - -> -1 }T

T{ 0 1+ -> 1 }T
T{ -1 1+ -> 0 }T
T{ 1 1+ -> 2 }T

T{ 2 1- -> 1 }T
T{ 1 1- -> 0 }T
T{ 0 1- -> -1 }T

T{ 0 NEGATE -> 0 }T
T{ 1 NEGATE -> -1 }T
T{ -1 NEGATE -> 1 }T
T{ 2 NEGATE -> -2 }T
T{ -2 NEGATE -> 2 }T

T{ 0 ABS -> 0 }T
T{ 1 ABS -> 1 }T
T{ -1 ABS -> 1 }T

T{ 0 0 * -> 0 }T            \ TEST IDENTITIES
T{ 0 1 * -> 0 }T
T{ 1 0 * -> 0 }T
T{ 1 2 * -> 2 }T
T{ 2 1 * -> 2 }T
T{ 3 3 * -> 9 }T
T{ -3 3 * -> -9 }T
T{ 3 -3 * -> -9 }T
T{ -3 -3 * -> 9 }T

T{ : GI1 IF 123 THEN ; -> }T
T{ : GI2 IF 123 ELSE 234 THEN ; -> }T
T{ 0 GI1 -> }T
T{ 1 GI1 -> 123 }T
T{ -1 GI1 -> 123 }T
T{ 0 GI2 -> 234 }T
T{ 1 GI2 -> 123 }T
T{ -1 GI1 -> 123 }T

T{ : GD1 DO I LOOP ; -> }T
T{ 4 1 GD1 -> 1 2 3 }T
T{ 2 -1 GD1 -> -1 0 1 }T

T{ : GD3 DO 1 0 DO J LOOP LOOP ; -> }T
T{ 4 1 GD3 -> 1 2 3 }T
T{ 2 -1 GD3 -> -1 0 1 }T

T{ : GD5 123 SWAP 0 DO I 4 > IF DROP 234 LEAVE THEN LOOP ; -> }T
T{ 1 GD5 -> 123 }T
T{ 5 GD5 -> 123 }T
T{ 6 GD5 -> 234 }T

T{ VARIABLE V1 -> }T
T{ 123 V1 ! -> }T
T{ V1 @ -> 123 }T

T{ : NOP : POSTPONE ; ; -> }T
T{ NOP NOP1 NOP NOP2 -> }T
T{ NOP1 -> }T
T{ NOP2 -> }T

T{ CREATE CR1 -> }T
T{ CR1 -> HERE }T
\ T{ ' CR1 >BODY -> HERE }T
T{ 1 , -> }T
T{ CR1 @ -> 1 }T



\ should support {1..31} graphical characters
: !"#$%&'()*+,-./0123456789:;<=>? 1 ;
T{ !"#$%&'()*+,-./0123456789:;<=>? -> 1 }T
: @ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^ 2 ;
T{ @ABCDEFGHIJKLMNOPQRSTUVWXYZ[\]^ -> 2 }T
: _`abcdefghijklmnopqrstuvwxyz{|} 3 ;
T{ _`abcdefghijklmnopqrstuvwxyz{|} -> 3 }T
: _`abcdefghijklmnopqrstuvwxyz{|~ 4 ;     \ Last character different
T{ _`abcdefghijklmnopqrstuvwxyz{|~ -> 4 }T
T{ _`abcdefghijklmnopqrstuvwxyz{|} -> 3 }T

VARIABLE OLD-BASE
DECIMAL
BASE OLD-BASE !
41 . CR

T{ #1289 -> 1289 }T
T{ #-1289 -> -1289 }T
T{ $12eF -> 4847 }T
T{ $-12eF -> -4847 }T
T{ %10010110 -> 150 }T
T{ %-10010110 -> -150 }T
T{ 'z' -> 122 }T
T{ 'Z' -> 90 }T
\ Check BASE is unchanged
T{ BASE OLD-BASE @ = -> TRUE }T
41 . CR . CR
41 . CR

\ Repeat in Hex mode
16 OLD-BASE !
16 BASE
T{ #1289 -> 509 }T
T{ #-1289 -> -509 }T
T{ $12eF -> 12EF }T
T{ $-12eF -> -12EF }T
T{ %10010110 -> 96 }T
T{ %-10010110 -> -96 }T
T{ 'z' -> 7a }T
T{ 'Z' -> 5a }T
43 . CR
\ Check BASE is unchanged
T{ BASE OLD-BASE @ = -> TRUE }T   \ 2
44 . CR . CR
BASE . CR OLD-BASE @ . CR

DECIMAL
\ Check number prefixes in compile mode
T{ : nmp  #8327 $-2cbe %011010111 ''' ; nmp -> 8327 -11454 215 39 }T

( T{ : UNS1 DUP 0 > IF 9 SWAP BEGIN 1+ DUP 3 > IF EXIT THEN REPEAT ; -> }T
T{ -6 UNS1 -> -6 }T
T{ 1 UNS1 -> 9 4 }T )

42 .
