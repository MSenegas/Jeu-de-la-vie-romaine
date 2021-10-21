#include <Imagine/Common.h>
#include <Imagine/Graphics.h>

#include "joueur.h"
#include "plateau.h"

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

void Joueur::prendre_cagnotte(unsigned int& cag, const int& de) {
    if (de==FACE_CAGNOTTE) {
        cash_flow(cag);
        cag=0;}
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

int Joueur::decision_chemin_risque_sur() {
    return Imagine::intRandom(1,2);} // à améliorer

void Joueur::mouv(Plateau& P,const int& de) {
    if (esclave) {
        case_esclave+=de;
        if (P.depasse_periode(0,1,case_esclave))
            sortir_esclavage();
        else // Pas de salaire à passer en esclavage
            P.tomber(0,1,case_esclave,*this);}
    else {
        unsigned int ancienne_case_libre=case_libre;
        case_libre+=de;
        if (P.depasse_periode(carriere,periode,case_libre)) {
            case_libre=P.size_periode(carriere,periode)-1;
            for (unsigned int i=ancienne_case_libre+1;i<=case_libre;i++)
                P.passer(carriere,periode,i,*this);
            if (P.derniere_periode(periode))
                a_fini=true;
            else {case_libre=0;
                periode++;
                carriere_nouv_periode();}}
        else for (unsigned int i=ancienne_case_libre+1;i<=case_libre;i++)
            P.passer(carriere,periode,i,*this);
        P.tomber(carriere,periode,case_libre,*this);}
}

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

void Joueur::dessine_icone(int i0, int j0) const {
    int lx[3]={j0+int(TAILLE_CASES_AFFICHAGE/2),j0+int(TAILLE_CASES_AFFICHAGE*0.79),j0+int(TAILLE_CASES_AFFICHAGE*0.21)};
    int ly[3]={i0+int(TAILLE_CASES_AFFICHAGE*0.25),i0+int(TAILLE_CASES_AFFICHAGE*0.75),i0+int(TAILLE_CASES_AFFICHAGE*0.75)};
    Imagine::drawPoly(lx,ly,3,Imagine::RED);
}

void Joueur::affichage(const Plateau& P) const {
    if (esclave)
        dessine_icone(0,TAILLE_CASES_AFFICHAGE*case_esclave);
    else {
        int sm=0;
        for (int pp=1;pp<periode;pp++)
            sm+=P.size_periode(carriere,pp); // HYPOTHÈSE: Les périodes des mêmes carrières ont la même longueur
        dessine_icone(TAILLE_CASES_AFFICHAGE*carriere,TAILLE_CASES_AFFICHAGE*(sm+case_libre));
    }
}
