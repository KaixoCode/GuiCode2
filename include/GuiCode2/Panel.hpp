#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Component.hpp"
#include "GuiCode2/BasicEvents.hpp"
#include "GuiCode2/Layout.hpp"

namespace GuiCode
{
	class Panel : public Component
	{
	public:
		Panel();

		template<std::derived_from<Component> Type, typename ...Args>
		Type& Emplace(Args&&...args)
		{
			auto& _component = m_Components.emplace_back(std::make_unique<Type>(std::forward<Args>(args)...));
			listener.RegisterComponent(*_component);
			return *static_cast<Type*>(_component.get());
		}

		template<std::derived_from<Component> Type>
		bool Erase(Type& obj)
		{
			auto _it = std::find_if(m_Components.begin(), m_Components.end(), [&](auto& c) { return c.get() == &obj; });
			if (_it != m_Components.end())
			{
				listener.UnregisterComponent(obj);
				m_Components.erase(_it);
				return true;
			}
			return false;
		}

		Div div;

	private:
		std::vector<std::unique_ptr<Component>> m_Components;
	};
}