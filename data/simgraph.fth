0 VARIABLE CEL
0 VARIABLE D
0 VARIABLE RES


0 VARIABLE DISTX
0 VARIABLE DISTY
: DISTANCE 
	DISTX ! DISTY ! 
      	DISTX @ - DISTX !
	DISTY @ - DISTY !
	DISTX @ DISTX @ *
	DISTY @ DISTY @ *
	+ SQRT 
;

: DIST 
	DUP >R A>S 
	>R DUP >R
	POSY SOMMET @
	R> POSX SOMMET @
	R>  DUP >R POSY SOMMET @
	R> POSX SOMMET @
	DISTANCE
	R> COUT ARC !
;

: MULT DUP POSZ SOMMET @ 10 * >R POSZ SOMMET R> SWAP ! ; 

: MIN 2DUP IF >R DROP R> ELSE DROP FI ;

0 VARIABLE II
0 VARIABLE IA
0 VARIABLE VAL
0 VARIABLE TEMP
: BELLMAN
	DUP CEL !
	0 VAL ! 0 TEMP !
	DEG D !
	0 D @ NDO		
		DUP II !
		CEL @ II @ IARC IA !
		( IA @ II @ EXTREMITE POSZ SOMMET @
		TEMP !
		VAL @ IA COUT ARC @ TEMP @ + MIN
		VAL ! )
		. 1+
	NLOOP
	DROP
	( VAL @ CEL POSZ SOMMET ! )
;

( MOYENNE DES HAUTEURS DES VOISINS D'UN SOMMET )
: MOY ( cel --- )
	0 RES !
	DUP DUP CEL ! ( cel cel --- )
	EXISTS IF ( cel --- )
	       DEG D ! ( --- )
	       0 ( iter --- )
	       D @  0 DO 
	       	       DUP >R CEL @ ( iter cel --- ) ( iter --- )
		       IVOISIN
		       POSZ SOMMET @ RES @ +
		       RES ! 
		       R> 1+ ( iter+1 --- )
	       LOOP DROP
	       RES @ D @ / ( moy --- )
	       CEL @ POSZ SOMMET !
	ELSE
		DROP
	FI
;