

---------------------------------------------------------------------------------BASE DE DONNEES BANQUE----------------------------------------------------------------------------

CREATE TABLE Agence (
	id_agence CHAR (6) NOT NULL ,
	adresse_agence VARCHAR (100) NOT NULL ,
	telephone_agence CHAR (10) NOT NULL ,
	CONSTRAINT agence_pk PRIMARY KEY (id_agence) ,
	CONSTRAINT ck_agence CHECK (id_agence LIKE 'A%')
);

CREATE TABLE Clients (
	id_client CHAR (6) NOT NULL ,
	nom_client VARCHAR (25) NOT NULL ,
	prenom_client VARCHAR (25) NOT NULL ,
	date_naissance DATE NOT NULL ,
	telephone_client CHAR (10) NOT NULL ,
	adresse_client VARCHAR (100) NOT NULL ,
	adresse_mail VARCHAR (100) ,
	profession VARCHAR (50) NOT NULL ,
	mdp VARCHAR (300) NOT NULL ,
	epargne_client FLOAT ,
	CONSTRAINT clients_pk PRIMARY KEY (id_client) ,
	CONSTRAINT ck_clients CHECK (id_client LIKE 'CL%' OR adresse_mail LIKE '%@%' OR date_naissance > '1900-01-01')
);

CREATE TABLE Conseiller(
	id_conseiller CHAR (6) NOT NULL ,
	nom_conseiller VARCHAR (25) NOT NULL ,
	prenom_conseiller VARCHAR (25) NOT NULL ,
	disponibilite BOOLEAN DEFAULT true ,
	telephone_conseiller CHAR (10) NOT NULL ,
	mdp VARCHAR (300) NOT NULL ,
	id_agence CHAR (6) NOT NULL ,
	CONSTRAINT conseiller_pk PRIMARY KEY (id_conseiller) ,
	CONSTRAINT conseiller_fk FOREIGN KEY (id_agence) REFERENCES Agence (id_agence) ,
	CONSTRAINT ck_conseiller CHECK (id_conseiller LIKE 'CO%' OR id_agence LIKE 'A%')
);

CREATE TABLE Directeur(
	id_directeur CHAR (6) NOT NULL ,
	annee_promu_directeur DATE NOT NULL ,
	CONSTRAINT directeur_pk PRIMARY KEY (id_directeur) ,
	CONSTRAINT directeur_fk FOREIGN KEY (id_directeur) REFERENCES Conseiller (id_conseiller) ,
	CONSTRAINT directeur_promu CHECK (annee_promu_directeur > '1950-01-01' OR id_directeur LIKE 'CO%')
);

CREATE TABLE A_pris_rendez_vous_conseiller(
	id_rdv CHAR (6) NOT NULL ,
	lieu_rdv VARCHAR (50) NOT NULL ,
	date_rdv TIMESTAMP NOT NULL ,
	motif VARCHAR (50) NULL ,
	id_conseiller CHAR (6) NOT NULL ,
	id_client CHAR (6) NOT NULL ,
	CONSTRAINT a_pris_rdv_pk PRIMARY KEY (id_rdv , id_client , id_conseiller) ,
	CONSTRAINT a_pris_rdv_fk_co FOREIGN KEY (id_conseiller) REFERENCES Conseiller (id_conseiller) ,
	CONSTRAINT a_pris_rdv_fk_cl FOREIGN KEY (id_client) REFERENCES Clients (id_client) ,
	CONSTRAINT ck_a_pris_rdv CHECK (id_rdv LIKE 'R%' OR id_conseiller LIKE 'CO%' OR id_client LIKE 'CL%')
);


CREATE TABLE Est_client(
	id_client CHAR (6) NOT NULL ,
	id_conseiller CHAR (6) NOT NULL ,
	CONSTRAINT est_client_pk PRIMARY KEY (id_client , id_conseiller) ,
	CONSTRAINT est_client_pf_cl FOREIGN KEY (id_client) REFERENCES Clients (id_client) ,
	CONSTRAINT est_client_pf_co FOREIGN KEY (id_conseiller) REFERENCES Conseiller (id_conseiller)
);

