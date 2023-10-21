#include <iostream>

#include <SDL3/SDL.h>

#include <eng/api.h>

int main(int argc, char* argv[]){
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0){
        std::cout << "Err with SDL init" << std::endl;
    }

    std::cout << test_sum(3, 6) << std::endl;
    //std::cout << test_min(3, 6) << std::endl;
    //std::cout << test_min(7, 1) << std::endl;

    SDL_Window* win = SDL_CreateWindow("test", 800, 800, 0);
    if (win == 0){
        std::cout << "Err window create" << std::endl;
    }
    SDL_Renderer* ren = SDL_CreateRenderer(win, 0, 0);
    if (ren == 0){
        std::cout << "Err renderer create" << std::endl;
    }

    // while (true)
    // {
    //     SDL_Event ev;

    //     if (SDL_PollEvent(&ev) != 0){
    //         if (ev.type == SDL_EVENT_QUIT){
    //             std::cout << "Exiting" << std::endl;
    //             exit(0);
    //         }
    //     }
    //     SDL_RenderPresent(ren);
    // }

    SDL_SetRenderDrawColor(ren, 0, 0, 0, 255);
    SDL_RenderClear(ren);
    SDL_RenderPresent(ren);
    SDL_Delay(3000);
    
    SDL_DestroyRenderer(ren);
    SDL_DestroyWindow(win);
    SDL_Quit();
    return 0;
}