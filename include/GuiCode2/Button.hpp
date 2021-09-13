#pragma once
#include "GuiCode2/Component.hpp"
#include "GuiCode2/BasicEvents.hpp"
#include "GuiCode2/Key.hpp"

namespace GuiCode
{
	class Button : public Component
	{
		struct GroupBase
		{
			void Unselect();

			std::vector<Button*> group;
		};

		struct GroupVal : Pointer<GroupBase>
		{
			GroupVal()
				: Pointer<GroupBase>(GroupBase{})
			{}
		};

	public:
		using Callback = std::function<void(bool)>;

		using Group = GroupVal;

		enum Type
		{
			Click, Toggle, Hover, Radio
		};

		struct Settings
		{
			Group group;
			Type type = Click;
			Callback callback;
			Key combo;
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