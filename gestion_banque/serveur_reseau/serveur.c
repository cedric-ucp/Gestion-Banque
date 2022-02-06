/**
 * @file serveur.c
 * @author ETAME CEDRIC 
 * @brief Ce fichier regroupe toutes les fonctions qui constituent le serveur réseau
 * @version 1.2
 * @date 2021-12-23
 * 
 * @copyright Copyright (c) 2021
 * 
 */

#include "serveur_projet.h"

/* On recupere la date puis on la met au format adapte */
void get_date (char *date){
	time_t now ;
	now = time (&now) ;
	struct tm *local = localtime (&now) ;
	char day [10] ;
	/*On convertit chacune des valeurs entieres reçues en char*/
	sprintf (day , "%d" , local->tm_mday) ;
	char mois [10] ;
	sprintf (mois , "%d" , local->tm_mon + 1) ;
	
	/*On realise la concatenation afin de constituer la chaine de caractere date au format souhaite*/
	char annee [10] ;
	sprintf (annee , "%d" , local->tm_year + 1900) ;
	strcpy (date , annee) ;
	strcat (date , "/") ;
	strcat (date , mois) ;
	strcat (date , "/") ;
	strcat (date , day) ;
}

void get_hour (char *date){
	time_t now ;
	now = time (&now) ;
	struct tm *local = localtime (&now) ;
	char hour [3] ;
	char min [3] ;
	char second [3] ;

	/*On convertit chacune des valeurs entieres reçues en char*/
	sprintf (hour , "%d" , local->tm_hour) ;
	sprintf (min , "%d" , local->tm_min) ;
	sprintf (second , "%d" , local->tm_sec) ;
	
	/*La structure local renvoie un chiffre au lieu de "0" et le chiffre si ce cas survient alors on remet l'heure
	 *la minute ou alors la seconde au format souhaite
	*/
	if (strlen (hour) == 1){
		char h = hour [0] ;
		hour [0] = '0' ;
		hour [1] = h ;
		hour [2] = '\0' ;
	}
	if (strlen (min) == 1){
		char h = min [0] ;
		min [0] = '0' ;
		min [1] = h ;
		min [2] = '\0' ;
	}
	if (strlen (second) == 1){
		char h = second [0] ;
		second [0] = '0' ;
		second [1] = h ;
		second [2] = '\0' ;
	}

	/*On realise la concatenation afin de constituer la chaine de caractere date au format souhaite*/
	strcat (date , " : ") ;
	strcat (date , hour) ;
	strcat (date , ":") ;
	strcat (date , min) ;
	strcat (date , ":") ;
	strcat (date , second) ;
	strcat (date , " : ") ;
}	
	
/* On recupere le solde du compte du client */
double get_solde_client (char *num_compte , PGconn *connect , int *code_retour){
	char *requete = (char*) malloc (sizeof (char) * 64) ;
	if (requete == NULL){
			perror ("ERREUR ALLOCATION MEMOIRE5\n") ;
			exit (1) ;
	}
	/*On construit la chaine de caractere pour formuler notre requete au serveur BD*/
	strcpy (requete , "SELECT epargne_Compte FROM Compte WHERE num_compte = '") ;
	concatener_chaine (requete , num_compte) ; ;
	concatener_chaine (requete , "'") ;
	
	PGresult *result = PQexec (connect , requete) ;
	/*On verifie le resulat de l'execution de la requete afin d'y detecter des erreurs ou non*/
	if (PQresultStatus (result) != PGRES_TUPLES_OK){
		fprintf (stderr , "REQUETE ECHOUEE_1 : %s", PQerrorMessage (connect)) ;
		*code_retour = SOLDE_INTROUVABLE ; //Code erreur de l'execution d'une requete qui sera tres utiles pour les logs
		PQclear (result) ;
		free (requete) ;
		return 0 ;
	}
	/*Si tout s'est bien passe alors on recupere le resulat de notre requet puis on la renvoie*/
	char *solde = PQgetvalue (result , 0 , 0) ;
	int solde_entier = atoi (solde) ; 
	
	free (requete) ;
	PQclear (result) ;
	
	return (double) solde_entier ;
}
	
