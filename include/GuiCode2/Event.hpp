#pragma once
#include "GuiCode2/pch.hpp"
#include "GuiCode2/Utils.hpp"

namespace GuiCode
{
	class Component;

	/**
	 * State specific settings.
	 */
	struct StateSettings
	{
		int limit = -1; // Set limit to sum of values for state of sub-components.
		bool visible = true; // Does component need to be visible for state handler to ask about state?
	};

	/**
	 * Event base class. Can be extended to make custom events.
	 * Override Forward to tell when this event should be forwarded to
	 * a component using the component's state.
	 */
	struct Event
	{
		Event(bool bsh = true)
			: beforeStateHandler(bsh)
		{}

		virtual bool Forward(const Component&) const = 0;

		void Handle() const { handled = true; }
		bool Handled() const { return handled; }

		/**
		 * Is this event handled before or after the state handler?
		 * default is true (before state handler)
		 */
		bool beforeStateHandler;

	private:
		mutable bool handled = false;
	};

	/**
	 * EventListener manages the event callbacks and the forwarding of events based on
	 * the Event::Forward(const Component&) method. Also manages state of contained
	 * components using a StateHandler, which contains callbacks to determine states
	 * based on incoming events.
	 */
	struct EventListener
	{
		/**
		 * EventListener needs the list of components to work with.
		 * @param c component list
		 */
		EventListener(std::list<Pointer<Component>>& c)
			: components(&c)
		{}
		
		// Don't copy anything, since callbacks may contain a this-pointer
		EventListener(const EventListener&) {}
		EventListener(EventListener&&) {}
		EventListener& operator=(const EventListener&) { return *this; };
		EventListener& operator=(EventListener&&) { return *this; };

		/**
		 * Type erasure using inheritance, base class contains Call
		 * method, derived class tries casting to contained Event Type and
		 * if successful, will call the contained callback.
		 */ 
		struct EventFunctionBase 
		{ 
			virtual void Call(const Event&) = 0;
		};

		template<typename, typename>
		struct EventFunction;
		template<typename Func, std::derived_from<Event> Type>
		struct EventFunction<Func, void(const Type&)> : EventFunctionBase
		{
			EventFunction(Func f) : fun(f) {}

			void Call(const Event& e) override
			{
				// Try casting
				const Type* _e = dynamic_cast<const Type*>(&e);
				if (_e) // if successfull, call callback
					fun(*_e);
			}

			Func fun;
		};

		/**
		 * Also uses inheritance to do type erasure for state callbacks.
		 * Tries dynamic casting the event and component to the contained
		 * event and component types, and if successful it will call the
		 * callback.
		 */
		struct StateFunctionBase
		{
			virtual int Call(const Event&, Component&, int) = 0;
		};

		template<typename, typename>
		struct StateFunction;
		template<typename Func, typename Type, typename Obj>
		struct StateFunction<Func, int(const Type&, Obj&, int)> : StateFunctionBase
		{
			StateFunction(int a, Func b) 
				: state(a), callback(b) 
			{}

			int Call(const Event& e, Component& c, int left) override
			{
				// Try casting both objects to the correct type
				const Type* _e = dynamic_cast<const Type*>(&e);
				Obj* _c = dynamic_cast<Obj*>(&c);

				// If successful, we can call the callback
				if (_e && _c)
				{
					// Calculate new state with callback function
					int _state = callback(*_e, *_c, left);
					_c->State(state, _state);
					return _state;
				}

				// Otherwise return 0
				return 0;
			}

			int state;
			Func callback;
		};

		/**
		 * A StateHandler handles a single state using several callbacks.
		 */
		struct StateHandler
		{
			int state;
			StateSettings settings; // State specific settings, like limit etc.

			/**
			 * Add state handler callback.
			 * @param t callback
			 */
			template<typename T>
			StateHandler& operator+=(T t)
			{
				handlers.push_back(std::make_unique<StateFunction<T, LambdaSignatureT<T>>>(state, t));
				return *this;
			}

			std::vector<std::unique_ptr<StateFunctionBase>> handlers;
		};

	public:

		/**
		 * Add event listener callback.
		 * @param t callback
		 */
		template<typename T>
		void operator+=(T t)
		{
			m_Listeners.push_back(std::make_unique<EventFunction<T, LambdaSignatureT<T>>>(t));
		}

		/**
		 * Add a state to the StateHandler.
		 * @param s settings (optional)
		 * @return StateHandler
		 */
		template<int state>
		StateHandler& State(const StateSettings& s = {}) 
		{
			if (m_StateHandlers.contains(state))
				return m_StateHandlers.at(state);
			
			return m_StateHandlers[state] = { state, s };
		}

		/**
		 * Run event through this listener.
		 * @param e event
		 */
		void operator()(const Event& e);

	private:
		std::list<Pointer<Component>>* components = nullptr;
		std::list<std::unique_ptr<EventFunctionBase>> m_Listeners;
		std::map<int, StateHandler> m_StateHandlers;
	};
}