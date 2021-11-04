#include <Imagine/Common.h>
#include <Imagine/Graphics.h>
#include <stdexcept>

#include "case.h"
#include "game.h"

void Case::affichage(int i0, int j0) const {
    Imagine::drawRect(j0,i0,TAILLE_CASES_AFFICHAGE,TAILLE_CASES_AFFICHAGE,Imagine::BLACK);
}

CaseChance::CaseChance(Game& G):
    current_game(G) {}

//void CaseChance::tomber(Joueur& J) {}
//    // pioche_chance->back()->tirer(J);
//    // METTRE J.add_carte_chance_tresor(&this); DANS CarteSortezPrison::tirer(Joueur& J)
//    // pioche_chance->pop_back();

CaseTresor::CaseTresor(Game& G):
    current_game(G) {}

//void CaseTresor::tomber(Joueur& J) {}
//    // pioche_tresor->back()->tirer(J);
//    // METTRE J.add_carte_chance_tresor(&this); DANS CarteSortezPrison::tirer(Joueur& J)
//    // pioche_tresor->pop_back();

CaseSalaire::CaseSalaire(Game& G,const int& sal):
    current_game(G),salaire(sal) {}

//void CaseSalaire::tomber(Joueur& J) {}
//    // pioche_bonus->back()->tirer(J);
//    // METTRE J.cash_flow(gift); DANS CarteCash::tirer(Joueur& J)
//    // pioche_bonus->pop_back();

void CaseSalaire::passer(Joueur& J) {
    bool a_payer=true;
    for (unsigned int i=0;i<collected.size();i++)
        if (collected.at(i)==&J)
            a_payer=false;
    if (a_payer) {
        J.passer_salaire();
        J.cash_flow(salaire);
        ajouter_joueur_deja_paye(J);}
}

void CaseSalaire::ajouter_joueur_deja_paye(Joueur& J) {
    collected.push_back(&J);}

void CaseSalaire::reset_case() {
    collected=std::vector<Joueur*>();}

CasePari::CasePari(Game& G):
    current_game(G) {}

CaseLoto::CaseLoto(Game& G):
    current_game(G) {}

//void CasePari::tomber(Joueur& J) {} // Laissés vides pour l'instant
//void CaseLoto::tomber(Joueur& J) {}

CaseJustice::CaseJustice(Game& G):
    current_game(G) {}

//void CaseJustice::tomber(Joueur& J) {} // IA NÉCESSAIRE

CaseRejouez::CaseRejouez(Game& G):
    current_game(G) {}

void CaseRejouez::tomber(Joueur& J) {
    current_game.mouv_joueur(J,Imagine::intRandom(1,FACES_DE));}

CaseReculezAvancez::CaseReculezAvancez(Game& G):
    current_game(G) {}

void CaseReculezAvancez::tomber(Joueur& J) {
    if (Imagine::intRandom(0,1))
        current_game.mouv_joueur(J,Imagine::intRandom(1,FACES_DE));
    else
        current_game.mouv_joueur(J,-Imagine::intRandom(1,FACES_DE));
}

CaseEnfant::CaseEnfant(Game& G):
    current_game(G) {}

//void CaseEnfant::tomber(Joueur& J) {}
//    // J.add_enfant(pioche_enfant->back()->nb_enfants());
//    // pioche_enfant->pop_back();

void CasePrison::tomber(Joueur& J) {
    J.aller_prison();}

void CaseEsclave::tomber(Joueur& J) {
    J.aller_esclavage();}

CaseMarche::CaseMarche(Game& G):
    current_game(G) {}

//void CaseMarche::tomber(Joueur& J) {}

void CaseConseil::tomber(Joueur& J) {J.add_conseil();}

CaseCash::CaseCash(const int& gain):
    gift(gain) {}

void CaseCash::tomber(Joueur& J) {
    J.cash_flow(gift);}

CaseCashCagnotte::CaseCashCagnotte(Game& G,const int& gain):
    CaseCash(gain), current_game(G) {
    if (gift>0) throw std::domain_error("La cagnotte ne verse pas d'argent de cette manière.");}

void CaseCashCagnotte::tomber(Joueur& J) {
    J.cash_flow(gift);
    current_game.cagnotte-=gift;}

CaseCashJoueurs::CaseCashJoueurs(Game& G,const int& gain):
    CaseCash(gain), current_game(G) {}

void CaseCashJoueurs::tomber(Joueur& J) {
    for (unsigned int i=0;i<current_game.liste_joueurs.size();i++)
        current_game.liste_joueurs.at(i).cash_flow(-gift);
    J.cash_flow(current_game.liste_joueurs.size()*gift);
}

CaseCashCagnotteJoueurs::CaseCashCagnotteJoueurs(Game& G,const int& gain):
    CaseCashJoueurs(G,gain) {
    if (gift>0) throw std::domain_error("La cagnotte ne verse pas d'argent de cette manière.");}

void CaseCashCagnotteJoueurs::tomber(Joueur&) {
    for (unsigned int i=0;i<current_game.liste_joueurs.size();i++)
        current_game.liste_joueurs.at(i).cash_flow(gift);
    current_game.cagnotte-=current_game.liste_joueurs.size()*gift;
}
