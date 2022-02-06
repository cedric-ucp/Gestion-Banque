/**
 * Girard Vincent
 * 
 */

import java.io.IOException ;
import java.awt.image.PixelInterleavedSampleModel;
import java.io.BufferedReader ;
import java.io.InputStreamReader ;
import java.io.OutputStream;
import java.io.PrintWriter ;
import java.io.IOException ;
import java.net.Socket ;
import java.net.UnknownHostException ;

public class ClientTCP 
{	
    public static void main (String argv []) throws IOException
    {
    	BufferedReader flux_entree = null;
    	PrintWriter flux_sortie = null;
    	
    	Socket socket = null ;

    	String request = new String();
    	String numCompteTerminal;
        String chaine ;
        String rep = "";
        
       try 
       {
            // deuxieme argument : le numero de port que l'on contacte ip default: 127.0.0.1
            socket = new Socket ("192.168.1.162", 5000) ;
            flux_sortie = new PrintWriter (socket.getOutputStream (), true) ;
            flux_entree = new BufferedReader (new InputStreamReader (socket.getInputStream ())) ;
        } 
       	catch (UnknownHostException e) 
       	{
            System.err.println ("Hote inconnu") ;
            System.exit (1) ;
        } 
	
       // L'entree standard
       BufferedReader entree_standard = new BufferedReader ( new InputStreamReader ( System.in));
    
	    do
	    {
	    	System.out.println("Veuiller rentrer votre numero de compte: ");
	        chaine = entree_standard.readLine();
	    }while(!Transaction.estNumCompte(chaine));  
	    
	    numCompteTerminal = chaine;
	
		do 
		{	
			System.out.println("Pour commencer une transaction taper \"et\".");
			chaine = entree_standard.readLine();
			
			if(chaine.equals("et")) {
				//on commance une transaction
				flux_sortie.println(chaine);
				//demande la reponsse serveur
				rep = flux_entree.readLine();
				System.err.println("Le serveur a envoyer: " + rep);


				//verifie si le serveur est toujours co

				if (rep == null) {
					System.err.println("serveur déco");
					System.exit(1);
				}
				if (rep.equals("OK")) {
					//identification au pres du serveur
					flux_sortie.println("id:" + Transaction.ID + ";");
					rep = flux_entree.readLine();
					System.err.println("la reponsse du serveur: " + rep);

					//verifie si le serveur est toujours co
					if (rep == null) {
						System.err.println("serveur déco");
						System.exit(1);
					} else if (rep.equals("TRANSACTION FAILED")) {
						System.err.println("L'id terminal non reconnu");
					}
					if (rep.equals(Transaction.ID)) {

						//demande un montant
						System.out.println("Qu'elle est le montant de la transaction?");
						chaine = entree_standard.readLine();

						//verifie si il est valide
						if (Transaction.estMontant(chaine)) {
							//rentre les information de la transaction
							Transaction transaction = new Transaction(chaine, numCompteTerminal);
							request = transaction.toString();
							System.out.println("le montant est bon");

							//demande un numero de carte
							System.out.println("votre numéro de carte?");
							chaine = entree_standard.readLine();

							//vérivie si c'est un numero de carte valide
							if (Carte.isNumber(chaine)) {
								//rentre les infos de la carte dans la requette
								Carte card = new Carte(chaine);
								request = card.toString() + request;

								//on envoie la transaction au serveur
								flux_sortie.println(request);
								System.out.println(request);

								// on lit ce qu'a envoye le serveur
								rep = flux_entree.readLine();
								System.err.println("La reponse du serveur est: " + rep);

								//verifie si le serveur est toujours co
								if (rep == null) {
									System.err.println("serveur déco");
									System.exit(1);
								}

								//Annonce si paiment accepter ou non
								System.err.println("Le serveur m'envoie : " + rep);
								if (rep.equals("PAYMENT_ACCEPTED")) {
									System.out.println("paiement accepter");
								} else {
									System.out.println("paiement refuser");
								}
							}
							else{
								if (chaine.equals("end")) {
									System.err.println("Transaction echouee\n");
								} else{
									System.err.println("Numero de carte invalide.");
								}
							}

						} else {
							if (chaine.equals("end")) {
								System.err.println("Transaction echouee");
							} else {
								System.err.println("Montant incorrect\n");
							}

						}
					} else {
						System.err.println("Reponse serveur inconnue\n");
						System.err.println("Transaction echouee");
						chaine = "end";
					}
				} else {// 0000000000000005
					System.err.println("Reponse serveur inconnue");
					System.err.println("Transaction echouee");
					chaine = "end";
				}
			}
			else{
				if (chaine.equals ("end")) {
					System.err.println ("Transaction echouee") ;
				}
				else{
					System.err.println ("Chaine \"et\" non rentree\n") ;
				}
			}
			
		} while (!chaine.equals("end")) ;
			flux_sortie.println("quit");
	        flux_sortie.close () ;
	        flux_entree.close () ;
	        entree_standard.close () ;
	        socket.close () ;
    }
}
