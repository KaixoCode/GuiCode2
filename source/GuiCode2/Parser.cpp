#include "GuiCode2/Parser.hpp"
#include "GuiCode2/Component.hpp"
#include "GuiCode2/Panel.hpp"

namespace GuiCode
{
	std::map<std::string, int64_t> TagParser::enumMap;

	// Some default parsers
	template<> 
	double Parsers<double>::Parse(const std::string& c)
	{ 
		return std::strtod(c.c_str(), nullptr); 
	};

	template<>
	float Parsers<float>::Parse(const std::string& c)
	{
		return std::strtof(c.c_str(), nullptr);
	};

	template<>
	int Parsers<int>::Parse(const std::string& c)
	{
		return std::strtol(c.c_str(), nullptr, 10);
	};

	template<>
	Color Parsers<Color>::Parse(const std::string& c)
	{
		std::string_view _view{ c };
		auto _begin = _view.find_first_of("{");
		if (_begin == std::string_view::npos)
			_begin = 0;
		else
			_begin++;
		auto _end = _view.find_last_of("}");
		if (_end == std::string_view::npos)
			_end = _view.size();

		_view = _view.substr(_begin, _end - _begin);
		auto views = Split(_view, ',');
		Color res;
		if (views.size() == 0)
		{
			res = (int)std::strtol(c.c_str(), nullptr, 0);
			return res;
		}

		if (views.size() > 0) res.r = Parsers<float>::Parse(std::string{ views[0] });
		if (views.size() > 1) res.g = Parsers<float>::Parse(std::string{ views[1] });
		if (views.size() > 2) res.b = Parsers<float>::Parse(std::string{ views[2] });
		if (views.size() > 3) res.a = Parsers<float>::Parse(std::string{ views[3] });
		else res.a = 255;

		return res;
	};

	template<> 
	Vec4<float> Parsers<Vec4<float>>::Parse(const std::string& c)
	{
		std::string_view _view{ c };
		auto _begin = _view.find_first_of("{");
		if (_begin == std::string_view::npos)
			_begin = 0;
		else 
			_begin++;
		auto _end = _view.find_first_of("}");
		if (_end == std::string_view::npos)
			_end = _view.size();

		_view = _view.substr(_begin, _end - _begin);
		auto views = Split(_view, ',');
		Vec4<float> res{};
		if (views.size() > 0) res.r = Parsers<float>::Parse(std::string{ views[0] });
		if (views.size() > 1) res.g = Parsers<float>::Parse(std::string{ views[1] });
		if (views.size() > 2) res.b = Parsers<float>::Parse(std::string{ views[2] });
		if (views.size() > 3) res.a = Parsers<float>::Parse(std::string{ views[3] });

		return res;
	};

	template<>
	Vec3<float> Parsers<Vec3<float>>::Parse(const std::string& c)
	{
		auto views = Split(std::string_view{ c }, ',');
		Vec3<float> res{};
		if (views.size() > 0) res.r = Parsers<float>::Parse(std::string{ views[0] });
		if (views.size() > 1) res.g = Parsers<float>::Parse(std::string{ views[1] });
		if (views.size() > 2) res.b = Parsers<float>::Parse(std::string{ views[2] });

		return res;
	};

	template<>
	Vec2<float> Parsers<Vec2<float>>::Parse(const std::string& c)
	{
		auto views = Split(std::string_view{ c }, ',');
		Vec2<float> res{};
		if (views.size() > 0) res.r = Parsers<float>::Parse(std::string{ views[0] });
		if (views.size() > 1) res.g = Parsers<float>::Parse(std::string{ views[1] });

		return res;
	};

	TagParser::TagParser(const Settings& s)
		: settings(s)
	{}

	void TagParser::Set(Component& c, const std::string& name, const std::string& value)
	{
		auto _res = split(name, ".");
		if (_res.size() == 0)
			return;

		if (_res.size() == 1)
		{
			if (attributes.contains(_res[0]))
				attributes[_res[0]]->Set(c, value);
			return;
		}

		if (attributes.contains(_res[0]))
		{
			void* _ptr = attributes[_res[0]]->Get(c);
			for (int i = 1; i < _res.size() - 1; i++)
			{
				if (attributes.contains(_res[i]))
					_ptr = attributes[_res[i]]->Get(_ptr);
				else
					return;
			}
			if (attributes.contains(_res[_res.size() - 1]))
				attributes[_res[_res.size() - 1]]->Set(_ptr, value);
		}
	}

	Pointer<Component> Parser::Scope::Generate()
	{
		if (Parser::m_Parsers.contains(name))
		{
			auto& _parser = Parser::m_Parsers[name];
			auto _obj = _parser->Create();

			for (auto& [name, value] : attributes)
			{
				_parser->Set(_obj, name, value);
			}

			for (auto& i : sub)
			{
				Pointer<Component> _res = i.Generate();
				_parser->Append(_obj, _res);
			}

			return _obj;
		}

		return {};
	}
		
	Parser::Scope Parser::Parse(std::string_view s)
	{
		return ParseScope(s);
	}

	Parser::Scope Parser::ParseScope(std::string_view& s)
	{
		s = s.substr(s.find_first_of("<") + 1);
		s = s.substr(s.find_first_not_of(" "));
		Scope scope;
		scope.name = s.substr(0, std::min(s.find_first_of(" "), s.find_first_of(">")));

		s = s.substr(std::min(s.find_first_of(" "), s.find_first_of(">")));
		s = s.substr(s.find_first_not_of(" "));

		while (s.find_first_not_of(">") < s.find_first_of(">"))
			scope.attributes.emplace(ParseAttribute(s));

		s = s.substr(s.find_first_of(">") + 1);

		while (s.find_first_of("<") != s.find_first_of("/") - 1)
			scope.sub.emplace_back(ParseScope(s));

		s = s.substr(s.find_first_of(">") + 1);

		return scope;
	}

	std::pair<std::string, std::string> Parser::ParseAttribute(std::string_view& s)
	{
		std::string name, value;
		auto a = s.substr(0, s.find_first_of("="));
		name = a.substr(0, a.find_last_not_of(" ") + 1);

		s = s.substr(s.find_first_of("=") + 1);
		s = s.substr(s.find_first_not_of(" "));
		if (s.find_first_of("=") < s.find_first_of(">"))
		{
			auto b = s.substr(0, s.find_first_of("="));
			b = b.substr(0, b.find_last_not_of(" "));
			b = b.substr(0, b.find_last_of(" "));
			b = b.substr(1, b.find_last_not_of(" ") - 1);
			s = s.substr(b.size() + 2);
			s = s.substr(s.find_first_not_of(" "));
			value = b;
		}
		else
		{
			auto b = s.substr(0, s.find_first_of(">"));
			b = b.substr(1, b.find_last_not_of(" ") - 1);
			s = s.substr(b.size() + 2);
			s = s.substr(s.find_first_not_of(" "));
			value = b;
		}

		return { name, value };
	}
}