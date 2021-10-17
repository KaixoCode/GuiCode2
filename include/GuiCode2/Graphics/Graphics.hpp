#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Utils/Utils.hpp"
#include "GuiCode2/Utils/Color.hpp"
#include "GuiCode2/Graphics/Shader.hpp"
#include "GuiCode2/Graphics/Font.hpp"

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
        static inline std::map<std::string, GuiCode::Font, std::less<>> m_Fonts;
    public:
        static inline std::string DefaultFont = "segoeui";
        static void LoadFont(const std::string& path, const std::string& name);
        static bool LoadFont(const std::string& name);
        static float CharWidth(const char c, const std::string_view& font, float size);
        static float StringWidth(const std::string_view& c, const std::string_view& font, float size);

        virtual void Render() = 0;
        virtual void SetProjection(const glm::mat4& proj);

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
}