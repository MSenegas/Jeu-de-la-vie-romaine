#pragma once
#include <random>

// Paramètres fondamentaux
const int ARGENT_INITIAL=100; // Argent que les joueurs ont en début de partie
const int FACES_DE=6; // Nombre de faces du dé standard

// Paramètres de plateau
const int N_CARRIERES=2; // Nombre de carrières différentes (nombre maximal se produisant en même temps)
const int PERIODE_CHEMIN_SUR_RISQUE=4; // Numéro de période correspondant au chemin sûr/ risqué

// Paramètres revenu supplémentaire
const int FACE_CAGNOTTE=6; // Face du dé nécessaire pour prendre la cagnotte
const int REVENU_CONSEILS=50; // Revenu supplémentaire par conseil
const int DOMMAGES_JUSTICE=100; // Somme que doit payer un joueur lorsqu'il est attaqué en justice

// Paramètres dettes
const int MIN_DETTES=50; // Quantité minimale de dettes que l'on peut contracter
const double TAUX_COUPON=0.05; // Taux d'intérêt

// Paramètres enfants
const int MAX_ENFANTS=6; // Nombre maximum d'enfants
const int INDEMNITE_SUBSTITUTION_ENFANTS=75; // Indemnité lors du dépassement du nombre d'enfants

// Paramètres prison
const int FACE_SORTIR_PRISON=6; // Face du dé qu'il faut obtenir pour sortir de prison
const int N_MAX_ESSAIS_PRISON=3; // Nombre maximal d'essais pour sortir de prison
const int FRAIS_SORTIE_PRISON=100; // Frais qu'il faut payer pour sortir de prison si on a pas de dé favorable

// Paramètres esclavage
const int N_MAX_ESCLAVAGE=2; // Nombre maximum de fois que l'on peut aller en esclavage
const int FRAIS_SORTIE_ESCLAVAGE=100; // Frais que l'on paye à la fin de la période d'esclavage

// Paramètres pari/loto
const int MISE_STANDARD_PARI_LOTO=28; // Somme usuellement misée par face lors d'un par/loto
const int FACES_DE_MISE_PARI_LOTO=12; // Nombre de faces du dé dont on achète les cases pour le pari/loto
const int FACES_DE_GAIN_LOTO=10; // Nombre de faces du dé du dé de gain au loto
const int FACTEUR_LOTO=50; // Nombre par lequel on doit multiplier le résultat du dé du gain de loto pour obtenir le gain

// Paramètres cartes achetez
const int MAX_CARTES_ACHETEZ_ACHAT=2; // Nombre maximal de cartes achetez que l'on peut acheter en même temps
const int MAX_CARTES_ACHETEZ_VENTE=2; // Nombre maximal de cartes achetez que l'on peut vendre en même temps

// Paramètres jeu hasard avantage
const int MIN_MISE_POSSIBLE_JEU_HASARD_AVANTAGE=5; // Sommes que l'on peut miser au jeu de hasard avantageux lorsque l'on tire la carte chance correspondante
const int MAX_MISE_POSSIBLE_JEU_HASARD_AVANTAGE=75;
const int FACE_JEU_HASARD_AVANTAGE=6; // Face du dé correspondant à la victoire dans ce jeu
const int FACTEUR_JEU_HASARD_AVANTAGE=8; // Nombre de fois que l'on gagne la mise si l'on joue à ce jeu

// Autres paramètres
const int INDEMNITE_AUTRE_CARTE_BONUS=47; // Indemnité que les joueurs recoivent pour renoncer à la carte bonus s'ils tirent la carte chance leur proposant de le faire

// Affichage
const int TAILLE_CASES_AFFICHAGE=50; // Taille des éléments d'affichage

 // Lois de probabilité
const std::uniform_int_distribution<> LANCER_DE(1,FACES_DE);
const std::uniform_int_distribution<> LANCER_PIECE(0,1);
const std::uniform_int_distribution<> LANCER_DE_MISE_PARI_LOTO(0,FACES_DE_MISE_PARI_LOTO-1);
const std::uniform_int_distribution<> LANCER_DE_GAIN_LOTO(1,FACES_DE_GAIN_LOTO);
