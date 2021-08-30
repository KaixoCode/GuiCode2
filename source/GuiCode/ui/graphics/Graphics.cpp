#include "GuiCode/ui/graphics/Graphics.hpp"

#define SHADER(a) ABSOLUTE_SHADER_PATH###a

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

// --------------------------------------------------------------------------
// ----------------------------- Graphics -----------------------------------
// --------------------------------------------------------------------------
namespace Graphics
{
    struct Character
    {
        unsigned int TextureID;
        glm::ivec2   Size;
        glm::ivec2   Bearing;
        unsigned int Advance;
    };

    glm::mat4 m_Projection{ 0 };
    std::stack<glm::mat4> m_MatrixStack;
    glm::mat4 m_Matrix{ 1.0f };
    int m_CurrentWindowId = -1;
    bool m_WindowFocused = false;

    void m_Translate(const glm::vec2& v) { m_Matrix = glm::translate(m_Matrix, glm::vec3(v.x, v.y, 0)); }
    void m_Scale(const glm::vec2& v) { m_Matrix = glm::scale(m_Matrix, glm::vec3(v.x, v.y, 1)); }
    void m_PushMatrix() { m_MatrixStack.push(m_Matrix); }
    void m_PopMatrix() { if (m_MatrixStack.size() > 1) { m_Matrix = m_MatrixStack.top(); m_MatrixStack.pop(); } }
    void SetProjection(const glm::mat4& proj) { m_Projection = proj; }
    bool WindowFocused() { return m_WindowFocused; }
    void WindowFocused(bool a) { m_WindowFocused = a; }
    void CurrentWindow(int a) { m_CurrentWindowId = a; }

    void Init()
    {
        Graphics::LoadFont("../assets/fonts/gidole/Gidole-Regular.otf", Fonts::Gidole);
        Graphics::LoadFont("../assets/fonts/gidole/Gidole-Regular.otf", Fonts::Gidole14, 14);
        Textures::FileIcon.SetTexture("../assets/textures/file.png");
        Textures::AudioFileIcon.SetTexture("../assets/textures/audiofile.png");
        Textures::FolderIcon.SetTexture("../assets/textures/folder.png");

        Textures::Cross1.SetTexture("../assets/textures/cross1.png");
        Textures::Cross2.SetTexture("../assets/textures/cross2.png");
        Textures::Cross3.SetTexture("../assets/textures/cross3.png");
    
        Textures::Maxi1.SetTexture("../assets/textures/maxi1.png");
        Textures::Maxi2.SetTexture("../assets/textures/maxi2.png");
        Textures::Maxi3.SetTexture("../assets/textures/maxi3.png");
        Textures::Maxi4.SetTexture("../assets/textures/maxi4.png");
        Textures::Maxi5.SetTexture("../assets/textures/maxi5.png");
        Textures::Maxi6.SetTexture("../assets/textures/maxi6.png");
    
        Textures::Mini1.SetTexture("../assets/textures/mini1.png");
        Textures::Mini2.SetTexture("../assets/textures/mini2.png");
        Textures::Mini3.SetTexture("../assets/textures/mini3.png");

        Textures::Logo.SetTexture("../assets/textures/logosmall.png");
    }

    // --------------------------------------------------------------------------
    // --------------------------- Basic Shapes ---------------------------------
    // --------------------------------------------------------------------------

    void m_Quad(const glm::vec4& dim);
    void m_TexturedQuad(unsigned int texture, const glm::vec4& dim);
    void m_Ellipse(const glm::vec4& dim);
    void m_Triangle(const glm::vec4& dim, float rotation);
    void m_Text(const std::string* text, float x, float y);

    Color m_Fill{ 1, 1, 1, 1 };
    Color m_Stroke{ 1, 1, 1, 1 };

    
    // Text stuff
    float m_FontSize = 16.0f;
    Fonts m_Font = Fonts::Gidole;
    std::unordered_map<Fonts, std::unordered_map<char, Graphics::Character>> m_Fonts;
    std::unordered_map<Fonts, int> m_Fontsizes;
    
