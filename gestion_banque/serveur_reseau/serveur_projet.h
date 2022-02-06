/**
 * @file serveur_projet.h
 * @author ETAME CEDRIC (you@domain.com)
 * @brief  Ce fichier regroupe les prototypes de fonctions qui forment le serveur réseau
 * @version 1.2
 * @date 2021-12-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#endif

#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include "lib_postgres/libpq-fe.h"
#include <stdlib.h>
#include <time.h>
#include <errno.h>
#include "syntaxe_serveur.h"
#include <stdbool.h>


#ifndef __SERVEUR__PROJET__H__
#define __SERVEUR__PROJET__H__

//définition de constantes (il n'y aura pas besoin de chercher dans le code toutes les occurences d'une valeur qui se répeterait)
#define PORT_DEFAUT 5000
#define TAILLE_BUFFER 500

#define SOLDE_INTROUVABLE 50
#define ID_UNIQUE 10
#define ECHEC_REQUETE 20
#define SOCIETE_INTROUVABLE 30
#define ID_NO_EXISTS 300 // si id_terminal n'existe pas dans la BD
#define NUM_COMPTE_NO_EXISTS 400 // si un num de carte n'existe pas
#define INSERTION_ECHOUEE 60 // Insertion a echouee
#define MONTANT_INSUFFISANT 70

void exit_dial (PGconn *conn) ;
void bzeroWindows (void *blocs , size_t taille) ;
int readWindows (int socket , char *buffer , int taille_buffer) ;
int writeWindows (int socket , char *message , int taille_message) ;

void get_date (char *date) ;
void get_hour (char *date) ;

PGresult *effectuer_paiement (PGconn *connect , double montant , char *num_carte , char *id_terminal , char *compte_terminal , int *code_retour) ;
void recuperer_num_compte (char *num_carte , PGconn *connect , int *code_retour , char **num_compte) ;
bool unicite_id (char *id , const char *table , const char *nom_attribut , PGconn *connect , int *code_retour) ;
double get_solde_client (char *num_compte , PGconn *connect , int *code_retour) ;
char *num_transaction_aleatoire (char *num_transaction , PGconn *connect , int  *code_retour) ;
void reguler_solde (char *num_compte_debiteur , char *num_compte_crediteur , double montant , double solde , PGconn *connect , int *code_retour);
PGconn *se_connecter_bd (PGconn *connect) ;
bool montant_correct (char *montant) ;
bool caract_correcte (char carac) ;
void concatener_chaine (char *chaine_ , char *chaine) ;
bool id_exists (char *id , char *table , char *nom_attribut , PGconn *connect , int *code_retour) ;
char *ajouter_car (char *chaine , char caract) ;
char *construction_transaction (char *requete) ;


void recup_num_carte (char *chaine , char **num_carte) ;
double recup_montant (char *chaine) ;
void recup_id_terminal (char *chaine , char **id_terminal) ;
void recup_num_compte (char *chaine , char **num_compte) ;
int recup_pos_chaine (char *chaine , char *search) ;
void recup_societe_terminal (char *id_terminal , PGconn *connect , int *code_retour , char **societe) ;
void generer_log (int code_retour) ;
void generer_log_v2 (char *message) ;
bool point (char c) ;
bool est_chiffre (char c) ;

int get_min_actuelle () ;
int get_sec_actuelle () ;


#endif