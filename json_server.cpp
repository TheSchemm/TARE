#include "json_server.h"



json_server::json_server(std::string host, std::string port, json_request_callback callback):_http_handler(callback){

    json_http_server::options options(_http_handler);
    _svr = shared_ptr<json_http_server>(new json_http_server(options.address(host).port(port)));
    

} 

void json_server::run(){
	_svr->run();
}

json_server::json_request::json_request(const std::string& url){
    static const std::regex url_validation("((/(([^/+&?=\n\r]*)))+)((\\?([^&]+)(=([^&]+)))((&([^&]+)(=([^&]+)))*)?)?");
    static const std::regex query_search("([^=&?]+)=([^=&?]+)");


    if(!regex_match(url,url_validation)){
        throw malformed_request_exception();
    }

    size_t query_start = url.find('?');
    std::string p = url.substr(0,query_start);

    std::string q;
    if(query_start != std::string::npos){
        q = url.substr(query_start+1);
    }


    boost::split(_path, p, boost::is_any_of("/"));
    _path.pop_front();



    std::smatch match;
    if(!q.empty()){
        while (std::regex_search (q,match,query_search)) {

            if(match.size() != 3){
                throw malformed_request_exception();
            }
            //cout << match[1] << " " << match[2];
            _query[match[1]] = match[2];

            q = match.suffix().str();
        }
    }
}


std::string json_server::json_request::pop(){
	//std::copy(_path.begin(), _path.end(), std::ostream_iterator<std::string>(std::cout, " "));
    std::string temp = _path.front();
    _path.pop_front();
    return temp;
}

const json_server::json_request::query_path& json_server::json_request::path(){
    return _path;
}
const json_server::json_request::query_map& json_server::json_request::queries(){
    return _query;
}



void json_server::json_request::json(ptree& tree){
	
	ptree p;

	for(std::string s: _path){
		ptree temp;
		temp.put<std::string>("", s);
		p.push_back(std::make_pair("",temp));
	}
	tree.push_back(std::make_pair("path",p));

	ptree q;
	for(auto temp:_query){
		q.put<std::string>(temp.first, temp.second);
	}

	tree.push_back(std::make_pair("query",q));

	
}


void json_server::json_http_handler::operator() (json_server::json_http_server::request const &request,
                 json_server::json_http_server::response &response) {

	
	std::string dest = destination(request);




    ptree root;

    json_server::json_http_server::response::status_type status = json_server::json_http_server::response::ok;

    try{

        json_request req(dest);

        if(callback(req, root)){
        	status = json_server::json_http_server::response::bad_request;
        }



    }catch(std::exception& ex){
        status = json_server::json_http_server::response::bad_request;
    }


    std::stringstream ss;
    write_json(ss, root);

    response = json_server::json_http_server::response::stock_reply(status, ss.str());

    



    static const json_server::json_http_server::response::header_type content_type = {"Content-Type","application/json"};
    response.headers.push_back(content_type);
}

void json_server::json_http_handler::log(json_server::json_http_server::string_type const &info) {
    std::cerr << "ERROR: " << info << '\n';

}


