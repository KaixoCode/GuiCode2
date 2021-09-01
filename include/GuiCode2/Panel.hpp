#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Component.hpp"
#include "GuiCode2/BasicEvents.hpp"
#include "GuiCode2/Layout.hpp"

namespace GuiCode
{
	/**
	 * A Panel object is made to contain Components, it has StateHandlers for the 
	 * Hovering, Focused, and Pressed states. It also contains a Span for layouting.
	 */
	class Panel : public Component
	{
	public:
		Panel();

		/**
		 * Emplace a Component to this panel's storage, registers it for events.
		 * @tparam Type derived from Component
		 * @tparam ...Args arguments for the constructor of Type
		 * @param ...args arguments for the constructor of Type
		 * @return reference to created Type object
		 */
		template<std::derived_from<Component> Type, typename ...Args>
		Type& Emplace(Args&&...args)
		{
			auto& _component = m_Components.emplace_back(std::make_unique<Type>(std::forward<Args>(args)...));
			RegisterComponent(*_component);
			return *static_cast<Type*>(_component.get());
		}

		/**
		 * Remove object from this panel's storage using a reference.
		 * @tparam Type derived from Component
		 * @param obj object to erase from storage
		 * @return true if successful
		 */
		template<std::derived_from<Component> Type>
		bool Erase(Type& obj)
		{
			auto _it = std::find_if(m_Components.begin(), m_Components.end(), [&](auto& c) { return c.get() == &obj; });
			if (_it != m_Components.end())
			{
				UnregisterComponent(obj);
				m_Components.erase(_it);
				return true;
			}
			return false;
		}

		Span span;

		void ForwardUpdate() override;
		void ForwardRender(CommandCollection& d) override;

	private:
		std::vector<std::unique_ptr<Component>> m_Components;
	};
}