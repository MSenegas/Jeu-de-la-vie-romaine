#include <Imagine/Common.h>
#include <fstream>
#include <stdexcept>

#include "game.h"

void Game::lire_fichier(std::vector<std::vector<std::string>>& paragraph_list,const std::string path) {
    std::string abspath=stringSrcPath(path);
    std::ifstream file(abspath.c_str());
    if (!file.is_open())
        throw std::invalid_argument("Mauvais nom de fichier");
    paragraph_list.clear();
    std::vector<std::string> paragraph;
    std::string line;
    while (!(file.eof())) {
        std::getline(file,line);
        if (line=="") {
            paragraph_list.push_back(paragraph);
            paragraph.clear();}
        else
            paragraph.push_back(line);
    }
    paragraph_list.push_back(paragraph);
}

std::mt19937_64 Game::gene_alea=std::mt19937_64();
std::_Bind<std::uniform_int_distribution<>(std::mt19937_64)> Game::lancer_de=std::bind(LANCER_DE,gene_alea);
std::_Bind<std::uniform_int_distribution<>(std::mt19937_64)> Game::lancer_piece=std::bind(LANCER_PIECE,gene_alea);
std::_Bind<std::uniform_int_distribution<>(std::mt19937_64)> Game::lancer_de_mise_pari_loto=std::bind(LANCER_DE_MISE_PARI_LOTO,gene_alea);
std::_Bind<std::uniform_int_distribution<>(std::mt19937_64)> Game::lancer_de_gain_loto=std::bind(LANCER_DE_GAIN_LOTO,gene_alea);

Game::Game(const std::string& path_plateau,const std::string& path_cartes,unsigned int nb_joueurs):
    plateau(path_plateau,*this),banque(path_cartes,*this) {
    for (unsigned int i=0;i<nb_joueurs;i++)
        liste_joueurs.push_back(Joueur(1,Imagine::Color(255*(i==0||i==3||i==5),216*(i==1||i==3||i==4),255*(i==2||i==4||i==5))));
    reset();
}

std::vector<int> Game::tirage_carrieres(const unsigned int nb_joueurs) {
    std::vector<int> liste_carrieres;
    unsigned int nb_politicien=nb_joueurs/2;
    unsigned int nb_commercant=nb_joueurs-nb_politicien;
    while (nb_politicien!=0 || nb_commercant!=0) {
        liste_carrieres.push_back(lancer_piece()+1);
        switch (liste_carrieres.back()) {
            case 1:
                if (nb_politicien==0)
                    liste_carrieres.pop_back();
                else
                    nb_politicien--;
                break;
            case 2:
                if (nb_commercant==0)
                    liste_carrieres.pop_back();
                else
                    nb_commercant--;
                break;
            default:
                throw std::logic_error("Fonction tirage_carrieres mal mise à jour.");}}
    return liste_carrieres;
}

void Game::play_full_game() {
    while (!is_over())
        game_loop();
    reset();
}void Game::play_full_game(Imagine::Window& w1,Imagine::Window& w2,Imagine::Window& w3) {
    affichage(w1,w2,w3,0);
    while (!is_over())
        game_loop(w1,w2,w3);
    reset();
}

bool Game::is_over() const {
    for (unsigned int i=0;i<liste_joueurs.size();i++)
        if (!liste_joueurs.at(i).is_over())
            return false;
    return true;}

void Game::game_loop() {
    for (unsigned int i=0;i<liste_joueurs.size();i++)
        liste_joueurs.at(i).play(plateau,cagnotte);
    t++;
}void Game::game_loop(Imagine::Window& w1,Imagine::Window& w2,Imagine::Window& w3) {
    for (unsigned int i=0;i<liste_joueurs.size();i++) {
        liste_joueurs.at(i).play(plateau,cagnotte);
        affichage(w1,w2,w3,i);}
    t++;
}

void Game::mouv_joueur(Joueur& J, const int& de) {J.mouv(plateau,de);}

void Game::affichage(Imagine::Window& w1,Imagine::Window& w2,Imagine::Window& w3,int curr_player) const {
    std::vector<const Joueur*> LJ(liste_joueurs.size());
    for (unsigned int i=0;i<liste_joueurs.size();i++)
        LJ.at(i)=&liste_joueurs.at(i);
    Imagine::setActiveWindow(w3);
    Imagine::clearWindow();
    liste_joueurs.at(curr_player).dessine_icone(0,4.5*curr_player*TAILLE_CASES_AFFICHAGE,0.5*TAILLE_CASES_AFFICHAGE);
    for (unsigned int i=0;i<liste_joueurs.size();i++)
        liste_joueurs.at(i).affiche_cartes(0.5*TAILLE_CASES_AFFICHAGE,4.5*i*TAILLE_CASES_AFFICHAGE);
    Imagine::setActiveWindow(w2);
    Imagine::clearWindow();
    Imagine::drawString(0,0.25*TAILLE_CASES_AFFICHAGE,"Tour: "+std::to_string(t),Imagine::BLACK,0.25*TAILLE_CASES_AFFICHAGE);
    Imagine::drawString(2*TAILLE_CASES_AFFICHAGE,0.25*TAILLE_CASES_AFFICHAGE,"Cagnotte: "+std::to_string(cagnotte),Imagine::BLACK,0.25*TAILLE_CASES_AFFICHAGE);
    banque.affichage(0.25*TAILLE_CASES_AFFICHAGE,0);
    Imagine::setActiveWindow(w1);
    Imagine::clearWindow();
    plateau.affichage(0,0,LJ);
    Imagine::anyClick();
}

void Game::reset() {
    t=0;
    cagnotte=0;
    banque.reset();
    std::vector<int> liste_n_carr=tirage_carrieres(liste_joueurs.size());
    for (unsigned int i=0;i<liste_joueurs.size();i++)
        liste_joueurs.at(i).reset(liste_n_carr.at(i));
}