PGresult *effectuer_paiement (PGconn *connect , double montant , char *num_carte , char *id_terminal , char *compte_terminal , int *code_retour){
	char *date = (char*) malloc (sizeof (char) * 12) ;
	if (date == NULL){
		perror ("ERREUR ALLOCATION MEMOIRE1\n") ;
		exit (1) ;
	}
	/*On recupere les informations manquantes pour la creation d'une transaction*/
	get_date (date) ;
	char *num_transaction = (char*) malloc (sizeof (char) * 9) ;
	if (num_transaction == NULL){
		perror ("\nERREUR ALLOCATION MEMOIRE\n") ;
		exit (1) ;
	}

	strcpy (num_transaction , "00") ;
	char somme [20] ;
	PGresult *result = NULL ;
	char type_transaction [] = "paiement par carte" ;
	
	char *num_compte_ = (char*) malloc (sizeof (char) * 9) ;
	if (num_compte_ == NULL){
		perror ("\nERREUR ALLOCATION MEMOIRE\n") ;
		exit (1) ;
	}
	recuperer_num_compte (num_carte , connect , code_retour , &num_compte_) ;
	double solde = (double) get_solde_client (num_compte_ , connect , code_retour) ;
	/*On genere un num de transaction qui est absolument necessaire pour la transaction*/
	num_transaction = num_transaction_aleatoire (num_transaction , connect , code_retour) ;
	char *societe_ ;
	recup_societe_terminal (id_terminal , connect , code_retour , &societe_) ;

	char *requete = (char *)malloc(sizeof(char) * 180);
	if (requete == NULL){
		perror("ERREUR ALLOCATION MEMOIRE7\n");
		exit(1);
	}
	/* On verifie si le solde du compte est suffisant pour effectuer la transction */
	if (solde >= montant){
		
		/* On construit la chaine de caractere qui formulera notre requete */
		sprintf (somme  , "%f" , montant) ;
		strcpy (requete , "INSERT INTO Transaction (") ;
		requete = construction_transaction (requete) ;
		concatener_chaine (requete , " VALUES ('") ;
		concatener_chaine (requete , num_transaction) ;
		concatener_chaine (requete , "' ") ;
		concatener_chaine (requete , " , ") ;
		concatener_chaine (requete , "'") ;
		concatener_chaine (requete , type_transaction) ;
		concatener_chaine (requete , "' ") ;
		concatener_chaine (requete , " , ") ;
		concatener_chaine (requete , "'") ;
		concatener_chaine (requete , somme) ;
		concatener_chaine (requete , "' ") ;
		concatener_chaine (requete , " , ") ;
		concatener_chaine (requete , "'") ;
		concatener_chaine (requete , societe_) ;
		concatener_chaine (requete , "' ") ;
		concatener_chaine (requete , " , ") ;
		concatener_chaine (requete , "'") ;
		concatener_chaine (requete , date) ;
		concatener_chaine (requete , "' ") ;
		concatener_chaine (requete , " , ") ;
		concatener_chaine (requete , "'") ;
		concatener_chaine (requete , date) ;
		concatener_chaine (requete , "' ") ;
		concatener_chaine (requete , " , ") ;
		concatener_chaine (requete , "'") ;
		concatener_chaine (requete , num_compte_);
		concatener_chaine (requete , "' ") ;
		concatener_chaine (requete , " , ") ;
		concatener_chaine (requete , "'") ;
		concatener_chaine (requete , compte_terminal) ;
		concatener_chaine (requete , "' ") ;
		concatener_chaine (requete , " , ") ;
		concatener_chaine (requete , "'") ;
		concatener_chaine (requete , id_terminal) ;
		concatener_chaine (requete , "')") ;
		result = PQexec (connect , requete) ;
		
		
	}
	else{
		perror ("\nPAIEMENT REFUSE, SOLDE INSUFFISANT \n") ;
		*code_retour = MONTANT_INSUFFISANT ;
		free(requete);
		free(date);
		free(num_transaction);
		free(societe_);
		free(num_compte_);
		return NULL ;
	}
	/*Si une quelconque erreur est survenue lors de la requete alors on la signale (pour les logs) 
	 *et egalement afin que le serveur renvoie au client l'echec de la transaction
	*/
	if (PQresultStatus (result) != PGRES_COMMAND_OK && result != NULL){
		fprintf (stderr , "REQUETE ECHOUEE_FINAL: %s", PQerrorMessage (connect)) ;
		*code_retour = ECHEC_REQUETE ;
		PQclear (result) ;
		free(requete);
		free(date);
		free(num_transaction);
		free(societe_);
		free(num_compte_);
		return NULL ;
	}
	*code_retour = 0 ;
	reguler_solde (num_compte_ , compte_terminal , montant , solde , connect , code_retour) ;
	free(requete);
	free(date);
	free(num_transaction);
	free(societe_);
	free(num_compte_);
	
	return result ;
}

