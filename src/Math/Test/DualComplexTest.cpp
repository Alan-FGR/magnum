/*
    Copyright © 2010, 2011, 2012 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Magnum.

    Magnum is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Magnum is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include <sstream>
#include <TestSuite/Tester.h>

#include "Math/DualComplex.h"
#include "Math/DualQuaternion.h"

namespace Magnum { namespace Math { namespace Test {

class DualComplexTest: public Corrade::TestSuite::Tester {
    public:
        explicit DualComplexTest();

        void construct();
        void constructDefault();
        void constructFromVector();

        void constExpressions();

        void multiply();

        void lengthSquared();
        void length();
        void normalized();

        void complexConjugated();
        void dualConjugated();
        void conjugated();
        void inverted();
        void invertedNormalized();

        void rotation();
        void translation();
        void combinedTransformParts();

        void debug();
};

typedef Math::Deg<float> Deg;
typedef Math::Rad<float> Rad;
typedef Math::Complex<float> Complex;
typedef Math::Dual<float> Dual;
typedef Math::DualComplex<float> DualComplex;
typedef Math::Vector2<float> Vector2;

DualComplexTest::DualComplexTest() {
    addTests(&DualComplexTest::construct,
             &DualComplexTest::constructDefault,
             &DualComplexTest::constructFromVector,

             &DualComplexTest::constExpressions,

             &DualComplexTest::multiply,

             &DualComplexTest::lengthSquared,
             &DualComplexTest::length,
             &DualComplexTest::normalized,

             &DualComplexTest::complexConjugated,
             &DualComplexTest::dualConjugated,
             &DualComplexTest::conjugated,
             &DualComplexTest::inverted,
             &DualComplexTest::invertedNormalized,

             &DualComplexTest::rotation,
             &DualComplexTest::translation,
             &DualComplexTest::combinedTransformParts,

             &DualComplexTest::debug);
}

void DualComplexTest::construct() {
    DualComplex a({-1.0f, 2.5f}, {3.0f, -7.5f});
    CORRADE_COMPARE(a.real(), Complex(-1.0f, 2.5f));
    CORRADE_COMPARE(a.dual(), Complex(3.0f, -7.5f));
}

void DualComplexTest::constructDefault() {
    CORRADE_COMPARE(DualComplex(), DualComplex({1.0f, 0.0f}, {0.0f, 0.0f}));
    CORRADE_COMPARE(DualComplex().length(), 1.0f);
}

void DualComplexTest::constructFromVector() {
    CORRADE_COMPARE(DualComplex(Vector2(1.5f, -3.0f)), DualComplex({1.0f, 0.0f}, {1.5f, -3.0f}));
}

void DualComplexTest::constExpressions() {
    /* Default constructor */
    constexpr DualComplex a;
    CORRADE_COMPARE(a, DualComplex({1.0f, 0.0f}, {0.0f, 0.0f}));

    /* Value constructor */
    constexpr DualComplex b({-1.0f, 2.5f}, {3.0f, -7.5f});
    CORRADE_COMPARE(b, DualComplex({-1.0f, 2.5f}, {3.0f, -7.5f}));

    /* Vector constructor */
    constexpr DualComplex c(Vector2(-3.0f, 7.5f));
    CORRADE_COMPARE(c, DualComplex({}, {-3.0f, 7.5f}));

    /* Copy constructor */
    constexpr DualComplex d(b);
    CORRADE_COMPARE(d, DualComplex({-1.0f, 2.5f}, {3.0f, -7.5f}));
}

void DualComplexTest::multiply() {
    DualComplex a({-1.5f,  2.0f}, { 3.0f, -6.5f});
    DualComplex b({ 2.0f, -7.5f}, {-0.5f,  1.0f});;
    CORRADE_COMPARE(a*b, DualComplex({12.0f, 15.25f}, {1.75f, -9.0f}));
}

void DualComplexTest::lengthSquared() {
    DualComplex a({-1.0f, 3.0f}, {0.5f, -2.0f});
    CORRADE_COMPARE(a.lengthSquared(), 10.0f);
}

void DualComplexTest::length() {
    DualComplex a({-1.0f, 3.0f}, {0.5f, -2.0f});
    CORRADE_COMPARE(a.length(), 3.162278f);
}

