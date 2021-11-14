#include <stdexcept>
#include <iostream>

#include "game.h"

// De manière générale, TOUJOURS décider du placement de la carte après le tirage dans Carte::tirer() , s'assurer de la conservation du onmbre de cartes
// (i.e. mettre defausser() ou J.add_... à la fin)

// METTRE J.add_carte_chance_tresor(this); DANS CarteSortezPrison::tirer(Joueur& J)
// METTRE J.cash_flow(gift); DANS CarteCash::tirer(Joueur& J)
// METTRE J.add_enfant(this); DANS CarteEnfant::tirer(Joueur& J)

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