/*Cette fonction permet de mettre a jour le solde des differents comptes apres transaction*/
void reguler_solde (char *num_compte_debiteur , char *num_compte_crediteur ,double montant , double solde , PGconn *connect , int *code_retour){
	double new_solde = solde - montant ;
	char *requete = (char*) malloc (100 * sizeof (char)) ;
	if (requete == NULL){
		printf ("\nERREUR ALLOCATION MEMOIRE\n") ;
		exit (1) ;
	}
	char epargne [20] ;
	sprintf(epargne , "%f" , new_solde) ;

	//On met à jour le solde du compte_debiteur
	strcpy (requete , "UPDATE Compte SET epargne_compte = '") ;
	concatener_chaine (requete , epargne) ;
	concatener_chaine (requete , "' WHERE num_compte = '") ;
	concatener_chaine (requete , num_compte_debiteur) ;
	concatener_chaine (requete , "'") ;

	PGresult *result = PQexec (connect , requete) ;
	if (PQresultStatus(result) != PGRES_COMMAND_OK && result != NULL){
		fprintf (stderr , "REQUETE ECHOUEE_10 : %s" , PQerrorMessage(connect)) ;
	}
	PQclear (result) ;

	//On met a jour le solde du compte_crediteur
	requete = (char*) realloc (requete , 100 * sizeof(char)) ;
	if (requete == NULL){
		printf("\nERREUR ALLOCATION MEMOIRE\n");
		exit(1);
		*code_retour = ECHEC_REQUETE ;
		return ;
	}
	*code_retour = 0 ;

	double porte_feuille = (double) get_solde_client (num_compte_crediteur , connect , code_retour);
	memset (epargne , '\0' , 20) ;
	sprintf (epargne , "%f" , montant + porte_feuille) ;
	strcpy (requete, "UPDATE Compte SET epargne_compte = '") ;
	concatener_chaine (requete, epargne);
	concatener_chaine (requete, "' WHERE num_compte = '") ;
	concatener_chaine (requete, num_compte_crediteur) ;
	concatener_chaine (requete, "'") ;

	result = PQexec (connect , requete) ;
	if (PQresultStatus (result) != PGRES_COMMAND_OK && result != NULL){
		fprintf(stderr, "REQUETE ECHOUEE_10 : %s", PQerrorMessage (connect)) ;
		*code_retour = ECHEC_REQUETE ;
		return ;
	}
	PQclear (result) ;
	free (requete) ;
	*code_retour = 0 ;
}


/*Une simple concatenation des chaines de caracteres qui consitueront la requete pour la transaction*/
char *construction_transaction (char *requete){
	concatener_chaine (requete , "num_transaction, type_transaction, montant, motif, date_transaction, date_execution, num_compte_debiteur, ") ;
	concatener_chaine (requete , "num_compte_crediteur, id_terminal) ") ;
	return requete ;
}

/*On genere un num de transaction aleatoire qui est UNIQUE ce qui est primordial*/
char *num_transaction_aleatoire (char *num_transaction , PGconn *connect , int *code_retour){
	char caract [] = {'0' , '1' , '2' , '3' , '4' , '5' , '6' ,
					'7' , '8' , '9'} ;
	srand (time (NULL)) ;
	int size = 0 ;
	int max = 10 ;
	
	while (size < 6){
		/* Fonction rand amorce le generateur de nombre aleatoires */
		int ale =  rand () % max ;
		
		num_transaction = ajouter_car (num_transaction , caract [ale]) ;
		size++ ;
	}
	/*Tant que l'id cree n'est pas unique dans la table Transaction alors on en recree un*/
	if (unicite_id (num_transaction , "Transaction" , "num_transaction" , connect , code_retour)){
		num_transaction = strncpy (num_transaction , "" , strlen ("")) ;
		num_transaction_aleatoire (num_transaction , connect , code_retour) ;
	}
	return num_transaction ;
}

