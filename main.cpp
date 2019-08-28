#include <iostream>
#include <util/include/Logger.h>
#include <async/include/EventListener.h>
#include <future>
#include <async/include/Task.h>
#include <timer/include/TimerTask.h>
#include <ipc/include/MessageQueue.h>

using namespace std;
//unique_ptr<Core::Application> pApplication(new Core::Application());
Logger LOGGER;

void test()
{
	cout<<"IN TEST"<<endl;
	sleep(1);	
}

void fn(int x)
{
	cout<<"Expired Func"<<endl;
}

class TimerExample : public Async::ITimer
{
		shared_ptr<Async::TimerTicks> mTicks;
	public:
		TimerExample():mTicks(make_shared<Async::TimerTicks>(this))
		{
			this->mTicks->setInterval(1,true);
			this->mTicks->start();
		}

		~TimerExample()
		{

		}

		void onTimerExpired(Async::TimerTicks *pTicks)
		{
			LOG_INFO((LOGGER),("Timer Expired [%p]",pTicks));
			//Async::TimerTask::getTimer()->removeTimer(this->mTicks.get());
		}

};

int main()
{

	LockFreeQueue<int> Q;
	for(int i = 1; i <= 100;i++)
	{
		Q.push(i);
	}


	int ii = -1;
	while(Q.pop(ii))
	{
		cout<<"I = "<<ii<<endl;
	}
	//return 0;
	/*func([]()
	{
		cout<<"Called...."<<endl;
		return 100;
	},
	[](int x)
        {
                cout<<"Received "<<x<<endl;
        });

	return 0;
*/
	/*,
	[](int x)
	{
		cout<<"Received "<<x<<endl;
	});*/
	LOGGER.setLogFile("Logs","log.txt");
	LOGGER.setLoglevel(31);
	Async::EventListener *pEvent = new Async::EventListener();

	pEvent->addEvent("event1",[]()
                        {
                                cout<<"onEvent1_1"<<endl;
                        });
        pEvent->addEvent("event1",[]()
                        {
                                cout<<"onEvent1_2"<<endl;
                        });
        pEvent->addEvent("event1",[]()
                        {
                                cout<<"onEvent1_3"<<endl;
                        });
        pEvent->addEvent("event1",[]()
                        {
                                cout<<"onEvent1_4"<<endl;
                        });
	
	pEvent->addEvent("event4",[](int x,int y)
			{
                                cout<<"onEvent4"<<endl;
                        });
	pEvent->addEvent("event5",[]()
			{
                                cout<<"onEvent5"<<endl;
                        });
	pEvent->addEvent("event6",[](int x,int y)
                        {
                                cout<<"Event6 X = "<<x<<" Y = "<<y<<endl;
          		});
	//add_event(EVENT_1,[](){cout<<"Hai"<<endl;});
	//notify_event(EVENT_1);
	pEvent->notify("event1");
	pEvent->notify("event2");
	pEvent->notify("event3",-100,-110);
	pEvent->notify("event4",-100,-110);
	pEvent->notify("event5",-100,-110);
	pEvent->notify("event6",-123,-111);
	//pEvent->async_notify("event1");


	IPC::MessageQueue myQueue("/myQ",200,100,true);
	cout<<myQueue.open()<<endl;
	thread([&myQueue]
	{
		char *ptr = new char[100];
		while(1)
		{		
			if(myQueue.read(ptr,100) != -1)
			{
				LOG_INFONP((LOGGER),("Received [%s]",ptr));
			}
			usleep(20000);
		}		
	}).detach();
	/*myQueue.recv([](shared_ptr<char> ptr,size_t size)
	{
		LOG_INFONP((LOGGER),("Received [%s][%d]",ptr.get(),size));
	});*/


	TimerExample e[20000];
	int i = 0;
	while(1)
	{
		string hai = "Hai Vishnu_"+to_string(++i);
		LOG_INFONP((LOGGER),("Send %d ",myQueue.send((char*)hai.c_str(),hai.length())));
		usleep(1000000);
		break;
	}
	/*
	while(1)
	{
		char ptr[10];
		LOG_INFONP((LOGGER),("Send [%d]",myQueue.send(ptr,sizeof(int))));
		sleep(1);
	}
	return 0;
	Example e[2000];*/
	
	while(1)
	{
			std::shared_ptr<Async::CancellationToken> Token = make_shared<Async::CancellationToken>();
			Async::Task([]()
			{
				return 99;
			},
			[](int x)
			{ 
				cout<<"Got "<<x<<endl;
			})
			.add([]()
			{
				cout<<"Fn with no return"<<endl;
			})
			.add(&test)
			.setCancellationToken(Token)
			.execute_sync();
			
			Async::Task([]()
                        {
                                return -100;
                        },
                        [](int x)
                        {
                                cout<<"Got "<<x<<endl;
                        })
			.add([]()
			{
				cout<<"Fn with no return"<<endl;
			})
			.add(test)
			.setCancellationToken(Token)
			.execute_sync();

			usleep(100000);
			//Token->cancel();
			//usleep(10000000);	
	}
	return 0;
}
