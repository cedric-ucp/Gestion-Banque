#ifndef __SYNTAXE__SERVEUR__H__
#define __SYNTAXE__SERVEUR__H__

/**
 * @file syntaxe_serveur.h
 * @author ETAME CEDRIC 
 * @brief Ce fichier regroupe les prototypes des fonctions qui verifient la syntaxe des
 *        chaines de caracteres recues par le serveur ainsi que les CONSTANTES des
 *        codes erreurs
 * @version 1.2
 * @date 2021-12-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#define SYNTAX_ERROR 200 // pour toute erreur de syntaxe

#define NUM_CARTE_NO_EXISTS 500 // si num_carte n'existe pas dans la BD
#define MONTANT_INCORRECT 600 // si le montant est incorrect
#define ESPACE 100
#define ID_NO_EXISTS 300 // si id_terminal n'existe pas dans la BD
#define NUM_COMPTE_NO_EXISTS 400 // si un num de carte

#include <stdbool.h>




bool est_chiffre (char c) ;
bool point_virgule (char c) ;
bool deux_points (char c) ;
bool syntaxe_globale_correcte (char *chaine) ;
int syntaxe_id_terminal (char *chaine , int *type_erreur) ;
bool syntaxe_id_correct (char *chaine) ;

#endif