#include "GuiCode/ui/components/treeview/TreeItemType.hpp"
#include "GuiCode/ui/components/treeview/TreeItemGraphics.hpp"

// --------------------------------------------------------------------------
// --------------------------- TreeItem Type --------------------------------
// --------------------------------------------------------------------------

namespace TreeItemType 
{
    // --------------------------------------------------------------------------
    // ------------------------------- Normal -----------------------------------
    // --------------------------------------------------------------------------

    Normal::Normal(const std::filesystem::path& path, Vec2<int> size)
        : TreeItemBase(path, size)
    {
        m_Listener += [this](Event::MouseExited& event)
        {
            m_Hovering = false;
        };

        m_Listener += [this](Event::MouseEntered& event)
        {
            m_Hovering = true;
        };
    }

    // --------------------------------------------------------------------------
    // ------------------------------- Folder -----------------------------------
    // --------------------------------------------------------------------------

    Folder::Folder(const std::filesystem::path &path, Vec2<int> size)
        : TreeItemBase(path, size), m_TreePanel(&Container::Emplace<TreePanel>())
    {
        m_Folder = true;
        m_ActualHeight = size.height;
        m_TreePanel->Height(0);

        m_Listener += [this](Event::MouseExited& event)
        {
            m_Hovering = false;
        };

        m_Listener += [this](Event::MouseMoved& event)
        {
            m_Hovering = ActuallyWithinBounds({ event.x, event.y });
        };

        m_Listener += [this](Event::MousePressed& event)
        {
            if (ActuallyWithinBounds({ event.x, event.y }) && event.button == Event::MouseButton::LEFT)
                m_Expanding = m_Expanded ^= true;
        };
    }

    void Folder::Update(const Vec4<int>& viewport)
    {
        // Update the position and width of the child treepanel.
        int _padding = 20;
        m_TreePanel->Position({ X() + _padding, Y() });
        m_TreePanel->Width(Width() - _padding);

        // If the folder is expanding, read the path and load all the files.
        if (m_Expanding)
        {
            m_Expanding = false;

            // First clear the existing items in the panel and then read the directory
            m_TreePanel->Clear();
            for (const auto& entry : std::filesystem::directory_iterator(m_Path))
                if (entry.is_directory())
                    Emplace<TreeItem<TreeItemGraphics::Normal, TreeItemType::Folder>>(entry.path());

                else if (entry.is_regular_file())
                    Emplace<TreeItem<TreeItemGraphics::Normal, TreeItemType::Normal>>(entry.path());
            
            // Make sure to sort the items
            m_TreePanel->SortItems(false);
        }

        // The tree panel is only visible if the folder is expanded
        m_TreePanel->Visible(Expanded());
        
        // Update the container before setting the size IMPORTANT!
        // otherwise the size is too up to date for the parent folders
        // and the content jumps up and the next frame the parent will
        // adjust, making the content jump back down to the right place.
        Container::Update(viewport);
        
        // Set the right size depending on if the folder is expanded.
        if (Expanded())
            m_Size = { m_Size.width, m_ActualHeight + m_TreePanel->Height() };
        
        else
            m_Size = { m_Size.width, m_ActualHeight };
    }
}