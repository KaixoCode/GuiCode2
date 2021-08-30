#pragma once
#include "GuiCode/pch.hpp"

namespace Graphics
{
	enum Type
	{
		Fill, Stroke, StrokeWeight,

		FontSize, Font, TextAlign,

		Clip, ClearClip, Translate, PushMatrix, PopMatrix, Scale, Viewport,

		Text, Quad, TexturedQuad, Ellipse, Triangle
	};

	struct CommandBase
	{
		bool operator ==(const CommandBase& b)
		{
			if (b.type != type)
				return false;

			switch (type)
			{
			case Quad: return dimension == b.dimension;
			case Fill: return fill == b.fill;
			case Stroke: return stroke == b.stroke;
			}
		}

		Type type;

		CommandBase() {}

		// Stroke Weight
		CommandBase(float w) : weight(w) {}
		
		// Translate
		CommandBase(const Vec2<int>& t) : translate(t) {}

		// Stroke/Fill
		CommandBase(float r, float g, float b, float a = 1.0f) : fill({ r, g, b, a }) {}
		CommandBase(const Color& c) : fill(c) {}

		// Text
		CommandBase(const std::string* t, const Vec2<int>& p) : text(t), position(p) {}
		CommandBase(const std::string* t, int x, int y) : text(t), position({ x, y }) {}
		CommandBase(int t, float s) : font(t), fontSize(s) {}

		// Quad/Ellipse
		CommandBase(int x, int y, int w, int h) : dimension({ x, y, w, h }) {}
		CommandBase(const Vec4<int>& d) : dimension(d) {}
		CommandBase(const Vec2<int>& d1, const Vec2<int>& d2) : dimension({ d1.x, d1.y, d2.x, d2.y }) {}

		// Textured Quad
		CommandBase(int t, int x, int y, int w, int h) : texture(t), textureDimension({ x, y, w, h }) {}
		CommandBase(int t, const Vec4<int>& d) : texture(t), textureDimension(d) {}

		// Triangle
		CommandBase(int x, int y, int w, int h, float r) : dimension({ x, y, w, h }), rotation(r) {}
		CommandBase(const Vec4<int>& d, float r) : dimension(d), rotation(r) {}

		// TextAlign
		CommandBase(const Vec2<Align>& a) : align(a) {}
		CommandBase(Align a, Align b) : align({ a, b }) {}

		~CommandBase()
		{}

		union
		{
			// Fill/Stroke
			union { Color fill, stroke; };

			struct
			{
				int font;
				float fontSize;
			};

			struct // Text
			{
				const std::string* text;
				Vec2<int> position;
			};

			struct // Quad/Ellipse/Triangle
			{
				Vec4<int> dimension;
				float rotation;
			};

			struct
			{
				Vec4<int> textureDimension;
				unsigned int texture;
			};

			struct
			{
				union { Vec2<int> translate, scale; };
			};

			// Strokeweight
			union { float weight, fontSize; };

			// TextAlign
			Vec2<Align> align;
		};
	};

	template<Type T>
	struct Command : public CommandBase
	{
		template<typename ...Args>
		Command(Args ...args)
			: CommandBase(args...)
		{
			type = T;
		};
	};
}
