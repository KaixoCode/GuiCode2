#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Shader.hpp"

/**
 * 
 * Draw text:
 *  - Random point within bounding box, for all triangles same point
 *  - Draw triangles from point to all points in bounding box.
 * 
 * 
 * 
 * 
 * 
 * 
 */

namespace GuiCode
{
#define CHECK(x, msg, then) if (auto error = x) { std::cout << msg << std::endl; then; }


	class Font
	{
		static inline FT_Library library;
		static inline int references = 0;
	public:

		Font(const std::string& path)
			: m_Path(path)
		{
			// Init library if not initialized
			references++;
			if (!library)
				CHECK(FT_Init_FreeType(&library), "Failed to Initialize FreeType2 Library", return);

			CHECK(FT_New_Face(library, path.c_str(), 0, &m_Face), "Failed to Open Font File " << path, return);

			FT_Set_Char_Size(m_Face, 0, 50 << 6, 150, 150);
			FT_Load_Char(m_Face, 'e', FT_LOAD_NO_BITMAP);

			auto& _outline = m_Face->glyph->outline;

			FT_BBox _cbox;
			FT_Outline_Get_CBox(&_outline, &_cbox);

			m_Face->glyph->advance;

			// For each point there's 2 floats, and each point will be in there twice + 0,0
			int _points = _outline.n_points;
			int _sizeof = _points * 6;
			size = _points * 3;
			std::vector<float> _vertices;

			int _index = 0;
			for (int i = 0; i < _points; i++)
			{
				auto _tag = _outline.tags[i];

				//if ((_outline.tags[i] & 0b01) == 0b00)
				//	continue;

				auto _index1 = (i) % _outline.n_points;
				auto _index2 = (i + 1) % _outline.n_points;

				auto& _point1 = _outline.points[_index1];
				auto& _point2 = _outline.points[_index2];
				
				_vertices.push_back(0);
				_vertices.push_back(0);
				_vertices.push_back((_point1.x - _cbox.xMin) / (float)(_cbox.xMax - _cbox.xMin));
				_vertices.push_back((_point1.y - _cbox.yMin) / (float)(_cbox.yMax - _cbox.yMin));
				_vertices.push_back((_point2.x - _cbox.xMin) / (float)(_cbox.xMax - _cbox.xMin));
				_vertices.push_back((_point2.y - _cbox.yMin) / (float)(_cbox.yMax - _cbox.yMin));

				std::cout << 0 << ", " << 0 << std::endl;
				std::cout << (_point1.x - _cbox.xMin) / (float)(_cbox.xMax - _cbox.xMin) << ", " << (_point1.y - _cbox.yMin) / (float)(_cbox.yMax - _cbox.yMin) << std::endl;
				std::cout << (_point2.x - _cbox.xMin) / (float)(_cbox.xMax - _cbox.xMin) << ", " << (_point2.y - _cbox.yMin) / (float)(_cbox.yMax - _cbox.yMin) << std::endl;
			}

			glGenVertexArrays(1, &vao);
			glGenBuffers(1, &vbo);

			glBindVertexArray(vao);

			glBindBuffer(GL_ARRAY_BUFFER, vbo);
			glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(float), (void*)_vertices.data(), GL_STATIC_DRAW);

			glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
			glEnableVertexAttribArray(0);
		}

		~Font()
		{
			FT_Done_Face(m_Face);

			references--;
			if (references == 0)
				FT_Done_FreeType(library);
		}

		void Render(const glm::mat4& m_Matrix, const glm::mat4& m_Projection)
		{
			static Shader _shader
			{
				// Vertex shader
				"#version 450 core \n "
				"layout(location = 0) in vec2 aPos; "
				"uniform vec4 dim; "
				"void main() { "
				"    gl_Position = vec4(dim.x + aPos.x * dim.z, dim.y + aPos.y * dim.w, 0.0, 1.0); "
				"}",

				// Fragment shader
				"#version 450 core \n "
				"out vec4 FragColor; "
				"void main() { "
				"    FragColor = vec4(1,1,1,1); "
				"} "
			};
			static GLint dims = glGetUniformLocation(_shader.ID, "dim");
			
			glm::vec4 dim = { 0, 0, m_Face->glyph->metrics.width * 0.05, m_Face->glyph->metrics.height * 0.05 };

			glm::vec4 _dim;
			_dim.x = (dim.x + m_Matrix[3].x) * m_Projection[0].x + m_Projection[3].x;
			_dim.y = (dim.y + m_Matrix[3].y) * m_Projection[1].y + m_Projection[3].y;
			_dim.z = dim.z * m_Projection[0].x;
			_dim.w = dim.w * m_Projection[1].y;

			_shader.Use();
			_shader.SetVec4(dims, _dim);

			glEnable(GL_BLEND);
			glBlendFunc(GL_ONE_MINUS_DST_COLOR, GL_ZERO);
			glBindVertexArray(vao);
			glDrawArrays(GL_TRIANGLES, 0, size);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}



	private:
		std::string m_Path;
		FT_Face m_Face;

		int size;
	
		unsigned int vbo;
		unsigned int vao;
	
	};
}