    Vec2<Align> m_TextAlign = { Align::LEFT, Align::BOTTOM };

    CommandCollection m_PrevCollection;

    void RunCommands(const ::CommandCollection& d)
    {
        //glClear(GL_COLOR_BUFFER_BIT);
        //glClearColor(0, 0, 0, 0);
        LOG(d.Get().size());

       for (int i = 0; i < d.Get().size(); i++)
       {
           
            auto& a = d.Get()[i];
            switch (a->type)
            {
            case Fill: m_Fill = a->fill / 255.0; break;
            case Stroke: m_Stroke = a->stroke / 255.0; break;
            case TextAlign: m_TextAlign = a->align; break;
            case FontSize: m_FontSize = a->fontSize; break;
            case Font: m_Font = (Fonts)a->font, m_FontSize = a->fontSize; break;
            case Quad: m_Quad(a->dimension); break;
            case TexturedQuad: m_TexturedQuad(a->texture, a->textureDimension); break;
            case Text: m_Text(a->text, a->position.x, a->position.y); break;
            case Ellipse: m_Ellipse(a->dimension); break;
            case Triangle: m_Triangle(a->dimension, a->rotation); break;
            case Clip: glEnable(GL_SCISSOR_TEST); glScissor(a->dimension.x, a->dimension.y, a->dimension.z, a->dimension.w); break;
            case Viewport: glViewport(a->dimension.x, a->dimension.y, a->dimension.z, a->dimension.w); break;
            case ClearClip: glDisable(GL_SCISSOR_TEST); break;
            case Translate: m_Translate(a->translate); break;
            case Scale: m_Scale(a->scale); break;
            case PushMatrix: m_PushMatrix(); break;
            case PopMatrix: m_PopMatrix(); break;
            }
        }

 /*       m_PrevCollection.Clear();
        for (int i = 0; i < d.Get().size(); i++)
        {
            auto& a = d.Get()[i];

            m_PrevCollection.Add(a);
        }*/
    }

    // --------------------------------------------------------------------------
    // --------------------------- Basic Shapes ---------------------------------
    // --------------------------------------------------------------------------



