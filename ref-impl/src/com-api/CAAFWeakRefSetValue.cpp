//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CAAFWeakRefSetValue.cpp,v 1.6.2.1 2004/03/11 18:58:44 stuart_hc Exp $ $Name:  $
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



#include "CAAFWeakRefSetValue.h"
#include "ImplAAFWeakRefSetValue.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>


// CLSID for AAFWeakRefSetValue 
// {b1078f01-6897-11d4-a812-8f71e157e3e3}
EXTERN_C const CLSID CLSID_AAFWeakRefSetValue = { 0xb1078f01, 0x6897, 0x11d4, { 0xa8, 0x12, 0x8f, 0x71, 0xe1, 0x57, 0xe3, 0xe3 } };





CAAFWeakRefSetValue::CAAFWeakRefSetValue (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFPropertyValue (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplAAFWeakRefSetValue * newRep;
      newRep = new ImplAAFWeakRefSetValue;
      assert (newRep);
      InitRep (newRep);
    }
}


CAAFWeakRefSetValue::~CAAFWeakRefSetValue ()
{
}


//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CAAFWeakRefSetValue::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IAAFWeakRefSetValue)) 
    { 
        *ppvObj = (IAAFWeakRefSetValue *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFPropertyValue::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(AAFWeakRefSetValue)

