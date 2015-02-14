//#include "json_server.h"
#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/network/protocol/http/server.hpp>
#include <boost/network/uri.hpp>
#include <boost/network/uri/uri_io.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/property_tree/ptree.hpp>


#include <deque>
#include <exception>
#include <iostream>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <regex>
#include <sstream>
#include <string>
#include <string>


#include "json_server.h"
#include "AirDB.h"

using namespace std;



using boost::asio::ip::tcp;
using boost::property_tree::ptree;
using boost::property_tree::ptree;
using boost::property_tree::read_json;
using boost::property_tree::read_json;
using boost::property_tree::write_json;
using boost::property_tree::write_json;
using namespace boost::network::http;
using namespace boost::network;


class QueryReturn{

public:
    int query(json_server::json_request& req, ptree& root){
        req.json(root);
        return 0;
    }
};








class TravelAgencyRepository{
    shared_ptr<json_server> server;
    typedef typename std::map<std::string, json_server::json_request_callback> callback_map;
    callback_map callbacks;
public:
    TravelAgencyRepository(std::string host, std::string port){

        json_server::json_request_callback callback = std::bind( &TravelAgencyRepository::request, this, std::placeholders::_1, std::placeholders::_2 );

        server = shared_ptr<json_server>(new json_server(host, port, callback));
    }


    void add(std::string dir, json_server::json_request_callback callback){
        callbacks[dir] = callback;
    }

    int remove(std::string dir){
        callback_map::iterator it = callbacks.find(dir);
        if(it != callbacks.end()){
            callbacks.erase(it);
            return 0;
        }
        return 1;
    }


    int request(json_server::json_request& req, ptree& root){
        std::string dir = req.pop();
        callback_map::iterator it = callbacks.find(dir);
        if(it != callbacks.end()){
            return it->second(req, root);
        }

        return 1;
    }





    void run(){
        server->run();
    }

};

int main(int arg, char * argv[]) {
    



    TravelAgencyRepository tar("localhost","8000");

    QueryReturn query;
    tar.add("query",std::bind( &QueryReturn::query, &query, std::placeholders::_1, std::placeholders::_2));

    tar.run();



    

    return 0;
}