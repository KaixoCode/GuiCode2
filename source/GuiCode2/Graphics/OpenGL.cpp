#include "GuiCode2/Graphics/OpenGL.hpp"

namespace GuiCode
{
    OpenGL::OpenGL()
        : GraphicsBase()
    {
        CreateQuadBuffers();
        CreateLineBuffers();
        CreateTriangleBuffers();
        CreateEllipseBuffers();
        CreateTextBuffers();
        CreateTexturedQuadBuffers();
    }

    void OpenGL::Render()
    {
        glClear(GL_COLOR_BUFFER_BIT);
        glClearColor(0, 0, 0, 0);

        m_Clip = { -1, -1, m_Size.width + 2, m_Size.height + 2 };
        this->Clip(m_Clip);

        auto& _commands = collection.Get();

        while (!_commands.empty())
        {
            Command& a = _commands.front();
            switch (a.type) {
            case Command::Fill:         this->Fill(a.color); break;
            case Command::Quad:         this->Quad(FlipY(a.a), a.bs.a, a.bs.c); break;
            case Command::TexturedQuad: this->TexturedQuad(a.bs2.b, FlipY(a.a), a.bs2.a, a.bs2.c); break;
            case Command::Line:         this->Line(FlipY2(a.a), a.bs.a); break;
            case Command::Ellipse:      this->Ellipse(FlipYM(a.a), a.b); break;
            case Command::Triangle:     this->Triangle(FlipY(a.a), a.bs.a); break;
            case Command::Text:         this->Text(a.view, a.b); break;
            case Command::Font:         this->Font(a.view); break;
            case Command::FontSize:     this->FontSize(a.as2.a); break;
            case Command::TextAlign:    this->TextAlign(a.as3.a); break;
            case Command::Clip:         this->Clip(FlipY(a.a)); break;
            case Command::PushClip:     this->PushClip(); break;
            case Command::PopClip:      this->PopClip(); break;
            case Command::Viewport:     this->Viewport(a.a); break;
            case Command::ClearClip:    this->ClearClip(); break;
            case Command::Translate:    this->Translate(a.as.a); break;
            case Command::PushMatrix:   this->PushMatrix(); break;
            case Command::PopMatrix:    this->PopMatrix(); break;
            }
            _commands.pop();
        }

        m_PreviousShader = -1;
    }

    void OpenGL::FontSize(float size)
    {
        m_FontSize = size;
    }

    void OpenGL::LineHeight(float size)
    {
        m_LineHeight = size;
    }

    void OpenGL::Fill(const Color& color)
    {
        m_Fill = color / 256;
    }

    void OpenGL::PushClip()
    {
        m_ClipStack.push(m_Clip);
    }

    void OpenGL::Clip(const glm::vec4& a)
    {
        glEnable(GL_SCISSOR_TEST);
        Vec4<float> clip = {
            std::ceil((a.x + m_Matrix[3][0]) / m_Scaling),
                std::ceil((a.y + m_Matrix[3][1]) / m_Scaling),
                std::ceil(a.z / m_Scaling),
                std::ceil(a.w / m_Scaling)
        };
        clip = clip.Overlap(m_Clip);
        m_Clip = clip;
        glScissor(clip.x, clip.y, clip.width, clip.height);
    }

    void OpenGL::PopClip()
    {
        if (m_ClipStack.size() == 0)
        {
            glDisable(GL_SCISSOR_TEST);
        }
        else
        {
            glEnable(GL_SCISSOR_TEST);
            Vec4<float> clip2 = m_ClipStack.top();
            m_ClipStack.pop();
            m_Clip = clip2;
            glScissor(clip2.x, clip2.y, clip2.width, clip2.height);
        }
    }

    void OpenGL::ClearClip()
    {
        glDisable(GL_SCISSOR_TEST);
    }

    void OpenGL::Viewport(const glm::vec4& a)
    {
        glViewport(
            std::floor(a.x / m_Scaling),
            std::floor(a.y / m_Scaling),
            std::floor(a.z / m_Scaling),
            std::floor(a.w / m_Scaling));
    }

