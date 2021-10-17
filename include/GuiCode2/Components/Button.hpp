#pragma once
#include "GuiCode2/Components/Component.hpp"
#include "GuiCode2/Events/BasicEvents.hpp"
#include "GuiCode2/Events/Key.hpp"
#include "GuiCode2/Parser.hpp"
#include "GuiCode2/Utils/StateColors.hpp"
#include "GuiCode2/Utils/Utils.hpp"

namespace GuiCode
{
	/**
	 * Base for a button, does not contain any graphics.
	 */
	class ButtonParser;
	class Button : public Component
	{
		struct GroupBase
		{
			void Unselect() const;
			std::vector<Button*> group;
		};

	public:
		using Callback = Function<void(bool)>;
		struct Group : public Pointer<GroupBase> 
		{ 
			Group(); 
			Group& operator=(const Group& other);
			
		private:
			Button* me = nullptr;
			friend class Button; 
		};

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

	class NormalButton : public Button
	{
	public:
		struct Settings
		{
			Button::Group group;
			Button::Type type = Click;
			Button::Callback callback;
			Key combo;

			std::string name = "Button";

			StateColors color{ {
				.base{ 38, 38, 38, 0 },
				.colors{
					{ Disabled, { 0, 0, 0, 0 } },
					{ Pressed, 0x414141 },
					{ Hovering, 0x262626 },
				},
				.transition = 100
			} };

			struct Border
			{
				float width = 1;
				StateColors color{ {
					.base{ 118, 118, 118, 0 },
					.colors{
						{ Disabled, { 0, 0, 0, 0 } },
						{ Pressed, 0x919191 },
						{ Hovering, 0x767676 },
					},
					.transition = 100
				} };
			} border;

			struct
			{
				float size = 12;
				StateColors color{ {
					.base = 0xE0E0E0,
					.colors{
						{ Disabled, 0xA0A0A0 }
					}
				} };
			} text;

			std::string font = GraphicsBase::DefaultFont;

			void Link(Component* c) { color.Link(c), border.color.Link(c), text.color.Link(c); }
		};

		NormalButton(const Settings& settings = { });
		NormalButton(NormalButton&& other);
		NormalButton(const NormalButton& other);
		NormalButton& operator=(NormalButton&& other);

		void Update() override;
		void Render(CommandCollection& d) const override;

		Settings settings;

	private:
		void Init();

		Ref<std::string> m_Name = settings.name;
		Ref<StateColors> m_Color = settings.color;
		Ref<Settings::Border> m_Border = settings.border;
		Ref<float> m_BorderWidth = settings.border.width;
		Ref<StateColors> m_BorderColor = settings.border.color;
		Ref<float> m_TextSize = settings.text.size;
		Ref<StateColors> m_TextColor = settings.text.color;
		Ref<std::string> m_Font = settings.font;
		friend class NormalButtonParser;
	};

	/**
	 * Parsers
	 */
	struct ButtonParser : public ComponentParser
	{
		static inline std::map<std::string, Button::Group> buttonGroupMap;
		ButtonParser();
		Pointer<Component> Create() override;
	};

	struct NormalButtonParser : public ButtonParser
	{
		NormalButtonParser();
		Pointer<Component> Create() override;
	};
}