CREATE TYPE Type_Transaction AS ENUM(
	'Retrait',
	'Depot',
	'Chèque',
	'Virement',
	'paiement par carte'
);


CREATE TYPE Type_Compte AS ENUM(
	'Courant',
	'Joint',
	'Livret A',
	'Livret Jeune',
	'Compte Epargne Logement',
	'Plan Epargne Logement',
	'Compte Epargne',
	'Plan Epargne Retraite'
);


CREATE TYPE Type_Cartes AS ENUM(
	'Visa',
	'MasterCarde',
	'Gold MasterCard',
	'Visa Infinite',
	'Carte commerciale',
	'Carte de retrait',
	'Carte de paiement', 
	'Carte de credit'
);

CREATE TABLE Compte(
	num_compte CHAR(8) PRIMARY KEY, 
	type_compte Type_Compte NOT NULL, 
	taux FLOAT NOT NULL, 
	epargne_Compte FLOAT,
	CONSTRAINT taux_max CHECK (taux < 45.0)
);

CREATE TABLE Terminal(
	id_terminal CHAR(8) PRIMARY KEY, 
	nom_societe VARCHAR(50) NOT NULL
);

CREATE TABLE Carte(
	num_carte CHAR(16) PRIMARY KEY, 
	date_validite DATE, 
	type_carte Type_Cartes, 
	num_compte CHAR(8) REFERENCES Compte(num_compte)
);

CREATE TABLE Transaction(
	num_transaction CHAR(8) PRIMARY KEY, 
	type_transaction Type_Transaction NOT NULL, 
	montant FLOAT NOT NULL, 
	motif VARCHAR(100), 
	date_transaction DATE, 
	date_execution DATE,
	num_compte_debiteur CHAR(8) REFERENCES Compte(num_compte),
	num_compte_crediteur CHAR(8) REFERENCES Compte(num_compte),
	id_terminal CHAR(8) REFERENCES Terminal(id_terminal),
	CONSTRAINT num_compte_debiteur_null CHECK ( (num_compte_debiteur IS NULL AND num_compte_crediteur IS NOT NULL) OR (num_compte_debiteur IS NOT NULL AND num_compte_crediteur IS NULL) OR 
	(num_compte_debiteur IS NOT NULL AND num_compte_crediteur IS NOT NULL))
);

CREATE TABLE Possede_Compte(
	id_client CHAR(6) REFERENCES Clients(id_client),
	num_compte CHAR(8) REFERENCES Compte(num_compte),
	PRIMARY KEY (id_client, num_compte)
);

--------------------------------------------------------------------------------------------Sup tab------------------------------------------------------------------------------------------------------------

DROP TABLE A_pris_rendez_vous_conseiller ;
DROP TABLE Est_client ;
DROP TABLE Directeur ;
DROP TABLE Conseiller ;
DROP TABLE Agence ;
DROP TABLE Possede_Compte ;
DROP TABLE Clients ;
DROP TABLE Carte ;
DROP TABLE Transaction ;
DROP TABLE Compte ;
DROP TABLE Terminal ;

------------------------------------------------------------------------------------------------------Donnees-------------------------------------------------------------------------------------------------
INSERT INTO Agence (id_agence , adresse_agence , telephone_agence) VALUES ('A1845B' , '22 Boulevard du Port, 95000 Cergy' , '0110252412') ;
INSERT INTO Agence (id_agence , adresse_agence , telephone_agence) VALUES ('A1765C' , '44 Rue Roger Salengro, 60110 Meru' , '0344789555');
INSERT INTO Agence (id_agence , adresse_agence , telephone_agence) VALUES ('A1854V' , '11 Rue du Maresquel 59242 Templeuve' , '0223565847');

INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL0124' , 'ETAME' , 'Cédric' , '1992-11-21' , '0766116453', '20 Passage Courtois, 75011 Paris' , 'whoosdatblack@gmail.com' , 'etudiant' , 'reinealex', 300.25) ;
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL1254' , 'Girard' , 'Vincent' , '1947-05-14' , '0621245354' , '27 Rue de la Joie, 75013 Paris ' , 'vincent.girard@yahoo.com' , 'peintre' , '' , 2500.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL5345' , 'Mosquera' , 'Alexis' , '2006-03-31' , '0701020304' , '34 Avenue de la Paix, 78011 Maisons-Laffitte' , 'mosquera.alexis@gmail.com' , 'etudiant' , '' , 50.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL2569' , 'Denorme' , 'Alexis' , '1996-02-27' , '0627010301' , '12 Impasse Courtois, 75000 Paris' , 'denorme.alexis@hotmail.com' , 'etudiant' , '' , 1000.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL4785' , 'Dubuis' , 'Alexis' , '1949-04-30' , '0704025367' , '27 Boulevard de l Oise, 95000 Cergy' , 'Dubuis.Alexis@hotmail.com' , 'retraite' , '' , 5000.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL1236' , 'LeMaitre' , 'Guillaume' , '1949-04-30' , '0612035967' , '28 Boulevard de l Oise, 95000 Cergy' , 'lemaitre.guillaume@yahoo.com' , 'retraite' , '' , 75.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL3259' , 'Leveque' , 'Jean' , '1985-02-19' , '0694532456' , '29 Boulevard de l Oise, 95000 Cergy' , 'leveque.jean@gmail.com' , 'charcutier' , '' , 2000.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL2369' , 'Dhuicque' , 'Corine' , '1966-08-13' , '0732145397' , '30 Boulevard de l Oise, 95000 Cergy' , 'dhuicque.corine@outlook.com' , 'retraite' , '' , 8001.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL5869' , 'Feron' , 'Yvette' , '1958-02-28' , '0624353697' , '5 Boulevard de l Oise, 95000 Cergy' , 'feron.yvette@outlook.com' , 'retraite' , '' , 9000.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL2548' , 'Martin' , 'Adrastée' , '1963-10-10' , '0712365894' , '11 Boulevard de l Oise, 95000 Cergy' , 'martin.adrastee@gmail.com' , 'retraite' , '' , 3500.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL2567' , 'Bernard' , 'Agathe' , '2001-10-24' , '0705015987' , '36 Boulevard de l Oise, 95000 Cergy' , 'bernard.agathe@hotmail.com' , 'etudiant' , '' , 500.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL4656' , 'Morel' , 'Alaïs ' , '1971-07-06' , '0612345697' , '32 Boulevard de l Oise, 95000 Cergy' , 'morel.alais@yahoo.com' , 'charpentier' , '' , 1500.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL8023' , 'Lefevre' , 'Alice' , '1996-09-11' , '0754565894' , '2 Avenue de la Paix, 94000 Maisons-Alfort' , 'lefevre_alice@gmail.com' , 'etudiante' , '' , 200.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL0023' , 'Andre' , 'Agathe ' , '1931-03-31' , '0612323789' , '5 Avenue de la Paix, 94000 Maisons-Alfort' , 'andre_agathe@yahoo.com' , 'retraite' , '' , 6750.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL4580' , 'Garnier' , 'Alexandrine' , '1958-01-27' , '0756217862' , '10 Avenue de la Paix, 94000 Maisons-Alfort' , 'garnier.alexandrine@hotmail.com' , 'retraite' , '' , 7000.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL8050' , 'Francois' , 'Anaïs' , '1949-08-20' , '0654238954' , '11 Avenue de la Paix, 94000 Maisons-Alfort' , 'francois.anais@gmail.com' , 'retraite' , '' , 4200.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL2565' , 'Bouvier' , 'Angèle' , '2005-01-07' , '0624598715' , '25 Avenue de la Paix, 94000 Maisons-Alfort' , 'bouvier.angele@hotmail.com' , 'etudiante' , '' , 900.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL7507' , 'Bailly' , 'Annette' , '1966-11-12' , '0756214539' , '6 Avenue de la Paix, 94000 Maisons-Alfort' , 'bailly_anette8@yahoo.com' , 'architecte' , '' , 15000.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL7445' , 'Hubert' , 'Antoinette' , '1934-02-13' , '0624659824' , '13 Avenue de la Paix, 94000 Maisons-Alfort' , 'hubert_antoinette@gmail.com' , 'retraite' , '' , 2800.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL8798' , 'Bouvier' , 'Ariel' , '1940-12-15' , '0635369877' , '14 Avenue de la Paix, 94000 Maisons-Alfort' , 'bouvier_ariel@gmail.com' , 'retraite' , '' , 12000.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL7856' , 'Garcia' , 'Leo' , '1940-09-30' , '0725634589' , '100 Rue de Chevilly, 94800 Villejuif' , 'garcia_leo44@gmail.com' , 'retraite' , '' , 6450.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL1569' , 'Martin' , 'Nicolas' , '1949-06-09' , '0612356987' , '120 Rue de Chevilly, 94800 Villejuif' , 'martin.nicolas66@hotmail.com' , 'retraite' , '' , 9000.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL1235' , 'Martinez' , 'Matheo' , '2013-04-11' , '0755446691' , '125 Rue de Chevilly, 94800 Villejuif' , 'martinez_matheo66@gmail.com' , 'eleve' , '' , 150.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL1258' , 'Bonnet' , 'Lucie' , '1967-02-10' , '0754212364' , '126 bis Rue de Chevilly, 94800 Villejuif' , 'bonnet_lucie@hotmail.com' , 'medecin' , '' , 5550.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL2356' , 'Bernard' , 'Alexia' , '1985-11-19' , '0625423122' , '102 Rue de Chevilly, 94800 Villejuif' , 'bernard.alexia0@yahoo.com' , 'secretaire' , '' , 1805.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL9856' , 'Garcia' , 'Alice' , '1973-12-02' , '0765412356' , '130 Rue de Chevilly, 94800 Villejuif' , 'garcia.alice@hotmail.com' , 'ingenieur' , '' , 3200.0);
INSERT INTO Clients (id_client , nom_client , prenom_client , date_naissance , telephone_client , adresse_client , adresse_mail , profession , mdp , epargne_client) VALUES ('CL8569' , 'Raynal' , 'Jean' , '1977-04-20' , '0725123456' , '54 Rue de Chevilly, 94800 Villejuif' , 'raynal_jean44@gmail.com' , 'professeur' , '' , 2750.0);


