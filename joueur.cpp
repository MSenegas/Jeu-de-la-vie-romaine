#include <iostream>

#include "game.h"
#include "joueur.h"

Joueur::Joueur(const int n_carr,Imagine::Color col): couleur(col) {reset(n_carr);}

void Joueur::cash_flow(const int gain) {money+=gain;}

void Joueur::verif_dettes() {
    if (money<0) {
        dettes-=(money/MIN_DETTES)*MIN_DETTES;
        money%=MIN_DETTES;
        if (money<=0) {
            dettes+=MIN_DETTES;
            money+=MIN_DETTES;}}
    else if (money>=MIN_DETTES && dettes!=0) {
        if (money>=dettes) {
            money-=dettes;
            dettes=0;}
        else {
            dettes-=(money/MIN_DETTES)*MIN_DETTES;
            money%=MIN_DETTES;}}
}

void Joueur::prendre_cagnotte(unsigned int& cag) {
    cash_flow(cag);
    cag=0;
}

void Joueur::add_conseil() {conseils++;}

void Joueur::passer_salaire() {
    verif_dettes();
    unsigned int interets=TAUX_COUPON*dettes;
    if (interets/TAUX_COUPON!=dettes)
        interets++;
    cash_flow(-interets);
    cash_flow(conseils*REVENU_CONSEILS);
    for (unsigned int i=0;i<cartes_propriete.size();i++)
        cash_flow((*cartes_propriete.at(i))());
}

void Joueur::ajouter_salaire_deja_recu(const Case* C) {salaires_recus.push_back(C);}

void Joueur::aller_prison() {
    if (!remove_carte_sortez_prison()) {
        essais_prison=0;
        prison=true;}
}

void Joueur::sortir_prison() {prison=false;}

void Joueur::tenter_sortir_prison(const int de) {
    if (remove_carte_sortez_prison())
        sortir_prison();
    if (essais_prison+1<N_MAX_ESSAIS_PRISON) {
        essais_prison++;
        if (de==FACE_SORTIR_PRISON)
            sortir_prison();}
    else {
        cash_flow(-FRAIS_SORTIE_PRISON);
        sortir_prison();}
}

void Joueur::aller_esclavage() {
    if (fois_esclave<N_MAX_ESCLAVAGE) {
        case_esclave=0;
        esclave=true;}
}

void Joueur::sortir_esclavage() {
    cash_flow(-FRAIS_SORTIE_ESCLAVAGE);
    esclave=false;}

int Joueur::decision_attaquer_justice(const std::vector<const Joueur*>& LJ) const {
    int i0=-1,maxcash=INT32_MIN;
    for (unsigned int i=0;i<LJ.size();i++) {
        const Joueur* C=LJ.at(i);
        int currcash=C->money-C->dettes;
        if (this!=C && maxcash<currcash) {
            maxcash=currcash;
            i0=i;}}
    return i0;
}

int Joueur::decision_nb_faces_pari_loto(bool) const {
    std::cerr << "Warning: La décision du nombre de faces à acheter est mauvaise" << std::endl;
    return FACES_DE_MISE_PARI_LOTO/2+1;
}

void Joueur::add_carte_enfant(const Carte* C) {
    unsigned int enfants=0;
    for (unsigned int i=0;i<cartes_enfant.size();i++)
        enfants+=(*cartes_enfant.at(i))();
    if (enfants+(*C)()>MAX_ENFANTS) {
        cash_flow(INDEMNITE_SUBSTITUTION_ENFANTS);
        C->defausser();}
    else
        cartes_enfant.push_back(C);
}

void Joueur::add_carte_sortez_prison(const Carte* C) {cartes_sortez_prison.push_back(C);}

bool Joueur::remove_carte_sortez_prison() {
    if (cartes_sortez_prison.empty())
        return false;
    cartes_sortez_prison.back()->defausser();
    cartes_sortez_prison.pop_back();
    return true;
}

