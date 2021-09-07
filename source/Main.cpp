#include "GuiCode2/pch.hpp"

#include "GuiCode2/Component.hpp"
#include "GuiCode2/BasicEvents.hpp"
#include "GuiCode2/Panel.hpp"
#include "GuiCode2/Frame.hpp"
#include "GuiCode2/Font.hpp"
#include "GuiCode2/TextDisplayer.hpp"
#include "GuiCode2/Key.hpp"
#include "GuiCode2/TextArea.hpp"
#include "GuiCode2/TextBox.hpp"

using namespace GuiCode;

/*
 * aspects:
 *  - Events
 *  - Layouts
 *  - Scrolling
 *  - Graphics
 *  - Windows
 * 
 *  - Need to know which object the mouse is over.
 *  - Need to know the viewport of each object.
 * 
 * Your object -> Register for events in other object.
 * 
 * ideas:
 *  - ability to add own events
 * 
 * 
 * 
 * Layout
 *  
 * 
 * 
 * 
 */

class MyComponent : public Component
{

public:
	MyComponent(int align)
		: align(align)
	{}

	void Render(CommandCollection& d) const override
	{
		d.Font("gidole");
		d.Fill({ 255, 255, 255 });
		d.FontSize(36);
		d.TextAlign(align);
		d.Text("abcdefghijklmnopqrstuvw", { x + width / 2, y + height / 2 });
		d.Fill({ 0, 0, 0 });
		d.Quad({ x, y + (int)height / 2, width, 1 });
	}
	int align;
};

#define GIDOLE "C:\\Users\\Jeroen\\source\\repos\\GuiCode2\\assets\\fonts\\Gidole\\Gidole-Regular.otf"
#define SEGOEUI "C:\\Users\\Jeroen\\source\\repos\\GuiCode2\\assets\\fonts\\segoeui.ttf"


class TagParser
{
public:

	using AttributeType = std::variant<float, std::string>;
	
	class AttributeBase
	{
	public:
		AttributeBase(const std::string& name)
			: name(name)
		{}

		virtual void Set(void*, const AttributeType&) = 0;
		virtual void Set(Component&, const AttributeType&) = 0;
		virtual void* Get(void* c) = 0;
		virtual void* Get(Component& c) = 0;
		std::string name;
	};

	template<typename T, typename A>
	class AttributeTyped : public AttributeBase
	{
	public:
		AttributeTyped(const std::string& name, A T::*member)
			: member(member), AttributeBase(name)
		{}

		void Set(Component& c, const AttributeType& a) override
		{
			T* _t = dynamic_cast<T*>(&c);

			// If correct types
			if constexpr (GetIndex<A, AttributeType>::failed)
				return;
			else
				if (_t != nullptr && a.index() == GetIndex<A, AttributeType>::value)
					(_t->*member) = std::get<GetIndex<A, AttributeType>::value>(a);
		}

		void Set(void* c, const AttributeType& a) override
		{
			if (c == nullptr)
				return;

			T* _t = reinterpret_cast<T*>(c);

			// If correct types
			if constexpr (GetIndex<A, AttributeType>::failed)
				return;
			else
				if (_t != nullptr && a.index() == GetIndex<A, AttributeType>::value)
					(_t->*member) = std::get<GetIndex<A, AttributeType>::value>(a);
		}

		void* Get(void* c) override 
		{
			if (c == nullptr)
				return nullptr;

			T* _t = reinterpret_cast<T*>(c);
			return &(_t->*member);
		}

		void* Get(Component& c) override
		{
			T* _t = dynamic_cast<T*>(&c);
			if (_t == nullptr)
				return nullptr;

			return &(_t->*member);
		}

		A T::* member;
	};

	struct Settings
	{
		std::string name;
	};

	TagParser(const Settings& s)
		: settings(s)
	{}

	template<typename T, typename A>
	void Attribute(const std::string& name, A T::* member)
	{
		auto _res = split(name, ".");
		auto _final = _res[_res.size() - 1];
		attributes.emplace(_final, std::make_unique<AttributeTyped<T, A>>(name, member));
	}

	void Set(Component& c, const std::string& name, const AttributeType& value)
	{
		auto _res = split(name, ".");
		if (_res.size() == 0)
			return;

		if (_res.size() == 1)
		{
			attributes[_res[0]]->Set(c, value);
			return;
		}

		void* _ptr = attributes[_res[0]]->Get(c);
		for (int i = 1; i < _res.size() - 1; i++)
			_ptr = attributes[_res[1]]->Get(_ptr);

		attributes[_res[_res.size() - 1]]->Set(_ptr, value);
	}

