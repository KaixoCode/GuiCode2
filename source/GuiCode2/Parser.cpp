#include "GuiCode2/Parser.hpp"
#include "GuiCode2/Components/Component.hpp"
#include "GuiCode2/Components/Panel.hpp"

namespace GuiCode
{
	std::map<std::string, int64_t> TagParser::enumMap
	{
		{ "true", true },
		{ "false", false },
		{ "hovering", Hovering },
		{ "pressed", Pressed },
		{ "focused", Focused },
		{ "visible", Visible },
		{ "disabled", Disabled },
		{ "selected", Selected },
	};

	// Some default parsers
	template<>
	Enum Parsers<Enum>::Parse(std::string_view& c)
	{
		int a = c.size();
		for (auto& [name, val] : TagParser::enumMap)
			if (c.find(name) == 0 && (c.size() == name.size() || c.size() > name.size() && !(std::isalpha(c.at(name.size())) || c.at(name.size()) == '-' || c.at(name.size()) == '_')))
			{
				c = c.substr(name.size());
				return { true, val };
			}
		return { false };
	}

	template<>
	std::string_view Parsers<std::string_view>::Parse(std::string_view& c)
	{
		c = c.substr(c.find_first_not_of(" "));
		auto _find = c.find_first_of("\"");
		if (_find == std::string_view::npos)
			return c;

		c = c.substr(_find + 1);

		std::string_view _sub = c.substr(0, c.find_first_of("\"") - 1);
		c = c.substr(_sub.size());

		return _sub;
	};

	template<>
	std::string Parsers<std::string>::Parse(std::string_view& c)
	{
		return std::string{ Parsers<std::string_view>::Parse(c) };
	};

	template<> 
	double Parsers<double>::Parse(std::string_view& c)
	{ 
		c = c.substr(c.find_first_not_of(" "));
		const char* begin = c.data();
		char* end;
		auto res = std::strtod(c.data(), &end);
		
		// If res is 0, try enums
		if (res == 0)
		{
			auto parsed = Parsers<Enum>::Parse(c);
			if (parsed.success)
				return parsed.value;
		}
	
		c = { end, c.size() - (end - begin) };
		return res;
	};

	template<>
	float Parsers<float>::Parse(std::string_view& c)
	{
		c = c.substr(c.find_first_not_of(" "));
		const char* begin = c.data();
		char* end;
		auto res = std::strtof(c.data(), &end);

		// If res is 0, try enums
		if (res == 0)
		{
			auto parsed = Parsers<Enum>::Parse(c);
			if (parsed.success)
				return parsed.value;
		}

		c = { end, c.size() - (end - begin) };
		return res;
	};

	template<>
	int64_t Parsers<int64_t>::Parse(std::string_view& c)
	{
		c = c.substr(c.find_first_not_of(" "));
		const char* begin = c.data();
		char* end;
		auto res = std::strtoll(c.data(), &end, 10);

		// If res is 0, try enums
		if (res == 0)
		{
			auto parsed = Parsers<Enum>::Parse(c);
			if (parsed.success)
				return parsed.value;
		}

		c = { end, c.size() - (end - begin) };
		return res;
	};

	template<>
	int Parsers<int>::Parse(std::string_view& c)
	{
		return static_cast<int>(Parsers<int64_t>::Parse(c));
	};

	template<>
	uint8_t Parsers<uint8_t>::Parse(std::string_view& c)
	{
		return Parsers<int64_t>::Parse(c);
	};

	template<>
	State Parsers<State>::Parse(std::string_view& c)
	{
		return static_cast<State>(Parsers<int64_t>::Parse(c));
	};

	template<>
	Cursor Parsers<Cursor>::Parse(std::string_view& c)
	{
		return static_cast<Cursor>(Parsers<int64_t>::Parse(c));
	};

	template<>
	bool Parsers<bool>::Parse(std::string_view& c)
	{
		return Parsers<int64_t>::Parse(c);
	};

	template<>
	Color Parsers<Color>::Parse(std::string_view& c)
	{
		try { // try rgba
			auto [r, g, b, a] = Parsers<std::tuple<float, float, float, float>>::Parse(c);
			return { r, g, b, a };
		} catch(...) { }

		try { // try rgb
			auto [r, g, b] = Parsers<std::tuple<float, float, float>>::Parse(c);
			return { r, g, b };
		}
		catch (...) {}

		// Try hex
		c = c.substr(c.find_first_not_of(" "));
		const char* begin = c.data();
		char* end;
		auto res = std::strtol(c.data(), &end, 0);

		c = { end, c.size() - (end - begin) };
		return (int)res;
	};

	template<>
	StateColors Parsers<StateColors>::Parse(std::string_view& c)
	{
		StateColors colors;
		colors.base = Parsers<Color>::Parse(c);

		try
		{
			while (true)
			{
				auto[state, color] = Parsers<std::tuple<State, Color>>::Parse(c);
				colors.State(state) = color;
			}
		}
		catch (...) {}

		return colors;
	}

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

	void TagParser::SetState(Component& c, int state, const std::string& name, const std::string& value)
	{
		auto _res = split(name, ".");
		if (_res.size() == 0)
			return;

		if (_res.size() == 1)
		{
			if (attributes.contains(_res[0]))
				attributes[_res[0]]->SetState(c, state, value);
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
				attributes[_res[_res.size() - 1]]->SetState(_ptr, state, value);
		}
	}

