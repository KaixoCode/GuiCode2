#include "GuiCode2/Graphics.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

// --------------------------------------------------------------------------
// ----------------------------- Graphics -----------------------------------
// --------------------------------------------------------------------------
namespace GuiCode
{

    Color rgb2hsv(Color in)
    {
        Color       out;
        double      min, max, delta;
        out.a = in.a;

        min = in.r < in.g ? in.r : in.g;
        min = min < in.b ? min : in.b;

        max = in.r > in.g ? in.r : in.g;
        max = max > in.b ? max : in.b;

        out.v = max;                                // v
        delta = max - min;
        if (delta < 0.00001)
        {
            out.s = 0;
            out.h = 0; // undefined, maybe nan?
            return out;
        }
        if (max > 0.0) { // NOTE: if Max is == 0, this divide would cause a crash
            out.s = (delta / max);                  // s
        }
        else {
            // if max is 0, then r = g = b = 0              
            // s = 0, h is undefined
            out.s = 0.0;
            out.h = NAN;                            // its now undefined
            return out;
        }
        if (in.r >= max)                           // > is bogus, just keeps compilor happy
            out.h = (in.g - in.b) / delta;        // between yellow & magenta
        else
            if (in.g >= max)
                out.h = 2.0 + (in.b - in.r) / delta;  // between cyan & yellow
            else
                out.h = 4.0 + (in.r - in.g) / delta;  // between magenta & cyan

        out.h *= 60.0;                              // degrees

        if (out.h < 0.0)
            out.h += 360.0;

        return out;
    }

    Color hsv2rgb(Color in)
    {
        double      hh, p, q, t, ff;
        long        i;
        Color       out;
        out.a = in.a;

        if (in.s <= 0.0) {       // < is bogus, just shuts up warnings
            out.r = in.v;
            out.g = in.v;
            out.b = in.v;
            return out;
        }
        hh = in.h;
        if (hh >= 360.0) hh = 0.0;
        hh /= 60.0;
        i = (long)hh;
        ff = hh - i;
        p = in.v * (1.0 - in.s);
        q = in.v * (1.0 - (in.s * ff));
        t = in.v * (1.0 - (in.s * (1.0 - ff)));

        switch (i) {
        case 0:
            out.r = in.v;
            out.g = t;
            out.b = p;
            break;
        case 1:
            out.r = q;
            out.g = in.v;
            out.b = p;
            break;
        case 2:
            out.r = p;
            out.g = in.v;
            out.b = t;
            break;

        case 3:
            out.r = p;
            out.g = q;
            out.b = in.v;
            break;
        case 4:
            out.r = t;
            out.g = p;
            out.b = in.v;
            break;
        case 5:
        default:
            out.r = in.v;
            out.g = p;
            out.b = q;
            break;
        }
        return out;
    }

    int prevShader = -1;

    std::stack<Vec4<int>> m_ClipStack;

    glm::mat4 m_Projection{ 0 };
    glm::mat4 m_ViewProj{ 0 };
    std::stack<glm::mat4> m_MatrixStack;
    glm::mat4 m_Matrix{ 1.0f };
    int m_CurrentWindowId = -1;
    float m_Scaling = 1;
    bool m_WindowFocused = false;
    Vec2<int> m_Size;

    void m_Translate(const glm::vec2& v)
    {
        m_Matrix = glm::translate(m_Matrix, glm::vec3(v.x, v.y, 0));
        m_ViewProj = m_Projection * m_Matrix;
    }

    void m_Scale(const glm::vec2& v)
    {
        m_Matrix = glm::scale(m_Matrix, glm::vec3(v.x, v.y, 1));
        m_ViewProj = m_Projection * m_Matrix;
    }

    void m_PushMatrix() { m_MatrixStack.push(m_Matrix); }
    void m_PopMatrix()
    {
        if (m_MatrixStack.size() > 1)
        {
            m_Matrix = m_MatrixStack.top();
            m_MatrixStack.pop();
            m_ViewProj = m_Projection * m_Matrix;
        }
    }

