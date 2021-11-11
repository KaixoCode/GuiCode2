#include "GuiCode2/Events/Event.hpp"
#include "GuiCode2/Components/Component.hpp"

namespace GuiCode
{
	EventListener::EventListener(const EventListener& other)
		: components(other.components)
	{}

	EventListener::EventListener(EventListener&& other)
		: components(std::move(other.components))
	{}

	EventListener& EventListener::operator=(const EventListener& other)
	{
		components = other.components;
		return *this;
	}

	EventListener& EventListener::operator=(EventListener&& other)
	{
		components = std::move(other.components);
		return *this;
	}

	StateHandler& EventListener::StateHandler(const StateSettings& s)
	{
		if (m_StateHandlers.contains(s.state))
			return m_StateHandlers.at(s.state);

		return m_StateHandlers[s.state] = { s };
	}

	void EventListener::HandleEvent(const Event& e)
	{
		if (e.beforeStateHandler)
			for (auto& i : components)
				if (i->State(Visible))
					if (e.Forward(*i)) i->HandleEvent(e);

		// Then go through all the states that this listener handles
		for (auto& [state, h] : m_StateHandlers)
		{
			int _amount = 0; // Key track of limit
			for (auto& l : h.handlers) // Go through handlers for this state
				for (auto& i : components)
					if (i->State(Visible) || !h.settings.visible)
						_amount += l->Call(e, *i, h.settings.limit - _amount);
		}

		if (!e.beforeStateHandler)
			for (auto& i : components)
				if (i->State(Visible))
					if (e.Forward(*i)) i->HandleEvent(e);
		

		// Call all event listeners
		for (auto& l : m_Listeners)
			l->Call(e);
	}
}