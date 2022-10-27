/**
 *  LibEV.cpp
 * 
 *  Test program to check AMQP functionality based on LibEV
 * 
 *  @author Emiel Bruijntjes <emiel.bruijntjes@copernica.com>
 *  @copyright 2015 - 2018 Copernica BV
 */

/**
 *  Dependencies
 */
#include <ev.h>
#include <amqpcpp.h>
#include <amqpcpp/libev.h>
#include <openssl/ssl.h>
#include <openssl/opensslv.h>

/**
 *  Custom handler
 */
class MyHandler : public AMQP::LibEvHandler
{
private:
    /**
     *  Method that is called when a connection error occurs
     *  @param  connection
     *  @param  message
     */
    virtual void onError(AMQP::TcpConnection *connection, const char *message) override
    {
        std::cout << "error: " << message << std::endl;
    }

    /**
     *  Method that is called when the TCP connection ends up in a connected state
     *  @param  connection  The TCP connection
     */
    virtual void onConnected(AMQP::TcpConnection *connection) override 
    {
        std::cout << "connected" << std::endl;
    }

    /**
     *  Method that is called when the TCP connection ends up in a ready
     *  @param  connection  The TCP connection
     */
    virtual void onReady(AMQP::TcpConnection *connection) override 
    {
        std::cout << "ready" << std::endl;
    }

    /**
     *  Method that is called when the TCP connection is closed
     *  @param  connection  The TCP connection
     */
    virtual void onClosed(AMQP::TcpConnection *connection) override 
    {
        std::cout << "closed" << std::endl;
    }

    /**
     *  Method that is called when the TCP connection is detached
     *  @param  connection  The TCP connection
     */
    virtual void onDetached(AMQP::TcpConnection *connection) override 
    {
        std::cout << "detached" << std::endl;
    }
    
    
public:
    /**
     *  Constructor
     *  @param  ev_loop
     */
    MyHandler(struct ev_loop *loop) : AMQP::LibEvHandler(loop) {}

    /**
     *  Destructor
     */
    virtual ~MyHandler() = default;
};


/**
 *  Main program
 *  @return int
 */
int main()
{
    // access to the event loop
    auto *loop = EV_DEFAULT;
    
    // handler for libev
    MyHandler handler(loop);

    // init the SSL library
#if OPENSSL_VERSION_NUMBER < 0x10100000L
    SSL_library_init();
#else
    OPENSSL_init_ssl(0, NULL);
#endif

    // make a connection
    AMQP::Address address("amqp://user:123@ihtiandr9.fvds.ru/");
//    AMQP::Address address("amqps://guest:guest@localhost/");
    AMQP::TcpConnection connection(&handler, address);
    
    // we need a channel too
    AMQP::TcpChannel channel(&connection);

    // create a temporary queue
    channel.declareQueue("glass",AMQP::durable).onSuccess([&connection, &channel, loop](const std::string &name, uint32_t messagecount, uint32_t consumercount) {
        
        // report the name of the temporary queue
        std::cout << "declared queue " << name << std::endl;
        
        // close the channel
        //channel.close().onSuccess([&connection, &channel]() {
        //    
        //    // report that channel was closed
        //    std::cout << "channel closed" << std::endl;
        //    
        //    // close the connection
        //    connection.close();
        //});
        
        // construct a timer that is going to publish stuff
       channel.publish("","glass", "cpp mesage00");
        //connection.close();
    });
    
    // run the loop
    ev_run(loop, 0);

    // done
    return 0;
}

