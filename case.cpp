#include <Imagine/Graphics.h>
#include <stdexcept>

#include "case.h"
#include "game.h"

void Case::affichage(int i0, int j0) const {
    Imagine::drawRect(j0,i0,TAILLE_CASES_AFFICHAGE,TAILLE_CASES_AFFICHAGE,Imagine::BLACK);
}

void CaseChance::tomber(Joueur& J) {
    current_game.banque.pioche_chance.tirer(J);}

void CaseTresor::tomber(Joueur& J) {
    current_game.banque.pioche_tresor.tirer(J);}

void CaseSalaire::tomber(Joueur& J) {
    current_game.banque.pioche_bonus.tirer(J);}

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

void CasePari::tomber(Joueur& J) {std::cerr << "Warning: Case Pari non implémentée" << std::endl;}
void CaseLoto::tomber(Joueur& J) {std::cerr << "Warning: Case Loto non implémentée" << std::endl;}

void CaseJustice::tomber(Joueur& J) {std::cerr << "Warning: Case Justice non implémentée" << std::endl;}

void CaseRejouez::tomber(Joueur& J) {
    current_game.mouv_joueur(J,Game::lancer_de());}

void CaseReculezAvancez::tomber(Joueur& J) {
    if (Game::lancer_piece())
        current_game.mouv_joueur(J,Game::lancer_de());
    else
        current_game.mouv_joueur(J,-Game::lancer_de());
}

void CaseEnfant::tomber(Joueur& J) {
    current_game.banque.pioche_enfant.tirer(J);}

void CasePrison::tomber(Joueur& J) {
    J.aller_prison();}

void CaseEsclave::tomber(Joueur& J) {
    J.aller_esclavage();}

void CaseMarche::tomber(Joueur& J) {
    if (J.decision_acheter_vendre()) {
        std::vector<const Carte*> LC;
        for (int i=0;i<MAX_CARTES_ACHETEZ_ACHAT;i++) {
            LC.push_back(current_game.banque.pioche_achetez.pop());}
        J.decision_acheter_cartes(LC);
        J.acheter_cartes_achetez(LC);}
    else
        J.vendre_cartes_achetez();
}

void CaseConseil::tomber(Joueur& J) {J.add_conseil();}

void CaseCash::tomber(Joueur& J) {
    J.cash_flow(gift);}

CaseCashCagnotte::CaseCashCagnotte(Game& G,const int& gain):
    CaseCash(gain), current_game(G) {
    if (gift>0) throw std::domain_error("La cagnotte ne verse pas d'argent de cette manière.");}

void CaseCashCagnotte::tomber(Joueur& J) {
    J.cash_flow(gift);
    current_game.cagnotte-=gift;}

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