    Texture OpenGL::LoadTexture(const TextureData& data) 
    {
        Texture texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        //Load the image
        int channels = 4;
        unsigned char* _data = data.data;
        if (_data) {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, data.width, data.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, _data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        return texture;
    };

    void OpenGL::FreeTexture(Texture t)
    {
        glDeleteTextures(1, &t);
    };

    void OpenGL::CreateLineBuffers()
    {
        float _vertices[] = {
            0.0f, 0.0f,
            1.0f, 1.0f,
        };

        glGenVertexArrays(1, &line.vao);
        glGenBuffers(1, &line.vbo);

        glBindVertexArray(line.vao);

        glBindBuffer(GL_ARRAY_BUFFER, line.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void OpenGL::Line(const glm::vec4& dim, float width)
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
        static GLint dims = glGetUniformLocation(_shader.ID, "dim");
        static GLint realdim = glGetUniformLocation(_shader.ID, "realdim");
        static GLint widths = glGetUniformLocation(_shader.ID, "width");
        static GLint color = glGetUniformLocation(_shader.ID, "color");

        if (m_PreviousShader != 7)
        {
            _shader.Use();
            glBindVertexArray(line.vao);
        }
        m_PreviousShader = 7;

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
        _dim.x = (_tdim.x + m_Matrix[3].x) * m_Projection[0].x + m_Projection[3].x;
        _dim.y = (_tdim.y + m_Matrix[3].y) * m_Projection[1].y + m_Projection[3].y;
        _dim.z = (_tdim.z + m_Matrix[3].x) * m_Projection[0].x + m_Projection[3].x - _dim.x;
        _dim.w = (_tdim.w + m_Matrix[3].y) * m_Projection[1].y + m_Projection[3].y - _dim.y;

        glm::vec4 _rdim{
            (dim.x + m_Matrix[3].x) / m_Scaling,
            (dim.y + m_Matrix[3].y) / m_Scaling,
            (dim.z + m_Matrix[3].x) / m_Scaling,
            (dim.w + m_Matrix[3].y) / m_Scaling };

        delta_x = _rdim.z - _rdim.x;
        delta_y = _rdim.w - _rdim.y;
        angle = std::atan2(delta_y, delta_x);
        dx = std::cos(angle);
        dy = std::sin(angle);
        _rdim.x += dx * width * 0.25;
        _rdim.z -= dx * width * 0.25;
        _rdim.y += dy * width * 0.25;
        _rdim.w -= dy * width * 0.25;

        _shader.SetVec4(dims, _dim);
        _shader.SetVec4(realdim, _rdim);
        _shader.SetFloat(widths, width * 0.5f);
        _shader.SetVec4(color, m_Fill);

        glLineWidth(width);
        glDrawArrays(GL_LINES, 0, 2);
    }

    void OpenGL::CreateQuadBuffers()
    {
        float _vertices[] = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f
        };

        glGenVertexArrays(1, &quad.vao);
        glGenBuffers(1, &quad.vbo);

        glBindVertexArray(quad.vao);

        glBindBuffer(GL_ARRAY_BUFFER, quad.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void OpenGL::Quad(const glm::vec4& dim, float rotation, int radius)
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
        static GLint mvp = glGetUniformLocation(_shader.ID, "mvp");
        static GLint color = glGetUniformLocation(_shader.ID, "color");
        static Shader _shader2
        {
            // Vertex shader
            R"~~(
#version 450 core
layout(location = 0) in vec2 aPos;
uniform vec4 dim;
uniform vec4 rdim;
out vec2 uv;
out vec2 size;
void main() {
    gl_Position = vec4(dim.x + aPos.x * dim.z, dim.y + aPos.y * dim.w, 0.0, 1.0);
    uv = vec2(aPos.x * rdim.z, aPos.y * rdim.w);
    size = vec2(rdim.z, rdim.w);
}
            )~~",

            // Fragment shader
            R"~~(
#version 450 core
out vec4 FragColor;
uniform vec4 color;
uniform float radius;
in vec2 uv;
in vec2 size;

float roundedFrame (float r, float thickness)
{
    float res = 0;
    if (uv.x > r && uv.x < size.x - r
          || uv.y > r && uv.y < size.y - r)
        res = 1;
    else if (length(uv - vec2(r, r)) < r + thickness)
        res = 1 - (length(uv - vec2(r, r)) - r);

    else if (length(uv - vec2(r, size.y - r)) < r + thickness)
        res = 1 - (length(uv - vec2(r, size.y - r)) - r);

    else if (length(uv - vec2(size.x - r, r)) < r + thickness)
        res = 1 - (length(uv - vec2(size.x - r, r)) - r);

    else if (length(uv - vec2(size.x - r, size.y - r)) < r + thickness)
        res = 1 - (length(uv - vec2(size.x - r, size.y - r)) - r);
    
    return max(min(res, 1), 0);
}

void main() {
    
    FragColor = vec4(color.rgb, color.a * roundedFrame(radius, 2));
}
            )~~"
        };
        static GLint dims2 = glGetUniformLocation(_shader2.ID, "dim");
        static GLint rdims2 = glGetUniformLocation(_shader2.ID, "rdim");
        static GLint radius2 = glGetUniformLocation(_shader2.ID, "radius");
        static GLint color2 = glGetUniformLocation(_shader2.ID, "color");

        if (!radius && rotation != 0)
        {
            if (m_PreviousShader != 6)
            {
                _shader.Use();
                glBindVertexArray(quad.vao);
            }
            m_PreviousShader = 6;

            glm::mat4 _model{ 1.0f };
            _model = glm::translate(_model, glm::vec3{ dim.x, dim.y, 0 });
            _model = glm::translate(_model, glm::vec3{ dim.z / 2, dim.w / 2, 0 });
            _model = glm::rotate(_model, (float)glm::radians(rotation), glm::vec3{ 0, 0, 1 });
            _model = glm::translate(_model, glm::vec3{ -dim.z / 2, -dim.w / 2, 0 });
            _model = glm::scale(_model, glm::vec3{ dim.z, dim.w, 1 });
            _shader.SetMat4(mvp, m_ViewProj * _model);
            _shader.SetVec4(color, m_Fill);
        }
        else
        {
            if (m_PreviousShader != 5)
            {
                _shader2.Use();
                glBindVertexArray(quad.vao);
            }
            m_PreviousShader = 5;

            glm::vec4 _dim;
            _dim.x = (dim.x + m_Matrix[3].x) * m_Projection[0].x + m_Projection[3].x;
            _dim.y = (dim.y + m_Matrix[3].y) * m_Projection[1].y + m_Projection[3].y;
            _dim.z = dim.z * m_Projection[0].x;
            _dim.w = dim.w * m_Projection[1].y;

            _shader2.SetVec4(dims2, _dim);
            _shader2.SetVec4(rdims2, dim);
            _shader2.SetFloat(radius2, rotation);
            _shader2.SetVec4(color2, m_Fill);
        }

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }


