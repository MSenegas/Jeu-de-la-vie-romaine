#include "game.h"

// De manière générale, TOUJOURS décider du placement de la carte après le tirage dans Carte::tirer() , s'assurer de la conservation du onmbre de cartes
// (i.e. mettre defausser() ou J.add_... à la fin)

// METTRE J.add_carte_chance_tresor(this); DANS CarteSortezPrison::tirer(Joueur& J)
// METTRE J.cash_flow(gift); DANS CarteCash::tirer(Joueur& J)
// METTRE J.add_enfant(this); DANS CarteEnfant::tirer(Joueur& J)
// METTRE Warning dans CarteAchetez::tirer(Joueur& J)

// DÉFINIR Foncteurs de comparaison d'achat et de vente de CarteAchetez*

// S'assurer qu'aucune autre action ne doit être prise lors de la restitution d'une carte
void Carte::defausser() const {pioche_source.defausser(this);}
