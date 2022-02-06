#include "serveur.c"
#include "syntaxe_serveur.c"
/* Serveur TCP 
 *
 * Ce code implémente un seveur réseau dans le cadre de nore projet BD/Réseaux
 *
 * ETAME CEDRIC <etamecedric7@gmail.com>
 */
 
 int main (int argc , char **argv){
	/* Variables pour notre connexion serveur_réseau-serveur_client */
	int serveur_ecoute , serveur_dial , client_len ;
	int option = 1 ;
	struct sockaddr_in serv_addr , cli_addr ;
	char buf [TAILLE_BUFFER] ;
	int so_reuseaddr = 1 ;

	/* Variables qui serviront a recuperer les informations necessaires a la transaction */
	double montant ;
	char *num_carte = NULL ;
	char *id_terminal = NULL ;
	char *num_compte = NULL ;
	char *societe = NULL ;
	char *message = NULL ;
	char ip_client [20] ;
	/*Variables necessaires pour la gestion des erreurs*/
	int syntaxe ;
	int type_erreur ;
	int code_retour = 0 ;

	/* Variables pour notre connexion serveur_reseau-serveur_BD */
	PGconn *connect ;
	PGresult *result = NULL ;
	
	#ifdef WIN32
	int code_resultat;
	WSADATA wsaData;
	code_resultat = WSAStartup(2, &wsaData);
	if (code_resultat != 0) {
		printf("Erreur lors de l'initialisation de Winsocks (code d'erreur %d).\n", code_resultat);
		exit(-1);
	}
#endif

	/* on désigne l'adresse+port qu'on va ouvrir */
	serv_addr.sin_family = AF_INET ;
	if (inet_aton ("192.168.1.162" , &serv_addr.sin_addr) == 0){
		exit (1) ;
	}

	/* on attend sur toutes nos adresses */
	serv_addr.sin_port = htons (PORT_DEFAUT) ;												  /* on attend sur le port 5000 */
	memset (&serv_addr.sin_zero, 0, sizeof (serv_addr.sin_zero)) ;

	/* on crée la socket d'ecoute et on l'associe au couple (adresse,port) defini ci-dessus */
	serveur_ecoute = socket (PF_INET , SOCK_STREAM , 0);
	if(setsockopt (serveur_ecoute , SOL_SOCKET, SO_REUSEADDR, (const char*)&so_reuseaddr , sizeof (so_reuseaddr)) < 0){
        perror("setsockopt(SO_REUSEADDR) failed") ;
	}

#ifdef SO_REUSEPORT
    if (setsockopt (serveur_ecoute , SOL_SOCKET, SO_REUSEPORT, (const char*)&so_reuseaddr, sizeof (so_reuseaddr)) < 0){
        perror("setsockopt(SO_REUSEPORT) failed") ;
	}
#endif

	bind (serveur_ecoute, (struct sockaddr*)&serv_addr, sizeof (serv_addr)) ;
	listen (serveur_ecoute, 5) ; /* on definit la socket serveur_ecoute, comme etant une socket d'ecoute*/
	
	client_len = sizeof (cli_addr) ;
	
	/* On se met en ecoute sur la socket. C'est une fonction blocante qui ne
	   se debloque que lorsqu'un client vient se connecter sur cette socket
	   d'ecoute. La valeur de retour est alors le descripteur de la socket
	   de connexion permettant  de dialoguer avec CE client. */
	   	while (true){
			printf ("En attente de connexion client...\n") ;
			serveur_dial = accept (serveur_ecoute , (struct sockaddr*) &cli_addr , (socklen_t*) &client_len);
			message = (char*) malloc (sizeof (char) * 30) ;
			if (message == NULL){
				perror ("\nERREUR ALLOCATION MEMOIRE\n") ;
				exit (1) ;
			}
			memset (ip_client , '\0' , 20) ;
			strcpy (ip_client , inet_ntoa (cli_addr.sin_addr)) ;
			strcpy (message , ip_client) ;
			printf ("\nClient : %s connecte\n", ip_client) ;
			concatener_chaine (message , "_CONNECTED\n\0") ;
			generer_log_v2 (message) ;

			connect = se_connecter_bd (connect) ;
			printf ("CONNECTION A LA BASE DE DONNEES REUSSIE\n") ;
			generer_log_v2 ("CONNECTED_TO_DATA_BASE\n\0") ;
			int code_echange = 0 ;
			
			while (code_echange == 0){
				memset (buf , '\0' , TAILLE_BUFFER) ;
				int sec = get_sec_actuelle () ;
				int min = get_min_actuelle () + 1;
				printf ("\nEntree boucle \"et\" : [%dmin:%ds]\n" , get_min_actuelle () , get_sec_actuelle ()) ;
				int code = -1 ;
				/*On boucle sur la fonction recv ici non bloquante pendant une minute a l'attente d'un message du client*/
				while ((get_min_actuelle () != min || get_sec_actuelle () != sec) && strlen (buf) == 0 && code == -1){
					code = recv (serveur_dial, buf , TAILLE_BUFFER , MSG_DONTWAIT) ;
				}
				printf ("Sortie boucle \"et\" : [%dmin:%ds]\n", get_min_actuelle (), get_sec_actuelle ()) ;
				/*On a rien reçu et le timer s'est ecoule on estime que il y'a eu deconnexion du client*/
				if (strlen (buf) == 0 || code == 0 || code == -1){
					generer_log_v2 ("CONNECTION_LOST\n\0") ;
					strncpy (buf , "quit" , 4) ;
					break ;
				}
				/* Si le client a clique sur le boutton effectuer une transaction (le serveur lui reçoit 'et') */
				if (strncmp (buf , "et" , 2) == 0){
					
					write (serveur_dial , "OK\n" , strlen ("OK\n")) ;
					generer_log_v2 ("TRANSACTION_STARTED\n\0") ;
					memset (buf , '\0' , TAILLE_BUFFER) ;

					int sec = get_sec_actuelle () ;
					int min = get_min_actuelle () + 1 ; ;
					int code = -1 ;
					printf ("Entree boucle_id_terminal : [%dmin:%ds]\n", get_min_actuelle (), get_sec_actuelle ());
					/**
					 * On attend des donnees du client durant une periode d'1 minute
					 * Apres ce laps de temps le serveur en deduit que le client s'est deconnecte
					 */
					while ((get_min_actuelle () != min || get_sec_actuelle () != sec) && strlen (buf) == 0 && code == -1){
						code = recv (serveur_dial, buf, TAILLE_BUFFER, MSG_DONTWAIT) ;
					}
					printf ("Sortie boucle_id_terminal : [%dmin:%ds]\n" , get_min_actuelle () , get_sec_actuelle ());

					if (strlen (buf) == 0 || code == 0 || code == - 1){
						generer_log_v2 ("CONNECTION_LOST\n\0") ;
						strncpy (buf, "quit", 4) ;
						break ;
					}

					/* On recupere l'id_terminal et on verifie s'il est bien present dans la BD*/
					if (syntaxe_id_correct (buf)){
						/*On estime que l'id_terminal est correct alors on procede a l'identification du client*/
						generer_log_v2 ("SYNTAX_ID_CORRECT\n\0") ;
						recup_id_terminal (buf , &id_terminal) ;

						/*On verifie si l'id reçu existe bien dans la base de donnees*/
						if (id_exists (id_terminal , "Terminal" , "id_terminal" , connect , &code_retour)){
							generer_log_v2 ("ID_TERMINAL_EXISTS\n") ;
							recup_societe_terminal (id_terminal, connect, &code_retour , &societe) ; // On recupere la societe associe a l'id_terminal afin de l'identifier dans les logs

							if (code_retour != SOCIETE_INTROUVABLE){
								concatener_chaine (societe, " : CONNECTED\n\0");
								generer_log_v2 (societe) ;
								strcpy (buf , id_terminal) ;
								strcat (buf , "\n") ;
								write (serveur_dial , buf , strlen (buf)) ;
							}
							memset (buf, '\0', TAILLE_BUFFER) ;
							int sec = get_sec_actuelle () ;
							int min = get_min_actuelle () + 1 ;
							int code = -1 ;
							printf ("Entree boucle_carte : [%dmin:%ds]\n", get_min_actuelle(), get_sec_actuelle());
							while ((get_min_actuelle () != min || get_sec_actuelle () != sec) && strlen (buf) == 0 && code == -1){
								code = recv (serveur_dial, buf, TAILLE_BUFFER, MSG_DONTWAIT);
							}
							printf ("Sortie boucle_carte : %dmin:%ds\n", get_min_actuelle(), get_sec_actuelle());
							if (strlen (buf) == 0 || code == 0 || code == -1){
								generer_log_v2 ("CONNECTION_LOST\n\0") ;
								strncpy (buf, "quit", 4);
								break;
							}

							/*On verifie la syntaxe globale de la chaine de caractere reçu selon des criteres determines*/
							if (syntaxe_globale_correcte (buf)){
								/*On estime que la chaine est correcte pour la manipulation*/
								generer_log_v2("GLOBAL_SYNTAX_CORRECT\n\0");
								/*On recupere chacune des informations necessaires a la transaction dans la chaine reçue*/
								montant = recup_montant (buf) ;
								recup_num_carte (buf , &num_carte) ;
								recup_num_compte (buf , &num_compte) ;
										
								if (montant != 0){
									/* On verifie si le num_carte , id_terminal ou encore num_compte existent bien dans la BD */
									if (id_exists (num_carte , "Carte" , "num_carte", connect , &code_retour)){
									generer_log_v2("NUM_CARD_OK\n\0");
									/*Verfication du num_compte*/
									if (id_exists (num_compte , "Compte" , "num_compte" , connect , &code_retour)){
										generer_log_v2 ("NUM_ACCOUNT_OK\n\0") ;
										/*Si et seulement si toutes les informations reçues sont valides alors on effectue la transaction
										*Le retour de la fonction "effectuer_paiement" nous indiquera si touts s'est bien passe ou non
										*/
										result = effectuer_paiement (connect , montant , num_carte , id_terminal , num_compte , &code_retour) ;
													
										/*Selon la valeur du code_retour alors on detectera les erreurs survenues pour le rapport dans les logs*/	
										if (code_retour == ECHEC_REQUETE){
											fprintf (stderr , "Requete echouee : %s", PQerrorMessage (connect)) ;
											write (serveur_dial , "TRANSACTION FAILED\n" , strlen ("TRANSACTION FAILED\n"));
											printf ("TRANSACTION FAILED\n") ;
											generer_log(ECHEC_REQUETE);
										}
										if (code_retour == MONTANT_INSUFFISANT){
											write (serveur_dial , "TRANSACTION FAILED\n" , strlen ("TRANSACTION FAILED\n")) ;
											printf ("TRANSACTION FAILED\n") ;
											generer_log (MONTANT_INSUFFISANT) ;
										}
										if (code_retour == 0) {
											write (serveur_dial , "PAYMENT_ACCEPTED\n" , strlen ("PAYMENT_ACCEPTED\n")) ;
											generer_log_v2 ("PAYMENT_ACCEPTED\n\0") ;
											printf ("PAYMENT ACCEPTED\n") ;
										}
										if (result != NULL){
											PQclear(result);
										}
							/*Au cas où l'une des conditions executees plus haut n'est pas verifiees alors le serveur renvoie "TRANSACTION FAILED"
							*Tandis que dans les logs il sera un petit peu plus precis quant à la cause de l'echec de la transaction
							*/
									}
									else{
										if (strncmp(buf, "quit", 4) == 0){
											code_echange = 1;
										}
										else{
											write (serveur_dial , "TRANSACTION FAILED\n" , strlen ("TRANSACTION FAILED\n")) ;
											generer_log (NUM_COMPTE_NO_EXISTS) ;
											printf ("TRANSACTION FAILED\n") ;
										}
									}
								}
								else{
									if (strncmp (buf, "quit", 4) == 0){
										code_echange = 1;
									}
									else{
										write (serveur_dial, "TRANSACTION FAILED\n", strlen ("TRANSACTION FAILED\n"));
										generer_log (NUM_CARTE_NO_EXISTS) ;
										printf ("TRANSACTION FAILED\n") ;
									}
								}
							}
							else{
								if (strncmp (buf, "quit", 4) == 0){
									code_echange = 1;
								}
								else{
									generer_log (MONTANT_INCORRECT) ;
									write (serveur_dial , "TRANSACTION FAILED\n" , strlen ("TRANSACTION FAILED\n")) ;
									printf ("TRANSACTION FAILED\n") ;
								}
							}	
									
						}
						else{
							if (strncmp (buf, "quit", 4) == 0){
								code_echange = 1;
							}
							else{
								write (serveur_dial , "TRANSACTION FAILED\n" , strlen ("TRANSACTION FAILED\n"));
								generer_log (SYNTAX_ERROR) ;
								printf ("TRANSACTION FAILED\n") ;
							}
						}	
					}
					else{
						if (strncmp (buf, "quit", 4) == 0){
							code_echange = 1;
						}
						else{
							write(serveur_dial , "TRANSACTION FAILED\n" , strlen ("TRANSACTION FAILED\n"));
							generer_log (ID_NO_EXISTS) ;
							printf ("TRANSACTION FAILED\n") ;
						}
					}
				}
				else{
					if (strncmp (buf, "quit", 4) == 0){
						code_echange = 1;
					}
					else{
						write(serveur_dial , "TRANSACTION FAILED\n" , strlen ("TRANSACTION FAILED\n"));
						generer_log (SYNTAX_ERROR) ;
						printf ("TRANSACTION FAILED\n") ;
					}
				}
			}
			if (strncmp (buf, "quit", 4) == 0){
				code_echange = 1 ;
			}

				/*On n'oublie pas de nettoyer les memoires occupees pour chaque allocation memoire (si elles ont ete occupees)*/
				if (societe != NULL && num_carte != NULL && num_compte != NULL && id_terminal != NULL){
					free_chaine (&societe) ;
					free_chaine (&num_carte) ;
					free_chaine (&id_terminal) ;
					free_chaine (&num_compte) ;
				}
			}	
			strcpy (message , ip_client) ;
			concatener_chaine (message , "_DISCONNECTED\n\0") ;
			generer_log_v2 (message) ;
			free_chaine (&message) ;
			close (serveur_dial) ;
			printf ("Dialogue correctement ferme\n\n") ;
			generer_log_v2 ("DIAL_CLOSED\n") ;
		#ifdef WIN32
			WSACleanup() ;
		#endif
			PQfinish (connect) ;
	}
	close (serveur_ecoute)  ;
	return 0;
}
		
		