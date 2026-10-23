#include <pspkernel.h>
#include <psptest.h>
#include <vector>
#include <string>
#include <algorithm>
#include <memory>
#include <optional>
#include <typeinfo>
#include <stdexcept>
PSP_MODULE_INFO("PSPTEST C++ Runtime", 0, 1, 0);

namespace {
struct Polymorphic {
    virtual ~Polymorphic() = default;
    virtual int value() const { return 1; }
};
struct Derived final : Polymorphic {
    int value() const override { return 9; }
};
struct Startup {
    Startup() : initialized(0x426) {}
    int initialized;
};
static Startup startup;
}

PSPTEST_TEST(cxx_containers) {
    std::vector<int> values{4,1,3,2};
    std::sort(values.begin(),values.end());
    PSPTEST_ASSERT_EQ_INT(test,4,values.size());
    PSPTEST_ASSERT_EQ_INT(test,1,values.front());
    PSPTEST_ASSERT_EQ_INT(test,4,values.back());
    std::string text="psp";
    text+="dev";
    PSPTEST_ASSERT_EQ_INT(test,6,text.size());
    PSPTEST_ASSERT_TRUE(test,text=="pspdev");
}
PSPTEST_TEST(cxx_polymorphism_rtti) {
    auto obj=std::make_unique<Derived>();
    Polymorphic *base=obj.get();
    PSPTEST_ASSERT_EQ_INT(test,9,base->value());
    PSPTEST_ASSERT_TRUE(test,dynamic_cast<Derived *>(base)!=nullptr);
    PSPTEST_ASSERT_TRUE(test,typeid(*base)==typeid(Derived));
}
PSPTEST_TEST(cxx_exceptions) {
    int caught=0;
    try { throw std::runtime_error("PSP GCC 16 exception"); }
    catch (const std::runtime_error &e) { caught=e.what()[0]=='P'; }
    PSPTEST_ASSERT_EQ_INT(test,1,caught);
}
PSPTEST_TEST(cxx_static_initialization) {
    PSPTEST_ASSERT_EQ_INT(test,0x426,startup.initialized);
}
PSPTEST_TEST(cxx_optional) {
    std::optional<int> value=7;
    PSPTEST_ASSERT_TRUE(test,value.has_value());
    PSPTEST_ASSERT_EQ_INT(test,7,*value);
}
static const PspTestCase cases[]={
    PSPTEST_CASE(cxx_containers), PSPTEST_CASE(cxx_polymorphism_rtti),
    PSPTEST_CASE(cxx_exceptions), PSPTEST_CASE(cxx_static_initialization),
    PSPTEST_CASE(cxx_optional)
};
extern "C" {
PSPTEST_MODULE("pspsdk/cxx-runtime", cases, PSP_THREAD_ATTR_USER, NULL, NULL)
}
