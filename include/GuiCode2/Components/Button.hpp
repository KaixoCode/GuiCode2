#pragma once
#include "GuiCode2/Components/Component.hpp"
#include "GuiCode2/Events/BasicEvents.hpp"
#include "GuiCode2/Events/Key.hpp"
#include "GuiCode2/Utils/StateColors.hpp"
#include "GuiCode2/Utils/Utils.hpp"

namespace GuiCode
{
	class Button : public Component
	{
		struct GroupBase
		{
			void Unselect() const;
			std::vector<Button*> group;
		};

	public:
		using Callback = Function<void(bool)>;

		struct Graphics 
		{
			Button* button;
			virtual void Render(CommandCollection&) const {};
			virtual void Update() {};
			virtual void Link(Button* button) {};
		};

		struct Group : public Pointer<GroupBase> 
		{ 
			Group(); 
			Group(const Group&);
			Group(Group&&);
			Group& operator=(const Group& other);
			Group& operator=(Group&& other);
			
			void Link(Button* button);
			void Unlink(Button* button);

		private:
			Button* me = nullptr;
			friend class Button; 
		};

		enum Type { Click, Toggle, Hover, Radio };

		struct Settings
		{
			Group group;       // Only used for Radio buttons.
			Type type = Click; // Button behaviour.
			Callback callback; // Gets called whenever there is a change in state.
			Key combo;         // When set, this key combo will trigger the button behaviour.
			std::string name;  // Name of the button
			Pointer<Graphics> graphics; // Graphics for this button
		} settings;

		Button(const Settings& settings = Settings{});
		Button(const Button&);
		Button(Button&&);
		~Button();
		Button& operator=(const Button&);
		Button& operator=(Button&&);

		void Render(CommandCollection& d) const override;
		void Update() override;

	private:
		void InitListeners();
	};
}