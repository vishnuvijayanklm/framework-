#ifndef ASYNCTASK_H
#define ASYNCTASK_H
#include <containers/include/Stl.h>
#include <core/include/EventInfo.h>

namespace Async
{
	class Task
	{
			StlQueue<IEventInfo*> mTasks;
		public:
			template<typename T>
			Task(T fn)
			{
				fn();
			}

			~Task();
			template<typename T>
			Task& then(T fn)
			{
				fn();
				return *this;
			}
			Task& execute()
			{
				return *this;
			}
	};
}
#endif //ASYNCTASK_H