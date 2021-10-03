#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Component.hpp"

namespace GuiCode
{
	struct Enum { bool success; int64_t value; };

	/**
	 * Direct string parsers, must be implemented for
	 * all types that can be parsed.
	 */
	template<typename T>
	struct Parsers
	{
		static T Parse(std::string_view&);
	};

	/**
	 * base class for a tag parser, can be extended to 
	 * parse any class.
	 */
	class TagParser
	{
	public:
		static std::map<std::string, int64_t> enumMap;

		// Name aliases that only work inside this tag
		std::map<std::string, std::string> alias; 
		
		class AttributeBase
		{
		public:
			virtual void Set(void*, const std::string&) = 0;
			virtual void SetState(void*, int, const std::string&) = 0;
			virtual void Set(Component&, const std::string&) = 0;
			virtual void SetState(Component&, int, const std::string&) = 0;
			virtual void* Get(void* c) = 0;
			virtual void* Get(Component& c) = 0;
		};

		template<typename T, typename A>
		class AttributeTyped : public AttributeBase
		{
		public:
			AttributeTyped(A T::* member)
				: member(member)
			{}

			void Set(Component& c, const std::string& a) override
			{
				T* _t = dynamic_cast<T*>(&c);

				// If correct types
				if (_t != nullptr)
				{
					Assign(_t, a);
				}
			}

			void Set(void* c, const std::string& a) override
			{
				if (c == nullptr)
					return;

				T* _t = reinterpret_cast<T*>(c);

				if (_t != nullptr)
				{
					Assign(_t, a);
				}
			}

			void SetState(Component& c, int state, const std::string& a) override
			{
				T* _t = dynamic_cast<T*>(&c);

				// If correct types
				if (_t != nullptr)
				{
					AssignState(_t, state, a);
				}
			}

			void SetState(void* c, int state, const std::string& a) override
			{
				if (c == nullptr)
					return;

				T* _t = reinterpret_cast<T*>(c);

				if (_t != nullptr)
				{
					AssignState(_t, state, a);
				}
			}

			void* Get(void* c) override
			{
				if (c == nullptr)
					return nullptr;

				T* _t = reinterpret_cast<T*>(c);
				if constexpr (is_instance<A, Ref>::value)
					return &((typename A::Type&)(_t->*member));
				else
					return &(_t->*member);
			}

			void* Get(Component& c) override
			{
				T* _t = dynamic_cast<T*>(&c);
				if (_t == nullptr)
					return nullptr;

				if constexpr (is_instance<A, Ref>::value)
					return &((typename A::Type&)(_t->*member));
				else
					return &(_t->*member);
			}

			A T::* member;

		private:
			void Assign(T* obj, const std::string& val)
			{
				try
				{
					std::string_view _view = val;
					if constexpr (is_instance<A, Ref>::value)
						(obj->*member) = Parsers<A::Type>::Parse(_view);

					else
						(obj->*member) = Parsers<A>::Parse(_view);
					
				}
				catch (...) // If parsing failed, we catch and continue
				{ }
			}

			void AssignState(T* obj, int state, const std::string& val)
			{
				try
				{
					std::string_view _view = val;
					if constexpr (is_instance<A, Ref>::value)
					{
						if constexpr (StateType<A::Type>)
							((typename A::Type&)(obj->*member)).State(state, Parsers<Color>::Parse(_view));
					}
					else
					{
						if constexpr (StateType<A>)
							(obj->*member).State(state, Parsers<Color>::Parse(_view));
					}
				}
				catch (...) // If parsing failed, we catch and continue
				{}
			}
		};

		struct Settings
		{
			std::string name;
		};

		TagParser(const Settings& s = {});

		/**
		 * Add an attribute to this tag.
		 * @param name name
		 * @param member pointer-to-member of the member that this attribute relates to
		 */
		template<typename T, typename A>
		void Attribute(const std::string& name, A T::* member)
		{
			auto _res = split(name, ".");
			auto _final = _res[_res.size() - 1];
			
			if (attributes.contains(_final))
				attributes.erase(_final); 

			attributes.emplace(_final, std::make_unique<AttributeTyped<T, A>>(member));
		}

		void Set(Component& c, const std::string& name, const std::string& value);
		void SetState(Component& c, int state, const std::string& name, const std::string& value);
		virtual Pointer<Component> Create() = 0;
		virtual void Append(Component&, Pointer<Component>&&) {};

		Settings settings;
		std::map<std::string, std::unique_ptr<AttributeBase>> attributes;
	};

	/**
	 * Parser will parse a string and spit out a pointer to the component it generated.
	 */
	class Parser
	{
	public:
		struct Scope
		{
			std::string name;
			std::map<std::string, std::string> attributes;
			std::vector<std::pair<std::string, std::string>> insertOrder;

			std::vector<Scope> sub;

			Pointer<Component> Generate();
			void Emplace(const std::pair<std::string, std::string>& p) { attributes.emplace(p), insertOrder.emplace_back(p); };
		};

