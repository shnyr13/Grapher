#include "SignalEmiter.h"

SignalEmiter::SignalEmiter()
{

}

SignalEmiter::~SignalEmiter()
{

}

void SignalEmiter::EmitSignalSelectedTop(GraphTop *top)
{
	emit TopIsSelected(top);
}

void SignalEmiter::EmitSignalSelectedEdge (GraphEdge *edge)
{
	emit EdgeIsSelected(edge);
}