	virtual Component& Create() = 0;

	Settings settings;
	std::map<std::string, std::unique_ptr<AttributeBase>> attributes;
};

class PanelParser : public TagParser
{
public:
	PanelParser()
		: TagParser({.name = "panel"})
	{
		Attribute("width", &Panel::width);
		Attribute("settings", &Panel::settings);
		Attribute("settings.ratio", &Panel::Settings::ratio);
		Attribute("settings.padding", &Panel::Settings::padding);
		Attribute("settings.padding.left", &Vec4<float>::left);
		Attribute("settings.padding.right", &Vec4<float>::right);
		Attribute("settings.padding.top", &Vec4<float>::top);
		Attribute("settings.padding.bottom", &Vec4<float>::bottom);
	}

	Component& Create() override 
	{
		return *new Panel;
	};
};

class Parser
{
	


};



int main()
{
	constexpr int siez = sizeof Command;

	Frame window{ {
		.name = "A really epic window",
		.dimensions{ 500, 100, 500, 500 },
		.state = Show
	} };

	window.graphics->LoadFont(GIDOLE, "gidole");
	window.graphics->LoadFont(SEGOEUI, "segoeui");

	window.titlebar.font = "segoeui";
	window.titlebar.background = 0x39805f;
	window.titlebar.close.color.base = 0x39805f;
	window.titlebar.minimize.color.base = 0x39805f;
	window.titlebar.maximize.color.base = 0x39805f;
	window.titlebar.close.color.State<Hovering>(0xcc5c5c);
	window.titlebar.minimize.color.State<Hovering>(0x52B788);
	window.titlebar.maximize.color.State<Hovering>(0x52B788);
	window.titlebar.close.color.State<Pressed>(0x803939);
	window.titlebar.minimize.color.State<Pressed>(0x469c74);
	window.titlebar.maximize.color.State<Pressed>(0x469c74);
	window.background = 0x39805f;

	TextArea _text{};
	_text.font = "segoeui";
	_text.lineHeight = 14;
	_text.fontSize = 14;
	_text.wrap = Wrap::Word;
	_text.container.content = "apple juice";
	_text.textColor = { 255, 255, 255 };
	_text.scrollbar.x.background = 0x469c74;
	_text.scrollbar.y.background = 0x469c74;
	_text.scrollbar.x.bar.base = 0x67e6ab;
	_text.scrollbar.y.bar.base = 0x67e6ab;

	window.panel = {
		{
			.padding{ 8, 8, 8, 8 },
			.margin{ 8, 8, 8, 8 },
			.border{ 4, 0x469c74 },
			.background{ 0x52B788 }
		},
		_text
	};

	while (window.Loop())
	{
		LIMIT_FPS(60);
		//MEASURE_FPS;
	}
}


