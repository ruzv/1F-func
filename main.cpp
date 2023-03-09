#include <iostream>
#include "httplib.h"

using namespace std;

static string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
string sessions[1000];
int session_count = 0;

int main() {
    cout<<"listening on port 1234"<<endl;
    sessions[0] = "dev";
    session_count = 1;
    srand(time(NULL));
    httplib::Server svr;
    svr.Get(
        "/session", 
        [](const httplib::Request &req, httplib::Response &res) {
            string sess = req.get_header_value("authorization");
            if (sess == "" ) {
                res.status = 400;
                res.set_content(
                    "{\"code\":400,\"error\": \"missing authorization header\"}", 
                    "application/json"
                );
                return;
            }
            bool found = false;
            for (int i = 0; i < 1000; i++) {
                if (sessions[i] == sess) {
                    found = true;
                }
            }
            if (!found) {
                res.status = 401;
                res.set_content(
                    "{\"code\":401,\"message\": \"invalid session\"}", 
                    "application/json"
                );
                return;
            }
            if (!req.has_param("uid")) {
                res.status = 200;
                res.set_content(
                    "{\"code\":200,\"session\": \""+req.get_header_value("authorization")+"\"}", 
                    "application/json"
                );
                return;
            }
            int uid = atoi(req.get_param_value("uid").c_str());
            if (uid <= 0 || uid >= 1000) {
                res.status = 400;
                res.set_content(
                    "{\"code\":400,\"error\": \"invalid user id\"}", 
                    "application/json"
                );
                return;
            }
            uid -= 1;
            if (sessions[uid] == "") {
                sessions[uid].resize(32);
                for (int i = 0; i < 32; i++) {
                    sessions[uid][i] = charset[rand() % charset.length()];
                }
            }
            res.status = 200;
            res.set_content(
                "{\"code\":200,\"session\": \""+sessions[uid]+"\"}", 
                "application/json"
            );
        }
    ).listen("0.0.0.0", 1234);
    return 0;
}
