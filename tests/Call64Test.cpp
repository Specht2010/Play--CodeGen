#include "Call64Test.h"
#include "MemStream.h"

#define CONSTANT_1		(0x0000084108302989ull)
#define CONSTANT_2		(0x0017227878892871ull)

CCall64Test::CCall64Test()
{

}

CCall64Test::~CCall64Test()
{

}

uint64 CCall64Test::Add64(uint64 v1, uint64 v2)
{
	return v1 + v2;
}

uint64 CCall64Test::Sub64(uint64 v1, uint64 v2)
{
	return v1 - v2;
}

void CCall64Test::Compile(Jitter::CJitter& jitter)
{
	Framework::CMemStream codeStream;
	jitter.SetStream(&codeStream);

	jitter.Begin();
	{
		//Result 0
		{
			jitter.PushRel64(offsetof(CONTEXT, value0));
			jitter.PushRel64(offsetof(CONTEXT, value1));
			jitter.Call(reinterpret_cast<void*>(&CCall64Test::Add64), 2, Jitter::CJitter::RETURN_VALUE_64);
			jitter.PullRel64(offsetof(CONTEXT, result0));
		}
		
		//Result 1
		{
			jitter.PushCst64(CONSTANT_1);
			jitter.PushCst64(CONSTANT_2);
			jitter.Call(reinterpret_cast<void*>(&CCall64Test::Sub64), 2, Jitter::CJitter::RETURN_VALUE_64);
			jitter.PullRel64(offsetof(CONTEXT, result1));
		}
	}
	jitter.End();

	m_function = CMemoryFunction(codeStream.GetBuffer(), codeStream.GetSize());
}

void CCall64Test::Run()
{
	CONTEXT context;
	memset(&context, 0, sizeof(CONTEXT));
	
	context.value0 = CONSTANT_1;
	context.value1 = CONSTANT_2;

	m_function(&context);

	TEST_VERIFY(context.result0 == (CONSTANT_1 + CONSTANT_2));
	TEST_VERIFY(context.result1 == (CONSTANT_1 - CONSTANT_2));
}