bool Joueur::NulliteAchat(const Carte* A,const Carte* B) {
    std::cerr << "Warning: Mauvaise fonction de comparaison d'achat" << std::endl;
    return (*A)()>(*B)(); // !!!!!!!!!!!!!!!!! À CHANGER !!!!!!!!!!!!!!!!!
}
bool Joueur::QualiteVente(const Carte* A,const Carte* B) {
    // Vérifier s'il faut aussi prendre en compte la variance
    std::cerr << "Warning: Mauvaise fonction de comparaison de vente" << std::endl;
    return (*A)()<(*B)();
}

bool Joueur::decision_acheter_vendre() const {
    std::cerr << "Warning: Le joueur ne fera qu'acheter" << std::endl;
    return true; // !!!!!!!!!!!!!!!!!!!!!!!!!!!!! À CHANGER !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
}

void Joueur::decision_acheter_cartes(std::vector<const Carte*>& LC) const {
    // std::sort(LC.begin(),LC.end(),NulliteAchat);
    // Si on retire une carte, faire:
    // LC.back()->defausser();
    // LC.pop_back();
}

void Joueur::acheter_cartes_achetez(const std::vector<const Carte*>& LC) {
    for (unsigned int i=0;i<LC.size();i++) {
        cash_flow(-LC.at(i)->base_value());
        cartes_achetez.push_back(LC.at(i));}
}

void Joueur::vendre_cartes_achetez() {
    std::sort(cartes_achetez.begin(),cartes_achetez.end(),QualiteVente);
    for (unsigned int i=0;i<MAX_CARTES_ACHETEZ_VENTE;i++)
        if (!cartes_achetez.empty()) {
            const Carte* C=cartes_achetez.back();
            cartes_achetez.pop_back();
            int de=Game::lancer_de();
            switch (de) {
            case 1:
            case 2:
                cash_flow(round((*C)()*(1-C->variation())));
                break;
            case 3:
            case 4:
                cash_flow((*C)());
                break;
            case 5:
            case 6:
                cash_flow(round((*C)()*(1+C->variation())));
            }
            C->defausser();
        }
}

void Joueur::add_carte_propriete(const Carte *C) {cartes_propriete.push_back(C);}

int Joueur::decision_enchere_carte_propriete(Game& G, const Carte* C, int reduction) const {
    std::cerr << "Warning: Le joueur achètera les propriétés à leur prix de base" << std::endl;
    return C->base_value()+reduction; // !!!!!!!!!!!!!!!!!! À AMÉLIORER !!!!!!!!!!!!!!!!!!!!!
}

bool Joueur::decision_autre_chance_tresor() const {
    return Game::lancer_piece(); // à améliorer
}

bool Joueur::decision_autre_cash_bonus() const {
    return Game::lancer_piece(); // à améliorer
}

int Joueur::decision_jeu_hasard_avantage() const {
    std::cerr << "Warning: Le joueur mettra toujours la mise maximale" << std::endl;
    return MAX_MISE_POSSIBLE_JEU_HASARD_AVANTAGE; // à améliorer
}

void Joueur::carriere_nouv_periode() {
    if (periode==PERIODE_CHEMIN_SUR_RISQUE)
        carriere=decision_chemin_risque_sur();}

int Joueur::decision_chemin_risque_sur() const {
    return 1+Game::lancer_piece();} // à améliorer

void Joueur::play(Plateau& P,unsigned int& cag) {
    if (!a_fini) {
        int de=Game::lancer_de();
        if (prison)
            tenter_sortir_prison(de);
        else {
            if (de==FACE_CAGNOTTE && !esclave)
                prendre_cagnotte(cag);
            mouv(P,de);}}
}

