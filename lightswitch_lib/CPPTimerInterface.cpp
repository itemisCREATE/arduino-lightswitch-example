#include "CPPTimerinterface.h"
#include "TimedStatemachineInterface.h"

void TimerTask::proceed(sc_integer elapsed_time) {
	elapsed_time_ms += elapsed_time;
	if(elapsed_time_ms > time_ms) {
		handle->raiseTimeEvent(pt_evid);
		if(periodic) {
			elapsed_time_ms -= time_ms;
		}
	}
}

bool TimerTask::isTimerFor(TimedStatemachineInterface* statemachine, sc_eventid event) {
	return handle == statemachine && pt_evid == event;
}

/*!
	This function will be called for each time event that is relevant for a state when a state will be entered.
	\param evid An unique identifier of the event.
	\time_ms The time in milli seconds
	\periodic Indicates the the time event must be raised periodically until the timer is unset
*/
void CPPTimerInterface::setTimer( TimedStatemachineInterface* statemachine,
		sc_eventid event,
		sc_integer time,
		sc_boolean isPeriodic )
{

	/* create new task and go through all timers ... */
	for (int i = 0; i < num_timers; i++)
	{
		if((tasks[i]) == sc_null)
		{
			tasks[i] = new TimerTask(statemachine, event, time, isPeriodic);
			break;
		}
	}
}

/*!
	This function will be called for each time event that is relevant for a state when a state will be left.
	\param evid An unique identifier of the event.
*/
void CPPTimerInterface::unsetTimer(TimedStatemachineInterface* statemachine,
		sc_eventid event)
{

	/* go through all timers ... */
	for(int i = 0; i < num_timers; i++)
	{
		TimerTask *task = tasks[i];
		/* ... and find the used timer */
		if(task != sc_null && task->isTimerFor(statemachine, event))
		{
			/* reset the timer */
			delete tasks[i];
			tasks[i] = sc_null;
		}
	}
}


/*!
	This function must be called by the user. The elapsed time must be calculated every time, the function gets
	gets called.
 */
void CPPTimerInterface::proceed(long elapsed_ms)
{

	/* go through all timers ... */
	for (int i = 0; i < num_timers; i++)
	{
		TimerTask *task = this->tasks[i];
		if (task != sc_null)
		{
			task->proceed(elapsed_ms);
		}
	}
}

/* ! Cancel timer service. */
void CPPTimerInterface::cancel()
{

	/* delete all timers */
	for (int i = 0; i < num_timers; i++)
	{
		delete tasks[i];
		this->tasks[i] = sc_null;
	}
}
