#pragma once
#include "GuiCode2/Component.hpp"
#include "GuiCode2/BasicEvents.hpp"
#include "GuiCode2/Key.hpp"

namespace GuiCode
{
	/**
	 * Base for a button, does not contain any graphics.
	 */
	class Button : public Component
	{
		struct GroupBase
		{
			void Unselect() const;
			std::vector<Button*> group;
		};

	public:
		using Callback = std::function<void(bool)>;
		struct Group : private Pointer<GroupBase> { Group(); friend class Button; };

		enum Type
		{
			Click, Toggle, Hover, Radio
		};

		struct Settings
		{
			Group group;       // Only used for Radio buttons.
			Type type = Click; // Button behaviour.
			Callback callback; // Gets called whenever there is a change in state.
			Key combo;         // When set, this key combo will trigger the button behaviour.
		};

		Button(const Settings& settings = Settings{});
		Button(const Button& other);
		Button(Button&& other);

		~Button();

		Settings settings;

	private:
		void Init();
	};
}