INSERT INTO Conseiller (id_conseiller , nom_conseiller , prenom_conseiller , disponibilite , telephone_conseiller , mdp , id_agence) VALUES ('CO2455', 'AJANA' , 'Céline' , true , '0105240108' , 'rachidubois' , 'A1845B') ;
INSERT INTO Conseiller (id_conseiller , nom_conseiller , prenom_conseiller , disponibilite , telephone_conseiller , mdp , id_agence) VALUES ('CO2454' , 'Dhuicque' , 'Corine' , true , '0125301001' , '' , 'A1845B');
INSERT INTO Conseiller (id_conseiller , nom_conseiller , prenom_conseiller , disponibilite , telephone_conseiller , mdp , id_agence) VALUES ('CO1456' , 'Richard' , 'Axel' , true , '0125866959' , '' , 'A1845B');
INSERT INTO Conseiller (id_conseiller , nom_conseiller , prenom_conseiller , disponibilite , telephone_conseiller , mdp , id_agence) VALUES ('CO1223' , 'Petit' ,  'Lucas' , true , '0125965824' , '' , 'A1845B');
INSERT INTO Conseiller (id_conseiller , nom_conseiller , prenom_conseiller , disponibilite , telephone_conseiller , mdp , id_agence) VALUES ('CO1256' , 'Coralie' , 'Coralie' , false , '0258963252' , '' , 'A1765C');
INSERT INTO Conseiller (id_conseiller , nom_conseiller , prenom_conseiller , disponibilite , telephone_conseiller , mdp , id_agence) VALUES ('CO5447' , 'Klein' , 'Noemie' , true , '0185697425' , '' , 'A1765C');
INSERT INTO Conseiller (id_conseiller , nom_conseiller , prenom_conseiller , disponibilite , telephone_conseiller , mdp , id_agence) VALUES ('CO8732' , 'Maillard' , 'Lena' , true ,  '0258695211' , '' , 'A1765C');
INSERT INTO Conseiller (id_conseiller , nom_conseiller , prenom_conseiller , disponibilite , telephone_conseiller , mdp , id_agence) VALUES ('CO1458' , 'Lemaire' , 'Theo' , false ,  '012598364' , '' , 'A1854V');
INSERT INTO Conseiller (id_conseiller , nom_conseiller , prenom_conseiller , disponibilite , telephone_conseiller , mdp , id_agence) VALUES ('CO4032' , 'Barbier' , 'Elsa' , true , '0225895145' , '' , 'A1854V');