/* On verifie qu'un id donne est unique dans la bd (notamment ici ça sera le num_transaction) */
bool unicite_id (char *id , const char *table , const char *nom_attribut  , PGconn *connect , int *code_retour){
	char *requete = (char*) malloc (sizeof (char) * 82) ;
	if (requete == NULL){
		perror ("ERREUR ALLOCATION MEMOIRE8\n") ;
		exit (1) ;
	}
	/*On construit la chaine de caractere necessaire afin de formuler notre requete*/
	const char *char_temp = "SELECT COUNT (*) FROM " ;
	strcpy (requete , char_temp) ;
	strcat (requete , table) ;
	strcat (requete , " WHERE ") ;
	strcat (requete , nom_attribut) ;
	strcat (requete , " = ") ;
	strcat (requete , "'") ;
	strcat (requete , id) ;
	strcat (requete , "'") ;
	
	PGresult *result = PQexec (connect , requete) ;
	int occ ;
	/*Si la requete s'est bien exectuee alors on renvoie le resulat booleen de la fonction*/
	if (result != NULL && PQresultStatus (result) == PGRES_TUPLES_OK){
		occ = atoi (PQgetvalue (result , 0 , 0)) ;
		free (requete) ;
		PQclear (result) ;
		return occ >= 1 ;
	}
	/*Sinon on signale l'erreur et on renvoie faux donc l'id est unique (il existe pas dans la base de donnees)*/
	else{
		fprintf (stderr , "Requete echouee_3 : %s\n", PQerrorMessage (connect)) ;
		PQclear (result) ;
		*code_retour = ID_UNIQUE ;
		free (requete) ;
		return false ;
	}
}

/* On verifie si un id donne existe dans la bd */
bool id_exists (char *id , char *table , char *nom_attribut , PGconn *connect , int *code_retour){
	
	char *requete = (char*) malloc (sizeof (char) * 90) ;
	if (requete == NULL){
		perror ("ERREUR ALLOCATION MEMOIRE9\n") ;
		exit (1) ;
	}
	/*On realise la concatenenation pour former notre chaine de caractere afin de formuler notre requete*/
	strcpy (requete , "SELECT COUNT (*) FROM ") ;
	concatener_chaine (requete , table) ;
	concatener_chaine (requete , " WHERE ") ;
	concatener_chaine (requete , nom_attribut) ;
	concatener_chaine (requete , " = '") ;
	concatener_chaine (requete , id) ;
	concatener_chaine (requete , "'\0") ;
	
	PGresult *result = PQexec (connect , requete) ;
	int occ ;
	/*Si la requete s'est bien deroulee alors on renvoie le resulat booleen par rapport au resultat de la requete*/
	if (PQresultStatus (result) == PGRES_TUPLES_OK && result != NULL){
		occ = atoi (PQgetvalue (result , 0 , 0)) ;
		free (requete) ;
		PQclear (result) ;
		return occ >= 1 ;
	}
	/*Sinon on renvoie faux*/
	else{
		fprintf (stderr , "Requete echouee_2 : %s\n", PQerrorMessage (connect)) ;
		*code_retour = ID_NO_EXISTS ;
		free (requete) ;
		PQclear (result) ;
		return false ;
	}
}

void recuperer_num_compte (char *num_carte , PGconn *connect , int *code_retour , char **num_compte){
	PGresult *result ;
	char *requete = (char*) malloc (sizeof (char) * 80) ;
	if (requete == NULL){
		perror ("ERREUR ALLOCATION MEMOIRE4\n") ;
		exit (1) ;
	}
	
	const char *temp = "SELECT num_compte FROM Carte WHERE num_carte = '" ;
	strcpy (requete , temp) ;
	/* On realise une concatenation entre la requete et le parametre num_carte
	*  pour completer la requete et recuperer ainsi le num_compte adequat 
	*/
	concatener_chaine (num_carte , "'") ;
	concatener_chaine (requete , num_carte) ;
	result = PQexec (connect , requete) ;
	if (PQresultStatus (result) != PGRES_TUPLES_OK){
		fprintf (stderr , "Requete echouee_6 : %s", PQerrorMessage (connect)) ;
		*code_retour = NUM_COMPTE_NO_EXISTS ;
	}
	/*Si la requete s'est bien derouleeOn transfere vers num_compte le resulat de la requete 
	 *et on nettoie toutes les variables utilisees
	*/
	strcpy (*num_compte , PQgetvalue (result , 0 , 0)) ;
	PQclear (result) ;
	free (requete) ;
}

