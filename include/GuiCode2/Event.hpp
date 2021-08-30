#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Utils.hpp"
namespace GuiCode
{
	class Component;

	template<typename T>
	std::pair<int, T> operator->*(int state, T lambda) { return { state, lambda }; }

	struct Event
	{
		virtual bool Forward(const Component&) const = 0;
	};

	class EventListener
	{
		struct EventFunctionBase
		{
			virtual void Call(const Event&) = 0;
		};

		template<typename, typename>
		struct EventFunction;
		template<typename Func, typename Type>
		struct EventFunction<Func, void(const Type&)> : EventFunctionBase
		{
			EventFunction(Func f)
				: fun(f)
			{}

			void Call(const Event& e) override
			{
				const Type* _e = dynamic_cast<const Type*>(&e);
				if (_e)
					fun(*_e);
			}

			Func fun;
		};

		struct StateFunctionBase
		{
			virtual void Call(const Event&, Component& c) = 0;
		};

		template<typename, typename>
		struct StateFunction;

		template<typename Func, typename Type, typename Obj>
		struct StateFunction<Func, int(const Type&, const Obj&)> : StateFunctionBase
		{
			StateFunction(const std::pair<int, Func>& a)
				: state(a.first), fun(a.second)
			{}

			void Call(const Event& e, Component& c) override
			{
				const Type* _e = dynamic_cast<const Type*>(&e);
				Obj* _c = dynamic_cast<Obj*>(&c);
				if (_e && _c)
					_c->State(state, fun(*_e, *_c));
			}

			int state;
			Func fun;
		};

	public:
		template<typename T>
		void operator+=(T t)
		{
			m_Listeners.push_back(std::make_unique<EventFunction<T, LambdaSignatureT<T>>>(t));
		}

		template<typename T>
		void operator+=(const std::pair<int, T>& t)
		{
			m_Handlers.push_back(std::make_unique<StateFunction<T, LambdaSignatureT<T>>>(t));
		}

		void operator()(const Event& e) const;

		void RegisterComponent(Component& c) { m_Components.push_back(&c); }
		void UnregisterComponent(Component& obj)
		{
			auto _it = std::find_if(m_Components.begin(), m_Components.end(), [&](auto& c) { return c == &obj; });
			if (_it != m_Components.end())
				m_Components.erase(_it);
		}

	private:
		std::vector<std::unique_ptr<EventFunctionBase>> m_Listeners;
		std::vector<std::unique_ptr<StateFunctionBase>> m_Handlers;
		std::vector<Component*> m_Components;
	};
}