INSERT INTO Directeur (id_directeur , annee_promu_directeur) VALUES ('CO2454' , '2006-06-24') ;
INSERT INTO Directeur (id_directeur , annee_promu_directeur) VALUES ('CO1256' , '2004-01-2');
INSERT INTO Directeur (id_directeur , annee_promu_directeur) VALUES ('CO1458' , '2019-05-12');

INSERT INTO A_pris_rendez_vous_conseiller (id_rdv , lieu_rdv , date_rdv , motif , id_conseiller , id_client) VALUES ('R2541B' , '22 Boulevard du Port, 95000 Cergy' , '2021-11-24 10:30:00' , 'Gestion Compte' , 'CO2455' , 'CL0124') ;

INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL0124' , 'CO2455') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL1254' , 'CO2455') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL5345' , 'CO2455') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL2569' , 'CO2454') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL4785' , 'CO2454') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL1236' , 'CO2454') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL3259' , 'CO1456') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL2369' , 'CO1456') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL5869' , 'CO1456') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL2548' , 'CO1223') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL2567' , 'CO1223') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL4656' , 'CO1223') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL8023' , 'CO1256') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL0023' , 'CO1256') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL4580' , 'CO1256') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL8050' , 'CO5447') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL2565' , 'CO5447') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL7507' , 'CO5447') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL7445' , 'CO8732') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL8798' , 'CO8732') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL7856' , 'CO8732') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL1569' , 'CO1458') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL1235' , 'CO1458') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL1258' , 'CO1458') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL2356' , 'CO4032') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL9856' , 'CO4032') ;
INSERT INTO Est_client (id_client , id_conseiller) VALUES ('CL8569' , 'CO4032') ;

