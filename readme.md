# 1F functions code

## building

code uses external http lib https://github.com/yhirose/cpp-httplib version
v0.12.0 to handle requests. all lib code is contained in a single file header
file, that is included in this repo (`httplib.h`)

`cpp-httplib` uses C++11, so make sure to specify that when compiling

to build the function just compile. i used

```sh
g++ -std=c++11 main.cpp
```

but any other compiler should work as well.

## server

the function is a small http handler for route `/session` (no trailing slash).

server listens on a hard coded port 1234, so if you have anything already
running on that port, stop it first.

requests are required to have a single `authorization` header whose value is the
"sesijas marķieris"

at startup a single session is present with the marker `dev`. with this you
should be able to create up to 1000 new sessions.

user id is passed as a request param `uid`, that must be a valid int in the
range [1 999]

user id 1 corresponds with the session `dev`

to stop the server just kill it.

## examples

```sh
curl "localhost:1234/session" \
  -H "authorization: dev"
```

```sh
curl "localhost:1234/session?uid=3" \
  -H "authorization: dev"
```

# ᶘ ᵒᴥᵒᶅ go nuts
