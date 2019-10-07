#ifndef SRC_CPPTIMERINTERFACE_H_
#define SRC_CPPTIMERINTERFACE_H_

#include "TimerInterface.h"
#include "sc_types.h"

class TimerTask {
public:

	/* Constructor, which creates a new timer task */
	TimerTask(TimedStatemachineInterface* statemachine, sc_eventid event, sc_integer time, sc_boolean isPeriodic) :
		handle(statemachine),
		time_ms(time),
		periodic(isPeriodic),
		pt_evid(event),
		elapsed_time_ms(0)
	{	}

	/* Destructor */
	~TimerTask(){}

	void proceed(sc_integer elapsed_time);
	bool isTimerFor(TimedStatemachineInterface* statemachine, sc_eventid event);

private:
	TimedStatemachineInterface* handle;
	sc_integer time_ms;
	sc_boolean periodic;
	sc_eventid pt_evid;
	sc_integer elapsed_time_ms;
};

class CPPTimerInterface : public TimerInterface {
private:
	TimerTask** tasks;
	sc_integer num_timers;
public:

	/* Constructor */
	CPPTimerInterface(TimerTask** tasks, sc_integer num_timers) :
		tasks(tasks),
		num_timers(num_timers)
	{

		for(int i = 0; i<num_timers;i++)
		{
			this->tasks[i] = sc_null;
		}
	}

	/* Destructor */
    virtual ~CPPTimerInterface()
    {
        cancel();
    }

    /*!
    	This function will be called for each time event that is relevant for a state when a state will be entered.
    	\param evid An unique identifier of the event.
    	\time_ms The time in milli seconds
    	\periodic Indicates the the time event must be raised periodically until the timer is unset
    */
    virtual void setTimer(TimedStatemachineInterface* statemachine, sc_eventid event, sc_integer time, sc_boolean isPeriodic );

    /*!
    	This function will be called for each time event that is relevant for a state when a state will be left.
    	\param evid An unique identifier of the event.
    */
    virtual void unsetTimer(TimedStatemachineInterface* statemachine, sc_eventid event);

    /*!
    	This function must be called by the user. The elapsed time must be calculated every time, the function gets
    	gets called.
     */
    void proceed(long elapsed_ms);

    /* ! Cancel timer service. */
    virtual void cancel();
};


#endif /* SRC_CPPTIMERINTERFACE_H_ */
