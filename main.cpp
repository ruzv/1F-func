#include <iostream>
#include "httplib.h"
#include "func.h"

using namespace std;

int main()
{
    cout << "listening on port 1234" << endl;
    sessions[0] = "dev";
    srand(time(NULL));

    httplib::Server svr;
    svr.Get("/session", session_handler).listen("0.0.0.0", 1234);

    return 0;
}