	Pointer<Component> Parser::Scope::Generate()
	{
		if (Parser::m_Parsers.contains(name))
		{
			auto& _parser = Parser::m_Parsers[name];
			auto _obj = _parser->Create();

			for (auto& [name, value] : insertOrder)
			{
				std::string_view _view = name;
				std::string _name = std::string{ _view };
				auto _subattr = _view.find_first_of(":"); // If it has state specifier
				if (_subattr != std::string_view::npos)
				{
					try
					{
						_name = _view.substr(0, _subattr);
					
						// Get the enum
						_view = _view.substr(_subattr + 1);
						Enum _state = Parsers<Enum>::Parse(_view);
						if (!_state.success)
							continue;

						// Get the object
						_parser->SetState(_obj, _state.value, _name, value);
					}
					catch (...) {}
				}
				else
				{
					_parser->Set(_obj, _name, value);
				}
			}

			for (auto& i : sub)
			{
				// Use aliased name if exists
				if (_parser->alias.contains(i.name))
					i.name = _parser->alias[i.name];

				auto a = i.Generate();
				if (a)
					_parser->Append(_obj, std::move(a));
			}
			return _obj;
		}

		return {};
	}
		
	std::vector<Parser::Scope> Parser::Parse(std::string_view s)
	{
		std::vector<Parser::Scope> _scopes;

		auto _str = ExtractVariables(s);
		s = _str;

		try
		{
			while (true)
			{
				_scopes.push_back(ParseScope(s));
			}
		}
		catch (...) {}

		return _scopes;
	}

	std::string Parser::ExtractVariables(std::string_view& s)
	{
		std::vector<Variable> _variables;
		std::string _str = std::string{ s };
		try
		{
			while (true)
			{
				if (_variables.size())
				{
					auto& _prev = *(--_variables.end());
					_str = std::regex_replace(_str, std::regex{ _prev.name }, _prev.value);
				}
				std::string_view _view = _str;
				auto _var = ParseVariable(_view);
				_str = _view;
				_variables.emplace_back(_var);
			}
		}
		catch (...) {}

		return _str;
	}


	Parser::Variable Parser::ParseVariable(std::string_view& s)
	{
		// let apple = <button attr="val">;
		// let juice = <attr3="pizza">;
		// let carrot = { 45, 45, 0, 0 };
		auto _start = s.find("let ", 0, 4);
		if (_start == std::string_view::npos)
			throw nullptr;

		auto _end = s.find(";");
		auto _var = s.substr(_start + 4, _end - _start - 4);
		std::string _name = std::string{ _var.substr(0, _var.find_first_of("=")) };
		std::string _value = std::string{ _var.substr(_var.find_first_of("=") + 1) };
		s = s.substr(_end + 1);

		trim(_name);
		trim(_value);

		return { _name, _value };

		// <apple attr2="carrot" juice> -> <button attr="val" attr2="carrot" attr3="pizza">
	}

	Parser::Scope Parser::ParseScope(std::string_view& s)
	{
		s = s.substr(s.find_first_of("<") + 1);
		s = s.substr(s.find_first_not_of(" "));
		Scope scope;
		scope.name = s.substr(0, std::min({ s.find_first_of(" "), s.find_first_of(">"), s.find_first_of("/") }));

		s = s.substr(std::min({ s.find_first_of(" "), s.find_first_of(">"), s.find_first_of("/") }));
		s = s.substr(s.find_first_not_of(" "));

		while (s.find_first_not_of(">") < std::min(s.find_first_of(">"), s.find_first_of("/")))
			scope.Emplace(ParseAttribute(s));

		bool _singular = s.find_first_of(">") == s.find_first_of("/") + 1;

		s = s.substr(s.find_first_of(">") + 1);
		if (_singular)
			return scope;

		while (s.find_first_of("<") != s.find_first_of("/") - 1)
			scope.sub.emplace_back(ParseScope(s));

		s = s.substr(s.find_first_of(">") + 1);

		return scope;
	}

	std::pair<std::string, std::string> Parser::ParseAttribute(std::string_view& s)
	{
		std::string name, value;
		name = s.substr(0, s.find_first_of("="));
		trim(name);

		s = s.substr(s.find_first_of("=") + 1);
		s = s.substr(s.find_first_not_of(" "));
		if (s.find_first_of("=") < s.find_first_of(">"))
		{
			auto b = s.substr(0, s.find_first_of("="));
			b = b.substr(1, b.find_last_of("\"") - 1);
			s = s.substr(b.size() + 2);
			value = b;
		}
		else
		{
			auto b = s.substr(0, std::min(s.find_first_of(">"), s.find_first_of("/")));
			b = b.substr(1, b.find_last_not_of(" ") - 1);
			s = s.substr(b.size() + 2);
			s = s.substr(s.find_first_not_of(" "));
			value = b;
		}

		return { name, value };
	}


	ComponentParser::ComponentParser()
	{
		settings.name = "component";
		Attribute("z-index", &Component::zIndex);
		Attribute("cursor", &Component::cursor);
		Attribute("dimensions", &Component::dimensions);
		Attribute("size", &Component::size);
		Attribute("width", &Component::width);
		Attribute("height", &Component::height);
		Attribute("position", &Component::position);
		Attribute("x", &Component::x);
		Attribute("y", &Component::y);
		Attribute("min", &Component::min);
		Attribute("min.width", &Vec2<float>::width);
		Attribute("min.height", &Vec2<float>::height);
		Attribute("max", &Component::max);
		Attribute("max.width", &Vec2<float>::width);
		Attribute("max.height", &Vec2<float>::height);
	}

	Pointer<Component> ComponentParser::Create() { return new Component{ }; }

	void ComponentParser::Append(Component& c, Pointer<Component>&& obj)
	{
		c.components.push_back(std::move(obj));
	}
	
}