    void OpenGL::CreateTexturedQuadBuffers()
    {
        float _vertices[] = {
            0.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            0.0f, 1.0f, 0.0f, 1.0f
        };

        glGenVertexArrays(1, &textured.vao);
        glGenBuffers(1, &textured.vbo);

        glBindVertexArray(textured.vao);

        glBindBuffer(GL_ARRAY_BUFFER, textured.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }

    void OpenGL::TexturedQuad(Texture t, const glm::vec4& dim, float rotation, int radius)
    {
        static Shader _shader
        {
            // Vertex shader
            R"~~(
#version 450 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;
uniform mat4 mvp;
out vec2 texCoord;
void main() {
    gl_Position = mvp * vec4(aPos, 0.0, 1.0);
    texCoord = vec2(aTexCoord.x, aTexCoord.y);
}
            )~~",

            // Fragment shader
            R"~~(
#version 450 core
out vec4 FragColor;
uniform sampler2D theTexture;
in vec2 texCoord;
void main() {
    FragColor = texture(theTexture, texCoord).rgba;
}
            )~~"
        };
        static GLint mvp = glGetUniformLocation(_shader.ID, "mvp");
        static GLint texture = glGetUniformLocation(_shader.ID, "theTexture");
        static Shader _shader2
        {
            // Vertex shader
            R"~~(
#version 450 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec2 aTexCoord;
uniform vec4 dim;
uniform vec4 rdim;
out vec2 texCoord;
out vec2 uv;
out vec2 size;
void main() {
    gl_Position = vec4(dim.x + aPos.x * dim.z, dim.y + aPos.y * dim.w, 0.0, 1.0);
    texCoord = vec2(aTexCoord.x, aTexCoord.y);
    uv = vec2(aPos.x * rdim.z, aPos.y * rdim.w);
    size = vec2(rdim.z, rdim.w);
}
            )~~",

