
#ifndef ERROR_H
#define ERROR_H
	
	#define ERR_OK 0 
	#define ERR_SCANNER 1
	#define ERR_PARSER 2
	
	#define ERR_DEF 3 //nedef. funkcia/premenna 
	#define ERR_DEF_TYPE 4 //chyba pri pdvpdzovaní dat. typu novej premennej
	#define ERR_MATH_TYPE 5 //kompatibilita pri arit/rel/retaz. výrazoch
	#define ERR_FUNC 6 //zlý počet param./typov alebo návratových hodnôt pri volaní či návratu z funkcie
	#define ERR_SEM_OTHR 7 //ostatné sémantické chyby
	
	#define ERR_DIV_ZERO 9 //delenie nulou
	#define ERR_INTERNAL 99 //Interna chyba prekladača
#endif 