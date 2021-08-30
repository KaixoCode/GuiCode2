#pragma once
#include "GuiCode/ui/components/Component.hpp"
#include "GuiCode/ui/components/LayoutManager.hpp"
#include "GuiCode/ui/components/Layout.hpp"

class Container : public Component
{
public:
	Container(Vec2<int> size = { 40, 70 });

	/**
	 * Emplace a component to this container,
	 * Returns a reference to the emplaced component.
	 */
	template<typename T, typename ...Args>
	T& Emplace(Args&&... args) 
	{
		auto& _t = m_Components.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
		return *dynamic_cast<T*>(_t.get());
	}

	template<typename T, typename ...Args>
	T& Emplace(Layout::Hint h, Args&&... args)
	{
		auto& _t = m_Components.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
		_t->LayoutHint(h);
		return *dynamic_cast<T*>(_t.get());
	}

	int Cursor() const override { return m_Hovering ? m_Hovering->Cursor() : m_Cursor; }

	void Clear() { m_Components.clear(); m_Hovering = nullptr, m_Focussed = nullptr; }

	void Render(CommandCollection& d) override;
	void Update(const Vec4<int>& viewport) override;

protected:
	ComponentCollection m_Components;
	Component* m_Focussed = 0;
	Component* m_Hovering = 0;
	
private:
	/**
	 * Determines the hovering and focussed and sends 
	 * appropriate events to those components
	 */
	void Determine(Event& e);
};