PGconn *se_connecter_bd (PGconn *connect){
	const char *conn_info = "host = localhost port = 5432 dbname = gestion_banque user = postgres password = postgres" ;
	
	/* On realise la connexion a la base de donnees*/
	connect = PQconnectdb (conn_info) ;
	
	/* On verifie si la connexion est reussie */
	if (PQstatus (connect) != CONNECTION_OK){
		perror ("CONNEXION A LA BASE DE DONNEE ECHOUEE !\n") ;
		generer_log_v2 ("CONNECTION_TO_DATA_BASE_FAILED") ;
		exit (1) ;
	}
	return connect ;
}

bool montant_correct (char *montant){
	int i = 0 ;
	int compteur = 0 ;
	/*On parcout chacun des caracteres valides jusqu'a la fin de la chaine*/
	while (i < strlen (montant) && caract_correcte (montant [i])){
		if (montant [i] == '.'){
			compteur ++ ;
		}
		i++ ;
	}
	if (compteur <= 1){
		return i >= strlen (montant) ; // similaire à 'la chaine de caract est correcte'
	}
	else{
		return i < strlen (montant) ;
	}
}

bool caract_correcte (char carac){
	char caract [] = {'0' , '1' , '2' , '3' , '4' , '5' , '6' ,
					  '7' , '8' , '9' , '.'} ;
	int i = 0 ;
	/*Tant que un carac n'est pas present dans le tableau caract alors on continue de parcourir le tableau*/
	while (i < strlen (caract) && carac != caract [i]){
		i++ ;
	}
	return i < strlen (caract) ; // similaire à 'le caractere est correcte'
}

/*Fonction qui concataine deux chaines de caracteres totalement similaire à strcat*/
void concatener_chaine (char *chaine_ , char *chaine){
	int i = 0 ;
	int j = 0 ;
	while (chaine_ [i] != '\0'){
		i++ ;
	}
	while (chaine [j] != '\0'){
		chaine_ [i] = chaine [j] ;
		j++ ;
		i++ ;
	}
	chaine_ [i] = '\0' ;
	
}

/*Fonction qui concataine un caractere a une chaine de caracteres*/
char *ajouter_car (char *chaine , char caract){
	size_t len = strlen (chaine) ;
	chaine [len] = caract ;
	chaine [len + 1] = '\0' ;
	
	return chaine ;
}

/* 
 * On decoupe ici la chaine de caractere recu du client reseau pour en extraire les donnees suivantes 
 * numero de compte 
 * id_terminal 
 * montant
 * numero de carte
 * L'algorithme de de ces fonctions est similaire pour chacune d'entre elles seule difference est le 
 * mot_cle recherche dans la chaine de caracteres
 */

void recup_num_carte (char *chaine , char **num_carte){
	/*On recupere l'indice de la derniere lettre du mot recherche dans la chaine de caracteres*/
	int i = recup_pos_chaine (chaine , "carte") ;
	char ch [16] ;
	if (chaine [i + 1] != '\0' && chaine [i + 1] == ':'){
		i = i + 2 ; // Ici on est au caractere qui suit ':'
		int j = 0 ;
		/*On recupere tous les caracteres qui apres ':' tant que ce sont des chiffres*/
		while (est_chiffre (chaine [i])){
			ch [j] = chaine [i] ;
			i++ ;
			j++ ;
		}
		*num_carte = (char*) malloc (sizeof (char) * strlen (ch) + 1) ;
		if (*num_carte == NULL){
			perror ("\nERREUR ALLOCATION MEMOIRE\n") ;
			exit (1) ;
		}
		/*On stocke le num_carte recupere dans la variable adequate*/
		strncpy (*num_carte , ch , j) ;
	}
}

