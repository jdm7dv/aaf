//=---------------------------------------------------------------------=
//
// $Id: CAAFRawStorageTest.cpp,v 1.4 2006/10/10 17:52:36 akharkev Exp $ $Name:  $
//
// The contents of this file are subject to the AAF SDK Public
// Source License Agreement (the "License"); You may not use this file
// except in compliance with the License.  The License is available in
// AAFSDKPSL.TXT, or you may obtain a copy of the License from the AAF
// Association or its successor.
//
// Software distributed under the License is distributed on an "AS IS"
// basis, WITHOUT WARRANTY OF ANY KIND, either express or implied.  See
// the License for the specific language governing rights and limitations
// under the License.
//
// The Original Code of this file is Copyright 1998-2004, Licensor of the
// AAF Association.
//
// The Initial Developer of the Original Code of this file and the
// Licensor of the AAF Association is Avid Technology.
// All rights reserved.
//
//=---------------------------------------------------------------------=

#include "AAFTypes.h" //Use #include "AAF.h" for functional module test.
#include "AAFResult.h"
#include "ModuleTest.h"

// Required function prototype.
extern "C" HRESULT CAAFRawStorage_test(testMode_t mode,
    aafUID_t fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);

HRESULT CAAFRawStorage_test(testMode_t /* mode */,
    aafUID_t /* fileKind */,
    testRawStorageType_t /* rawStorageType */,
    aafProductIdentification_t /* productID */)
{
  // There is currently no support in the toolkit to make use if
  // non-Random Raw Storages (e.g. streams).  When that's in, we can
  // do some (non-random) raw storage tests.
  return AAFRESULT_NOT_IN_CURRENT_VERSION;
}
