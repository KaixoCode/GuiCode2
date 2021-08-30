#pragma once
#include "GuiCode/pch.hpp"
#include "GuiCode/ui/graphics/Command.hpp"

class CommandCollection
{
public:
	template<Graphics::Type T, typename ...Args>
	void Command(Args ...args)
	{
		m_CommandCollection.emplace_back(std::make_shared<Graphics::Command<T>>(std::forward<Args>(args)...));
	}

	const std::vector<std::shared_ptr<Graphics::CommandBase>>& Get() const
	{
		return m_CommandCollection;
	}

	void Clear()
	{
		m_CommandCollection.clear();
	}

	void Add(const std::shared_ptr<Graphics::CommandBase>& c)
	{
		m_CommandCollection.emplace_back(std::move(c));
	}

private:
	std::vector<std::shared_ptr<Graphics::CommandBase>> m_CommandCollection;
};