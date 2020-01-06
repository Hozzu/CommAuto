#include "ActiveMQ.h" 
#include <iostream>
using namespace std;

 /* Description: 
  * This is a destructor for Producer
  * Inputs: None 
  * Return value: None
  */ 
Producer::~Producer(){
         cleanup();
}
 
 /* Description: 
  * This is a close function that cleans up every private variables that are used in Producer class
  * Inputs: None 
  * Return value: None
  */ 
void Producer::close() {
         this->cleanup();
}

 /* Description: 
  * This is a connect function of Producer. It gets a broker URI as an input and make a connetion between
  * a broker and a producer. It uses connectionFactory to create a connection and the function also creates sessioin
  * using ActiveMQ library function. 
  * Inputs: string address 
  * Return value: None
  */ 
void Producer::connect(string address) {
 
       try {
	    
             // Create a ConnectionFactory
             auto_ptr<ConnectionFactory> connectionFactory(
                 ConnectionFactory::createCMSConnectionFactory(address));
	     
             // Create a Connection
             connection = connectionFactory->createConnection();
             connection->start();
	      
             // Create a Session
             if (this->sessionTransacted) {
                 session = connection->createSession(Session::SESSION_TRANSACTED);
             } else {
                 session = connection->createSession(Session::AUTO_ACKNOWLEDGE);
             }
 
            
	} catch(CMSException& e) {
	     e.printStackTrace();
	}
}

 /* Description: 
  * This is a send function of Producer. It gets tag and data as input values. Both of them are string.
  * Input tag value is used as the name of topic or queue. MessageProducer is created from the session to the Topic or Queue.
  * Created MessageProducer then sends message to the connected broker. The deliverymode is set to NON_PERSISTENT.
  * Inputs: string tag, string data
  * Return value: None
  */ 	
void Producer::send(string tag, string data){
	       if(count==0){		           
	       // Create the destination (Topic or Queue)
               if (useTopic) { 
                   destination = session->createTopic(tag);
               } else {
                   destination = session->createQueue(tag);
               }
  	       
               // Create a MessageProducer from the Session to the Topic or Queue
               producer = session->createProducer(destination);
               producer->setDeliveryMode(DeliveryMode::NON_PERSISTENT);
               }
	       count++;
               // Send a message
               auto_ptr<TextMessage> message(session->createTextMessage(data));
               producer->send(message.get());
           
}

 /* Description: 
  * This is a clean up function for Producer. This could be called when to reset private instances such as destination etc. 
  * Inputs: None 
  * Return value: None
  */ 
void Producer::cleanup() {
 
         if (connection != NULL) {
             try {
                 connection->close();
             } catch (cms::CMSException& ex) {
                 ex.printStackTrace();
             }
         }
 
         // Destroy resources.
         try {
             delete destination;
             destination = NULL;
             delete producer;
             producer = NULL;
             delete session;
             session = NULL;
             delete connection;
             connection = NULL;
         } catch (CMSException& e) {
             e.printStackTrace();
         }
}

 /* Description: 
  * This is a destructor for Consumer
  * Inputs: None 
  * Return value: None
  */ 
Consumer::~Consumer() {
         //cleanup();

}

 /* Description: 
  * This is a close function that cleans up every private variables that are used in Consumer class
  * Inputs: None 
  * Return value: None
  */ 
void Consumer::close() {
         //this->cleanup();
}
 
void Consumer::waitUntilReady() {
         latch.await();
}

/* Description: 
 * This is a connect function of Consumer. It gets a broker URI as an input and make a connetion between
 * a broker and a consumer. It uses connectionFactory to create a connection and the function also creates sessioin
 * using ActiveMQ library function. 
 * Inputs: string address 
 * Return value: None
 */ 
void Consumer::connect(string address) {
 
       
         // Create a ConnectionFactory
         auto_ptr<ConnectionFactory> connectionFactory(
         ConnectionFactory::createCMSConnectionFactory(address));
 
         // Create a Connection
         connection = connectionFactory->createConnection(); 
         connection->start();
         connection->setExceptionListener(this);
 
         // Create a Session
         if (this->sessionTransacted == true) {
               session = connection->createSession(Session::SESSION_TRANSACTED);
         } else {
               session = connection->createSession(Session::AUTO_ACKNOWLEDGE);
         }
 
}

/* Description: 
 * This is a receive_async function of Producer. It gets string tag and vector<string> receive_vector as inputs.
 * This functions receives message from broker asynchronously by setting ActiveMQ MessageListener.
 * Tag is used as the name of Topic or Queue, so the consumer could receive messgaes from corresponding Topic or Queue.
 * The received messages are stored ad receive_vector. 
 * Inputs: string tag, vector<string> receive_vector
 * Return value: None
 */	
