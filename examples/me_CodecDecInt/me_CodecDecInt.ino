// [me_CodecDecInt] test/demo

/*
  Author: Martin Eden
  Last mod.: 2024-12-19
*/

#include <me_CodecDecInt.h>

#include <me_BaseTypes.h>
#include <me_Console.h>
#include <me_Uart.h>
#include <me_UartSpeeds.h>
#include <me_MemorySegment.h>

void setup()
{
  me_Uart::Init(me_UartSpeeds::Bps_115k);

  Console.Print("[me_CodecDecInt] Okay, we are here.");
  RunTest();
  Console.Print("[me_CodecDecInt] Done.");
}

void loop()
{
}

// --

void RunTest()
{
  {
    Console.Indent();
    Console.Print("* Formatting integers");

    {
      Console.Indent();

      using
        me_MemorySegment::TMemorySegment,
        me_MemorySegment::Freetown::FromAddrSize,
        me_CodecDecInt::FormatUint_4,
        me_CodecDecInt::FormatSint_4;

      TUint_1 BufferSize = 20;
      TUint_1 Buffer[BufferSize];
      TMemorySegment BuffSeg = FromAddrSize((TUint_2) &Buffer, BufferSize);

      FormatUint_4(BuffSeg, 123);
      Console.Print(BuffSeg);

      FormatSint_4(BuffSeg, (TSint_4) 0x80000000);
      Console.Print(BuffSeg);

      FormatUint_4(BuffSeg, (TUint_4) 0x80000000);
      Console.Print(BuffSeg);

      Console.Unindent();
    }

    Console.Unindent();
  }
}

/*
  2024-10 # # # #
  2024-12-19
*/