bool est_chiffre (char c){
	return c == '0' || c == '1' || c == '2' || c == '3' || c == '4' || c == '5' || c == '6' || c == '7' || c == '8' || c == '9' ;
}

double recup_montant (char *chaine){
	double montant = 0 ;
	int i = recup_pos_chaine (chaine , "montant") ;
	if (chaine [i + 1] != '\0' && chaine [i + 1] == ':'){
		i = i + 2 ;
		char ch [7]  ;
		memset (ch , '\0' , 7) ;
		int j = 0 ;
		while (est_chiffre (chaine [i]) || point (chaine [i])){
			ch [j] = chaine [i] ;
			i++ ; 
			j++ ;
		}
		
		if (montant_correct (ch)){
			montant = strtod (ch , NULL) ;
			return montant ;
		}
		else{
			return 0 ;
		}
	}
}
	
void recup_id_terminal (char *chaine , char **id_terminal){
	int i = recup_pos_chaine (chaine , "id") ;
	char ch [8] ;
	if (chaine [i + 1] != '\0' && chaine [i + 1] == ':'){
		i = i + 2 ; 
		int j = 0 ;
		
		while (est_chiffre (chaine [i])){
			ch [j] = chaine [i] ;
			i++ ;
			j++ ;
		}
		*id_terminal = (char *)malloc(sizeof(char) * strlen (ch) + 2);
		if (*id_terminal == NULL){
			perror("ERREUR ALLOCATION MEMOIRE\n");
			exit(1);
		}
		strcpy (*id_terminal , ch) ;

	}
}

void recup_num_compte (char *chaine , char **num_compte){
	int i = recup_pos_chaine (chaine , "compte") ;
	char ch [9] ;
	if (chaine [i + 1] != '\0' && chaine [i + 1] == ':'){
		i = i + 2 ; 
		int j = 0 ;
		while (est_chiffre (chaine [i])){
			ch [j] = chaine [i] ;
			i++ ;
			j++ ;
		}
		ch [j + 1] = '\0' ;
		*num_compte = (char*) malloc (sizeof (char) * strlen (ch) + 1) ;
		if (*num_compte == NULL){
			perror("ERREUR ALLOCATION MEMOIRE\n");
			exit(1);
		}
		
		strcpy (*num_compte , ch) ;
	}

}
		
/* On recupere la position de la derniere lettre d'une sous-chaine de caracteres
 * dans une chaine de caracteres donnee
 */
int recup_pos_chaine (char *chaine , char *search){
	int posfound = 0 ;
	if (strlen (chaine) > 0){
		char *sous_chaine = strstr (chaine , search) ;
		if (sous_chaine != NULL){
			posfound = (int) (sous_chaine - chaine) ; // Renvoie la position de la premiere lettre du mot recherche
			posfound = (int) strlen (search) + posfound - 1 ; // Renvoie la derniere lettre du mot recherche
		}
		
	}
	return posfound ;
}

void recup_societe_terminal (char *id_terminal , PGconn *connect , int *code_retour , char **societe){
	char *requete = (char*) malloc (sizeof (char) * 80) ;
	if (requete == NULL){
		perror ("ERREUR ALLOCATION MEMOIRE\n") ;
		exit (1) ;
	}
	/*On construit la chaine de caractere necessaire pour formuler notre requete*/
	strcpy (requete , "SELECT nom_societe FROM Terminal WHERE id_terminal = '") ;
	concatener_chaine (requete , id_terminal) ;
	concatener_chaine (requete , "'") ;
	PGresult *result = PQexec (connect , requete) ;
	/*En cas d'erreur lors de la requete on retourne le code erreur adequat*/
	if (PQresultStatus (result) != PGRES_TUPLES_OK){
		fprintf (stderr , "Requete echouee_5 : %s", PQerrorMessage (connect)) ;
		*code_retour = SOCIETE_INTROUVABLE ;
	}
	/*Sinon on stocke le resultat de la requete dans la variable prevue pour*/
	*societe = (char*) malloc (sizeof (char) * strlen (PQgetvalue (result , 0 , 0)) + 1) ;
	if (*societe == NULL){
		perror ("\nERREUR ALLOCATION MEMOIRE\n") ;
		exit (1) ;
	}
	strcpy (*societe , PQgetvalue (result , 0 , 0)) ;
	free (requete) ;
	PQclear (result) ;
}

