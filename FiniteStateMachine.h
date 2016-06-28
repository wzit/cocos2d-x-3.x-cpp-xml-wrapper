/******************************************************************************/
/*
 * Copyright 2014-2015 Vladimir Tolmachev
 *
 * Author: Vladimir Tolmachev
 * Project: ml
 * e-mail: tolm_vl@hotmail.com
 * If you received the code is not the author, please contact me
 */
/******************************************************************************/


#ifndef __FINITE_STATE_MACHINE__
#define __FINITE_STATE_MACHINE__

#include <list>
#include <map>
#include <assert.h>
#include <functional>
#include <queue> 
#include <string>
#include <thread>
#include <mutex>
//#include <function.h>

namespace FiniteState
{

#define state_enum(name) enum class name : FiniteState::Tag
#define event_enum(name) enum class name : FiniteState::Tag

	class State;
	class Event;
	class Machine;

	typedef int Tag;
	typedef std::function<void()> CallBack;
	typedef std::function<void( void* )> CallBackUpdate;
	typedef std::list<State*> StatesList;
	typedef StatesList::iterator StatesIterator;
	typedef StatesList::const_iterator StatesCIterator;
	typedef std::list<Event*> EventsList;
	typedef EventsList::iterator EventsIterator;
	typedef EventsList::const_iterator EventsCIterator;
	typedef std::list<CallBack> CallBacksList;
	typedef CallBacksList::iterator CallBacksIterator;
	typedef CallBacksList::const_iterator CallBacksCIterator;

	const Tag InvalidTag = -1;

	class State
	{

	public:
		State( Machine& machine, const CallBack& cb );
		virtual ~State();

		virtual bool add_transition( const Tag& onEvent, const Tag& toState );
		virtual void inherit_transitions( const Tag& state );
		virtual Tag process( const Event& event )const;

		void set_name( const Tag& name );
		void set_string_name( const std::string& name );
		const Tag& get_name()const;
		const std::string& get_string_name()const;

		virtual void update( void * data );
		virtual void onActivate();
		virtual void onDeactivate();

		void add_onActivateCallBack( const CallBack& function );
		void add_onDeactivateCallBack( const CallBack& function );
		void set_updateCallback( const CallBackUpdate& function );
		void clear_onActivateCallBack();
		void clear_onDeactivateCallBack();

	protected:
		void _execute( const CallBacksList& list );
		void _add_callback( CallBacksList& list, const CallBack& function );
		Machine& machine();
		const Machine& machine()const;
	private:
		Machine& _machine;
		std::string _string_name;
		Tag _name;
		std::map<const Event*, Tag> _transitions;
		std::list<Tag> _inherited_transitions;

		CallBacksList _onActivate;
		CallBacksList _onDeactivate;
		CallBackUpdate _onUpdate;
	};


	class Event
	{
	public:
		Event( Machine& machine );
		void set_name( const Tag& name );
		void set_string_name( const std::string& name );
		const Tag& get_name()const;
		const std::string& get_string_name()const;
	private:
		Machine& _machine;
		std::string _string_name;
		Tag _name;
	};

	class Machine
	{
	public:
		Machine();
		virtual~Machine();
		//const Machine& operator = (const Machine& other);

		void start( const Tag& nameState );

		bool is_exist_state( const Tag& name )const;
		bool is_exist_state( const std::string& name )const;
		bool is_exist_event( const Tag& name )const;
		bool is_exist_event( const std::string& name )const;

		Event& add_event( const Tag& nameEvent );
		Event& event( const Tag& tag );
		Event& event( const std::string& name );
		State& add_state( const Tag& nameState, const CallBack& onActivate );

		template <class T>
		T& add_state( const Tag& nameState, const CallBack& onActivate )
		{
			StatesCIterator i = _state( nameState );
			assert( _isvalid( i ) == false );
			T * state = new T( *this, onActivate );
			state->set_name( nameState );
			_states.push_back( state );
			return *state;
		}

		State& state( const Tag& tag );
		State& state( const std::string& name );

		template <class T> T& state( const Tag& tag )
		{
			auto i = _state( tag );
			assert( i != _states.end() );
			return *((T*)(*i));
		}

		State* previous_state();
		State& current_state();
		void process();
		virtual void push_event( const Tag& event );

	protected:
		bool _isvalid( const EventsCIterator& iterator )const;
		bool _isvalid( const StatesCIterator& iterator )const;
		EventsIterator _event( Tag name );
		StatesIterator _state( Tag name );
		EventsCIterator _event( Tag name )const;
		StatesCIterator _state( Tag name )const;
		void _set_state( State * state );

	protected:
		State * _currentState;
		State * _previousState;
		StatesList _states;
		EventsList _events;
		std::mutex _mutexQueueEvents;
		std::queue<Event*>_eventsQueue;
	};


}

#endif