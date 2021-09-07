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
		struct CharMap
		{
			struct Character
			{
				unsigned int index = static_cast<unsigned int>(-1);
				Vec2<int> size;
				Vec2<int> bearing;
				unsigned int advance;
			};

			CharMap(int size, FT_Face& face)
				: m_Size(size), m_Face(face)
			{
				Initialize();
			}

			Character& Char(char c)
			{
				return m_CharMap[c];
			}

			float Height() { return m_Ascender - m_Descender; }
			float Ascender() { return m_Ascender; }
			float Descender() { return m_Descender; }

			unsigned int texture;

		private:
			void Initialize()
			{
				FT_Set_Pixel_Sizes(m_Face, 0, m_Size);
				glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

				glGenTextures(1, &texture);
				glBindTexture(GL_TEXTURE_2D_ARRAY, texture);
				glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RED, m_Size, m_Size, 128, 0, GL_RED, GL_UNSIGNED_BYTE, 0);

				m_Ascender = m_Face->size->metrics.ascender / 64.;
				m_Descender = m_Face->size->metrics.descender / 64.;
				m_Height = m_Face->size->metrics.height / 64.;

				unsigned char* empty = new unsigned char[m_Size * m_Size];
				for (int i = 0; i < m_Size * m_Size; i++)
					empty[i] = 0;

				for (int _c = 0; _c < 128; _c++)
				{
					if (FT_Load_Char(m_Face, _c, FT_LOAD_RENDER))
					{
						LOG("ERROR::FREETYTPE: Failed to load Glyph");
						continue;
					}

					Character _character = {
						_c,
						{ m_Face->glyph->bitmap.width, m_Face->glyph->bitmap.rows },
						{ m_Face->glyph->bitmap_left, m_Face->glyph->bitmap_top },
						static_cast<unsigned int>(m_Face->glyph->advance.x)
					};

					int _xpos = _character.bearing.x;
					int _ypos = _character.size.y - _character.bearing.y;

					glTexSubImage3D(
						GL_TEXTURE_2D_ARRAY,
						0, 0, 0, _c,
						m_Size,
						m_Size,
						1, GL_RED,
						GL_UNSIGNED_BYTE,
						empty
					);

					glTexSubImage3D(
						GL_TEXTURE_2D_ARRAY,
						0, 0, m_Size - _character.size.y, _c,
						m_Face->glyph->bitmap.width,
						m_Face->glyph->bitmap.rows,
						1, GL_RED,
						GL_UNSIGNED_BYTE,
						m_Face->glyph->bitmap.buffer
					);

					m_CharMap[_c] = _character;
				}
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

				glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
				delete[] empty;
			}

			Character nullchar;
			Character m_CharMap[128];
			int m_Size;
			float m_Ascender;
			float m_Descender;
			float m_Height;
			FT_Face& m_Face;
		};

		Font(const std::string& path)
			: m_Path(path)
		{
			references++;
			if (!library)
				CHECK(FT_Init_FreeType(&library), "Failed to initialize FreeType2 library", return);

			CHECK(FT_New_Face(library, path.c_str(), 0, &m_Face), "Failed to open font file " << path, return);
		}

		Font(const Font& other)
			: m_Path(other.m_Path)
		{
			references++;
			CHECK(FT_New_Face(library, m_Path.c_str(), 0, &m_Face), "Failed to open font file " << m_Path, return);
		}

		~Font()
		{
			CHECK(FT_Done_Face(m_Face), "Failed to free font face");

			references--;
			if (references == 0)
				CHECK(FT_Done_FreeType(library), "Failed to free FreeType2 library");
		}

		CharMap& Size(int size)
		{
			return m_SizeMap.contains(size) 
				? m_SizeMap.find(size)->second
				: m_SizeMap.insert({ size, { size, m_Face } }).first->second;
		}


	private:
		std::string m_Path;
		FT_Face m_Face;

		std::map<int, CharMap> m_SizeMap;
	};
}