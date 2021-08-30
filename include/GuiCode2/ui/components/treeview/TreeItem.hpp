#pragma once
#include "GuiCode/ui/components/panels/Panel.hpp"

// --------------------------------------------------------------------------
// ----------------------------- TreeItem Base ------------------------------
// --------------------------------------------------------------------------

class TreeItemBase : public Container
{
public:
	TreeItemBase(const std::filesystem::path& path, Vec2<int> size = { 250, 22 })
		: m_Path(path), m_Name(path.filename().string()), 
		m_Extension(m_Path.extension().string()), Container(size)
	{}

	bool Folder()                       const { return m_Folder; }
	virtual bool Hovering()             const { return m_Hovering; }
	const std::string& Name()           const { return m_Name; }
	const std::string& Extension()      const { return m_Extension; }
	const std::filesystem::path& Path() const { return m_Path; }

protected:
	std::filesystem::path m_Path;
	std::string m_Name;
	std::string m_Extension;

	bool m_Enabled = true,
		m_Hovering = false,
		m_Folder = false;
};

// --------------------------------------------------------------------------
// -------------------------------- TreeItem --------------------------------
// --------------------------------------------------------------------------

template<typename TreeItemGraphics, typename TreeItemType = TreeItemBase>
struct TreeItem : public TreeItemType
{
	using TreeItemType::TreeItemType;

	void Render(CommandCollection& d) override
	{
		TreeItemType::Render(d);
		TreeItemGraphics::Render(*this, d);
	};
};