/*
class Apple : public Component
{
public:
	Apple(int ids)
		: id(ids)
	{
		listener += [this](const Focus& e)
		{
			std::cout << "Focus!" << id << std::endl;
		};

		listener += [this](const Unfocus& e)
		{
			std::cout << "Unfocus!" << id << std::endl;
		};

		listener += [this](const MouseEnter& e)
		{
			std::cout << "Enter!" << id << std::endl;
		};

		listener += [this](const MouseExit& e)
		{
			std::cout << "Exit!" << id << std::endl;
		};

		listener += [this](const MousePress& e)
		{
			press = e.pos;
			if (e.pos.x > x + width - 10)
				if (e.pos.y > y + height - 10)
					dragging = true;
			std::cout << "Press!" << id << std::endl;
		};

		listener += [this](const MouseClick& e)
		{
			std::cout << "Click!" << id << std::endl;
		};

		listener += [this](const MouseRelease& e)
		{
			zIndex = 0;
			std::cout << "Release!" << id << std::endl;
		};

		listener += [this](const MouseMove& e)
		{
			//std::cout << "Move!" << id << std::endl;
		};

		listener += [this](const MouseDrag& e)
		{
			if (dragging)
				size = e.pos - position;



			//else {
			//	position += e.pos - press;
			//	press = e.pos;
			//	zIndex = 10;
			//}
			//std::cout << "Drag!" << id << std::endl;
		};
	}

	void Render(CommandCollection& d) const override
	{
		if (State<Focused>())
			d.Fill({ 0xFFD5B2 });
		else if (State<Pressed>())
			d.Fill({ 0x95D5B2 });
		else if (State<Hovering>())
			d.Fill({ 0xD8F3DC });
		else
			d.Fill({ 0xB7E4C7 });

		d.Quad(dimensions);
	}

	void Update() override
	{

	}

	Vec2<float> press;
	int id;
	bool dragging = false;
};


	Apple _comp{1};
	Apple _comp2{2};
	Apple _comp3{3};
	Apple _comp4{4};
	Apple _comp5{5};
	Apple _comp6{6};
	Apple _comp7{7};
	Apple _comp8{8};
	Apple _comp9{9};
	Apple _comp10{10};
	Apple _comp11{11};
	Apple _comp12{12};

	_comp12.size = _comp11.size = _comp10.size = _comp9.size = _comp8.size = _comp7.size = _comp6.size =
		_comp5.size = _comp4.size = _comp3.size = _comp2.size = _comp.size = { 50, 50 };

	window.titlebar.close.hover = { 0xed5b51 };
	window.titlebar.close.press = { 0xeb4034 };
	window.titlebar.minimize.hover = { 0x2D6A4F };
	window.titlebar.minimize.press = { 0x1B4332 };
	window.titlebar.maximize.hover = { 0x2D6A4F };
	window.titlebar.maximize.press = { 0x1B4332 };
	window.titlebar.color = { 0x40916C };
	window.background = { 0x40916C };

	Panel::Id _scrollpanel1;
	Panel::Id _scrollpanel2;
	Panel::Id _scrollpanel3;

	window.panel =
	{
		{
			.layout = Layout::Row,
			.overflow = Overflow::Hide,
			.padding{ 8, 8, 8, 8 },
			.margin{ 8, 8, 8, 8 },
			.background{ 0x52B788 }
		},
		{
			{
				{
					.id = _scrollpanel1,
					.ratio = 1,
					.layout = Layout::Row,
					.overflow = Overflow::Scroll,
					.padding{ 8, 8, 8, 8 },
					.margin{ 4, 4, 4, 4 },
					.background{ 0x74C69D }
				},
				{
					{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ 110, -1 } }, _comp4 },
					{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ 120, -1 } }, _comp5 },
					{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ 150, -1 } }, _comp6 },
					{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ 190, -1 } }, _comp8 },
				}
			},
			{
				{
					.id = _scrollpanel2,
					.ratio = 1,
					.layout = Layout::Column,
					.overflow = Overflow::Scroll,
					.padding{ 8, 8, 8, 8 },
					.margin{ 4, 4, 4, 4 },
					.background{ 0x74C69D }
				},
				{
					{
						{
							.id = _scrollpanel3,
							.ratio = 5,
							.layout = Layout::Column,
							.overflow = Overflow::Scroll,
							.padding{ 8, 8, 8, 8 },
							.margin{ 4, 4, 4, 4 },
							.border{ 4, 0x52B788 },
							.min{ -1, 300 },
							.background{ 0x74C69D }
						},
						{
							{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ -1, 110 }, .align = Align::Center  }, _comp },
							{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ -1, 150 }, .align = Align::Right }, _comp2 },
							{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ -1, 180 }, .align = Align::Left }, _comp3 },
							{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ -1, 120 }, .align = Align::Right }, _comp7 },
						}
					},
					{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ -1, 180 }, .align = Align::Left }, _comp9 },
					{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ -1, 120 }, .align = Align::Right }, _comp10 },
					{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ -1, 150 }, .align = Align::Center }, _comp11 },
					{ {.ratio = 0, .margin{ 4, 4, 4, 4 }, .size{ Auto, Auto }, .min{ -1, 110 }, .align = Align::Right }, _comp12 },
				}
			},
		}
	};

	Panel* _scroll1 = window.panel.Find(_scrollpanel1);
	Panel* _scroll2 = window.panel.Find(_scrollpanel2);
	Panel* _scroll3 = window.panel.Find(_scrollpanel3);

	_scroll1->scrollbar.x.background =
	_scroll1->scrollbar.y.background =
	_scroll2->scrollbar.x.background =
	_scroll2->scrollbar.y.background =
	_scroll3->scrollbar.x.background =
	_scroll3->scrollbar.y.background = { 0x4ba67c };
	_scroll1->scrollbar.x.bar.base = { 0x40916C };
	_scroll1->scrollbar.x.bar.State<Pressed>(0x327356);
	_scroll1->scrollbar.x.bar.State<Hovering>(0x3b8765);
	_scroll1->scrollbar.y.bar =
	_scroll2->scrollbar.x.bar =
	_scroll2->scrollbar.y.bar =
	_scroll3->scrollbar.x.bar =
	_scroll3->scrollbar.y.bar = _scroll1->scrollbar.x.bar;
*/