void DualComplexTest::normalized() {
    DualComplex a({-1.0f, 3.0f}, {0.5f, -2.0f});
    DualComplex b({-0.316228f, 0.948683f}, {0.5f, -2.0f});
    CORRADE_COMPARE(a.normalized().length(), 1.0f);
    CORRADE_COMPARE(a.normalized(), b);
}

void DualComplexTest::complexConjugated() {
    DualComplex a({-1.0f,  2.5f}, {3.0f, -7.5f});
    DualComplex b({-1.0f, -2.5f}, {3.0f,  7.5f});
    CORRADE_COMPARE(a.complexConjugated(), b);
}

void DualComplexTest::dualConjugated() {
    DualComplex a({-1.0f, 2.5f}, { 3.0f, -7.5f});
    DualComplex b({-1.0f, 2.5f}, {-3.0f,  7.5f});
    CORRADE_COMPARE(a.dualConjugated(), b);
}

void DualComplexTest::conjugated() {
    DualComplex a({-1.0f,  2.5f}, { 3.0f, -7.5f});
    DualComplex b({-1.0f, -2.5f}, {-3.0f, -7.5f});
    CORRADE_COMPARE(a.conjugated(), b);
}

void DualComplexTest::inverted() {
    DualComplex a({-1.0f, 1.5f}, {3.0f, -7.5f});
    DualComplex b({-0.307692f, -0.461538f}, {4.384616f, -0.923077f});
    CORRADE_COMPARE(a*a.inverted(), DualComplex());
    CORRADE_COMPARE(a.inverted(), b);
}

void DualComplexTest::invertedNormalized() {
    DualComplex a({-0.316228f,  0.9486831f}, {     3.0f,    -2.5f});
    DualComplex b({-0.316228f, -0.9486831f}, {3.320391f, 2.05548f});

    std::ostringstream o;
    Error::setOutput(&o);
    DualComplex notInverted = DualComplex({-1.0f, -2.5f}, {}).invertedNormalized();
    CORRADE_VERIFY(notInverted != notInverted);
    CORRADE_COMPARE(o.str(), "Math::Complex::invertedNormalized(): complex number must be normalized\n");

    DualComplex inverted = a.invertedNormalized();
    CORRADE_COMPARE(a*inverted, DualComplex());
    CORRADE_COMPARE(inverted*a, DualComplex());
    CORRADE_COMPARE(inverted, b);
}

void DualComplexTest::rotation() {
    DualComplex a = DualComplex::rotation(Deg(120.0f));
    CORRADE_COMPARE(a.length(), 1.0f);
    CORRADE_COMPARE(a, DualComplex({-0.5f, 0.8660254f}, {0.0f, 0.0f}));
    CORRADE_COMPARE_AS(a.rotationAngle(), Deg(120.0f), Rad);
}

void DualComplexTest::translation() {
    Vector2 vec(1.5f, -3.5f);
    DualComplex a = DualComplex::translation(vec);
    CORRADE_COMPARE(a.length(), 1.0f);
    CORRADE_COMPARE(a, DualComplex({}, {1.5f, -3.5f}));
    CORRADE_COMPARE(a.translation(), vec);
}

void DualComplexTest::combinedTransformParts() {
    Vector2 translation = Vector2(-1.5f, 2.75f);
    DualComplex a = DualComplex::translation(translation)*DualComplex::rotation(Deg(23.0f));
    DualComplex b = DualComplex::rotation(Deg(23.0f))*DualComplex::translation(translation);

    CORRADE_COMPARE_AS(a.rotationAngle(), Deg(23.0f), Rad);
    CORRADE_COMPARE_AS(b.rotationAngle(), Deg(23.0f), Rad);
    CORRADE_COMPARE(a.translation(), translation);
    CORRADE_COMPARE(b.translation(), Complex::rotation(Deg(23.0f)).transformVector(translation));
}

void DualComplexTest::debug() {
    std::ostringstream o;

    Debug(&o) << DualComplex({-1.0f, -2.5f}, {-3.0f, -7.5f});
    CORRADE_COMPARE(o.str(), "DualComplex({-1, -2.5}, {-3, -7.5})\n");
}

}}}

CORRADE_TEST_MAIN(Magnum::Math::Test::DualComplexTest)
