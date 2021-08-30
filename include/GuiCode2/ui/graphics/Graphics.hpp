#pragma once
#include "GuiCode/pch.hpp"
#include "GuiCode/ui/graphics/Shader.hpp"
#include "GuiCode/ui/graphics/Collection.hpp"

// --------------------------------------------------------------------------
// ----------------------------- Graphics -----------------------------------
// --------------------------------------------------------------------------

namespace Graphics
{
    void SetProjection(const glm::mat4& proj);
    bool WindowFocused();
    void WindowFocused(bool a);
    void CurrentWindow(int a);

    void RunCommands(const ::CommandCollection&);
    
    // --------------------------------------------------------------------------
    // ---------------------------- Textures ------------------------------------
    // --------------------------------------------------------------------------

    struct Texture 
    {
        unsigned int TextureID = 0;

        int Width = -1;
        int Height = -1;

        Texture();
        Texture(unsigned int a);
        Texture(const std::string& path);
        void SetTexture(const std::string& path);
        operator unsigned int() { return TextureID; }
    };

    struct Textures
    {
        inline static Texture
            FileIcon,
            FolderIcon,
            AudioFileIcon,
            Cross1, Cross2, Cross3,
            Maxi1, Maxi2, Maxi3,
            Maxi4, Maxi5, Maxi6,
            Mini1, Mini2, Mini3,
            Logo;
    };

    // --------------------------------------------------------------------------
    // -------------------------- Text Rendering --------------------------------
    // --------------------------------------------------------------------------
    
    enum Fonts
    {
        Gidole, Gidole14
    };

    void LoadFont(const std::string& path, Fonts name);
    void LoadFont(const std::string& path, Fonts name, unsigned int size);

    void Init();
};

