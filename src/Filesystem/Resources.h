/**********************************************************************************************
*
*   Ngine - A (mainly) 2D game engine.
*
*   Copyright (C) 2019 NerdThings
*
*   LICENSE: Apache License 2.0
*   View: https://github.com/NerdThings/Ngine/blob/master/LICENSE
*
**********************************************************************************************/

#ifndef RESOURCES_H
#define RESOURCES_H

#include "../ngine.h"

#include "../Audio/Music.h"
#include "../Audio/Sound.h"
#include "../Graphics/Font.h"
#include "../Graphics/Texture2D.h"

namespace NerdThings::Ngine::Filesystem {
    /*
     * Resource management class
     */
    class NEAPI Resources {
        // Private Fields

        static std::unordered_map<std::string, Graphics::TFont> _Fonts;

        /*
         * All named music
         */
        static std::unordered_map<std::string, Audio::TMusic> _Music;

        /*
         * All named sounds
         */
        static std::unordered_map<std::string, Audio::TSound> _Sounds;

        /*
         * All named textures
         */
        static std::unordered_map<std::string, Graphics::TTexture2D> _Textures;

        // Private Methods

        /*
         * Processes the path to get it relative to the install dir
         */
        static std::string ProcessPath(std::string path_);

#if defined(PLATFORM_UWP)
        /*
         * Recursiveley get files in folder (UWP)
         */
        static void UWPGetFiles(Windows::Storage::StorageFolder ^folder, std::string currentPath, std::vector<std::string> *files);
#endif
    public:

        // Public Fields

        /*
         * The directory to load resources from
         */
        static std::string ResourcesDirectory;

        // TODO: Directory config - provides information on what directories contain what filetypes

        // Public Methods

        /*
         * Delete all resources
         */
        static void DeleteAll();

        /*
         * Delete a named font
         */
        static void DeleteFont(const std::string &name_);

        /*
         * Delete a named music
         */
        static void DeleteMusic(const std::string &name_);

        /*
         * Delete a named sound
         */
        static void DeleteSound(const std::string &name_);

        /*
         * Delete a named texture
         */
        static void DeleteTexture(const std::string &name_);

        /*
         * Get the directory from a path to a file
         */
        static std::string GetDirectoryPath(std::string path_);

        /*
         * Get the path to the directory that the game's executable is in
         */
        static std::string GetExecutableDirectory(bool &success_);

        /*
         * Get the location of the game executable
         */
        static std::string GetExecutablePath(bool &success_);

        /*
         * Get file extension
         */
        static std::string GetFileExtension(const std::string &path_);

        /*
         * Get a named font
         */
        static Graphics::TFont GetFont(const std::string &name_);

        /*
         * Get a named music
         */
        static Audio::TMusic GetMusic(const std::string &name_);

        /*
         * Get a named sound
         */
        static Audio::TSound GetSound(const std::string &name_);

        /*
         * Get a named texture
         */
        static Graphics::TTexture2D GetTexture(const std::string &name_);

        /*
         * Get the working directory.
         * This is the directory that the game was run from
         */
        static std::string GetWorkingDirectory();

        /*
         * Loads all files in the resources directory.
         * All names will be set to their relative path without their extension
         */
        static void LoadResources();

        /*
         * Load font from file
         */
        static bool LoadFont(const std::string &inPath_, const std::string &name_);

        /*
         * Load music from file
         */
        static bool LoadMusic(const std::string &inPath_, const std::string &name_);

        /*
         * Load sound from file
         */
        static bool LoadSound(const std::string &inPath_, const std::string &name_);

        /*
         * Load texture from file
         */
        static bool LoadTexture(const std::string &inPath_, const std::string &name_);
    };
}

#endif //RESOURCES_H