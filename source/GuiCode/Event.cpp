#include "GuiCode2/Event.hpp"
#include "GuiCode2/Component.hpp"

namespace GuiCode
{
	void EventListener::operator()(const Event& e) const
	{
		// First call all event listeners
		for (auto& l : m_Listeners)
			l->Call(e);

		// Then go through all the states that this listener handles
		for (auto& [state, h] : m_StateHandlers)
		{
			int _amount = 0; // Key track of limit
			for (auto& l : h.handlers) // Go through handlers for this state
				for (auto& i : m_Components) // If over limit, set overflow = true
					if (i->State<Visible>())
						_amount += l->Call(e, *i, _amount >= h.settings.limit);
		}

		// Go through components and check if we should forward event.
		for (auto& i : m_Components)
			if (i->State<Visible>())
				if (e.Forward(*i)) i->listener(e);
	}
}