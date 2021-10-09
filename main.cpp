#include <iostream>
#include <string>

#include "SDL.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

#include "ppm/include/PPMImage.hpp"

int main(int argc, char** argv) {
    auto logger = spdlog::stderr_color_mt("console");

    if (argc != 2) {
        logger->error(R"usage(
        Usage:
            ppmimage <path to ppm image file>
        )usage");
        return -1;
    }

    std::string path_to_image = argv[1];

    SDL_Init(SDL_INIT_EVERYTHING);

    auto ppm = PPMImage::PPMImage(path_to_image);
    ppm.ReadImageFile();
    auto image_data = ppm.GetImageData();
    auto width = ppm.GetImageWidth();
    auto height = ppm.GetImageHeight();

    spdlog::info("a PPM image was read with width = {} and height = {}\n", width, height);

    //ppm.WriteImageFile("/Users/abtiwary/temp/temp.ppm");

    SDL_Window* window = nullptr;
    SDL_Renderer* renderer = nullptr;
    SDL_Texture* texture = nullptr;
    SDL_Event event;

    auto success = SDL_CreateWindowAndRenderer(width, height, 0, &window, &renderer);
    int quit = 0;
    if (success == 0) {
        SDL_RenderSetLogicalSize(renderer, width, height);
        texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGB24,
                                    SDL_TEXTUREACCESS_STATIC, width, height);

        while (!quit) {
            if (SDL_PollEvent(&event))
            {
                switch (event.type) {
                    case SDL_QUIT:
                        quit = 1;
                    case SDL_KEYDOWN:
                        switch (event.key.keysym.sym) {
                            case SDLK_q:
                                quit = 1;
                            default: {}
                        }
                    default: {}
                }
            }

            SDL_UpdateTexture(texture, nullptr, image_data.data(), static_cast<int>(width)*3);
            SDL_UpdateWindowSurface(window);

            SDL_RenderClear(renderer);
            SDL_RenderCopy(renderer, texture, 0, 0);

            SDL_RenderPresent(renderer);
            SDL_Delay(64);
        }

        SDL_DestroyWindow(window);
        SDL_DestroyRenderer(renderer);
    }
    
    SDL_Quit();

    return 0;
}
