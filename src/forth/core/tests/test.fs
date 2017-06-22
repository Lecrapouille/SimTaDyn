( COMMENTAIRE ( IMBRIQUE ) OU PAS ) \ OU est inconnu /* /* */ */ou alors  
) \ erreur ou pas ?
: 65536 HERE ! HERE @ \ doit donner 65536
: 65536 42 ; 65536 . \ Doit donner 42

42 R> R> . \ ca segfault sur pforth fonction sur gforth marchait sur simForth avec la pile alternative

HEX
CREATE QUATRE
4 HERE !
HERE @ . CR \ 4
QUATRE @ . CR \ 4

CREATE DEUX
2 HERE !
DEUX @ . CR \ 2
QUATRE @ . CR \ Ne doit pas afficher 2

4 VARIABLE QUATRE
QUATRE ?
2 QUATRE +! QUATRE ?
65536 QUATRE ! QUATRE ?

: GT1 123 ; ' GT1 EXECUTE . \ 123
