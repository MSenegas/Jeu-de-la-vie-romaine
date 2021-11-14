#include <stdexcept>
#include <iostream>

#include "game.h"

int Carte::base_value() const {throw std::logic_error("Seules les cartes achetez et propriété définissent la fonction base_value");}
int Carte::operator()() const {throw std::logic_error("Seules les cartes enfant,achetez et propriété définissent la fonction d'appel");}
double Carte::variation() const {throw std::logic_error("Seules les cartes achetez définissent la fonction variation");}
const std::vector<int>& Carte::symboles() const {throw std::logic_error("Seules les cartes propriété et symboles définissent la fonction symboles");}

void CarteAchetez::tirer(Joueur&) const {std::logic_error("Les cartes achetez ne devraient pas activer la fonction tirer");}
void CartePropriete::tirer(Joueur&) const {std::logic_error("Les cartes propriété ne devraient pas activer la fonction tirer");}
void CarteSymboles::tirer(Joueur&) const {std::logic_error("Les cartes symboles ne devraient pas activer la fonction tirer");}

// S'assurer qu'aucune autre action ne doit être prise lors de la restitution d'une carte
void Carte::defausser() const {pioche_source.defausser(this);}

int operator*(const std::vector<int>& a,const std::vector<int>& b) {
    int rep=0;
    for (unsigned int i=0;i<a.size();i++)
        rep+=a.at(i)*b.at(i);
    return rep;
}

void CarteMettezProprieteEncheres::tirer(Joueur& J) const {
    const Carte* P=current_game.banque.pioche_propriete.pop();
    if (!P) {defausser();return;}
    unsigned int ind_j=0;
    while (&current_game.liste_joueurs.at(ind_j)!=&J) ind_j++; // On favorise les joueurs faisant l'enchère en premier (en particulier celui qui tire la carte)
    unsigned int joueur_gagnant=ind_j;
    int enchere_gagnante=P->base_value()-1;
    int reduction_gagnante=0;
    for (unsigned int i=ind_j,start=1;i!=ind_j || start;i++,i%=current_game.liste_joueurs.size(),start=0) {
        const Carte* S=current_game.banque.pioche_symboles.pop();
        if (!S) throw std::out_of_range("Il n'y a pas de cartes symboles");
        int reduction=P->symboles()*S->symboles();
        int enchere=current_game.liste_joueurs.at(i).decision_enchere_carte_propriete(current_game,P,reduction);
        if (enchere>enchere_gagnante) {
            joueur_gagnant=i;
            enchere_gagnante=enchere;
            reduction_gagnante=reduction;}
        S->defausser();
    }
    if (enchere_gagnante<P->base_value()) {
        P->defausser();
        return;}
    current_game.liste_joueurs.at(joueur_gagnant).cash_flow(reduction_gagnante-enchere_gagnante);
    current_game.liste_joueurs.at(joueur_gagnant).add_carte_propriete(P);
    defausser();
}

void CarteSortezPrison::tirer(Joueur& J) const {
    J.add_carte_sortez_prison(this);
}

void CarteTirezEnfant::tirer(Joueur& J) const {
    current_game.banque.pioche_enfant.tirer(J);
    defausser();
}

void CarteEnfant::tirer(Joueur& J) const {
    J.add_carte_enfant(this);
}

void CarteRejouez::tirer(Joueur& J) const {
    current_game.mouv_joueur(J,Game::lancer_de());
    defausser();
}

void CarteReculezAvancez::tirer(Joueur& J) const {
    current_game.mouv_joueur(J,amount);
    defausser();
}

void CarteReculezAvancezJoueurs::tirer(Joueur&) const {
    for (unsigned int i=0;i<current_game.liste_joueurs.size();i++)
        current_game.mouv_joueur(current_game.liste_joueurs.at(i),amount);
    defausser();
}

void CarteAutreChanceTresor::tirer(Joueur& J) const {
    if (J.decision_autre_chance_tresor())
        current_game.banque.pioche_chance.tirer(J);
    else
        current_game.banque.pioche_tresor.tirer(J);
    defausser();
}

void CarteAutreCashBonus::tirer(Joueur& J) const {
    if (J.decision_autre_cash_bonus())
        current_game.banque.pioche_bonus.tirer(J);
    else
        J.cash_flow(INDEMNITE_AUTRE_CARTE_BONUS);
    defausser();
}

void CarteJeuHasardAvantage::tirer(Joueur& J) const {
    int mise=J.decision_jeu_hasard_avantage();
    J.cash_flow(-mise);
    if (Game::lancer_de()==FACE_JEU_HASARD_AVANTAGE)
        J.cash_flow(FACTEUR_JEU_HASARD_AVANTAGE*mise);
    defausser();
}

void CartePari::tirer(Joueur& J) const {
    std::cerr << "Warning: Carte Pari non implémentée" << std::endl;
    defausser();
}

void CartePrison::tirer(Joueur& J) const {
    J.aller_prison();
    defausser();
}

void CarteEsclave::tirer(Joueur& J) const {
    J.aller_esclavage();
    defausser();
}

void CarteCash::tirer(Joueur& J) const {
    J.cash_flow(gift);
    defausser();
}

