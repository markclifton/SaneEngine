#ifndef INPUTEVENTS_H
#define INPUTEVENTS_H

namespace se
{
	namespace events
	{
		struct KeyEvent
		{
			int key, scancode, action, mods;
		};

		struct MousePressEvent
		{
			int button, action, mods;
		};

		struct MouseMoveEvent
		{
			double x, y;
		};
	}
}

#endif