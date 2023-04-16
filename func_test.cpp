#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "func.h"
#include "httplib.h"

TEST(SessionHandler, MissingAuth)
{
    httplib::Request req;
    httplib::Response res;

    session_handler(req, res);

    EXPECT_EQ(res.status, 400);
    EXPECT_EQ(res.get_header_value("Content-Type"), "application/json");
    EXPECT_EQ(res.body, "{\"code\":400,\"error\": \"missing authorization header\"}");
}

TEST(SessionHandler, SessNotFound)
{
    httplib::Request req = httplib::Request();
    req.headers.insert({"authorization", "123"});

    httplib::Response res;

    session_handler(req, res);

    EXPECT_EQ(res.status, 401);
    EXPECT_EQ(res.get_header_value("Content-Type"), "application/json");
    EXPECT_EQ(res.body, "{\"code\":401,\"message\": \"invalid session\"}");
}

TEST(SessionHandler, CurrentSess)
{
    sessions[0] = "dev";

    httplib::Request req = httplib::Request();
    req.headers.insert({"authorization", "dev"});

    httplib::Response res;

    session_handler(req, res);

    EXPECT_EQ(res.status, 200);
    EXPECT_EQ(res.get_header_value("Content-Type"), "application/json");
    EXPECT_EQ(res.body, "{\"code\":200,\"session\": \"dev\"}");
}

TEST(SessionHandler, UIDParamOutOfBounds)
{
    sessions[0] = "dev";

    httplib::Request req = httplib::Request();
    req.headers.insert({"authorization", "dev"});
    req.params.insert({"uid", "-1"});

    httplib::Response res;

    session_handler(req, res);

    EXPECT_EQ(res.status, 400);
    EXPECT_EQ(res.get_header_value("Content-Type"), "application/json");
    EXPECT_EQ(res.body, "{\"code\":400,\"error\": \"invalid user id\"}");
}

TEST(SessionHandler, InvalidUIDParam)
{
    sessions[0] = "dev";

    httplib::Request req = httplib::Request();
    req.headers.insert({"authorization", "dev"});
    req.params.insert({"uid", "invalid"});

    httplib::Response res;

    session_handler(req, res);

    EXPECT_EQ(res.status, 400);
    EXPECT_EQ(res.get_header_value("Content-Type"), "application/json");
    EXPECT_EQ(res.body, "{\"code\":400,\"error\": \"invalid user id\"}");
}

TEST(SessionHandler, NewSess)
{
    sessions[0] = "dev";

    httplib::Request req = httplib::Request();
    req.headers.insert({"authorization", "dev"});
    req.params.insert({"uid", "5"});

    httplib::Response res;

    session_handler(req, res);

    EXPECT_EQ(res.status, 200);
    EXPECT_EQ(res.get_header_value("Content-Type"), "application/json");
    EXPECT_THAT(res.body, testing::MatchesRegex("{\"code\":200,\"session\": \"[a-zA-Z0-9]{32}\"}"));
}

TEST(SessionHandler, ExistingSess)
{
    sessions[0] = "dev";
    sessions[1] = "existing_sess";

    httplib::Request req = httplib::Request();
    req.headers.insert({"authorization", "dev"});
    req.params.insert({"uid", "2"});

    httplib::Response res;

    session_handler(req, res);

    EXPECT_EQ(res.status, 200);
    EXPECT_EQ(res.get_header_value("Content-Type"), "application/json");
    EXPECT_EQ(res.body, "{\"code\":200,\"session\": \"existing_sess\"}");
}