INSERT INTO Compte (num_compte , type_compte , taux , epargne_Compte) VALUES ('00000001' , 'Courant', 2 , 100.0) ;
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('00000002' ,'Livret Jeune' , 3 , 500.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('00000003' , 'Courant' , 1 , 200.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('01456826' , 'Courant' , 1 , 1000.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('14598636' , 'Courant' , 3 , 900.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('12548693' , 'Livret A' , 2 , 550.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('14589515' , 'Courant' , 1 , 200.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('14526896' , 'Livret Jeune' , 3 , 5000.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('12586328' , 'Courant' , 2 , 2250.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('14569872' , 'Compte Epargne' , 4 , 1150.30);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('14583958' , 'Courant' , 2 , 1450.5);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('12356982' , 'Livret Jeune', 3 , 2000.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('14569874' , 'Courant', 1 , 150.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('12356987' , 'Livret A', 1 , 3550.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('14523694' , 'Courant', 1 , 240.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('58963214' , 'Plan Epargne Logement', 2, 1600.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('12536786' , 'Courant' , 2 ,1400.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('25694213' , 'Courant' , 3 , 650.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('10250842' , 'Livret A' , 2 , 350.64);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('01233544' , 'Courant' , 2 , 210.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('75613211' , 'Livret A' , 2 , 940.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('78965126' , 'Compte Epargne' , 1 , 400.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('85420021' , 'Courant' , 2 , 230.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('78512369' , 'Courant' , 3 , 640.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('47546289' , 'Livret A' , 1 , 320.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('15816874' , 'Compte Epargne' , 2 , 740.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('87952156' , 'Compte Epargne' , 1 , 550.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('75695248' , 'Courant' , 1 , 390.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('75439958' , 'Courant' , 2 , 170.0);
INSERT INTO Compte(num_compte, type_compte, taux, epargne_Compte) VALUES ('12458768' , 'Compte Epargne' , 1 , 460.80);

INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL0124' , '00000001') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL1254' , '00000002') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL5345' , '00000003') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL2569' , '01456826') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL4785' , '14598636') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL1236' , '12548693') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL3259' , '14589515') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL2369' , '14526896') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL5869' , '12586328') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL2548' , '14569872') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL2567' , '14583958') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL4656' , '12356982') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL8023' , '14569874') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL0023' , '14523694') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL4580' , '58963214') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL8050' , '12536786') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL2565' , '25694213') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL7507' , '10250842') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL7445' , '01233544') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL8798' , '75613211') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL7856' , '78965126') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL1569' , '85420021') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL1235' , '78512369') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL1258' , '47546289') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL2356' , '15816874') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL9856' , '87952156') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL8569' , '75695248') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL0124' , '75439958') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL1569' , '12458768') ;
INSERT INTO Possede_Compte (id_client , num_compte) VALUES ('CL0023' , '00000001') ;



INSERT INTO Terminal(id_terminal, nom_societe) VALUES ('00000001', 'Fnac');
INSERT INTO Terminal(id_terminal, nom_societe) VALUES ('00000002', 'Boulanger');
INSERT INTO Terminal(id_terminal, nom_societe) VALUES ('01258621' , 'Google');
INSERT INTO Terminal(id_terminal, nom_societe) VALUES ('12521221' , 'Amazon');
INSERT INTO Terminal(id_terminal, nom_societe) VALUES ('21568521' , 'Mcdonald');
INSERT INTO Terminal(id_terminal, nom_societe) VALUES ('15932884' , 'Auchan');
INSERT INTO Terminal(id_terminal, nom_societe) VALUES ('15981359' , 'leclerc');
INSERT INTO Terminal(id_terminal, nom_societe) VALUES ('15841254' , 'Mcdonald');
INSERT INTO Terminal(id_terminal, nom_societe) VALUES ('15586854' , 'Louvre');
INSERT INTO Terminal(id_terminal, nom_societe) VALUES ('12586355' , 'Burger king');

INSERT INTO Carte(num_carte, date_validite, type_carte, num_compte) VALUES ('0125353698712585', '2023-11-25', 'Visa', '00000001');
INSERT INTO Carte(num_carte, date_validite, type_carte, num_compte) VALUES ('0256324789201585', '2023-05-02', 'Visa', '00000003');
INSERT INTO Carte(num_carte, date_validite, type_carte, num_compte) VALUES ('5055126581872687', '2024-02-15', 'MasterCarde', '01456826');
INSERT INTO Carte(num_carte, date_validite, type_carte, num_compte) VALUES ('1492205881036588', '2022-07-24', 'Visa Infinite', '14598636');
INSERT INTO Carte(num_carte, date_validite, type_carte, num_compte) VALUES ('1235896472156369', '2023-10-12', 'Gold MasterCard', '14589515');
INSERT INTO Carte(num_carte, date_validite, type_carte, num_compte) VALUES ('1025669815476300', '2024-01-10', 'Carte de retrait', '12586328');
INSERT INTO Carte(num_carte, date_validite, type_carte, num_compte) VALUES ('2458962565324563', '2022-12-23', 'Carte de credit', '14583958');
INSERT INTO Carte(num_carte, date_validite, type_carte, num_compte) VALUES ('2165889621596328', '2023-09-04', 'Visa', '14569874');
INSERT INTO Carte(num_carte, date_validite, type_carte, num_compte) VALUES ('0235845236468264', '2023-07-15', 'Carte commerciale', '14523694');
INSERT INTO Carte(num_carte, date_validite, type_carte, num_compte) VALUES ('2568134659788521', '2023-05-10', 'Carte de credit', '12536786');
INSERT INTO Carte(num_carte, date_validite, type_carte, num_compte) VALUES ('1026587542136985', '2023-03-12', 'Carte commerciale', '25694213');
INSERT INTO Carte(num_carte, date_validite, type_carte, num_compte) VALUES ('1023689520145669', '2022-05-12', 'MasterCarde', '01233544');
INSERT INTO Carte(num_carte, date_validite, type_carte, num_compte) VALUES ('1547896325698426', '2024-05-05', 'Visa', '85420021');
INSERT INTO Carte(num_carte, date_validite, type_carte, num_compte) VALUES ('1568925468255458', '2022-12-12', 'Carte commerciale' , '78512369');
INSERT INTO Carte(num_carte, date_validite, type_carte, num_compte) VALUES ('1235699854122252', '2023-08-25', 'Carte commerciale' , '75695248');
INSERT INTO Carte(num_carte, date_validite, type_carte, num_compte) VALUES ('1256589632565666', '2024-02-24', 'Visa' , '75439958');
INSERT INTO Carte(num_carte, date_validite, type_carte, num_compte) VALUES ('1456325896566256', '2023-04-20', 'Gold MasterCard' , '75439958');

INSERT INTO Transaction( num_transaction, type_transaction, montant, motif, date_transaction, date_execution, num_compte_debiteur) VALUES ('00210000','Retrait', '100', 'achat nouriture', '01/11/2021', '01/10/2021', '00000001');
INSERT INTO Transaction( num_transaction, type_transaction, montant, motif, date_transaction, date_execution, num_compte_debiteur, num_compte_crediteur) VALUES ('00000007','Virement', '100', 'achat nouriture', '02/09/2019', '02/09/2019', '00000001', '00000001') ;
INSERT INTO Transaction( num_transaction, type_transaction, montant, motif, date_transaction, date_execution, num_compte_crediteur) VALUES ('00000750','Depot', '100', 'achat nouriture', '10/01/2020', '11/01/2020', '00000003') ;

INSERT INTO Transaction( num_transaction, type_transaction, montant, motif, date_transaction, date_execution, num_compte_debiteur, num_compte_crediteur) VALUES ('00245436','Virement', 200.0 , 'achat materiels', '13/11/2021', '13/11/2021', '00000001' , '14583958') ;
INSERT INTO Transaction( num_transaction, type_transaction, montant, motif, date_transaction, date_execution, num_compte_debiteur, num_compte_crediteur) VALUES ('00000054','Virement', 500.0 , 'achat chaussures', '02/09/2019', '02/09/2019', '00000001', '14583958') ;
INSERT INTO Transaction( num_transaction, type_transaction, montant, motif, date_transaction, date_execution, num_compte_crediteur) VALUES ('00245978','Depot', 100.0 , '', '10/01/2020', '10/01/2020', '00000003') ;
INSERT INTO Transaction( num_transaction, type_transaction, montant, motif, date_transaction, date_execution, num_compte_debiteur) VALUES ('00246538','Retrait', 50.0 , '' , '01/11/2021', '01/10/2021', '00000001') ;
INSERT INTO Transaction( num_transaction, type_transaction, montant, motif, date_transaction, date_execution, num_compte_debiteur, num_compte_crediteur) VALUES ('00123546' , 'Virement', 200.0 , 'Remboursement', '02/09/2019', '02/09/2019', '75695248' , '00000001') ; 
INSERT INTO Transaction( num_transaction, type_transaction, montant, motif, date_transaction, date_execution, num_compte_debiteur , num_compte_crediteur) VALUES ('00000300', 'Chèque' , 100.0 , 'achat nouriture', '10/01/2020', '14/01/2020', '00000003', '00000002') ;
INSERT INTO Transaction( num_transaction, type_transaction, montant, motif, date_transaction, date_execution, num_compte_debiteur, num_compte_crediteur , id_terminal) VALUES ('00054213' ,'paiement par carte', 30.0 , 'Mcdonald' , '14/11/2021', '14/11/2021', '00000001', '12536786' , '21568521') ;
INSERT INTO Transaction( num_transaction, type_transaction, montant, motif, date_transaction, date_execution, num_compte_debiteur, num_compte_crediteur) VALUES ('00001234','Virement', 150.0 , 'aides scolaires' , '02/09/2019', '02/09/2019', '01233544', '00000001') ;
INSERT INTO Transaction( num_transaction, type_transaction, montant, motif, date_transaction, date_execution, num_compte_crediteur) VALUES ('00000500', 'Depot' , 200.0 , '' , '10/01/2020', '10/01/2020', '00000003') ;
INSERT INTO Transaction( num_transaction, type_transaction, montant, motif, date_transaction, date_execution, num_compte_debiteur) VALUES ('00000850' , 'Retrait', 100.0 , '' , '01/11/2021', '01/11/2021', '00000002') ;
INSERT INTO Transaction( num_transaction, type_transaction, montant, motif, date_transaction, date_execution, num_compte_debiteur, num_compte_crediteur) VALUES ('00000421' , 'paiement par carte', 75.0 , 'Auchan', '02/06/2021', '02/06/2021', '75695248', '75439958', '15932884') ;
INSERT INTO Transaction( num_transaction, type_transaction, montant, motif, date_transaction, date_execution, num_compte_debiteur , num_compte_crediteur) VALUES ('00000033','Cheque', 200.0, 'achat mobilier' , '10/01/2020', '14/01/2020', '00000003', '00000002') ;
INSERT INTO Transaction( num_transaction, type_transaction, montant, motif, date_transaction, date_execution, num_compte_debiteur) VALUES ('00000401' , 'Retrait' , 100.0 , '', '22/10/2021', '22/10/2021' , '00000001') ;
INSERT INTO Transaction( num_transaction, type_transaction, montant, motif, date_transaction, date_execution, num_compte_debiteur, num_compte_crediteur) VALUES ('00000302' , 'Virement', 120.0 , 'reglement scolarite', '02/09/2019', '02/09/2019', '14583958', '00000001') ;
INSERT INTO Transaction( num_transaction, type_transaction, montant, motif, date_transaction, date_execution, num_compte_crediteur) VALUES ('00000225','Depot', 100.0, '', '10/01/2020', '11/01/2020', '00000003') ;
INSERT INTO Transaction( num_transaction, type_transaction, montant, motif, date_transaction, date_execution, num_compte_debiteur, num_compte_crediteur , id_terminal) VALUES ('00000464','paiement par carte', 150.0 , 'Amazon' , '01/05/2021', '01/05/2021', '00000001', '00000002' , '12521221') ; 
INSERT INTO Transaction( num_transaction, type_transaction, montant, motif, date_transaction, date_execution, num_compte_debiteur, num_compte_crediteur) VALUES ('00000666','Virement', 75.0 , 'Facture Navigo', '02/09/2019', '02/09/2019', '14523694', '00000001') ;
