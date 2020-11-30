#include "Action.h"

Action::Action()
{
	type = ActionType::Roll;
	written = ScoreType::Ones;
	kept.clear();
}
