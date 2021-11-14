#include <stdexcept>
#include <algorithm>
#include <iostream>

#include "game.h"
#include "paquet.h"

void interprete_attributs(const std::string& ligne,std::string& comm,int& arg1,int& arg2,double& arg3) {
    unsigned int ancien_ind_apres_espace;
    unsigned int ind_apres_espace=ligne.find(' ');
    comm=ligne.substr(0,ind_apres_espace);
    while (ind_apres_espace<ligne.size() && ligne.at(ind_apres_espace)==' ') ind_apres_espace++;
    std::vector<int*> args12;args12.push_back(&arg1);args12.push_back(&arg2);
    for (unsigned int yeah=0;yeah<3;yeah++) {
        if (ind_apres_espace<ligne.size()) {
            if (yeah==2) {
                std::string lstarg=ligne.substr(ind_apres_espace);
                while (lstarg.back()==' ') lstarg.pop_back();
                if (lstarg.back()=='%') {
                    lstarg.pop_back();
                    arg3=std::stod(lstarg)/100;}
                else
                    arg3=std::stod(lstarg);}
            else {
                ancien_ind_apres_espace=ind_apres_espace;
                ind_apres_espace=ligne.find(' ',ind_apres_espace);
                while (ind_apres_espace<ligne.size() && ligne.at(ind_apres_espace)==' ') ind_apres_espace++;
                *args12.at(yeah)=std::stoi(ligne.substr(ancien_ind_apres_espace,ind_apres_espace-ancien_ind_apres_espace-1));}}}
}void interprete_symboles(const std::string& ligne,std::vector<int>& sym) {
    unsigned int ancien_ind_apres_espace=0;
    unsigned int ind_espace=0;
    while (ancien_ind_apres_espace<ligne.size()) {
        if (ind_espace!=0) sym.push_back(std::stoi(ligne.substr(ancien_ind_apres_espace,ind_espace-ancien_ind_apres_espace)));
        while (ind_espace<ligne.size() && ligne.at(ind_espace)==' ') ind_espace++;
        ancien_ind_apres_espace=ind_espace;
        ind_espace=ligne.find(' ',ind_espace);
    }
}

void Pioche::interprete_ligne(const std::string& ligne,std::string& comm,int& arg1,int& arg2,double& arg3,std::vector<int>& sym) {
    // Interprète une chaîne de caractères de la forme <chaîne> [int1] [int2] [double3] | [vector<int>]
    unsigned int ind_barre=ligne.find('|');
    interprete_attributs(ligne.substr(0,ind_barre),comm,arg1,arg2,arg3);
    if (ind_barre<ligne.size()) interprete_symboles(ligne.substr(ind_barre+1),sym);
}

void Pioche::creer(const std::vector<std::string>& paragraph,Game& G) {
    std::string carte_type;
    for (unsigned int i=0;i<paragraph.size();i++) {
        int carte_cost=0;int carte_rv=0;double carte_prc=0;std::vector<int> sym;
        interprete_ligne(paragraph.at(i),carte_type,carte_cost,carte_rv,carte_prc,sym);
        if (carte_type=="achetez") pioche.push_back(new CarteAchetez(*this,carte_cost,carte_rv,carte_prc));
        else if (carte_type=="propriete") pioche.push_back(new CartePropriete(*this,carte_cost,carte_rv,sym));
        else if (carte_type=="encheres") pioche.push_back(new CarteMettezProprieteEncheres(*this,G));
        else if (carte_type=="sortez_prison") pioche.push_back(new CarteSortezPrison(*this));
        else if (carte_type=="tirez_enfant") pioche.push_back(new CarteTirezEnfant(*this,G));
        else if (carte_type=="enfant") pioche.push_back(new CarteEnfant(*this,carte_cost));
        else if (carte_type=="rejouez") pioche.push_back(new CarteRejouez(*this,G));
        else if (carte_type=="reculez_avancez") pioche.push_back(new CarteReculezAvancez(*this,G,carte_cost));
        else if (carte_type=="reculez_avancez_joueurs") pioche.push_back(new CarteReculezAvancezJoueurs(*this,G,carte_cost));
        else if (carte_type=="autre_chance_tresor") pioche.push_back(new CarteAutreChanceTresor(*this,G));
        else if (carte_type=="cash_ou_bonus") pioche.push_back(new CarteAutreCashBonus(*this,G));
        else if (carte_type=="jeu_avantage") pioche.push_back(new CarteJeuHasardAvantage(*this));
        else if (carte_type=="pari") pioche.push_back(new CartePari(*this,G));
        else if (carte_type=="prison") pioche.push_back(new CartePrison(*this));
        else if (carte_type=="esclave") pioche.push_back(new CarteEsclave(*this));
        else if (carte_type=="cash") pioche.push_back(new CarteCash(*this,carte_cost));
        else if (carte_type=="cash_joueurs") pioche.push_back(new CarteCashJoueurs(*this,G,carte_cost));
        else if (carte_type=="cash_cagnotte_joueurs") pioche.push_back(new CarteCashCagnotteJoueurs(*this,G,carte_cost));
        else if (carte_type=="symboles") pioche.push_back(new CarteSymboles(*this,sym));
        else {
            std::string err_msg=" n'est pas un nom de carte reconnu";
            throw std::invalid_argument(carte_type+err_msg);}}
}

Banque::Banque(const std::string& path,Game& G) {
    std::vector<std::vector<std::string>> paragraph_list;
    Game::lire_fichier(paragraph_list,path);
    for (unsigned int i=0;i<paragraph_list.size();i++) {
        std::string nom_paquet=paragraph_list.at(i).front();
        paragraph_list.at(i).erase(paragraph_list.at(i).begin());
        if (nom_paquet=="chance")
            pioche_chance.creer(paragraph_list.at(i),G);
        else if (nom_paquet=="tresor")
            pioche_tresor.creer(paragraph_list.at(i),G);
        else if (nom_paquet=="bonus")
            pioche_bonus.creer(paragraph_list.at(i),G);
        else if (nom_paquet=="achetez")
            pioche_achetez.creer(paragraph_list.at(i),G);
        else if (nom_paquet=="enfant")
            pioche_enfant.creer(paragraph_list.at(i),G);
        else if (nom_paquet=="propriete")
            pioche_propriete.creer(paragraph_list.at(i),G);
        else if (nom_paquet=="symboles")
            pioche_symboles.creer(paragraph_list.at(i),G);
        else {
            std::string err_msg=" n'est pas un nom de paquet reconnu.";
            throw std::invalid_argument(nom_paquet+err_msg);}}
}

void Pioche::melanger() {
    for (unsigned int i=0;i<defausse.size();i++)
        pioche.push_back(defausse.at(i));
    defausse.clear();
    std::shuffle(pioche.begin(),pioche.end(),Game::gene_alea);
}

const Carte* Pioche::pop() {
    if (pioche.empty()) {
        if (defausse.empty()) {
            std::cerr << "Warning: Il n'y a plus de cartes dans ce paquet" << std::endl;
            return 0;}
        melanger();}
    const Carte* rep=pioche.back();
    pioche.pop_back();
    return rep;
}

void Pioche::tirer(Joueur& J) {
    const Carte* C=pop();
    if (C)
        C->tirer(J);
}

void Pioche::defausser(const Carte* C) {defausse.push_back(C);}

void Banque::reset() {
    pioche_chance.melanger();
    pioche_tresor.melanger();
    pioche_bonus.melanger();
    pioche_achetez.melanger();
    pioche_enfant.melanger();
    pioche_propriete.melanger();
    pioche_symboles.melanger();
}