void Consumer::receive_async(string tag, vector <string> receive_vector){
	 this->receive_vector_ = receive_vector;
         // Create the destination (Topic or Queue)
       
	 if (useTopic) {
              destination = session->createTopic(tag);
         } else {
              destination = session->createQueue(tag);
         }

         // Create a MessageConsumer from the Session to the Topic or Queue
         consumer = session->createConsumer(destination);
         consumer->setMessageListener(this);
         
 	 count++;
         std::cout.flush();
         std::cerr.flush();

         // Indicate we are ready for messages.
         latch.countDown();

         // Wait while asynchronous messages come in.
         doneLatch.await(waitMillis);

}

/* Description: 
 * This is a receive function of Producer. It gets string tag and vector<string> receive_vector as inputs.
 * This functions receives message from broker synchronously by setting ActiveMQ MessageListener.
 * Tag is used as the name of Topic or Queue, so the consumer could receive messgaes from corresponding Topic or Queue.
 * The received messages are stored ad receive_vector. 
 * Inputs: string tag, vector<string> receive_vector
 * Return value: None
 */     
void Consumer::receive(string tag, vector <string>* receive_vector){
	  // Create the destination (Topic or Queue)
         if(count==0){  
	  if (useTopic) {
       		destination = session->createTopic(tag);
          } else {
       		destination = session->createQueue(tag);
  	  }    
	  
  	  // Create a MessageConsumer from the Session to the Topic or Queue
  	  consumer = session->createConsumer(destination);
	  }
	  cout<<count<<endl;
	  count++;
	  cout << "receive!!" << endl;
          Message *message = consumer->receive();
	 
	  const TextMessage* textMessage = dynamic_cast<const TextMessage*> (message);
	  string text = "";
 
          if (textMessage != NULL) {
                  text = textMessage->getText();
          } else { 
                  text = "NOT A TEXTMESSAGE!";
          }
	  cout << text << endl;
	  //receive_vector->push_back(text);
}
 
// Called from the consumer since this class is a registered MessageListener.
void Consumer::onMessage(const Message* message) {
 
         static int count = 0;
 
         try {
             count++;
             const TextMessage* textMessage = dynamic_cast<const TextMessage*> (message);
             string text = "";
 
             if (textMessage != NULL) {
                 text = textMessage->getText();
             } else {
                 text = "NOT A TEXTMESSAGE!";
             }
 	    // receive_vector_.push_back(text);
             printf("Message #%d Received: %s\n", count, text.c_str());
 
         } catch (CMSException& e) {
             e.printStackTrace();
         }
 
         // Commit all messages.
         if (this->sessionTransacted) {
             session->commit();
         }
 
         // No matter what, tag the count down latch until done.
         doneLatch.countDown();
}
 
// If something bad happens you see it here as this class is also been
// registered as an ExceptionListener with the connection.
void Consumer::onException(const CMSException& ex AMQCPP_UNUSED) {
         printf("CMS Exception occurred.  Shutting down client.\n");
         ex.printStackTrace();
         exit(1);
}

 /* Description: 
  * This is a clean up function for Consumer. This could be called when to reset private instances such as destination etc. 
  * Inputs: None 
  * Return value: None
  */  
void Consumer::cleanup() {
         if (connection != NULL) {
             try {
                 connection->close();
             } catch (cms::CMSException& ex) {
                 ex.printStackTrace();
             }
         }
 
         // Destroy resources.
         try {
             delete destination;
             destination = NULL;
             delete consumer;
             consumer = NULL;
             delete session;
             session = NULL;
             delete connection;
             connection = NULL;
         } catch (CMSException& e) {
             e.printStackTrace();
         }
}
 
 /* Description: 
  * This is a constructor for ActiveMQ_Solution class
  * Inputs: None 
  * Return value: None
  */
ActiveMQ_Solution::ActiveMQ_Solution(){
	activemq::library::ActiveMQCPP::initializeLibrary();
}


 /* Description: 
  * This is a destructor for ActiveMQ_Solution class
  * Inputs: None 
  * Return value: None
  */
ActiveMQ_Solution::~ActiveMQ_Solution(){
	//producer_vector.clear();
	//consumer_vector.clear();
	activemq::library::ActiveMQCPP::shutdownLibrary();
}

vector<string> URL_v;


size_t WriteCallback(char *contents, size_t size, size_t nmemb, void *userp){
     ((std::string*)userp)->append((char*)contents, size * nmemb);
     return size * nmemb;
}


 /* Description: 
  * This is a function that parses URL. It takes IP(e.g. 147.47.208.44:8080) as an input.
  * It then parses broker URIs by using regex. It store all the possible broker URIs into a string vector.
  * Inputs: string IP
  * Return value: None
  */ 
