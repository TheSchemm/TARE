#ifndef _JSON_SERVER_H_
#define _JSON_SERVER_H_


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


class json_server {
    class json_http_handler;
public:
    typedef http::server<json_http_handler> json_http_server;
    class json_request;


    typedef typename std::function<int(json_request& req, ptree& root)> json_request_callback;


private:

    std::shared_ptr<json_http_server> _svr;
    

public:
    //class json_request_handler;

    json_server(std::string host, std::string port, json_request_callback); 
    void run();






    class json_request{
    public:
        typedef std::map<std::string,std::string>        query_map;
        typedef std::deque<std::string>                 query_path;

    private:
        query_path  _path;
        query_map _query;


    public:
        class malformed_request_exception: public std::exception{
        public:
            const char * what() const noexcept{
                return "json_request: malformed_string";
            }
        };

        json_request(const std::string& url);
        std::string pop();

        void json(ptree& tree);

        const query_path& path();
        const query_map&  queries();

    };

    
    class json_request_handler{

    public:

        virtual int operator()(json_request& req, ptree& root) = 0;

        ~json_request_handler(){}

    };



private:


    class json_http_handler{
    public:
        //typedef std::function<json_http_handler::response::status_type(const uri::uri& url, ptree& pt)> function_callback;


        typedef json_request::query_path query_path;
        typedef json_request::query_map query_map;
        typedef std::function<int(query_path&, const query_map&, ptree&)> query_handler;



    private:
        std::map<std::string,int> handlers;
        json_server::json_request_callback callback;

    public:
        json_http_handler(json_server::json_request_callback cb){
            callback = cb;
        }

        void operator() (json_server::json_http_server::request const &request, json_server::json_http_server::response &response);
            

        void log(json_server::json_http_server::string_type const &info);

    };

    json_http_handler _http_handler;

};

#endif

