
/**
 * @file syntaxe_serveur.c
 * @author ETAME CEDRIC (you@domain.com)
 * @brief  Ce fichier regroupe les differentes fonctions qui serviront à verifier la syntaxe
 * 		   de chaque chaine de caractere attendue par le serveur
 * @version 1.2
 * @date 2021-12-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "syntaxe_serveur.h"

bool point_virgule (char c){
	return c == ';' ;
}

bool deux_points (char c){
	return c == ':' ;
}

bool point (char c){
	return c == '.' ;
}
/* On verifie si la syntaxe globale de la chaine que l'on reçoit est correcte ou pas notamment
 * la chaine "carte:xxxxxxxxxxxxxxxxx;montant:xxxxxx;compte:xxxxxxx;"
 */
bool syntaxe_globale_correcte (char *chaine){
	int i = 0 ;
	int posfound = 0 ;
	bool syntax_correct = false ;
	bool syntax_correct_m = false ;
	bool syntax_correct_compte = false ;
	int size = strlen (chaine) ;
	/*On recupere la position de la derniere lettre du mot clé "carte" donc "e"*/
	posfound = recup_pos_chaine (chaine , "carte") ;
	if (posfound != 0){
		/*Puis on verifie si on retrouve bien à la position exacte les caracteres attendus*/
		if (posfound + 1 < size && chaine [posfound + 1] == ':'){
			int j = posfound + 2 ;
			int compt = 0 ;
			while (j < size && est_chiffre (chaine [j]) && compt < 16){
				compt++ ;
				j++ ;
			}
			if (chaine [j] == ';' && compt == 16){
				syntax_correct = true ;
			}
		}
	}
	/*Processus similaire pour le mot clé "compte"*/
	posfound = recup_pos_chaine (chaine , "compte") ;
	if (posfound != 0){
		if (posfound + 1 < size && chaine [posfound + 1] == ':'){
			int j = posfound + 2 ;
			int compt = 0 ;
			while (j < size && est_chiffre (chaine [j]) && compt < 8){
				compt++ ;
				j++ ;
			}
			if (chaine [j] == ';' && compt == 8){
				syntax_correct_compte = true ;
			}
		}
	}
	/*Processus similaire pour le mot clé "montant"*/
	posfound = recup_pos_chaine (chaine , "montant") ;
	if (posfound != 0){
		if (posfound + 1 < size && chaine [posfound + 1] == ':'){
			int j = posfound + 2 ;
			int compt = 0 ;
			while (j < size && (est_chiffre (chaine [j]) || point (chaine [j])) && compt < 7){
				compt++ ;
				j++ ;
			}
			if (chaine [j] == ';' && compt <= 7){
				syntax_correct_m = true ;
			}
		}
	}
	
	return  syntax_correct && syntax_correct_m && syntax_correct_compte ;
}

/* On verifie si la syntaxe de l'id_terminal est correcte ou pas 
 * Pour cela on procedera de la meme maniere que la syntaxe globale
 */
bool syntaxe_id_correct (char *chaine){
	bool syntax_correct = false ;
	int posfound = recup_pos_chaine (chaine , "id") ;
	int size = strlen (chaine) ;
	if (posfound != 0){
		if (posfound + 1 < size && chaine [posfound + 1] == ':'){
			int j = posfound + 2 ;
			int compt = 0 ;
			while (j < size && est_chiffre (chaine [j]) && compt < 8){
				compt++ ;
				j++ ;
			}
			if (chaine [j] == ';' && compt == 8){
				syntax_correct = true ;
			}
		}
	}
	return syntax_correct ;
}