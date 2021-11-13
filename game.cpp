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
    plateau(path_plateau,*this),banque(path_cartes,*this),liste_joueurs(nb_joueurs,Joueur(1)) {reset();}

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
    while (!is_over()) {
        game_loop();}
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
    t++;}

void Game::mouv_joueur(Joueur& J, const int& de) {J.mouv(plateau,de);}

void Game::reset() {
    t=0;
    cagnotte=0;
    banque.reset();
    std::vector<int> liste_n_carr=tirage_carrieres(liste_joueurs.size());
    for (unsigned int i=0;i<liste_joueurs.size();i++)
        liste_joueurs.at(i).reset(liste_n_carr.at(i));
}
