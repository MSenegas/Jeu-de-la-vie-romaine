#include "game.h"
#include "case.h"
#include "carte.h"

int main(){
    Game G("testplateau.txt","testcartes.txt",6);
    Imagine::Window WJ=Imagine::openWindow(1500,400);
    Imagine::Window WB=Imagine::openWindow(1000,800);
    Imagine::Window WP=Imagine::openWindow(420,300);
    G.play_full_game(WP,WB,WJ);
    Imagine::endGraphics();
    return 0;
}
