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

bool Case::demand_pari_loto(int nb_faces,const std::vector<int>& C) {
    int count=0;
    for (unsigned int i=0;i<C.size();i++)
        if (C.at(i)) count++;
    return count<=nb_faces || !count;
}

void Case::tomber_pari_loto(Game& G,Joueur& J,bool loto) {
    std::vector<int> faces_voulues;
    for (unsigned int i=0;i<G.liste_joueurs.size();i++)
        faces_voulues.push_back(G.liste_joueurs.at(i).decision_nb_faces_pari_loto(loto));
    unsigned int ind_j=0;
    while (&G.liste_joueurs.at(ind_j)!=&J) ind_j++; // Le joueur J joue en premier
    std::vector<int> attribution_faces(FACES_DE_MISE_PARI_LOTO,-1);
    {int j=0;
    for (;j<std::min(faces_voulues.at(ind_j),FACES_DE_MISE_PARI_LOTO);j++)
        attribution_faces.at(j)=ind_j;
    faces_voulues.at(ind_j)=0;
    for (unsigned int i=(ind_j+1)%G.liste_joueurs.size();demand_pari_loto(FACES_DE_MISE_PARI_LOTO-j,faces_voulues);j++,i++,i%=G.liste_joueurs.size()) {
        attribution_faces.at(j)=i; // La lecture littérale du mode d'emploi conduit à ce code là, cependant ce n'est pas ce que je m'imaginais lorsque j'ai écrit les règles,
        faces_voulues.at(i)--; // ce n'est pas la solution la plus juste. envisager des changements?
    }}
    int somme=0;
    for (unsigned int i=0;i<attribution_faces.size();i++)
        if (attribution_faces.at(i)!=-1) {
            G.liste_joueurs.at(attribution_faces.at(i)).cash_flow(-MISE_STANDARD_PARI_LOTO);
            somme+=MISE_STANDARD_PARI_LOTO;}
    int joueur_gagnant=-1;
    while (joueur_gagnant==-1)
        joueur_gagnant=attribution_faces.at(Game::lancer_de_mise_pari_loto());
    G.liste_joueurs.at(joueur_gagnant).cash_flow(somme);
}

void CasePari::tomber(Joueur& J) const {tomber_pari_loto(current_game,J,false);}

void CaseLoto::tomber(Joueur& J) const {tomber_pari_loto(current_game,J,true);}

void CaseJustice::tomber(Joueur& J) const {
    std::vector<const Joueur*> LJ;
    for (unsigned int i=0;i<current_game.liste_joueurs.size();i++)
        LJ.push_back(&current_game.liste_joueurs.at(i));
    current_game.liste_joueurs.at(J.decision_attaquer_justice(LJ)).cash_flow(-DOMMAGES_JUSTICE);
    J.cash_flow(DOMMAGES_JUSTICE);
}

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
