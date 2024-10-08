// String formatting and related functions

/*
  Author: Martin Eden
  Last mod.: 2024-10-08
*/

/*
  Actually we're using string formatting implementation from
  snprintf().

  Point of creating this module was not new functionality
  but presenting functionality in convenient and sane way.
*/

#include "me_String.h"

#include <me_BaseTypes.h>
#include <me_ManagedMemory.h>
#include <me_MemorySegment.h> // TMemorySegment

#include <stdarg.h> // va_list, va_start, va_end
#include <stdio.h> // vsnprintf()

using namespace me_String;

using
  me_ManagedMemory::TManagedMemory,
  me_MemorySegment::TMemorySegment;

/*
  Return our data span
*/
me_MemorySegment::TMemorySegment TString::GetData()
{
  return Data.GetData();
}

// Copy from memory segment
TBool TString::CopyFrom(
  me_MemorySegment::TMemorySegment Memseg
)
{
  return Data.LoadFrom(Memseg);
}

// Copy from ASCIIZ
TBool TString::CopyFrom(
  const TChar * Asciiz
)
{
  return Data.LoadFrom(Asciiz);
}

// Copy from our specie
TBool TString::CopyFrom(
  TString String
)
{
  return Data.LoadFrom(&String.Data);
}

// Copy to provided memory segment
TBool TString::CopyTo(
  me_ManagedMemory::TManagedMemory * Memseg
)
{
  return Memseg->LoadFrom(&Data);
}

/*
  Format string

  Implementation uses vsnprintf().
*/
void TString::Format(
  const TChar * FormatStr,
  ...
)
{
  /*
    Implementation will use vsnprintf() from <stdio.h>.

    That fancy function accepts TUint_2 as buffer capacity.
    And returns length of formatted string in TSint_2.. Fuck you C!

    Anyway.. We don't know how long final string will be.
    We can obligate you to estimate buffer size. But we won't.
    Because you can't too.

    Instead we will do two calls to vsnprintf(). First with empty
    buffer just to get length of formatted string. Then we'll
    allocate memory and do the real call.

    And that's not all!

    Our buffer don't need trailing zero byte from so-called "string".
    So we'll do another memory allocation to copy that chunk without
    zero byte. 2*N memory consumption at runtime..

    Great API design guys!
  */

  // "..." is now "Args"
  va_list Args;
  // Return code from vsnprintf()
  TSint_2 ReturnCode;
  {
    // "Args" are starting after last named argument. Which is "FormatStr"
    va_start(Args, FormatStr);

    ReturnCode = vsnprintf(0, 0, FormatStr, Args);

    va_end(Args);
  }

  // If snprintf() failed, return
  if (ReturnCode < 0)
  {
    return;
  }

  // Assert: ReturnCode >= 0

  // Positive return code is theoretical result string length.
  TUint_2 MemoryRequired = (TUint_2) ReturnCode;

  /*
    Okay, let's allocate memory and do _real_ call!
  */

  TManagedMemory Asciiz;

  // If no memory for ASCIIZ, return
  // ASCIIZ requires one more byte for tail
  if (!Asciiz.ResizeTo(MemoryRequired + 1))
  {
    return;
  }

  /*
    We don't expect 2nd call to fail. It's same format string
    and arguments. Only buffer address and buffer size changed.
    So we're not checking for return code.
  */
  {
    va_start(Args, FormatStr);

    ReturnCode =
      vsnprintf(
        (TChar *) Asciiz.GetData().Start.Addr,
        Asciiz.GetSize(),
        FormatStr,
        Args
      );

    va_end(Args);
  }

  // Finally! But if no memory for <Data>, return
  if (!Data.LoadFrom(&Asciiz))
    return;

  // Trim tail zero byte from ASCIIZ
  Data.ResizeTo(Data.GetSize() - 1);
}

/*
  2024-10-04
  2024-10-07
*/
