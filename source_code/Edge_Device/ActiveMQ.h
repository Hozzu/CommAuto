#ifndef ACTIVEMQ_H
#define ACTIVEMQ_H 
 
#include <activemq/library/ActiveMQCPP.h>
#include <decaf/lang/Thread.h>
#include <decaf/lang/Runnable.h>
#include <decaf/util/concurrent/CountDownLatch.h>
#include <decaf/lang/Integer.h>
#include <decaf/lang/Long.h>
#include <decaf/lang/System.h>
#include <activemq/core/ActiveMQConnectionFactory.h>
#include <activemq/util/Config.h>
#include <cms/Connection.h>
#include <cms/Session.h>
#include <cms/TextMessage.h>
#include <cms/BytesMessage.h>
#include <cms/MapMessage.h>
#include <cms/ExceptionListener.h>
#include <cms/MessageListener.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <memory>
#include <vector>
#include <string>
#include <curl/curl.h>
#include <algorithm>
#include <boost/regex.hpp>


using namespace activemq::core;
using namespace decaf::util::concurrent;
using namespace decaf::util;
using namespace decaf::lang;
using namespace cms;
using namespace std;
using namespace boost;

/* Description: 
 * This is a Producer class.
 * The class contains functions such as constructor, destructor, connect, send etc. 
 * This class is used by ActiveMQ_Solution class
 * Inputs: None 
 * Return value: None
 */ 
class Producer {

 private:       
     	 Connection* connection;
     	 Session* session;
     	 Destination* destination;
     	 MessageProducer* producer;
     	 int numMessages;
         bool useTopic;
         bool sessionTransacted;
     	 std::string brokerURI;
         int count =0;
 
  public:
     	 Producer(const Producer&);
     	 Producer& operator=(const Producer&);
    	 Producer(const std::string& brokerURI, int numMessages, bool useTopic = false, bool sessionTransacted = false) :
         connection(NULL),
         session(NULL),
         destination(NULL),
         producer(NULL),
         numMessages(numMessages),
         useTopic(useTopic),
         sessionTransacted(sessionTransacted),
         brokerURI(brokerURI) {}
	 virtual ~Producer();
         void close();
         void connect(string address);
         void send(string tag, string data); 
 
 private:
         void cleanup(); 
 
};

/* Description: 
 * This is a Consumer class.
 * The class contains functions such as constructor, destructor, connect, receive etc.
 * This class is used by ActiveMQ_Solution class
 * Inputs: None 
 * Return value: None
 */ 
 
class Consumer : public ExceptionListener, public MessageListener{
 
 private:
     	 CountDownLatch latch;
     	 CountDownLatch doneLatch;
     	 Connection* connection;
     	 Session* session;
    	 Destination* destination;
    	 MessageConsumer* consumer;
    	 long waitMillis;
    	 bool useTopic;
    	 bool sessionTransacted;
    	 string brokerURI;
	 vector<string> receive_vector_;
	 int count=0;    
 
  public:
         Consumer& operator=(const Consumer&);
         Consumer(Consumer const&);
         Consumer(const std::string& brokerURI, int numMessages, bool useTopic = false, bool sessionTransacted = false, int waitMillis = 30000) :
         latch(1),
         doneLatch(numMessages),
         connection(NULL),
         session(NULL),
         destination(NULL),
         consumer(NULL),
         waitMillis(waitMillis),
         useTopic(useTopic),
         sessionTransacted(sessionTransacted),
         brokerURI(brokerURI) {}
    
         virtual ~Consumer();
 	 void close();
 	 void waitUntilReady(); 
 	 void connect(string address); 
	 void receive_async(string tag, vector<string> receive_vector);
	 void receive(string tag, vector<string>* receive_vector);
 	 // Called from the consumer since this class is a registered MessageListener.
     	 virtual void onMessage(const Message* message); 
 
   	 // If something bad happens you see it here as this class is also been
     	 // registered as an ExceptionListener with the connection.
    	 virtual void onException(const CMSException& ex AMQCPP_UNUSED);
 
 private:
 
     	 void cleanup();
 
 };

 /* Description: 
  * This is a structure for producer broker.
  * The structure contains information such as URL, broker connection, broker session, broker destination etc.
  * Inputs: None 
  * Return value: None
  */ 
 typedef struct{
         string URL;
         Connection* connection;
         Session* session;
         Destination* destination;
         MessageProducer* producer_;
 
 }p_broker;
 
 /* Description: 
  * This is a structure for consumer broker.
  * The structure contains information such as URL, broker connection, broker session, broker destination etc.
  * Inputs: None 
  * Return value: None
  */ 
 typedef struct{
         string URL;
         Connection* connection;
         Session* session;
         Destination* destination;
         MessageConsumer* consumer_;
 
 }c_broker;


 /* Description: 
  * This is an ActiveMQ_Solution class.
  * The class contains functions such as producer connect, send, consumer connect, receive, receive_async. 
  * receive is a pull method to get messages from brokers.
  * receive_async is a push method to get messages from brokers.
  * This class is used by ActiveMQ_Solution class
  * Inputs: None 
  * Return value: None
  */ 

 class ActiveMQ_Solution{

	private:
		vector<Producer*> producer_vector;
		vector<Consumer*> consumer_vector;
	public:
		
		ActiveMQ_Solution();

		~ActiveMQ_Solution();
		void p_connect(string IP);
		void send(string tag, string data);
		void c_connect(string IP);
		void receive_async(string tag, vector<string> receive_vector);
		void receive(string tag, vector<string>* receive_vector);

};
 
#endif
                                                                                                                                                                                                                                         
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
 