void Joueur::mouv(Plateau& P,const int de) {
    if (esclave) {
        case_esclave+=de;
        if (case_esclave>=0) {
            if (P.depasse_periode(0,1,case_esclave))
                sortir_esclavage();
            else // Pas de salaire à passer en esclavage
                P.tomber(0,1,case_esclave,*this);}
        else
            case_esclave=0;} // Case 0 de toute période est un stop
    else {
        unsigned int ancienne_case_libre=case_libre;
        if (de<0) a_fini=false; // On réveille le joueur s'il doit reculer et qu'il a fini
        case_libre+=de;
        if (case_libre>=0) {
            if (P.depasse_periode(carriere,periode,case_libre)) {
                case_libre=P.size_periode(carriere,periode)-1;
                for (int i=ancienne_case_libre+1;i<=case_libre;i++)
                    P.passer(carriere,periode,i,*this);
                if (P.derniere_periode(periode))
                    a_fini=true;
                else {case_libre=0;
                    periode++;
                    carriere_nouv_periode();}}
            else for (int i=ancienne_case_libre+1;i<=case_libre;i++)
                P.passer(carriere,periode,i,*this);
            P.tomber(carriere,periode,case_libre,*this);}
        else
            case_libre=0;} // Case 0 de toute période est un stop, pas de salaire en reculant
}

bool Joueur::is_over() const {return a_fini;}

void Joueur::reset(const int& n_carr) {
    money=ARGENT_INITIAL;
    dettes=0;
    carriere=n_carr;
    periode=1;
    case_libre=0;
    a_fini=false;
    salaires_recus.clear();
    prison=false;
    essais_prison=0;
    esclave=false;
    fois_esclave=0;
    case_esclave=0;
    conseils=0;
    cartes_sortez_prison.clear();
    cartes_achetez.clear();
    cartes_enfant.clear();
    cartes_propriete.clear();
}

void Joueur::dessine_icone(int i0, int j0,int taille_cadre) const {
    int lx[3]={j0+int(taille_cadre/2),j0+int(taille_cadre*0.79),j0+int(taille_cadre*0.21)};
    int ly[3]={i0+int(taille_cadre*0.25),i0+int(taille_cadre*0.75),i0+int(taille_cadre*0.75)};
    Imagine::drawPoly(lx,ly,3,couleur);
}

void Joueur::affiche_cartes(int i0, int j0) const {
    Imagine::drawString(j0,i0+0.25*TAILLE_CASES_AFFICHAGE,"Argent: "+std::to_string(money),Imagine::BLACK,0.25*TAILLE_CASES_AFFICHAGE);
    Imagine::drawString(j0+2*TAILLE_CASES_AFFICHAGE,i0+0.25*TAILLE_CASES_AFFICHAGE,"Dettes: "+std::to_string(dettes),Imagine::BLACK,0.25*TAILLE_CASES_AFFICHAGE);
    if (carriere==1 && periode!=PERIODE_CHEMIN_SUR_RISQUE)
        Imagine::drawString(j0,i0+0.5*TAILLE_CASES_AFFICHAGE,"Conseils: "+std::to_string(conseils),Imagine::BLACK,0.25*TAILLE_CASES_AFFICHAGE);
    if (prison) Imagine::drawString(j0+2*TAILLE_CASES_AFFICHAGE,i0+0.5*TAILLE_CASES_AFFICHAGE,"Essais prison: "+std::to_string(essais_prison),Imagine::BLACK,0.25*TAILLE_CASES_AFFICHAGE);
    for (unsigned int i=0;i<cartes_sortez_prison.size();i++)
        cartes_sortez_prison.at(i)->affichage(i0+(0.5+i)*TAILLE_CASES_AFFICHAGE,j0);
    for (unsigned int i=0;i<cartes_achetez.size();i++)
        cartes_achetez.at(i)->affichage(i0+(0.5+i)*TAILLE_CASES_AFFICHAGE,j0+1.4*TAILLE_CASES_AFFICHAGE);
    for (unsigned int i=0;i<cartes_enfant.size();i++)
        cartes_enfant.at(i)->affichage(i0+(0.5+i)*TAILLE_CASES_AFFICHAGE,j0+2.8*TAILLE_CASES_AFFICHAGE);
    for (unsigned int i=0;i<cartes_propriete.size();i++)
        cartes_propriete.at(i)->affichage(i0+(0.5+i)*TAILLE_CASES_AFFICHAGE,j0+4.2*TAILLE_CASES_AFFICHAGE);
}