    void m_Quad(const glm::vec4& dim)
    {
        static Shader _shader
        {
            SHADER(QuadVertex.shader),
            SHADER(QuadFragment.shader)
        };

        if (m_CurrentWindowId == -1)
            return;

        static std::unordered_map<int, unsigned int> _VAOs;
        std::unordered_map<int, unsigned int>::iterator _it;
        static unsigned int _VAO, _VBO, _EBO;
        if ((_it = _VAOs.find(m_CurrentWindowId)) != _VAOs.end())
            _VAO = std::get<1>(*_it);
    
        else
        {
            float _vertices[] = { 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };
            unsigned int _indices[] = { 0, 1, 3, 1, 2, 3 };

            glGenVertexArrays(1, &_VAO);
            glGenBuffers(1, &_VBO);
            glGenBuffers(1, &_EBO);

            glBindVertexArray(_VAO);

            glBindBuffer(GL_ARRAY_BUFFER, _VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            _VAO = std::get<1>(_VAOs.emplace(m_CurrentWindowId, _VAO));
        }

        _shader.Use();
        glm::mat4 _model{ 1.0f };
        _model = glm::translate(_model, glm::vec3{ dim.x, dim.y, 0 });
        _model = glm::scale(_model, glm::vec3{ dim.z, dim.w, 1 });


        _shader.SetMat4("model", _model);
        _shader.SetMat4("view", m_Matrix);
        _shader.SetMat4("projection", m_Projection);
        _shader.SetVec4("color", m_Fill);

        glBindVertexArray(_VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    }

    void m_TexturedQuad(unsigned int texture, const glm::vec4& dim)
    {
        static Shader _shader
        {
            SHADER(TexturedQuadVertex.shader),
            SHADER(TexturedQuadFragment.shader)
        };
    
        if (m_CurrentWindowId == -1)
            return;

        static std::unordered_map<int, unsigned int> _VAOs;
        std::unordered_map<int, unsigned int>::iterator _it;
        static unsigned int _VAO, _VBO, _EBO;
        if ((_it = _VAOs.find(m_CurrentWindowId)) != _VAOs.end())
            _VAO = std::get<1>(*_it);
    
        else
        {
            float _vertices[] = {
                // positions   // texCoords
                 1.0f,  1.0f,  1.0f, 1.0f,
                 1.0f,  0.0f,  1.0f, 0.0f,
                 0.0f,  0.0f,  0.0f, 0.0f,
                 0.0f,  1.0f,  0.0f, 1.0f,
            };

            unsigned int indices[] = {
                0, 1, 3, // first triangle
                1, 2, 3  // second triangle
            };

            glGenVertexArrays(1, &_VAO);
            glGenBuffers(1, &_VBO);
            glGenBuffers(1, &_EBO);

            glBindVertexArray(_VAO);

            glBindBuffer(GL_ARRAY_BUFFER, _VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
            glEnableVertexAttribArray(1);

            _VAO = std::get<1>(_VAOs.emplace(m_CurrentWindowId, _VAO));
        }

        _shader.Use();
        glm::mat4 _model{ 1.0f };
        _model = glm::translate(_model, glm::vec3{ dim.x, dim.y, 0 });
        _model = glm::scale(_model, glm::vec3{ dim.z, dim.w, 1 });

        _shader.SetMat4("model", _model);
        _shader.SetMat4("view", m_Matrix);
        _shader.SetMat4("projection", m_Projection);

        _shader.SetInt("theTexture", 1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(_VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void m_Ellipse(const glm::vec4& dim)
    {
        static Shader _shader
        {
            SHADER(EllipseVertex.shader),
            SHADER(EllipseFragment.shader)
        };

        if (m_CurrentWindowId == -1)
            return;

        static std::unordered_map<int, unsigned int> _VAOs;
        std::unordered_map<int, unsigned int>::iterator _it;
        static unsigned int _VAO, _VBO, _EBO;
        if ((_it = _VAOs.find(m_CurrentWindowId)) != _VAOs.end())
            _VAO = std::get<1>(*_it);

        else
        {
            float _vertices[] = { 0.0f,  0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 1.0f };
            unsigned int _indices[] = { 0, 1, 3, 1, 2, 3 };

            glGenVertexArrays(1, &_VAO);
            glGenBuffers(1, &_VBO);
            glGenBuffers(1, &_EBO);

            glBindVertexArray(_VAO);

            glBindBuffer(GL_ARRAY_BUFFER, _VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _EBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(_indices), _indices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            _VAO = std::get<1>(_VAOs.emplace(m_CurrentWindowId, _VAO));
        }

        _shader.Use();
        glm::mat4 _model{ 1.0f };
        _model = glm::translate(_model, glm::vec3{ dim.x, dim.y, 0 });
        _model = glm::scale(_model, glm::vec3{ dim.z, dim.w, 1 });

        _shader.SetMat4("model", _model);
        _shader.SetMat4("view", m_Matrix);
        _shader.SetMat4("projection", m_Projection);
        _shader.SetVec4("color", m_Fill);
        _shader.SetVec4("dimensions", dim);

        glBindVertexArray(_VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
    }

    void m_Triangle(const glm::vec4& dim, float rotation)
    {
        static Shader _shader
        {
            SHADER(QuadVertex.shader),
            SHADER(QuadFragment.shader)
        };

        if (m_CurrentWindowId == -1)
            return;

        static std::unordered_map<int, unsigned int> _VAOs;
        std::unordered_map<int, unsigned int>::iterator _it;
        static unsigned int _VAO, _VBO, _EBO;
        if ((_it = _VAOs.find(m_CurrentWindowId)) != _VAOs.end())
            _VAO = std::get<1>(*_it);

        else
        {
            float _vertices[] = { -0.5f, -0.5f, 0.5f, 0.0f, -0.5f, 0.5f };

            glGenVertexArrays(1, &_VAO);
            glGenBuffers(1, &_VBO);

            glBindVertexArray(_VAO);

            glBindBuffer(GL_ARRAY_BUFFER, _VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            _VAO = std::get<1>(_VAOs.emplace(m_CurrentWindowId, _VAO));
        }

        _shader.Use();
        glm::mat4 _model{ 1.0f };
        _model = glm::translate(_model, glm::vec3{ dim.x, dim.y, 0 });
        _model = glm::scale(_model, glm::vec3{ dim.z, dim.w, 1 });
        _model = glm::rotate(_model, glm::radians(rotation), glm::vec3{ 0, 0, 1 });

        _shader.SetMat4("model", _model);
        _shader.SetMat4("view", m_Matrix);
        _shader.SetMat4("projection", m_Projection);
        _shader.SetVec4("color", m_Fill);

        glBindVertexArray(_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    // --------------------------------------------------------------------------
    // -------------------------- Text Rendering --------------------------------
    // --------------------------------------------------------------------------

    void m_Text(const std::string* text, float x, float y)
    {
        int _size = m_Fontsizes[m_Font];
        float _scale = 1;
        if (_size != 0)
            _scale = m_FontSize / _size;

        static Shader _shader
        {
            SHADER(TextVertex.shader),
            SHADER(TextFragment.shader)
        };

        if (m_CurrentWindowId == -1)
            return;

        static std::unordered_map<int, unsigned int> _VAOs;
        static std::unordered_map<int, unsigned int> _VBOs;
        std::unordered_map<int, unsigned int>::iterator _it;
        static unsigned int _VAO, _VBO;
        if ((_it = _VAOs.find(m_CurrentWindowId)) != _VAOs.end())
        {
            _VAO = std::get<1>(*_it);
            _VBO = _VBOs.at(m_CurrentWindowId);
        }
        else
        {
            glGenVertexArrays(1, &_VAO);
            glGenBuffers(1, &_VBO);
            glBindVertexArray(_VAO);
            glBindBuffer(GL_ARRAY_BUFFER, _VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glBindVertexArray(0);

            _VAO = std::get<1>(_VAOs.emplace(m_CurrentWindowId, _VAO));
            _VBO = std::get<1>(_VBOs.emplace(m_CurrentWindowId, _VBO));
        }

        glm::vec3 _pos = m_Matrix[3];

        _shader.Use();
        _shader.SetVec4("color", m_Fill);
        _shader.SetMat4("projection", m_Projection);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(_VAO);

        long _totalWidth = 0;
        long _totalHeight = m_FontSize * 0.7;
        std::string::const_iterator _c;
        if (m_TextAlign.x != Align::LEFT || m_TextAlign.y != Align::BOTTOM)
            for (_c = text->begin(); _c != text->end(); _c++)
            {
                Character _ch = Graphics::m_Fonts[m_Font][*_c];
                _totalWidth += (_ch.Advance >> 6);
            }

        for (_c = text->begin(); _c != text->end(); _c++)
        {
            Character _ch = Graphics::m_Fonts[m_Font][*_c];

            int _xpos = x * m_Matrix[0][0] + _ch.Bearing.x * _scale + _pos.x;
            if (m_TextAlign.x == Align::CENTER)
                _xpos -= 0.5 * _totalWidth * _scale;
            else if (m_TextAlign.x == Align::RIGHT)
                _xpos -= _totalWidth * _scale;

            int _ypos = y - (_ch.Size.y - _ch.Bearing.y) * _scale + _pos.y;
            if (m_TextAlign.y == Align::CENTER)
                _ypos -= 0.5 * _totalHeight * _scale;
            else if (m_TextAlign.y == Align::TOP)
                _ypos += _totalHeight * _scale;

            float _w = _ch.Size.x * _scale;
            float _h = _ch.Size.y * _scale;
            float _vertices[6][4] = {
                { _xpos ,     _ypos + _h,   0.0f, 0.0f },
                { _xpos ,     _ypos ,       0.0f, 1.0f },
                { _xpos + _w, _ypos ,       1.0f, 1.0f },

                { _xpos ,     _ypos + _h,   0.0f, 0.0f },
                { _xpos + _w, _ypos ,       1.0f, 1.0f },
                { _xpos + _w, _ypos + _h,   1.0f, 0.0f }
            };

            glBindTexture(GL_TEXTURE_2D, _ch.TextureID);
            glBindBuffer(GL_ARRAY_BUFFER, _VBO);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(_vertices), _vertices);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            x += (_ch.Advance >> 6) * _scale / m_Matrix[0][0];
        }
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void LoadFont(const std::string& path, Graphics::Fonts name)
    {
        LoadFont(path, name, 48);
    }

    void LoadFont(const std::string& path, Graphics::Fonts name, unsigned int size)
    {
        static FT_Library _ft;
        if (!_ft && FT_Init_FreeType(&_ft))
        {
            LOG("ERROR::FREETYPE: Could not init FreeType Library");
            return;
        }

        FT_Face _face;
        if (FT_New_Face(_ft, path.c_str(), 0, &_face))
        {
            LOG("ERROR::FREETYPE: Failed to load font");
            return;
        }
        else
        {
            Graphics::m_Fonts.insert(
                std::pair<Fonts, std::unordered_map<char, Graphics::Character>>(
                    name, std::unordered_map<char, Graphics::Character>{}));

            FT_Set_Pixel_Sizes(_face, 0, size);
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            for (unsigned char _c = 0; _c < 128; _c++)
            {
                if (FT_Load_Char(_face, _c, FT_LOAD_RENDER))
                {
                    LOG("ERROR::FREETYTPE: Failed to load Glyph");
                    continue;
                }

                unsigned int _texture;
                glGenTextures(1, &_texture);
                glBindTexture(GL_TEXTURE_2D, _texture);
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_RED,
                    _face->glyph->bitmap.width,
                    _face->glyph->bitmap.rows,
                    0,
                    GL_RED,
                    GL_UNSIGNED_BYTE,
                    _face->glyph->bitmap.buffer
                );

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                Graphics::Character _character = {
                    _texture,
                    glm::ivec2(_face->glyph->bitmap.width, _face->glyph->bitmap.rows),
                    glm::ivec2(_face->glyph->bitmap_left, _face->glyph->bitmap_top),
                    static_cast<unsigned int>(_face->glyph->advance.x)
                };

                Graphics::m_Fonts[name].insert(
                    std::pair<char, Graphics::Character>(_c, _character));
            }
            glBindTexture(GL_TEXTURE_2D, 0);
            m_Fontsizes.emplace(name, size);
        }

        FT_Done_Face(_face);
        _ft = {};
        FT_Done_FreeType(_ft);
    }

    // --------------------------------------------------------------------------
    // ---------------------------- Textures ------------------------------------
    // --------------------------------------------------------------------------

    Texture::Texture() 
    {}

    Texture::Texture(unsigned int a)
        : TextureID(a)
    {}

    Texture::Texture(const std::string& path)
    {
        SetTexture(path);
    }

    void Texture::SetTexture(const std::string& path)
    {
        glGenTextures(1, &TextureID);
        glBindTexture(GL_TEXTURE_2D, TextureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.

        //Load the image
        int channels;
        unsigned char* _data = stbi_load(path.c_str(), &Width, &Height, &channels, 0);
        if (_data) {
            if (channels == 4) 
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, Width, Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
            else
                glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, _data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else {
            LOG("Failed to load texture, path: " << path);
        }

        stbi_image_free(_data);
    } 
}