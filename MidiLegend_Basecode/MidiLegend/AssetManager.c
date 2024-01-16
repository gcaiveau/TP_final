/*
  Copyright (c) Arnaud BANNIER and Nicolas BODIN.
  Licensed under the MIT License.
  See LICENSE.md in the project root for license information.
*/

#include "AssetManager.h"

typedef struct TextureSpec
{
    SDL_Texture **ptr;
    char *path;
} TextureSpec;

typedef struct FontSpec
{
    TTF_Font **ptr;
    char *path;
    int size;
} FontSpec;

void setColor(SDL_Color *colors, Uint8 r, Uint8 g, Uint8 b)
{
    colors->r = r;
    colors->g = g;
    colors->b = b;
    colors->a = 255;
}

AssetManager *AssetManager_create(SDL_Renderer *renderer)
{
    AssetManager *self = (AssetManager *)calloc(1, sizeof(AssetManager));
    AssertNew(self);

    // -------------------------------------------------------------------------
    // Chargement des textures

    struct Textures *textures = &(self->textures);
    TextureSpec texSpecs[] = {
        {
            .ptr = &textures->background,
            .path = "../Assets/Images/ibijau.png"
        },
        {
            .ptr = &textures->logo,
            .path = "../Assets/Images/logo.png"
        },
		{  // TODO : decommenter
			.ptr = &textures->trackFill,
			.path = "../Assets/Images/track_fill.png"
		},
        {
            .ptr = &textures->trackMask,
            .path = "../Assets/Images/track_mask.png"
        },
        {
            .ptr = &textures->note,
            .path = "../Assets/Images/note.png"
        },
        {
            .ptr = &textures->keyUp,
            .path = "../Assets/Images/key_up.png"
        },
        {
            .ptr = &textures->keyDown,
            .path = "../Assets/Images/key_down.png"
        },
    };
    int texSpecCount = sizeof(texSpecs) / sizeof(TextureSpec);

    for (int i = 0; i < texSpecCount; i++)
    {
        SDL_Texture **texPtr = texSpecs[i].ptr;
        char *path = texSpecs[i].path;

        *texPtr = IMG_LoadTexture(renderer, path);
        if (*texPtr == NULL)
        {
            printf("ERROR - Loading texture %s\n", path);
            printf("      - %s\n", IMG_GetError());
            assert(false);
            abort();
        }
    }


    // -------------------------------------------------------------------------
    // Chargement des fonts

    struct Fonts *fonts = &(self->fonts);
    FontSpec fontSpecs[] = {
        {
            .ptr = &fonts->small,
            .path = "../Assets/Font/NotoSans-ExtraBold.ttf",
            .size = 18,
        },
        {
            .ptr = &fonts->normal,
            .path = "../Assets/Font/NotoSans-ExtraBold.ttf",
            .size = 36,
        },
        {
            .ptr = &fonts->big,
            .path = "../Assets/Font/NotoSans-ExtraBold.ttf",
            .size = 72,
        },
    };
    int fontSpecCount = sizeof(fontSpecs) / sizeof(FontSpec);

    for (int i = 0; i < fontSpecCount; i++)
    {
        TTF_Font **fontPtr = fontSpecs[i].ptr;
        char *path = fontSpecs[i].path;
        int size = fontSpecs[i].size;

        *fontPtr = TTF_OpenFont(path, size);
        if (*fontPtr == NULL)
        {
            printf("ERROR - Loading font %s\n", path);
            printf("      - %s\n", TTF_GetError());
            assert(false);
            abort();
        }
    }

    // -------------------------------------------------------------------------
    // Chargement des couleurs

    setColor(&(self->colors.blue), 0, 0, 255);
    setColor(&(self->colors.black), 0, 0, 0);
    setColor(&(self->colors.white), 255, 255, 255);
    setColor(&(self->colors.green), 0, 255, 0);

    return self;
}

void AssetManager_destroy(AssetManager *self)
{
    if (!self) return;

    // -------------------------------------------------------------------------
    // Libère les textures

    struct Textures *textures = &(self->textures);
    SDL_Texture *texPointers[] = {
        textures->background,
        textures->logo,
        textures->trackFill, // TODO : decommenter
        textures->trackMask,
        textures->note,
        textures->keyUp,
        textures->keyDown,
    };
    int texCount = sizeof(texPointers) / sizeof(SDL_Texture *);

    for (int i = 0; i < texCount; i++)
    {
        if (texPointers[i])
            SDL_DestroyTexture(texPointers[i]);
    }

    // -------------------------------------------------------------------------
    // Libère les fonts

    struct Fonts *fonts = &(self->fonts);
    TTF_Font *fontPointers[] = {
        fonts->small,
        fonts->normal,
        fonts->big
    };
    int fontCount = sizeof(fontPointers) / sizeof(TTF_Font *);

    for (int i = 0; i < fontCount; i++)
    {
        if (fontPointers[i])
            TTF_CloseFont(fontPointers[i]);
    }

    // Pas de libération des couleurs (allocation statique)

    free(self);
}