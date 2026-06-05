#ifndef TEST_UTILS_HPP
#define TEST_UTILS_HPP

#include <cmath>
#include <iostream>

inline int g_pass = 0;
inline int g_fail = 0;

#define EXPECT(cond) do {                                        \
    if (cond) ++g_pass;                                          \
    else { ++g_fail;                                             \
        std::cerr << "FAIL " << __FILE__ << ":" << __LINE__      \
                  << "  " << #cond << "\n"; }                    \
} while (0)

#define EXPECT_NEAR(a, b, tol) EXPECT(std::abs((a) - (b)) <= (tol))

#define EXPECT_THROW(expr) do {                                  \
    bool threw = false;                                          \
    try { expr; }                                               \
    catch (...) { threw = true; }                               \
    EXPECT(threw);                                               \
} while (0)

// Place TEST_SECTION("Name") at the top of a *_tests() function.
// On scope exit it prints how many checks passed/failed in that section.
struct TestSection {
    const char* name;
    int startPass;
    int startFail;
    TestSection(const char* n) : name(n), startPass(g_pass), startFail(g_fail) {}
    ~TestSection() {
        std::cout << name << ": " << (g_pass - startPass) << " passed, "
                  << (g_fail - startFail) << " failed\n";
    }
};

#define TEST_SECTION(name) TestSection _section(name)

#endif
