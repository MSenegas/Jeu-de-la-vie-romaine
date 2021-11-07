#pragma once
#include <random>

// Ce fichier contient tous les paramètres numériques du jeu
const int N_CARRIERES=2; // Nombre de carrières différentes (nombre maximal se produisant en même temps)
const int N_MAX_ESSAIS_PRISON=3; // Nombre maximal d'essais pour sortir de prison
const int FACE_SORTIR_PRISON=6; // Face du dé qu'il faut obtenir pour sortir de prison
const int FRAIS_SORTIE_PRISON=100; // Frais qu'il faut payer pour sortir de prison si on a pas de dé favorable
const int FRAIS_SORTIE_ESCLAVAGE=100; // Frais que l'on paye à la fin de la période d'esclavage
const double TAUX_COUPON=0.05; // Taux d'intérêt
const int MIN_DETTES=50; // Quantité minimale de dettes que l'on peut contracter
const int FACE_CAGNOTTE=6; // Face du dé nécessaire pour prendre la cagnotte
const int REVENU_CONSEILS=50; // Revenu supplémentaire par conseil
const int FACES_DE=6; // Nombre de faces du dé standard
const int MAX_ENFANTS=6; // Nombre maximum d'enfants
const int INDEMNITE_SUBSTITUTION_ENFANTS=75; // Indemnité lors du dépassement du nombre d'enfants
const int PERIODE_CHEMIN_SUR_RISQUE=4; // Numéro de période correspondant au chemin sûr/ risqué
const int ARGENT_INITIAL=100; // Argent que les joueurs ont en début de partie
const int TAILLE_CASES_AFFICHAGE=20; // Taille des cases
const int FACES_DE_MISE_PARI_LOTO=12; // Nombre de faces du dé dont on achète les cases pour le pari/loto
const int FACTEUR_LOTO=50; // Nombre par lequel on doit multiplier le résultat du dé du gain de loto pour obtenir le gain
const int FACES_DE_GAIN_LOTO=10; // Nombre de faces du dé du dé de gain au loto

const std::uniform_int_distribution<> LANCER_DE(1,FACES_DE); // Lois de probabilité
const std::uniform_int_distribution<> LANCER_PIECE(0,1);
const std::uniform_int_distribution<> LANCER_DE_MISE_PARI_LOTO(1,FACES_DE_MISE_PARI_LOTO);
const std::uniform_int_distribution<> LANCER_DE_GAIN_LOTO(1,FACES_DE_GAIN_LOTO);
