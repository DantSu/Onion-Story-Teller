#ifndef STORYTELLER_SDL_HELPER__
#define STORYTELLER_SDL_HELPER__

#include <SDL/SDL.h>
#include <SDL/SDL_mixer.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_ttf.h>

#include "utils/str.h"

#define FALLBACK_FONT "/customer/app/Exo-2-Bold-Italic.ttf"

static SDL_Surface *video;
static SDL_Surface *screen;
static Mix_Music *music;

static TTF_Font *font;
static SDL_Color color = {255, 255, 255};
static int fontPosition = 0;


void font_write(const char *text) 
{
    SDL_Surface *sdlText = TTF_RenderUTF8_Blended(font, text, color);
    if (sdlText) {
        fontPosition += 16;
        SDL_BlitSurface(sdlText, NULL, screen, &(SDL_Rect){20, fontPosition});
        SDL_FreeSurface(sdlText);
        SDL_BlitSurface(screen, NULL, video, NULL);
        SDL_Flip(video);
    }
}


void video_audio_init(void) 
{
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO);
    IMG_Init(IMG_INIT_PNG);
    TTF_Init();
    if(Mix_OpenAudio(44000, 32784, 2, 2048) < 0) {
        Mix_Volume(-1, MIX_MAX_VOLUME);
        Mix_VolumeMusic(MIX_MAX_VOLUME);
    }
    video = SDL_SetVideoMode(640, 480, 32, SDL_HWSURFACE | SDL_DOUBLEBUF);
    screen = SDL_CreateRGBSurface(SDL_HWSURFACE, 640, 480, 32, 0, 0, 0, 0);
    font = TTF_OpenFont(FALLBACK_FONT, 14);
}


void video_audio_quit(void) 
{
    SDL_BlitSurface(screen, NULL, video, NULL);
    SDL_Flip(video);

    TTF_Quit();

    Mix_FreeMusic(music);
    Mix_CloseAudio();

    SDL_FreeSurface(screen);
    SDL_FreeSurface(video);
    SDL_Quit();
}

void video_checkFilename(char *filename)
{
    int length = strlen(filename);
    if(strcmp((char *)filename + length - 4, ".jpg") == 0 || strcmp((char *)filename + length - 4, ".JPG") == 0) {
        filename[length - 3] = 'p';
        filename[length - 2] = 'n';
        filename[length - 1] = 'g';
    } else if (strcmp((char *)filename + length - 5, ".jpeg") == 0 || strcmp((char *)filename + length - 5, ".JPEG") == 0) {
        filename[length - 4] = 'p';
        filename[length - 3] = 'n';
        filename[length - 2] = 'g';
        filename[length - 1] = '\0';
    }
}

void video_displayImage(const char *dir, char *name)
{
    video_checkFilename(name);
    
    char image_path[STR_MAX * 2];
    sprintf(image_path, "%s%s", dir, name);
    SDL_Surface *image = IMG_Load(image_path);

    SDL_FillRect(screen, NULL, 0);
    SDL_BlitSurface(image, NULL, screen, &(SDL_Rect){(screen->w - image->w) / 2, (screen->h - image->h) / 2});
    SDL_FreeSurface(image);
    SDL_BlitSurface(screen, NULL, video, NULL);
    SDL_Flip(video);
}

void video_displayBlackScreen(void)
{
    SDL_FillRect(screen, NULL, 0);
    SDL_BlitSurface(screen, NULL, video, NULL);
    SDL_Flip(video);
}


void audio_play(const char *dir, const char *name) {
    if(music != NULL) {
        Mix_FreeMusic(music);
    }
    char sound_path[STR_MAX * 2];
    sprintf(sound_path, "%s%s", dir, name);
    music = Mix_LoadMUS(sound_path);
    Mix_PlayMusic(music, 1);
}


#endif // STORYTELLER_SDL_HELPER__