/*Selon l'erreur obtenue on ecrit un message adapte	dans le fichier log*/
void generer_log (int code_retour){
	FILE *fichier = fopen ("log_messages.txt", "a+") ;
	
	char *message = (char*) malloc (sizeof (char) * 100) ;
	char *date_ = (char*) malloc (sizeof (char) * 100) ;
	if (message == NULL || date_ == NULL){
		printf ("\nERREUR ALLOCATION MEMOIRE\n") ;
		exit (1) ;
	}
		
	get_date (date_) ;
	get_hour (date_) ;
	strcpy (message , date_) ;
		
	switch (code_retour){
		case NUM_CARTE_NO_EXISTS :
			concatener_chaine (message , "ERROR 500 : NUM_CARD_NO_EXISTS\n\0") ;
			break ;
		case MONTANT_INCORRECT :
			concatener_chaine (message , "ERROR 600 : INCORRECT_AMOUNT\n\0") ;
			break ;
		case ID_NO_EXISTS :
			concatener_chaine (message , "ERROR 300 : ID_NO_EXISTS\n\0") ;
			break ;
		case NUM_COMPTE_NO_EXISTS :
			concatener_chaine (message , "ERROR 400 : NUM_ACCOUNT_NO_EXISTS\n\0") ;
			break ;
		case SOLDE_INTROUVABLE : 
			concatener_chaine (message , "ERROR 50 : BALANCE_NOT_FOUND\n\0") ;
			break ;
		case ECHEC_REQUETE :
			concatener_chaine (message , "ERROR 20 : REQUEST_FAILURE\n\0") ;
			break ;
		case SOCIETE_INTROUVABLE :
			concatener_chaine (message , "ERROR 30 : COMPAGNY_NOT_FOUND\n\0") ;
			break ;
		case INSERTION_ECHOUEE :
			concatener_chaine (message , "ERROR 60 : INSERT_FAILED\n\0") ; // Insertion a echouee
			break ;
		case MONTANT_INSUFFISANT :
			concatener_chaine (message , "ERROR 70 : AMOUNT_INSUFFICIENT\n\0") ;
			break ;
		case SYNTAX_ERROR :
			concatener_chaine (message , "ERROR 200 : SYNTAX_ERROR\n\0") ;
			break ;
		case 0 :
			concatener_chaine (message , "SUCCESS\n\0") ;
			break ;
		default :
				;
	}
	int retour = fputs (message , fichier) ;
	if (retour == EOF){
		printf ( "Results == %d %d %d\n" , retour , ferror (fichier), errno) ;
		perror ("My message") ;
	}
	free (message) ;
	free (date_) ;
	fclose (fichier) ;
	
}
/*On ecrit dans le fichier log la chaine de caractere le message decrivant l'erreur ou le succes d'une operation quelconque*/
void generer_log_v2 (char *message){
	FILE *fichier = fopen ("log_messages.txt" , "a+") ;
	char *date = (char*) malloc (sizeof (char) * 100) ;
	if (date == NULL){
		printf ("\nERREUR ALLOCATION MEMOIRE\n") ;
		exit (1) ;
	}
	
	get_date (date) ; 
	get_hour (date) ;
	concatener_chaine (date , message) ;
	
	int retour = fputs (date , fichier) ;
	if (retour == EOF){
		printf ( "Results == %d %d %d\n" , retour , ferror (fichier), errno) ;
		perror ("My message") ;
	}
	free (date) ;
	fclose (fichier) ;
}

/*On retourne la minute acutelle (necessaire pour gerer le timer pour la deconnexion d'un client)*/
	
void free_chaine (char **chaine){
	free (*chaine) ;
	*chaine = NULL ;
}

int get_min_actuelle (){
	time_t now;
	now = time(&now);
	struct tm *local = localtime(&now);
	return local->tm_min ;
}
int get_sec_actuelle (){
	time_t now ;
	now = time (&now) ;
	struct tm *local = localtime (&now) ;
	return local->tm_sec ;
}