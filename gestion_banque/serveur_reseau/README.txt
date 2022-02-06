-----------------------------------------------------SERVEUR RESEAU-------------------------------------------------

SERVEUR RESEAU qui fait office de terminal dans le cadre du projet BD/Réseaux.

Pour le démarrer 3 étapes sont nécessaires pour la compilation jusqu'au lancememnt du serveur :
    * cc -c -I ./ client_serveur.c  ==> compiler le serveur
    * cc -o nom_executable client_serveur.o -L usr/lib/x86_64-linux-gnu -lpq  ==> générer un exécutable
    * ./nom_executable  ==> Lancement du serveur 

Les logs du serveur se situent dans le fichier log_messages.txt

NB : Ce serveur tourne uniquement sous système Linux.

-----------------------------------------------------SERVEUR RESEAU----------------------------------------------------

