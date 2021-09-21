#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Shader.hpp"
#include "GuiCode2/Font.hpp"

namespace GuiCode
{
    struct Command
    {
        union
        {
            Color color;
            Vec4<float> a;
            struct { Vec2<float> a; Vec2<float> b; } as;
            struct { float a; float b; float c; float d; } as2;
            struct { int a; int b; int c; int d; } as3;
            std::string_view view;
        };

        union
        {
            Vec2<float> b;
            struct { float a; float b; } bs;
        };

        const enum Type : char {
            Fill = 0, Clip, PushClip, PopClip, ClearClip, Translate, PushMatrix, PopMatrix, 
            Viewport, Line, Quad, Ellipse, Triangle, Text, FontSize, Font, TextAlign, LineHeight, 
        } type;
    };

    struct CommandCollection
    {
        std::queue<Command>& Get() { return m_Commands; }

        void Fill(const Color& a) { m_Commands.push(Command{ .color = a, .type = Command::Fill }); };
        void Clip(const Vec4<float>& a) { viewport = viewport.Overlap(a); m_Commands.push(Command{ .a = a, .type = Command::Clip }); };
        void PushClip() { m_ViewportStack.push(viewport); m_Commands.push(Command{ .type = Command::PushClip }); };
        void PopClip() { viewport = m_ViewportStack.top(); m_ViewportStack.pop(); m_Commands.push(Command{ .type = Command::PopClip }); };
        void Translate(const Vec2<float>& a) { m_Commands.push(Command{ .as = {.a = a }, .type = Command::Translate }); };
        void PushMatrix() { m_Commands.push(Command{ .type = Command::PushMatrix }); };
        void PopMatrix() { m_Commands.push(Command{ .type = Command::PopMatrix }); };
        void Viewport(const Vec4<float>& a) { m_Commands.push(Command{.a = a, .type = Command::Viewport }); };
        void Line(const Vec4<float>& a, float thickness) { m_Commands.push(Command{.a = a, .bs = {.a = thickness }, .type = Command::Line }); };
        void Quad(const Vec4<float>& a, float b = 0) { m_Commands.push(Command{ .a = a, .bs = {.a = b }, .type = Command::Quad }); };
        void Ellipse(const Vec4<float>& a, const Vec2<float>& b = { 0, 0 }) { m_Commands.push(Command{ .a = a, .b = b, .type = Command::Ellipse }); };
        void Triangle(const Vec4<float>& a, float b = 0) { m_Commands.push(Command{ .a = a, .bs{.a = b }, .type = Command::Triangle }); };
        void Text(std::string_view str, const Vec2<float>& b) { m_Commands.push(Command{.view = str, .b = b, .type = Command::Text }); };
        void Font(std::string_view str) { m_Commands.push(Command{.view = str, .type = Command::Font }); };
        void TextAlign(int align) { m_Commands.push(Command{ .as3 = {.a = align }, .type = Command::TextAlign }); };
        void FontSize(float size) { m_Commands.push(Command{ .as2 = {.a = size }, .type = Command::FontSize }); };
        void LineHeight(float mult) { m_Commands.push(Command{ .as2 = {.a = mult }, .type = Command::FontSize }); };

        Vec4<float> viewport;
    private:
        std::stack<Vec4<float>> m_ViewportStack;
        std::queue<Command> m_Commands;
    };

    class GraphicsBase
    {
        static inline int m_GraphicsIdCounter = 0;
    protected:
        static inline std::map<std::string_view, GuiCode::Font> m_Fonts;
    public:
        static inline std::string DefaultFont = "segoeui";

        virtual void Render() = 0;
        virtual void SetProjection(const glm::mat4& proj);
        static void LoadFont(const std::string& path, const char* name);
        static void LoadFont(const char* name);

        static inline float CharWidth(const char c, const std::string_view& font, float size)
        {
            if (!m_Fonts.contains(font))
                return 0;

            return m_Fonts.at(font).Size(size).Char(c).advance >> 6;
        }

        static inline float StringWidth(const std::string_view& c, const std::string_view& font, float size)
        {
            if (!m_Fonts.contains(font))
                return 0;

            float _width = 0;
            auto& _font = m_Fonts.at(font).Size(size);
            for (auto& _c : c)
                _width += _font.Char(_c).advance >> 6;
            return _width;
        }

        CommandCollection collection;

    protected:
        virtual void Translate(const glm::vec2&);
        virtual void Scale(const glm::vec2&);
        virtual void PushMatrix();
        virtual void PopMatrix();
        virtual void Fill(const Color&) = 0;
        virtual void Quad(const glm::vec4&, float) = 0;
        virtual void Line(const glm::vec4&, float) = 0;
        virtual void Ellipse(const glm::vec4&, const glm::vec2&) = 0;
        virtual void Triangle(const glm::vec4&, float) = 0;
        virtual void Text(std::string_view, const glm::vec2&) = 0;
        virtual void Font(std::string_view) = 0;
        virtual void TextAlign(int) = 0;
        virtual void FontSize(float) = 0;
        virtual void LineHeight(float) = 0;
        virtual void Clip(const glm::vec4&) = 0;
        virtual void PushClip() = 0;
        virtual void PopClip() = 0;
        virtual void ClearClip() = 0;
        virtual void Viewport(const glm::vec4&) = 0;

        std::stack<Vec4<float>> m_ClipStack;
        Vec4<float> m_Clip;

        glm::mat4 m_Projection{ 0 };
        glm::mat4 m_ViewProj{ 0 };
        std::stack<glm::mat4> m_MatrixStack;
        glm::mat4 m_Matrix{ 1.0f }; 
        Vec2<float> m_Size;
        int m_GraphicsId = m_GraphicsIdCounter++;
        float m_Scaling = 1;
    };

    class OpenGL : public GraphicsBase
    {
    public:
        OpenGL();
        void Render() override;

    private:
        glm::vec4 FlipY(const glm::vec4& v) { return { v.x, m_Size.height - v.y - v.w, v.z, v.w }; };
        glm::vec4 FlipY2(const glm::vec4& v) { return { v.x, m_Size.height - v.y, v.z, m_Size.height - v.w }; };

        void Fill(const Color&) override;
        void CreateQuadBuffers();
        void Quad(const glm::vec4&, float) override;
        void CreateLineBuffers();
        void Line(const glm::vec4&, float) override;
        void CreateEllipseBuffers();
        void Ellipse(const glm::vec4&, const glm::vec2&) override;
        void CreateTriangleBuffers();
        void Triangle(const glm::vec4&, float) override;
        void CreateTextBuffers();
        void Text(std::string_view, const glm::vec2&) override;
        void Font(std::string_view) override;
        void TextAlign(int) override;
        void FontSize(float) override;
        void LineHeight(float) override;
        void Clip(const glm::vec4&) override;
        void PushClip() override;
        void PopClip() override;
        void ClearClip() override;
        void Viewport(const glm::vec4&) override;

        int m_PreviousShader = -1;
        Color m_Fill{ 1, 1, 1, 1 };
        float m_FontSize = 16;
        float m_LineHeight = 1.2;
        int m_TextAlign = Align::Left | Align::Bottom;

        GuiCode::Font* m_CurrentFont = nullptr;

        struct { unsigned int vao, vbo; } quad;
        struct { unsigned int vao, vbo; } line;
        struct { unsigned int vao, vbo; } ellipse;
        struct { unsigned int vao, vbo; } triangle;
        struct { unsigned int vao, vbo; } text;
    };
}