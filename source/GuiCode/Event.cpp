#include "GuiCode2/Event.hpp"
#include "GuiCode2/Component.hpp"
namespace GuiCode
{
	void EventListener::operator()(const Event& e) const
	{
		for (auto& l : m_Listeners)
			l->Call(e);

		for (auto& i : m_Components)
			for (auto& h : m_Handlers)
				h->Call(e, *i);

		for (auto& i : m_Components)
			if (e.Forward(*i)) i->listener(e);
	}
}