#include <stdexcept>
#include <algorithm>

#include "game.h"
#include "paquet.h"

void Pioche::interprete_ligne(const std::string& ligne,std::string& comm,int& arg1,int& arg2,double& arg3) {
    // Interprète une chaîne de caractères de la forme <chaîne> [int1] [int2] [double3]
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
}

Pioche::Pioche(std::vector<std::string> paragraph,Game& G) {
    std::string carte_type;
    for (unsigned int i=0;i<paragraph.size();i++) {
        int carte_cost=0;int carte_rv=0;double carte_prc=0;
        interprete_ligne(paragraph.at(i),carte_type,carte_cost,carte_rv,carte_prc);
        if (carte_type=="achetez") pioche.push_back(new CarteAchetez(*this,carte_cost,carte_rv,carte_prc));
        else if (carte_type=="propriete") pioche.push_back(new CartePropriete(*this,carte_cost,carte_rv,));
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
        else if (carte_type=="symboles") pioche.push_back(new CarteSymboles(*this,));
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
            pioche_chance=Pioche(paragraph_list.at(i),G);
        else if (nom_paquet=="tresor")
            pioche_tresor=Pioche(paragraph_list.at(i),G);
        else if (nom_paquet=="bonus")
            pioche_bonus=Pioche(paragraph_list.at(i),G);
        else if (nom_paquet=="achetez")
            pioche_achetez=Pioche(paragraph_list.at(i),G);
        else if (nom_paquet=="enfant")
            pioche_enfant=Pioche(paragraph_list.at(i),G);
        else if (nom_paquet=="propriete")
            pioche_propriete=Pioche(paragraph_list.at(i),G);
        else if (nom_paquet=="symboles")
            pioche_symboles=Pioche(paragraph_list.at(i),G);
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
    if (pioche.empty())
        melanger();
    const Carte* rep=pioche.back();
    pioche.pop_back();
    return rep;
}

void Pioche::tirer(Joueur& J) {pop()->tirer(J);}

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