void CarteCashJoueurs::tirer(Joueur& J) const {
    for (unsigned int i=0;i<current_game.liste_joueurs.size();i++)
        current_game.liste_joueurs.at(i).cash_flow(-gift);
    J.cash_flow(current_game.liste_joueurs.size()*gift);
    defausser();
}

CarteCashCagnotteJoueurs::CarteCashCagnotteJoueurs(Pioche& P,Game& G,int gain): CarteCashJoueurs(P,G,gain) {
    if (gift>0) throw std::domain_error("La cagnotte ne verse pas d'argent de cette manière.");}

void CarteCashCagnotteJoueurs::tirer(Joueur&) const {
    for (unsigned int i=0;i<current_game.liste_joueurs.size();i++)
        current_game.liste_joueurs.at(i).cash_flow(gift);
    current_game.cagnotte-=current_game.liste_joueurs.size()*gift;
    defausser();
}

void Carte::affichage(int i0, int j0) const {
    Imagine::drawRect(j0,i0,1.4*TAILLE_CASES_AFFICHAGE,TAILLE_CASES_AFFICHAGE,Imagine::BLACK);
}

void CarteAchetez::affichage(int i0, int j0) const {
    Carte::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Achetez",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
    Imagine::drawString(j0,i0+0.4*TAILLE_CASES_AFFICHAGE,std::to_string(prix_achat),Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
    Imagine::drawString(j0+0.8*TAILLE_CASES_AFFICHAGE,i0+0.4*TAILLE_CASES_AFFICHAGE,std::to_string(prix_vente),Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
    Imagine::drawString(j0+TAILLE_CASES_AFFICHAGE,i0+0.4*TAILLE_CASES_AFFICHAGE,std::to_string(risk),Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CartePropriete::affichage(int i0, int j0) const {
    Carte::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Propriete",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
    Imagine::drawString(j0,i0+0.4*TAILLE_CASES_AFFICHAGE,std::to_string(prix_base),Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
    Imagine::drawString(j0+0.5*TAILLE_CASES_AFFICHAGE,i0+0.4*TAILLE_CASES_AFFICHAGE,std::to_string(revenu),Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CarteMettezProprieteEncheres::affichage(int i0, int j0) const {
    Carte::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Enchère",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CarteSortezPrison::affichage(int i0, int j0) const {
    Carte::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"SortezPrison",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CarteTirezEnfant::affichage(int i0, int j0) const {
    Carte::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"TirezEnfant",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CarteEnfant::affichage(int i0, int j0) const {
    Carte::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Enfant",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
    Imagine::drawString(j0,i0+0.4*TAILLE_CASES_AFFICHAGE,std::to_string(enfants),Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CarteRejouez::affichage(int i0, int j0) const {
    Carte::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Rejouez",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CarteReculezAvancez::affichage(int i0, int j0) const {
    Carte::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Bougez",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
    Imagine::drawString(j0,i0+0.4*TAILLE_CASES_AFFICHAGE,std::to_string(amount),Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CarteReculezAvancezJoueurs::affichage(int i0, int j0) const {
    Carte::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Bougez Tous",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
    Imagine::drawString(j0,i0+0.4*TAILLE_CASES_AFFICHAGE,std::to_string(amount),Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CarteAutreChanceTresor::affichage(int i0, int j0) const {
    Carte::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Tirez",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
    Imagine::drawString(j0,i0+0.4*TAILLE_CASES_AFFICHAGE,"Chance ou Tresor",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CarteAutreCashBonus::affichage(int i0, int j0) const {
    Carte::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Tirez Bonus",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
    Imagine::drawString(j0,i0+0.4*TAILLE_CASES_AFFICHAGE,"ou"+std::to_string(INDEMNITE_AUTRE_CARTE_BONUS),Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CarteJeuHasardAvantage::affichage(int i0, int j0) const {
    Carte::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"JeuAvantage",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CartePari::affichage(int i0, int j0) const {
    Carte::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Pari",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CartePrison::affichage(int i0, int j0) const {
    Carte::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Prison",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CarteEsclave::affichage(int i0, int j0) const {
    Carte::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Esclave",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CarteCash::affichage(int i0, int j0) const {
    Carte::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Cash",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
    Imagine::drawString(j0,i0+0.4*TAILLE_CASES_AFFICHAGE,std::to_string(gift),Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CarteCashJoueurs::affichage(int i0, int j0) const {
    Carte::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Cash Joueurs",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
    Imagine::drawString(j0,i0+0.4*TAILLE_CASES_AFFICHAGE,std::to_string(gift),Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CarteCashCagnotteJoueurs::affichage(int i0, int j0) const {
    Carte::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Cagnotte Tous",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
    Imagine::drawString(j0,i0+0.4*TAILLE_CASES_AFFICHAGE,std::to_string(gift),Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}

void CarteSymboles::affichage(int i0, int j0) const {
    Carte::affichage(i0,j0);
    Imagine::drawString(j0,i0+0.2*TAILLE_CASES_AFFICHAGE,"Symboles",Imagine::BLACK,0.2*TAILLE_CASES_AFFICHAGE);
}
