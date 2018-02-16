#include "../router.hpp"

#include <catch.hpp>

namespace msrv {
namespace router_tests {

#define DEFINE_DUMMY_FACTORY(Name) \
    class Name : public RequestHandlerFactory \
    { \
        virtual RequestHandlerPtr createHandler(Request*) override \
        { \
            return RequestHandlerPtr(); \
        } \
    }

DEFINE_DUMMY_FACTORY(GetRoot);
DEFINE_DUMMY_FACTORY(GetSimple);
DEFINE_DUMMY_FACTORY(GetWithParam);
DEFINE_DUMMY_FACTORY(GetWithLongParam);
DEFINE_DUMMY_FACTORY(GetHandler);
DEFINE_DUMMY_FACTORY(PostHandler);

TEST_CASE("router")
{
    Router router;

    router.defineRoute(HttpMethod::GET, "", std::make_unique<GetRoot>());
    router.defineRoute(HttpMethod::GET, "simple", std::make_unique<GetSimple>());
    router.defineRoute(HttpMethod::GET, "param/:param", std::make_unique<GetWithParam>());
    router.defineRoute(HttpMethod::GET, "lparam/:param*", std::make_unique<GetWithLongParam>());
    router.defineRoute(HttpMethod::GET, "data", std::make_unique<GetHandler>());
    router.defineRoute(HttpMethod::POST, "data", std::make_unique<PostHandler>());

    SECTION("GET root")
    {
        Request request(HttpMethod::GET, "/");
        auto result = router.dispatch(&request);
        REQUIRE(result->factory != nullptr);
        REQUIRE(typeid(*result->factory) == typeid(GetRoot));
    }

    SECTION("GET simple")
    {
        Request request(HttpMethod::GET, "/simple");
        auto result = router.dispatch(&request);
        REQUIRE(result->factory != nullptr);
        REQUIRE(typeid(*result->factory) == typeid(GetSimple));
    }

    SECTION("GET with param")
    {
        Request request(HttpMethod::GET, "/param/hello");
        auto result = router.dispatch(&request);

        REQUIRE(result->factory != nullptr);
        REQUIRE(typeid(*result->factory) == typeid(GetWithParam));

        auto param = result->params.find("param");
        REQUIRE(param != result->params.end());
        REQUIRE(param->second == "hello");
    }

    SECTION("GET with long param")
    {
        Request request(HttpMethod::GET, "/lparam/hello/world");
        auto result = router.dispatch(&request);

        REQUIRE(result->factory != nullptr);
        REQUIRE(typeid(*result->factory) == typeid(GetWithLongParam));

        auto param = result->params.find("param");
        REQUIRE(param != result->params.end());
        REQUIRE(param->second == "hello/world");
    }

    SECTION("GET and POST with different handlers")
    {
        Request getRequest(HttpMethod::GET, "/data");
        auto result = router.dispatch(&getRequest);
        REQUIRE(result->factory != nullptr);
        REQUIRE(typeid(*result->factory) == typeid(GetHandler));

        Request postRequest(HttpMethod::POST, "/data");
        result = router.dispatch(&postRequest);
        REQUIRE(result->factory != nullptr);
        REQUIRE(typeid(*result->factory) == typeid(PostHandler));
    }
}

}}