    void SetProjection(const glm::mat4& proj)
    {
        m_Projection = proj;
        m_ViewProj = m_Projection * m_Matrix;
        m_Size.width = 2 / m_Projection[0][0];
        m_Size.height = 2 / m_Projection[1][1];
    }

    void CurrentWindow(int a) { m_CurrentWindowId = a; }


    // --------------------------------------------------------------------------
    // --------------------------- Basic Shapes ---------------------------------
    // --------------------------------------------------------------------------

    void m_Line(const glm::vec4& dim, float width);
    void m_Quad(const glm::vec4& dim, float rotation);
    void m_Ellipse(const glm::vec4& dim, const glm::vec2& a);
    void m_Triangle(const glm::vec4& dim, float rotation);

    Color m_Fill{ 1, 1, 1, 1 };
    Color m_Stroke{ 1, 1, 1, 1 };

    struct OverlayQuad
    {
        Vec4<int> dims;
        Color color;
    };

    std::vector<OverlayQuad> m_Overlays;


    void RunCommands(CommandCollection& d)
    {
        enum Type {
            Fill = 0, Clip, PopClip, ClearClip, Translate, PushMatrix,
            PopMatrix, Viewport, Line, Quad, Ellipse, Triangle
        };

        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 0);

        for (int i = 0; i < d.Get().size(); i++)
        {
            Command& a = d.Get()[i];

            switch(a.data.index()) {
            case Fill: m_Fill = std::get<Fill>(a.data).color / 256; break;
            case Quad: m_Quad(std::get<Quad>(a.data).dimensions, std::get<Quad>(a.data).rotation); break;
            case Line: m_Line(Vec4<double>{ std::get<Line>(a.data).start, std::get<Line>(a.data).end }, std::get<Line>(a.data).thickness); break;
            case Ellipse: m_Ellipse(std::get<Ellipse>(a.data).dimensions, std::get<Ellipse>(a.data).angles); break;
            case Triangle: m_Triangle(std::get<Triangle>(a.data).dimensions, std::get<Triangle>(a.data).rotation); break;
            case Clip:
            {
                glEnable(GL_SCISSOR_TEST);
                auto command = std::get<Clip>(a.data);
                Vec4<int> clip = {
                    std::ceil((command.area.x + m_Matrix[3][0]) / m_Scaling),
                        std::ceil((command.area.y + m_Matrix[3][1]) / m_Scaling),
                        std::ceil(command.area.z / m_Scaling),
                        std::ceil(command.area.w / m_Scaling)
                };
                if (!m_ClipStack.empty())
                    clip = clip.Overlap(m_ClipStack.top());
                m_ClipStack.push(clip);
                glScissor(clip.x, clip.y, clip.width, clip.height); break;
            }
            case PopClip:
            {
                m_ClipStack.pop();
                if (m_ClipStack.size() == 0)
                {
                    glDisable(GL_SCISSOR_TEST);
                }
                else
                {
                    glEnable(GL_SCISSOR_TEST);
                    Vec4<int> clip2 = m_ClipStack.top();
                    glScissor(clip2.x, clip2.y, clip2.width, clip2.height);
                }
                break;
            }
            case Viewport: 
            {
                auto command = std::get<Viewport>(a.data);
                glViewport(
                std::floor(command.area.x / m_Scaling),
                std::floor(command.area.y / m_Scaling),
                std::floor(command.area.z / m_Scaling),
                std::floor(command.area.w / m_Scaling)); break;
            }
            case ClearClip: glDisable(GL_SCISSOR_TEST); break;
            case Translate: m_Translate(std::get<Translate>(a.data).amount); break;
            case PushMatrix: m_PushMatrix(); break;
            case PopMatrix: m_PopMatrix(); break;
            }
        }

