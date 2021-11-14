#include <Imagine/Graphics.h>
#include <stdexcept>

#include "case.h"
#include "game.h"

void CaseChance::tomber(Joueur& J) const {
    current_game.banque.pioche_chance.tirer(J);}

void CaseTresor::tomber(Joueur& J) const {
    current_game.banque.pioche_tresor.tirer(J);}

void CaseSalaire::tomber(Joueur& J) const {
    current_game.banque.pioche_bonus.tirer(J);}

void CaseSalaire::passer(Joueur& J) const {
    for (unsigned int i=0;i<J.salaires_recus.size();i++)
        if (J.salaires_recus.at(i)==this)
            return;
    J.passer_salaire();
    J.cash_flow(salaire);
    J.ajouter_salaire_deja_recu(this);
}

void CasePari::tomber(Joueur& J) const {std::cerr << "Warning: Case Pari non implémentée" << std::endl;}
void CaseLoto::tomber(Joueur& J) const {std::cerr << "Warning: Case Loto non implémentée" << std::endl;}

void CaseJustice::tomber(Joueur& J) const {std::cerr << "Warning: Case Justice non implémentée" << std::endl;}

void CaseRejouez::tomber(Joueur& J) const {
    current_game.mouv_joueur(J,Game::lancer_de());}

void CaseReculezAvancez::tomber(Joueur& J) const {
    if (Game::lancer_piece())
        current_game.mouv_joueur(J,Game::lancer_de());
    else
        current_game.mouv_joueur(J,-Game::lancer_de());
}

void CaseEnfant::tomber(Joueur& J) const {
    current_game.banque.pioche_enfant.tirer(J);}

void CasePrison::tomber(Joueur& J) const {
    J.aller_prison();}

void CaseEsclave::tomber(Joueur& J) const {
    J.aller_esclavage();}

void CaseMarche::tomber(Joueur& J) const {
    if (J.decision_acheter_vendre()) {
        std::vector<const Carte*> LC;
        for (int i=0;i<MAX_CARTES_ACHETEZ_ACHAT;i++) {
            const Carte* C=current_game.banque.pioche_achetez.pop();
            if (C)
                LC.push_back(C);}
        J.decision_acheter_cartes(LC);
        J.acheter_cartes_achetez(LC);}
    else
        J.vendre_cartes_achetez();
}

void CaseConseil::tomber(Joueur& J) const {J.add_conseil();}

void CaseCash::tomber(Joueur& J) const {
    J.cash_flow(gift);}

CaseCashCagnotte::CaseCashCagnotte(Game& G,const int& gain):
    CaseCash(gain), current_game(G) {
    if (gift>0) throw std::domain_error("La cagnotte ne verse pas d'argent de cette manière.");}

void CaseCashCagnotte::tomber(Joueur& J) const {
    J.cash_flow(gift);
    current_game.cagnotte-=gift;}

void CaseCashJoueurs::tomber(Joueur& J) const {
    for (unsigned int i=0;i<current_game.liste_joueurs.size();i++)
        current_game.liste_joueurs.at(i).cash_flow(-gift);
    J.cash_flow(current_game.liste_joueurs.size()*gift);
}

CaseCashCagnotteJoueurs::CaseCashCagnotteJoueurs(Game& G,const int& gain):
    CaseCashJoueurs(G,gain) {
    if (gift>0) throw std::domain_error("La cagnotte ne verse pas d'argent de cette manière.");}

void CaseCashCagnotteJoueurs::tomber(Joueur&) const {
    for (unsigned int i=0;i<current_game.liste_joueurs.size();i++)
        current_game.liste_joueurs.at(i).cash_flow(gift);
    current_game.cagnotte-=current_game.liste_joueurs.size()*gift;
}

void Case::affichage(int i0, int j0) const {
    Imagine::drawRect(j0,i0,TAILLE_CASES_AFFICHAGE,TAILLE_CASES_AFFICHAGE,Imagine::BLACK);
}

void CaseStop::affichage(int i0, int j0) const {
    Case::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Stop",Imagine::RED,0.2*TAILLE_CASES_AFFICHAGE);
}

void CaseChance::affichage(int i0, int j0) const {
    Case::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Chance",Imagine::Color(216,183,0),0.2*TAILLE_CASES_AFFICHAGE);
}

void CaseTresor::affichage(int i0, int j0) const {
    Case::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Trésor",Imagine::Color(216,183,0),0.2*TAILLE_CASES_AFFICHAGE);
}

void CaseSalaire::affichage(int i0, int j0) const {
    Case::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Salaire",Imagine::Color(0,178,0),0.2*TAILLE_CASES_AFFICHAGE);
    Imagine::drawString(j0,i0+0.4*TAILLE_CASES_AFFICHAGE,std::to_string(salaire),Imagine::Color(0,178,0),0.2*TAILLE_CASES_AFFICHAGE);
}

void CasePari::affichage(int i0, int j0) const {
    Case::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Pari",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CaseLoto::affichage(int i0, int j0) const {
    Case::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Loto",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CaseJustice::affichage(int i0, int j0) const {
    Case::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Justice",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CaseRejouez::affichage(int i0, int j0) const {
    Case::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Rejouez",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CaseReculezAvancez::affichage(int i0, int j0) const {
    Case::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Reculez",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
    Imagine::drawString(j0,i0+0.4*TAILLE_CASES_AFFICHAGE,"Avancez",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CaseEnfant::affichage(int i0, int j0) const {
    Case::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Enfant",Imagine::MAGENTA,0.2*TAILLE_CASES_AFFICHAGE);
}

void CasePrison::affichage(int i0, int j0) const {
    Case::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Prison",Imagine::Color(128,64,0),0.2*TAILLE_CASES_AFFICHAGE);
}

void CaseEsclave::affichage(int i0, int j0) const {
    Case::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Esclave",Imagine::Color(128,64,0),0.2*TAILLE_CASES_AFFICHAGE);
}

void CaseVacances::affichage(int i0, int j0) const {
    Case::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Vacances",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CaseMarche::affichage(int i0, int j0) const {
    Case::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Marche",Imagine::ORANGE,0.2*TAILLE_CASES_AFFICHAGE);
}

void CaseConseil::affichage(int i0, int j0) const {
    Case::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Conseil",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CaseCash::affichage(int i0, int j0) const {
    Case::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Cash",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
    Imagine::drawString(j0,i0+0.4*TAILLE_CASES_AFFICHAGE,std::to_string(gift),Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CaseCashCagnotte::affichage(int i0, int j0) const {
    Case::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Cagnotte",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
    Imagine::drawString(j0,i0+0.4*TAILLE_CASES_AFFICHAGE,std::to_string(gift),Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CaseCashJoueurs::affichage(int i0, int j0) const {
    Case::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Cash",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
    Imagine::drawString(j0,i0+0.4*TAILLE_CASES_AFFICHAGE,"Joueurs",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
    Imagine::drawString(j0,i0+0.6*TAILLE_CASES_AFFICHAGE,std::to_string(gift),Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CaseCashCagnotteJoueurs::affichage(int i0, int j0) const {
    Case::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Cagnotte",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
    Imagine::drawString(j0,i0+0.4*TAILLE_CASES_AFFICHAGE,"Joueurs",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
    Imagine::drawString(j0,i0+0.6*TAILLE_CASES_AFFICHAGE,std::to_string(gift),Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}
