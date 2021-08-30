#pragma once
#include "GuiCode/ui/components/panels/TreePanel.hpp"
#include "GuiCode/ui/components/treeview/TreeItem.hpp"
#include "GuiCode/ui/graphics/Graphics.hpp"

// --------------------------------------------------------------------------
// --------------------------- TreeItem Type --------------------------------
// --------------------------------------------------------------------------

namespace TreeItemType
{
    // --------------------------------------------------------------------------
    // ------------------------------- Normal -----------------------------------
    // --------------------------------------------------------------------------
    // ---------------------- Normal treeitem behaviour -------------------------
    // --------------------------------------------------------------------------

    class Normal : public TreeItemBase     
    {
    public:
        Normal(const std::filesystem::path &path, Vec2<int> size = { 250, 22 });
    };

    // --------------------------------------------------------------------------
    // ------------------------------- Folder -----------------------------------
    // --------------------------------------------------------------------------
    // ------------------------ Expandable tree item ----------------------------
    // --------------------------------------------------------------------------

    class Folder : public TreeItemBase
    {
    public:
        Folder(const std::filesystem::path& path, Vec2<int> size = { 250, 22 });

        auto Size(const Vec2<int>& size) -> void override { m_Size = size; m_ActualHeight = size.height; }
        auto Width(int w)                -> void override { m_Size = { w, m_Size.height }; }
        auto Height(int h)               -> void override { m_Size = { m_Size.width, h }; m_ActualHeight = h; }

        Vec2<int> Size()   const override { return m_Size; }
        int       Height() const override { return m_Size.height; }
        int       Width()  const override { return m_Size.width; }
        int       ActualHeight()    const { return m_ActualHeight; }
        bool      Expanded()        const { return m_Expanded; }

        template<typename T, typename ...Args>
        T& Emplace(Args&&... args)
        {
            return m_TreePanel->Emplace<T>(std::forward<Args>(args)...);
        }

        void Update(const Vec4<int>& viewport) override;

    private:
        TreePanel* m_TreePanel;

        bool m_Expanded = false,
            m_Expanding = false;
        
        int m_ActualHeight;        

        bool ActuallyWithinBounds(const Vec2<int>& pos) const
        {
            return pos.x >= Position().x && pos.x <= Position().x + Size().width
                && pos.y > Position().y + Size().height - m_ActualHeight && pos.y <= Position().y + Size().height;
        }
    };
}
