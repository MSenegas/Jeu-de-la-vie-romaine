#include "game.h"
#include "case.h"
#include "carte.h"

int main(){
    Game G("testplateau.txt","testcartes.txt",6);
    Imagine::Window WP=Imagine::openWindow(500,500);
    Imagine::Window WB=Imagine::openWindow(1000,800);
    Imagine::Window WJ=Imagine::openWindow(1500,400);
    G.affichage(WP,WB,WJ);
    Imagine::endGraphics();
    return 0;
}
