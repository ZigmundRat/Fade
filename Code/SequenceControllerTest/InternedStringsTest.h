#include "InternedStrings.h"

class InternedStringsTest
{
  static void TestAddNewString()
  {
    StringChunk stringChunk(10);

    const char* pString = stringChunk.LookupOrAdd("Testy");

    Assert::AreEqual("Testy", pString);
  }

  static void TestAddExistingStringReturnsSameInstance()
  {
    StringChunk stringChunk(10);

    char buffer1[32];
    char buffer2[32];

    strcpy(buffer1, "Testy");
    strcpy(buffer2, "Testy");

    const char* pString1 = stringChunk.LookupOrAdd(buffer1);

    const char* pString2 = stringChunk.LookupOrAdd(buffer2);

    Assert::AreEqual((void*) pString1, (void*) pString2);
  }

  static void TestTwoStringsWork()
  {
    StringChunk stringChunk(20);

    Assert::AreEqual(stringChunk.LookupOrAdd("testa"), stringChunk.LookupOrAdd("testa"));
    Assert::AreEqual(stringChunk.LookupOrAdd("testb"), stringChunk.LookupOrAdd("testb"));

  }

  static void TestFullChunk()
  {
    StringChunk stringChunk(10);

    const char* pString1 = stringChunk.LookupOrAdd("12345");

    const char* pString2 = stringChunk.LookupOrAdd("123");    
    Assert::AreEqual(0, (void*)pString2);

    const char* pString3 = stringChunk.LookupOrAdd("12");
    Assert::AreEqual("12", pString3);
  }




  static void TestMultipleInternsWorkTogether()
  {
    InternedStrings internedStrings;

    const char* pointers[128];

    for (int i = 0; i < 100; i++)
    {
      char buffer[128];
      sprintf(buffer, "String # %d", i);

      pointers[i] = internedStrings.LookupOrAdd(buffer);
    }

    for (int i = 0; i < 100; i++)
    {
      char buffer[128];
      sprintf(buffer, "String # %d", i);

      Assert::AreEqual(pointers[i],internedStrings.LookupOrAdd(buffer));
    }
  }

public:
  static void Run()
  {
    TestAddNewString();
    TestAddExistingStringReturnsSameInstance();
    TestTwoStringsWork();
    TestFullChunk();

    
    TestMultipleInternsWorkTogether();
  }

};