            // Fragment shader
            R"~~(
#version 450 core
out vec4 FragColor;
uniform sampler2D theTexture;
uniform float radius;
in vec2 texCoord;
in vec2 uv;
in vec2 size;

float roundedFrame (float r, float thickness)
{
    float res = 0;
    if (uv.x > r && uv.x < size.x - r
          || uv.y > r && uv.y < size.y - r)
        res = 1;
    else if (length(uv - vec2(r, r)) < r + thickness)
        res = 1 - (length(uv - vec2(r, r)) - r);

    else if (length(uv - vec2(r, size.y - r)) < r + thickness)
        res = 1 - (length(uv - vec2(r, size.y - r)) - r);

    else if (length(uv - vec2(size.x - r, r)) < r + thickness)
        res = 1 - (length(uv - vec2(size.x - r, r)) - r);

    else if (length(uv - vec2(size.x - r, size.y - r)) < r + thickness)
        res = 1 - (length(uv - vec2(size.x - r, size.y - r)) - r);
    
    return max(min(res, 1), 0);
}

void main() {
    vec4 clr = texture(theTexture, texCoord).rgba;
    FragColor = vec4(clr.rgb, clr.a * roundedFrame(radius, 2));
}
            )~~"
        };
        static GLint dims2 = glGetUniformLocation(_shader2.ID, "dim");
        static GLint rdims2 = glGetUniformLocation(_shader2.ID, "rdim");
        static GLint radius2 = glGetUniformLocation(_shader2.ID, "radius");
        static GLint texture2 = glGetUniformLocation(_shader2.ID, "theTexture");

        if (!radius && rotation != 0)
        {
            if (m_PreviousShader != 8)
            {
                _shader.Use();
                glBindVertexArray(textured.vao);
            }
            m_PreviousShader = 8;

            glm::mat4 _model{ 1.0f };
            _model = glm::translate(_model, glm::vec3{ dim.x, dim.y, 0 });
            _model = glm::translate(_model, glm::vec3{ dim.z / 2, dim.w / 2, 0 });
            _model = glm::rotate(_model, (float)glm::radians(rotation), glm::vec3{ 0, 0, 1 });
            _model = glm::translate(_model, glm::vec3{ -dim.z / 2, -dim.w / 2, 0 });
            _model = glm::scale(_model, glm::vec3{ dim.z, dim.w, 1 });
            _shader.SetMat4(mvp, m_ViewProj * _model);
            _shader.SetInt(texture, 1);
        }
        else
        {
            if (m_PreviousShader != 9)
            {
                _shader2.Use();
                glBindVertexArray(textured.vao);
            }
            m_PreviousShader = 9;

            glm::vec4 _dim;
            _dim.x = (dim.x + m_Matrix[3].x) * m_Projection[0].x + m_Projection[3].x;
            _dim.y = (dim.y + m_Matrix[3].y) * m_Projection[1].y + m_Projection[3].y;
            _dim.z = dim.z * m_Projection[0].x;
            _dim.w = dim.w * m_Projection[1].y;

            _shader2.SetVec4(dims2, _dim);
            _shader2.SetVec4(rdims2, dim);
            _shader2.SetFloat(radius2, rotation);
            _shader2.SetInt(texture2, 1);
        }

        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, t);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void OpenGL::CreateEllipseBuffers()
    {
        float _vertices[] = {
            -.5f, -.5f,
             .5f, -.5f,
            -.5f,  .5f,
             .5f, -.5f,
             .5f,  .5f,
            -.5f,  .5f
        };

        glGenVertexArrays(1, &ellipse.vao);
        glGenBuffers(1, &ellipse.vbo);

        glBindVertexArray(ellipse.vao);

        glBindBuffer(GL_ARRAY_BUFFER, ellipse.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void OpenGL::Ellipse(const glm::vec4& dim, const glm::vec2& a)
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
            "    else if (r > 1) { discard;  } "
            "    else if (r > 0.90) { FragColor = vec4(color.rgb, 10 * (1 - r) * color.a); } "
            "    else { FragColor = color; } "
            "} "
        };
        static GLint mvp = glGetUniformLocation(_shader.ID, "mvp");
        static GLint color = glGetUniformLocation(_shader.ID, "color");
        static GLint angles = glGetUniformLocation(_shader.ID, "angles");
        static GLint dimensions = glGetUniformLocation(_shader.ID, "dimensions");

        if (m_PreviousShader != 3)
        {
            _shader.Use();
            glBindVertexArray(ellipse.vao);
        }
        m_PreviousShader = 3;

        glm::mat4 _model{ 1.0f };
        _model = glm::translate(_model, glm::vec3{ dim.x, dim.y, 0 });
        _model = glm::scale(_model, glm::vec3{ dim.z, dim.w, 1 });

        _shader.SetMat4(mvp, m_ViewProj * _model);
        _shader.SetVec4(color, m_Fill);

        if (a.x == 0 && a.y == 0)
            _shader.SetVec2(angles, { 0, M_PI * 2 });
        else
            _shader.SetVec2(angles, { std::fmod(a.y + 4.0 * M_PI, 2.0 * M_PI), std::fmod(a.x + 4.0 * M_PI, 2.0 * M_PI) });

        glm::vec4 _dim{ (dim.x + m_Matrix[3].x) / m_Scaling, (dim.y + m_Matrix[3].y) / m_Scaling, dim.z / m_Scaling, dim.w / m_Scaling };
        _shader.SetVec4(dimensions, _dim);

        glDrawArrays(GL_TRIANGLES, 0, 6);
    }

    void OpenGL::CreateTriangleBuffers()
    {
        float _vertices[] = { -0.5f, -0.5f, 0.5f, 0.0f, -0.5f, 0.5f };

        glGenVertexArrays(1, &triangle.vao);
        glGenBuffers(1, &triangle.vbo);

        glBindVertexArray(triangle.vao);

        glBindBuffer(GL_ARRAY_BUFFER, triangle.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void OpenGL::Triangle(const glm::vec4& dim, float rotation)
    {
        static Shader _shader
        {
            "#version 330 core \n "
            "layout(location = 0) in vec2 aPos; "
            "uniform mat4 projection; "
            "uniform mat4 view; "
            "uniform mat4 model; "
            "void main() { "
            "    gl_Position = projection * view * model * vec4(aPos, 0.0, 1.0); "
            "}",
            "#version 330 core \n "
            "out vec4 FragColor; "
            "uniform vec4 color; "
            "void main() { "
            "    FragColor = color; "
            "} "
        };
        static GLint model = glGetUniformLocation(_shader.ID, "model");
        static GLint view = glGetUniformLocation(_shader.ID, "view");
        static GLint projection = glGetUniformLocation(_shader.ID, "projection");
        static GLint color = glGetUniformLocation(_shader.ID, "color");

        if (m_PreviousShader != 2)
        {
            _shader.Use();
            glBindVertexArray(triangle.vao);
        }
        m_PreviousShader = 2;

        glm::mat4 _model{ 1.0f };
        _model = glm::translate(_model, glm::vec3{ dim.x, dim.y + dim.w, 0 });
        _model = glm::scale(_model, glm::vec3{ dim.z, dim.w, 1 });
        if (rotation != 0) // Rotate only if necessary
            _model = glm::rotate(_model, glm::radians(rotation), glm::vec3{ 0, 0, 1 });

        _shader.SetMat4(model, _model);
        _shader.SetMat4(view, m_Matrix);
        _shader.SetMat4(projection, m_Projection);
        _shader.SetVec4(color, m_Fill);

        glDrawArrays(GL_TRIANGLES, 0, 3);
    }

    void OpenGL::CreateTextBuffers()
    {
        float _vertices[] = {
            0.0f, 0.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
            0.0f, 1.0f
        };

        glGenVertexArrays(1, &text.vao);
        glGenBuffers(1, &text.vbo);

        glBindVertexArray(text.vao);

        glBindBuffer(GL_ARRAY_BUFFER, text.vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(_vertices), _vertices, GL_STATIC_DRAW);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
    }

    void OpenGL::Text(std::string_view str, const glm::vec2& pos)
    {
        static Shader _shader
        {
            // Vertex shader
            "#version 330 core \n "
            "layout(location = 0) in vec2 aPos; "
            "uniform vec4 dim; "
            "out vec2 texpos; "
            "void main() { "
            "    gl_Position = vec4(dim.x + aPos.x * dim.z, dim.y + aPos.y * dim.w, 0.0, 1.0); "
            "    texpos = vec2(aPos.x, 1-aPos.y); "
            "}",

            // Fragment shader
            "#version 330 core                                                                                      \n"
            "                                                                                                       \n"
            "out vec4 col;                                                                                          \n"
            "                                                                                                       \n"
            "uniform vec4 color;                                                                                    \n"
            "uniform sampler2DArray Texture;                                                                        \n"
            "                                                                                                       \n"
            "uniform int theTexture;                                                                                \n"
            "in vec2 texpos;                                                                                        \n"
            "                                                                                                       \n"
            "void main() {                                                                                          \n"
            "    vec3 sampled = texture(Texture, vec3(texpos.x, texpos.y, theTexture)).rgb;                         \n"
            "    col.a = (sampled.r + sampled.g + sampled.b) / 3;                                                   \n"
            "    col.r = sampled.r * color.r;                     \n"
            "    col.g = sampled.g * color.g;                     \n"
            "    col.b = sampled.b * color.b;                     \n"
            "}",
        };

        static GLint color = glGetUniformLocation(_shader.ID, "color");
        static GLint texture = glGetUniformLocation(_shader.ID, "Texture");
        static GLint dims = glGetUniformLocation(_shader.ID, "dim");
        static GLint theTexture = glGetUniformLocation(_shader.ID, "theTexture");

        // If no charmap is loaded, return
        if (!m_CurrentFont)
            return;

        glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
        if (m_PreviousShader != 1)
        {
            _shader.Use();
            glBindVertexArray(text.vao);
        }
        m_PreviousShader = 1;

        // Load the charmap for the current text size
        GuiCode::Font::CharMap* _charMap = &m_CurrentFont->Size(std::round(m_FontSize));

        // Calculate the total width if we need it.
        //std::vector<float> _totalWidth{ 0.f };
        float _totalWidth = 0.0f;
        int _index = 0;
        if (m_TextAlign & Align::Right || m_TextAlign & Align::CenterX)
            for (int i = 0; i < str.size(); i++)
            {
                //if (str[i] == '\n')
                //    _index++, _totalWidth.push_back(0.f);
                //else
                //    _totalWidth[_index] += _charMap->Char(str[i]).advance >> 6;
                _totalWidth += _charMap->Char(str[i]).advance >> 6;
            }
        // Bind the 3d texture for this charmap
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D_ARRAY, _charMap->texture);
        _shader.SetVec4(color, m_Fill);
        _shader.SetInt(texture, 0); // We need to set the texture like this

        float _scale = m_FontSize / std::round(m_FontSize);

        float x = pos.x;
        float y = m_Size.height - pos.y;

        _index = 0;
        for (int i = 0; i < str.size(); i++)
        {
            char _c = str[i];

            if (_c == '\n' || i == 0)
            {
                x = pos.x;

                // Alignment
                if (m_TextAlign & Align::Middle)
                    y -= _charMap->Middle();
                else if (m_TextAlign & Align::TextBottom)
                    y -= _charMap->Descender();
                else if (m_TextAlign & Align::Baseline)
                    y;
                else
                    y -= _charMap->Ascender() + _charMap->Descender();

                if (m_TextAlign & Align::CenterX)
                    //    x -= 0.5 * _totalWidth[_index] * _scale;
                    x -= 0.5 * _totalWidth * _scale;
                else if (m_TextAlign & Align::Right)
                    //    x -= _totalWidth[_index] * _scale;
                    x -= _totalWidth * _scale;

                _index++;
                //if (x == '\n')
                //    y += m_LineHeight * m_FontSize;
            }

            Font::CharMap::Character& _ch = _charMap->Char(_c);

            if (_c != ' ' && _c != '\f' && _c != '\r' && _c != '\t' && _c != '\v' && _c != '\n')
            {
                float _xpos = x * m_Matrix[0][0] + _ch.bearing.x * _scale;
                float _ypos = y - (_ch.size.y - _ch.bearing.y) * _scale;

                _ypos = (int)_ypos,
                    _xpos = (int)_xpos;

                glm::vec4 _dim;
                _dim.x = (_xpos + m_Matrix[3].x) * m_Projection[0].x + m_Projection[3].x;
                _dim.y = (_ypos + m_Matrix[3].y) * m_Projection[1].y + m_Projection[3].y;
                _dim.z = m_FontSize * m_Projection[0].x;
                _dim.w = m_FontSize * m_Projection[1].y;

                _shader.SetVec4(dims, _dim);
                _shader.SetInt(theTexture, _c);

                glDrawArrays(GL_TRIANGLES, 0, 6);
            }

            x += (_ch.advance >> 6) * _scale;
        }

        glBindTexture(GL_TEXTURE_2D, 0);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }

    void OpenGL::Font(std::string_view font)
    {
        auto _it = m_Fonts.find(font);
        if (_it != m_Fonts.end())
            m_CurrentFont = &_it->second;
        else
        {
            if (LoadFont(std::string{ font }));
                m_CurrentFont = &m_Fonts.find(font)->second;
        }
    }

    void OpenGL::TextAlign(int align)
    {
        m_TextAlign = align;
    }
}