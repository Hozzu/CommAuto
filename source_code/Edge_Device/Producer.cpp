#include <iostream>
#include "ActiveMQ.h"

/* Description: 
 * This is a main function for Producer. The user can connect, and send data to brokers. 
 * User has a right to set a tag and the content of data. These can be changed by the user.   	      * Inputs: None 
 * Return value: None
 */
int main(int argc AMQCPP_UNUSED, char* argv[] AMQCPP_UNUSED){

	ActiveMQ_Solution active_producer;	
	active_producer.p_connect("147.47.208.44:8080");
	std::string tag = "tag1";
	std::string data = "tag1 data\n";
 	for(int i=0; i<5000; i++){
		active_producer.send(tag, data);
	}

}
                                                                                                                                                                                                   
                                                                                                                                                                                                    
                                                                                                                                                                                                                                         
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
                                                                                                                                                                                                                                          
 

