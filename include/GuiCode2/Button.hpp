#pragma once
#include "GuiCode2/Component.hpp"
#include "GuiCode2/BasicEvents.hpp"
#include "GuiCode2/Key.hpp"
#include "GuiCode2/Parser.hpp"

namespace GuiCode
{
	/**
	 * Parsers
	 */
	struct ButtonParser : public ComponentParser
	{
		ButtonParser();
		Pointer<Component> Create() override;
	};

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
		using Callback = Function<void(bool)>;
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
		Button(Button&& other);
		Button(const Button& other) = delete;
		~Button();
		Button& operator=(Button&& other);
		Button& operator=(const Button&) = delete;

		Settings settings;

	private:
		Ref<Group> m_Group = settings.group;
		Ref<Type> m_Type = settings.type;
		Ref<Callback> m_Callback = settings.callback;
		Ref<Key> m_Combo = settings.combo;

		void Init();
		friend class ButtonParser;
	};
}