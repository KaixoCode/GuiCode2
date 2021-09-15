#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Component.hpp"

namespace GuiCode
{
	template<typename T>
	struct Parsers
	{
		static T Parse(const std::string&);
	};

	class TagParser
	{
	public:

		static std::map<std::string, int64_t> enumMap;
		
		class AttributeBase
		{
		public:
			virtual void Set(void*, const std::string&) = 0;
			virtual void Set(Component&, const std::string&) = 0;
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
					if constexpr (is_instance<A, Ref>::value)
						(obj->*member) = Parsers<A::Type>::Parse(val);

					else
						(obj->*member) = Parsers<A>::Parse(val);
					
				}
				catch (...) // If parsing failed, we catch and continue
				{ }
			}
		};

		struct Settings
		{
			std::string name;
		};

		TagParser(const Settings& s);

		template<typename T, typename A>
		void Attribute(const std::string& name, A T::* member)
		{
			auto _res = split(name, ".");
			auto _final = _res[_res.size() - 1];

			attributes.emplace(_final, std::make_unique<AttributeTyped<T, A>>(member));
		}

		void Set(Component& c, const std::string& name, const std::string& value);
		virtual Pointer<Component> Create() = 0;
		virtual void Append(Component&, Pointer<Component>&) = 0;

		Settings settings;
		std::map<std::string, std::unique_ptr<AttributeBase>> attributes;
	};

	class Parser
	{
	public:
		struct Scope
		{
			std::string name;
			std::map<std::string, std::string> attributes;

			std::vector<Scope> sub;

			Pointer<Component> Generate();
		};

		template<std::derived_from<TagParser> T>
		static inline void Link()
		{
			auto _ptr = std::make_unique<T>();
			m_Parsers.emplace(_ptr->settings.name, std::move(_ptr));
		}

		static Scope Parse(std::string_view s);
		static Scope ParseScope(std::string_view& s);
		static std::pair<std::string, std::string> ParseAttribute(std::string_view& s);

	private:
		static inline std::map<std::string, std::unique_ptr<TagParser>> m_Parsers;
	};
}