void Url_parsing(string IP){
     curl_global_init(CURL_GLOBAL_ALL);
 
     CURL* easyhandle = curl_easy_init();
     string Buffer;
     curl_easy_setopt(easyhandle, CURLOPT_URL, IP.c_str());
 
     curl_easy_setopt(easyhandle, CURLOPT_WRITEFUNCTION, WriteCallback);
     curl_easy_setopt(easyhandle, CURLOPT_WRITEDATA, &Buffer);

     curl_easy_perform(easyhandle);
     regex regexp("\\b([0-9]+.[0-9]+.[0-9]+.[0-9]+:[0-9]+)"); 
     const sregex_iterator itEnd;
 
     for(sregex_iterator it(Buffer.begin(), Buffer.end(), regexp); it!=itEnd; ++it){
	string elem = it->str();
     	string temp = "failover:(tcp://";
     	temp = temp+(string)elem+")";
     	URL_v.push_back(temp);     
     }   
}
                                                                                                
/* Description: 
 * This is a producer connect function of ActiveMQ_Solution. In the function, Url_parsing function is called with IP as a ninput. 
 * It then connects to all the possible brokers which are stored in URL_v vector, which is filled in Url_parsing function.	
 * The function also creates producer by calling producer constructor with brokerURI, numMessages, and useTopics for inputs.
 * Created producers are stored in producer_vector.
 * Inputs: string IP 
 * Return value: None
 */
void ActiveMQ_Solution::p_connect(string IP){
		//IP address parsing here
		
		Url_parsing("http://" + IP + "/get_broker_list");		
	
     		bool useTopics = false;
     		bool sessionTransacted = false;
     		int numMessages = 1;
     		//the size of the list that contains URIs

        	for(unsigned int i=0; i<URL_v.size(); i++){
			     string brokerURI = URL_v[i];
			     Producer* producer = new Producer(brokerURI, numMessages, useTopics);
			     (*producer).connect(brokerURI);
                     	     producer_vector.push_back(producer);
  	          }

	
}

 /* Description: 
  * This is a send function of ActiveMQ_Solution. It simply calls send function for producer for as many producer it has in
  * producer_vector vector.
  * Inputs: string tag, string data
  * Return value: None
  */
void ActiveMQ_Solution::send(string tag, string data){
			
		for(unsigned int i=0; i<producer_vector.size(); i++){
				(*producer_vector[i]).send(tag, data);
		}
		
}

/* Description: 
 * This is a consumer connect function of ActiveMQ_Solution. In the function, Url_parsing function is called with IP as a ninput. 
 * It then connects to all the possible brokers which are stored in URL_v vector, which is filled in Url_parsing function.   
 * The function also creates consumer by calling consumer constructor with brokerURI, numMessages, and useTopics for inputs.
 * Created consumers are stored in consumer_vector.
 * Inputs: string
 * Return value: None
 */
void ActiveMQ_Solution::c_connect(string IP){
		//IP address parsing here
  		Url_parsing("http://" + IP + "/get_broker_list");
		bool useTopics = true;
     		bool sessionTransacted = false;
     		int numMessages = 1;
     	
         	for(int i=0; i<URL_v.size(); i++){
			  string brokerURI = URL_v[i];
                	  Consumer* consumer = new Consumer(brokerURI, numMessages, useTopics, sessionTransacted);
                  	  (*consumer).connect(brokerURI);
			  consumer_vector.push_back(consumer);
         	}

	
}

 /* Description: 
  * This is a receive function of ActiveMQ_Solution. It simply calls receive function for consumer for as many consumer it has in
  * consumer_vector vector.
  * Inputs: string tag, vector<string>* receive_vector
  * Return value: None
  */ 
void ActiveMQ_Solution::receive(string tag, vector<string>* receive_vector){
			
		for(unsigned int i=0; i<consumer_vector.size(); i++){
				(*consumer_vector[i]).receive(tag, receive_vector);
		}
}

/* Description: 
 * This is a receive_async function of ActiveMQ_Solution. It simply calls receive_async function for consumer for as many 
 * consumer it has in consumer_vector vector.
 * Inputs: string tag, vector<string>* receive_vector
 * Return value: None
 */ 
void ActiveMQ_Solution::receive_async(string tag, vector<string> receive_vector){

	        for(unsigned int i=0; i<consumer_vector.size(); i++){
				(*consumer_vector[i]).receive_async(tag, receive_vector);
		}

}


                                                                                                                                                                                                    
                                                                                                                                                                                                    
                                                                                                                                                                                                    
                                                                                                                                                                                                                                         
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
 