        prevShader = -1;
    }

    // --------------------------------------------------------------------------
    // --------------------------- Basic Shapes ---------------------------------
    // --------------------------------------------------------------------------

    void m_Line(const glm::vec4& dim, float width)
    {
        static Shader _shader
        {
            // Vertex shader
            "#version 330 core \n "
            "layout(location = 0) in vec2 aPos; "
            "uniform vec4 dim; "
            "void main() { "
            "    gl_Position = vec4(aPos.x * dim.z + dim.x, aPos.y * dim.w + dim.y, 0.0, 1.0); "
            "}",

            // Fragment shader
            "#version 330 core \n "
            "out vec4 FragColor; \n"
            "uniform vec4 color; \n"
            "uniform vec4 realdim; \n"
            "uniform float width; \n"
            "float minimum_distance(vec2 v, vec2 w, vec2 p) {\n"
            "    // Return minimum distance between line segment vw and point p\n"
            "    float l2 = pow(distance(w, v), 2);  // i.e. |w-v|^2 -  avoid a sqrt\n"
            "    if (l2 == 0.0) return distance(p, v);   // v == w case\n"
            "    // Consider the line extending the segment, parameterized as v + t (w - v).\n"
            "    // We find projection of point p onto the line.\n "
            "    // It falls where t = [(p-v) . (w-v)] / |w-v|^2\n"
            "    // We clamp t from [0,1] to handle points outside the segment vw.\n"
            "    float t = max(0, min(1, dot(p - v, w - v) / l2));\n"
            "    vec2 projection = v + t * (w - v);  // Projection falls on the segment\n"
            "    return distance(p, projection);\n"
            "}\n"
            "void main() { \n"
            "    float dist = minimum_distance(realdim.zw, realdim.xy, gl_FragCoord.xy);\n"
            "    if (dist / width > 0.5) FragColor = vec4(color.rgb, 2 * (1 - (dist / width)) * color.a); \n"
            "    else FragColor = color; \n"
            "} \n"

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
                0.0f, 0.0f,
                1.0f, 1.0f,
            };

            glGenVertexArrays(1, &_VAO);
            glGenBuffers(1, &_VBO);

            glBindVertexArray(_VAO);

            glBindBuffer(GL_ARRAY_BUFFER, _VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            _VAO = std::get<1>(_VAOs.emplace(m_CurrentWindowId, _VAO));
        }

        if (prevShader != 7)
            _shader.Use();
        prevShader = 7;

        width /= m_Scaling;

        glm::vec4 _tdim = dim;
        float delta_x = _tdim.z - _tdim.x;
        float delta_y = _tdim.w - _tdim.y;
        float angle = std::atan2(delta_y, delta_x);
        float dx = std::cos(angle);
        float dy = std::sin(angle);
        _tdim.x -= dx * width * 0.25;
        _tdim.z += dx * width * 0.25;
        _tdim.y -= dy * width * 0.25;
        _tdim.w += dy * width * 0.25;

        glm::vec4 _dim;
        _dim.x = (_tdim.x + m_Matrix[3][0]) * m_Projection[0][0] + m_Projection[3][0];
        _dim.y = (_tdim.y + m_Matrix[3][1]) * m_Projection[1][1] + m_Projection[3][1];
        _dim.z = (_tdim.z + m_Matrix[3][0]) * m_Projection[0][0] + m_Projection[3][0] - _dim.x;
        _dim.w = (_tdim.w + m_Matrix[3][1]) * m_Projection[1][1] + m_Projection[3][1] - _dim.y;

        glm::vec4 _rdim{
            (dim.x + m_Matrix[3][0]) / m_Scaling,
            (dim.y + m_Matrix[3][1]) / m_Scaling,
            (dim.z + m_Matrix[3][0]) / m_Scaling,
            (dim.w + m_Matrix[3][1]) / m_Scaling };

        delta_x = _rdim.z - _rdim.x;
        delta_y = _rdim.w - _rdim.y;
        angle = std::atan2(delta_y, delta_x);
        dx = std::cos(angle);
        dy = std::sin(angle);
        _rdim.x += dx * width * 0.25;
        _rdim.z -= dx * width * 0.25;
        _rdim.y += dy * width * 0.25;
        _rdim.w -= dy * width * 0.25;

        _shader.SetVec4("dim", _dim);
        _shader.SetVec4("realdim", _rdim);
        _shader.SetFloat("width", width * 0.5f);
        _shader.SetVec4("color", m_Fill);

        glLineWidth(width);
        glBindVertexArray(_VAO);
        glDrawArrays(GL_LINES, 0, 2);
    }


    void m_Quad(const glm::vec4& dim, float rotation)
    {
        static Shader _shader
        {
            // Vertex shader
            "#version 450 core \n "
            "layout(location = 0) in vec2 aPos; "
            "uniform mat4 mvp; "
            "void main() { "
            "    gl_Position = mvp * vec4(aPos, 0.0, 1.0); "
            "}",

            // Fragment shader
            "#version 450 core \n "
            "out vec4 FragColor; "
            "uniform vec4 color; "
            "void main() { "
            "    FragColor = color; "
            "} "

        };

        static Shader _shader2
        {
            // Vertex shader
            "#version 330 core \n "
            "layout(location = 0) in vec2 aPos; "
            "uniform vec4 dim; "
            "void main() { "
            "    gl_Position = vec4(dim.x + aPos.x * dim.z, dim.y + aPos.y * dim.w, 0.0, 1.0); "
            "}",

            // Fragment shader
            "#version 330 core \n "
            "out vec4 FragColor; "
            "uniform vec4 color; "
            "void main() { "
            "    FragColor = color; "
            "} "

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
                0.0f, 0.0f,
                1.0f, 0.0f,
                0.0f, 1.0f,
                1.0f, 0.0f,
                1.0f, 1.0f,
                0.0f, 1.0f
            };

            glGenVertexArrays(1, &_VAO);
            glGenBuffers(1, &_VBO);

            glBindVertexArray(_VAO);

            glBindBuffer(GL_ARRAY_BUFFER, _VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            _VAO = std::get<1>(_VAOs.emplace(m_CurrentWindowId, _VAO));
        }

        if (rotation != 0)
        {
            if (prevShader != 6)
                _shader.Use();
            prevShader = 6;

            glm::mat4 _model{ 1.0f };
            _model = glm::translate(_model, glm::vec3{ dim.x, dim.y, 0 });
            _model = glm::translate(_model, glm::vec3{ dim.z / 2, dim.w / 2, 0 });
            _model = glm::rotate(_model, glm::radians(rotation), glm::vec3{ 0, 0, 1 });
            _model = glm::translate(_model, glm::vec3{ -dim.z / 2, -dim.w / 2, 0 });
            _model = glm::scale(_model, glm::vec3{ dim.z, dim.w, 1 });
            _shader.SetMat4("mvp", m_ViewProj * _model);
            _shader.SetVec4("color", m_Fill);
        }
        else
        {
            if (prevShader != 5)
                _shader2.Use();
            prevShader = 5;

            glm::vec4 _dim;
            _dim.x = (dim.x + m_Matrix[3][0]) * m_Projection[0][0] + m_Projection[3][0];
            _dim.y = (dim.y + m_Matrix[3][1]) * m_Projection[1][1] + m_Projection[3][1];
            _dim.z = dim.z * m_Projection[0][0];
            _dim.w = dim.w * m_Projection[1][1];

            _shader2.SetVec4("dim", _dim);
            _shader2.SetVec4("color", m_Fill);
        }

        glBindVertexArray(_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void m_Ellipse(const glm::vec4& dim, const glm::vec2& a)
    {
        static Shader _shader
        {
            "#version 330 core \n "

            "layout(location = 0) in vec2 aPos; "

            "uniform mat4 mvp; "

            "void main() { "
            "    gl_Position = mvp * vec4(aPos, 0.0, 1.0);"
            "} ",

            "#version 330 core \n "
            "out vec4 FragColor; "
            "uniform vec4 color; "
            "uniform vec2 angles; "
            "uniform vec4 dimensions; "
            "void main() { "
            "    vec2 pos = gl_FragCoord.xy; "
            "    float x = dimensions.x; "
            "    float y = dimensions.y; "
            "    float l = sqrt(pow(x - pos.x, 2) + pow(y - pos.y, 2)); "
            "    float a = acos((pos.x - x)/l); "
            "    if (y > pos.y) a = 6.28318530718-a; "
            "    float astart = 0; "
            "    float aend = angles.y - angles.x;"
            "    if (aend < 0) aend = aend + 6.28318530718; "
            "    float aa = a - angles.x;"
            "    if (aa < 0) aa = aa + 6.28318530718; "
            "    float r = (pow(pos.x - x, 2) / pow(dimensions.z / 2, 2)) + (pow(pos.y - y, 2) / pow(dimensions.w / 2, 2)); "
            "    if (aa > aend) { discard; } "
            "    else if (r > 1) { discard; } "
            "    else if (r > 0.90) { FragColor = vec4(color.rgb, 10 * (1 - r) * color.a); } "
            "    else { FragColor = color; } "
            "} "
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
                -.5f, -.5f,
                .5f, -.5f,
                -.5f, .5f,
                .5f, -.5f,
                .5f, .5f,
                -.5f, .5f
            };

            glGenVertexArrays(1, &_VAO);
            glGenBuffers(1, &_VBO);

            glBindVertexArray(_VAO);

            glBindBuffer(GL_ARRAY_BUFFER, _VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);

            _VAO = std::get<1>(_VAOs.emplace(m_CurrentWindowId, _VAO));
        }

        if (prevShader != 3)
            _shader.Use();
        prevShader = 3;

        glm::mat4 _model{ 1.0f };
        _model = glm::translate(_model, glm::vec3{ dim.x, dim.y, 0 });
        _model = glm::scale(_model, glm::vec3{ dim.z, dim.w, 1 });

        _shader.SetMat4("mvp", m_ViewProj * _model);
        _shader.SetVec4("color", m_Fill);

        if (a.x == 0 && a.y == 0)
            _shader.SetVec2("angles", { 0, M_PI * 2 });
        else
            _shader.SetVec2("angles", { std::fmod(a.y + 4.0 * M_PI, 2.0 * M_PI), std::fmod(a.x + 4.0 * M_PI, 2.0 * M_PI) });

        glm::vec4 _dim{ (dim.x + m_Matrix[3][0]) / m_Scaling, (dim.y + m_Matrix[3][1]) / m_Scaling, dim.z / m_Scaling, dim.w / m_Scaling };
        _shader.SetVec4("dimensions", _dim);

        glBindVertexArray(_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void m_Triangle(const glm::vec4& dim, float rotation)
    {
        static Shader _shader
        {
            "#version 330 core \n layout(location = 0) in vec2 aPos; uniform mat4 projection; uniform mat4 view; uniform mat4 model; void main() { gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0); }",
            "#version 330 core \n out vec4 FragColor; uniform vec4 color; void main() { FragColor = color; } "
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

        if (prevShader != 2)
            _shader.Use();
        prevShader = 2;

        glm::mat4 _model{ 1.0f };
        _model = glm::translate(_model, glm::vec3{ dim.x, dim.y, 0 });
        _model = glm::scale(_model, glm::vec3{ dim.z, dim.w, 1 });
        if (rotation != 0)
        {
            _model = glm::rotate(_model, glm::radians(rotation), glm::vec3{ 0, 0, 1 });
        }

        _shader.SetMat4("model", _model);
        _shader.SetMat4("view", m_Matrix);
        _shader.SetMat4("projection", m_Projection);
        _shader.SetVec4("color", m_Fill);

        glBindVertexArray(_VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
    }
}
