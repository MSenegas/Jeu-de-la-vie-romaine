#include <Imagine/Graphics.h>

#include "game.h"
#include "joueur.h"

Joueur::Joueur(const int& n_carr):
    money(ARGENT_INITIAL),dettes(0),carriere(n_carr),periode(1),
    case_libre(0),a_fini(false),prison(false),essais_prison(0),
    esclave(false),case_esclave(0),conseils(0),enfants(0) {}

void Joueur::cash_flow(const int& gain) {money+=gain;}

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
    for (unsigned int i=0;i<cartes_propriete.size();i++) {}
    // REVENU DES CARTES PROPRIÉTÉ !!!
}

void Joueur::aller_prison() {
    essais_prison=0;
    prison=true;}

void Joueur::sortir_prison() {prison=false;}

void Joueur::tenter_sortir_prison(const int& de) {
    // RAJOUTER EFFET CARTE SORTEZ DE PRISON !!!
    if (essais_prison<N_MAX_ESSAIS_PRISON) {
        essais_prison++;
        if (de==FACE_SORTIR_PRISON)
            sortir_prison();}
    else {
        cash_flow(-FRAIS_SORTIE_PRISON);
        sortir_prison();}
}

void Joueur::aller_esclavage() {
    case_esclave=0;
    esclave=true;}

void Joueur::sortir_esclavage() {
    cash_flow(-FRAIS_SORTIE_ESCLAVAGE);
    esclave=false;}

void Joueur::add_enfant(const int& nb_enfants) {
    if (enfants+nb_enfants>MAX_ENFANTS)
        cash_flow(INDEMNITE_SUBSTITUTION_ENFANTS);
    else
        enfants+=nb_enfants;
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

void Joueur::mouv(Plateau& P,const int& de) {
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
    prison=false;
    essais_prison=0;
    esclave=false;
    case_esclave=0;
    conseils=0;
    enfants=0;
    cartes_chance_tresor.clear();
    cartes_achetez.clear();
    cartes_propriete.clear();
    cartes_symboles.clear();
}

void Joueur::dessine_icone(int i0, int j0,int taille_cadre) const {
    int lx[3]={j0+int(taille_cadre/2),j0+int(taille_cadre*0.79),j0+int(taille_cadre*0.21)};
    int ly[3]={i0+int(taille_cadre*0.25),i0+int(taille_cadre*0.75),i0+int(taille_cadre*0.75)};
    Imagine::drawPoly(lx,ly,3,Imagine::RED);
}
