#include "CppUnitTest.h"

#include "Day1-InverseCaptcha.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(Day1InverseCaptcha)
{
public:

	TEST_METHOD(sumOfDigitsMatchNextDigit_SimpleTests)
	{
		Assert::AreEqual(3u, sumOfDigitsMatchNextDigit("1122"));
		Assert::AreEqual(4u, sumOfDigitsMatchNextDigit("1111"));
		Assert::AreEqual(0u, sumOfDigitsMatchNextDigit("1234"));
		Assert::AreEqual(9u, sumOfDigitsMatchNextDigit("91212129"));
	}

};