		struct Variable
		{
			std::string name;
			std::string value;
		};

		template<std::derived_from<TagParser> T>
		static inline void Link()
		{
			auto _ptr = std::make_unique<T>();
			m_Parsers.emplace(_ptr->settings.name, std::move(_ptr));
		}

		static std::vector<Scope> Parse(std::string_view s);
		static std::string ExtractVariables(std::string_view& s);
		static Variable ParseVariable(std::string_view& s);
		static Scope ParseScope(std::string_view& s);
		static std::pair<std::string, std::string> ParseAttribute(std::string_view& s);

		template<typename T>
		static inline void Callback(const std::string& name, const T& f)
		{
			if constexpr (is_instance<T, Function>::value)
			{
				m_Callbacks.emplace(name, f.m_Storage);
				f.m_Storage->m_RefCount++;
			}
			else
			{
				Function a = f;
				m_Callbacks.emplace(name, a.m_Storage);
				a.m_Storage->m_RefCount++;
			}
		}

		template<typename ...Args>
		static inline void Call(const std::string& name, const std::string_view& other, Args&&...args)
		{
			std::string_view _other = other;
			if constexpr (sizeof...(Args) == 0)
			{
				Parser::m_Callbacks[name]->CallWithString(nullptr, 0, _other);
			}
			else
			{
				std::any _arr[]{ std::forward<Args>(args)... };
				Parser::m_Callbacks[name]->CallWithString(_arr, sizeof...(Args), _other);
			}
		}

	private:
		static inline std::map<std::string, std::unique_ptr<TagParser>> m_Parsers;
		static inline std::map<std::string, _FunctionStorageBase*> m_Callbacks;

		template<typename>
		friend class Parsers;
	};

	/**
	 * Direct Parsers standard types implementations.
	 */
	template<typename ...Args>
	struct Parsers<std::tuple<Args...>>
	{
		static std::tuple<Args...> Parse(std::string_view& c)
		{
			std::string_view _view{ c };
			auto _begin = _view.find_first_of("{");
			if (_begin == std::string_view::npos)
				_begin = 0;
			else
				_begin++;
			//auto _end = _view.find_last_of("}");
			//if (_end == std::string_view::npos)
			//	_end = _view.size();
			_view = _view.substr(_begin);
			int _beginSize = _view.size();
			std::tuple<Args...> tuple;

			auto a = [&]<size_t ...Is>(std::index_sequence<Is...>)
			{
				int _good = 2;
				((std::get<Is>(tuple) = Parsers<NthTypeOf<Is, Args...>>::Parse(_view),
					_view.find_first_of(",") > _view.find_first_of("}") && ((void)(_good--), true) ||
					(_view = _view.substr(_view.find_first_of(",") + 1), false))
					, ...);
				return _good > 0;
			};

			if (!a(std::make_index_sequence<sizeof...(Args)>{}))
				throw nullptr;

			c = c.substr(_begin + _beginSize - _view.size());
			auto end = c.find_first_of("}");
			if (end != std::string_view::npos)
				c = c.substr(end + 1);

			return tuple;
		}
	};

	template<typename T>
	struct Parsers<Vec4<T>>
	{
		static Vec4<T> Parse(std::string_view& c)
		{
			auto [r, g, b, a] = Parsers<std::tuple<T, T, T, T>>::Parse(c);
			return { r, g, b, a };
		}
	};

	template<typename T>
	struct Parsers<Vec3<T>>
	{
		static Vec3<T> Parse(std::string_view& c)
		{
			auto [r, g, b] = Parsers<std::tuple<T, T, T>>::Parse(c);
			return { r, g, b };
		}
	};

	template<typename T>
	struct Parsers<Vec2<T>>
	{
		static Vec2<T> Parse(std::string_view& c)
		{
			auto [r, g] = Parsers<std::tuple<T, T>>::Parse(c);
			return { r, g };
		}
	};

	template<typename Ret, typename ...Args>
	struct Parsers<Function<Ret(Args...)>>
	{
		static Function<Ret(Args...)> Parse(std::string_view& c)
		{
			std::string name = std::string{ c.substr(0, c.find_first_of("(")) };
			auto args = c.substr(c.find_first_of("(") + 1);
			std::string _res = std::string{ args.substr(0, args.find_first_of(")")) };

			return [=](Args... arg) -> Ret
			{ 
				std::any _arr[]{ arg... };
				std::string_view myArgs = _res;
				std::any _value = Parser::m_Callbacks[name]->CallWithString(_arr, sizeof...(Args), myArgs);
				if constexpr (std::is_same_v<Ret, void>)
					return;

				else
					return std::any_cast<Ret>(_value);
			};
		}
	};

	struct ComponentParser : public TagParser
	{
		ComponentParser();
		Pointer<Component> Create() override;
		void Append(Component& c, Pointer<Component>&& obj) override;
	};
}