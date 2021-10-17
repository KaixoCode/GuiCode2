#include "GuiCode2/Graphics/Graphics.hpp"
#include "GuiCode2/Graphics/Font.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

namespace GuiCode
{
    bool GraphicsBase::LoadFont(const std::string& name)
    {
        using namespace std::string_literals;
        TCHAR szPath[MAX_PATH];
        SHGetFolderPathA(nullptr, CSIDL_FONTS, nullptr, 0, szPath);

        std::string _noExtension = szPath;
        _noExtension += "/"s + name;
        std::filesystem::path _font = _noExtension + ".ttf";

        if (std::filesystem::exists(_font))
        {
            char* _name = new char[name.size() + 1];
            for (int i = 0; i < name.size(); i++)
                _name[i] = name[i];
            _name[name.size()] = '\0';
            m_Fonts.emplace(_name, GuiCode::Font{ _font.string() });
            return true;
        }
        else
            return false;
    }

    void GraphicsBase::LoadFont(const std::string& path, const std::string& name)
    {
        m_Fonts.emplace(name, GuiCode::Font{ path });
    }

    void GraphicsBase::Translate(const glm::vec2& v)
    {
        m_Matrix = glm::translate(m_Matrix, glm::vec3(v.x, v.y, 0));
        m_ViewProj = m_Projection * m_Matrix;
    }

    void GraphicsBase::Scale(const glm::vec2& v)
    {
        m_Matrix = glm::scale(m_Matrix, glm::vec3(v.x, v.y, 1));
        m_ViewProj = m_Projection * m_Matrix;
    }

    void GraphicsBase::PushMatrix()
    {
        m_MatrixStack.push(m_Matrix);
    }

    void GraphicsBase::PopMatrix()
    {
        if (m_MatrixStack.size() > 1)
        {
            m_Matrix = m_MatrixStack.top();
            m_MatrixStack.pop();
            m_ViewProj = m_Projection * m_Matrix;
        }
    }

    void GraphicsBase::SetProjection(const glm::mat4& proj)
    {
        m_Projection = proj;
        m_ViewProj = m_Projection * m_Matrix;
        m_Size.width = 2 / m_Projection[0][0];
        m_Size.height = 2 / m_Projection[1][1];
    }

    float GraphicsBase::CharWidth(const char c, const std::string_view& font, float size)
    {
        if (!m_Fonts.contains(font))
            return 0;

        return (*m_Fonts.find(font)).second.Size(size).Char(c).advance >> 6;
    }

    float GraphicsBase::StringWidth(const std::string_view& c, const std::string_view& font, float size)
    {
        if (!m_Fonts.contains(font))
            return 0;

        float _width = 0;
        auto& _font = (*m_Fonts.find(font)).second.Size(size);
        for (auto& _c : c)
            _width += _font.Char(_c).advance >> 6;
        return _width;
    }

}

