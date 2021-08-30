#include "GuiCode/ui/components/panels/TreePanel.hpp"
#include "GuiCode/ui/components/treeview/TreeItem.hpp"
#include "GuiCode/ui/components/treeview/TreeItemType.hpp"

// --------------------------------------------------------------------------
// ----------------------------- Tree Panel ---------------------------------
// --------------------------------------------------------------------------

TreePanel::TreePanel()
	: Panel()
{
	Layout<Layout::Stack>(0);
	AutoResize(false, true);
}

void TreePanel::SortItems(bool reverse)
{
	std::sort(m_Components.begin(), m_Components.end(), [=](std::unique_ptr<Component>& a, std::unique_ptr<Component>& b) -> bool
		{
			TreeItemBase* _tib1{ 0 };
			if ((_tib1 = dynamic_cast<TreeItemBase*>(a.get())) == nullptr)
				return false;

			TreeItemBase* _tib2{ 0 };
			if ((_tib2 = dynamic_cast<TreeItemBase*>(b.get())) == nullptr)
				return false;

			if (_tib1->Folder() && !_tib2->Folder())
				return true;

			if (!_tib1->Folder() && _tib2->Folder())
				return false;

			return reverse ? _tib1->Name() > _tib2->Name() : _tib1->Name() < _tib2->Name();
		}
	);
}