//=---------------------------------------------------------------------=
//
// This file was GENERATED for the AAF SDK
//
// $Id: CEnumAAFMobSlots.cpp,v 1.6.2.1 2004/03/11 18:58:44 stuart_hc Exp $ $Name:  $
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



#include "CEnumAAFMobSlots.h"
#include "ImplEnumAAFMobSlots.h"
#include "AAFResult.h"
#include "CAAFEnumValidation.h"

#include "OMAssertions.h"
#include "OMExceptions.h"

#include <assert.h>
#include <string.h>


#include "CAAFMobSlot.h"
#include "ImplAAFMobSlot.h"

// CLSID for EnumAAFMobSlots 
// {B1A21389-1A7D-11D2-BF78-00104BC9156D}
EXTERN_C const CLSID CLSID_EnumAAFMobSlots = { 0xB1A21389, 0x1A7D, 0x11D2, { 0xBF, 0x78, 0x00, 0x10, 0x4B, 0xC9, 0x15, 0x6D } };





CEnumAAFMobSlots::CEnumAAFMobSlots (IUnknown * pControllingUnknown, aafBool doInit)
  : CAAFRoot (pControllingUnknown, kAAFFalse)
{
  if (doInit)
    {
      ImplEnumAAFMobSlots * newRep;
      newRep = new ImplEnumAAFMobSlots;
      assert (newRep);
      InitRep (newRep);
    }
}


CEnumAAFMobSlots::~CEnumAAFMobSlots ()
{
}


HRESULT STDMETHODCALLTYPE
    CEnumAAFMobSlots::NextOne (IAAFMobSlot ** ppMobSlot)
{
  HRESULT hr;

  ImplEnumAAFMobSlots * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFMobSlots*> (pO);
  assert (ptr);

  //
  // set up for ppMobSlot
  //
  ImplAAFMobSlot * internalppMobSlot = NULL;
  ImplAAFMobSlot ** pinternalppMobSlot = NULL;
  if (ppMobSlot)
    {
      pinternalppMobSlot = &internalppMobSlot;
    }

  try
    {
      hr = ptr->NextOne
       (pinternalppMobSlot);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UNEXPECTED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNEXPECTED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // cleanup for ppMobSlot
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppMobSlot)
        {
          pUnknown = static_cast<IUnknown *> (internalppMobSlot->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IAAFMobSlot, (void **)ppMobSlot);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppMobSlot->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFMobSlots::Next (aafUInt32  count,
        IAAFMobSlot ** ppMobSlots,
        aafUInt32 *  pNumFetched)
{
  HRESULT hr;

  ImplEnumAAFMobSlots * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFMobSlots*> (pO);
  assert (ptr);

  //
  // set up for ppMobSlots
  //
  ImplAAFMobSlot ** internalppMobSlots = NULL;
  assert (count >= 0);
  internalppMobSlots = new ImplAAFMobSlot*[count];
  assert (internalppMobSlots);

  ImplAAFMobSlot ** pinternalppMobSlots = NULL;
  if (ppMobSlots)
    {
      pinternalppMobSlots = internalppMobSlots;
    }

  try
    {
      hr = ptr->Next
       (count,
        pinternalppMobSlots,
        pNumFetched);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UNEXPECTED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNEXPECTED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // cleanup for ppMobSlots
  //
  if (SUCCEEDED(hr)||hr==AAFRESULT_NO_MORE_OBJECTS)
    {
      IUnknown *pUnknown;
      HRESULT hStat;
      aafUInt32 localIdx;
	  assert (count >= 0);
	  for (localIdx = 0; localIdx < *pNumFetched; localIdx++)
		{
		  pUnknown = static_cast<IUnknown *> (internalppMobSlots[localIdx]->GetContainer());
		  hStat = pUnknown->QueryInterface(IID_IAAFMobSlot, (void **)(ppMobSlots+localIdx));
		  assert (SUCCEEDED (hStat));
		  //pUnknown->Release();
		  internalppMobSlots[localIdx]->ReleaseReference(); // We are through with this pointer.
		}
    }
  delete[] internalppMobSlots;
  internalppMobSlots = 0;
  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFMobSlots::Skip (aafUInt32  count)
{
  HRESULT hr;

  ImplEnumAAFMobSlots * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFMobSlots*> (pO);
  assert (ptr);


  try
    {
      hr = ptr->Skip
       (count);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UNEXPECTED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNEXPECTED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFMobSlots::Reset ()
{
  ImplEnumAAFMobSlots * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFMobSlots*> (pO);
  assert (ptr);
  HRESULT hr;

  try
    {
      hr = ptr->Reset();
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UNEXPECTED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNEXPECTED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  return hr;
}



HRESULT STDMETHODCALLTYPE
    CEnumAAFMobSlots::Clone (IEnumAAFMobSlots ** ppEnum)
{
  HRESULT hr;

  ImplEnumAAFMobSlots * ptr;
  ImplAAFRoot * pO;
  pO = GetRepObject ();
  assert (pO);
  ptr = static_cast<ImplEnumAAFMobSlots*> (pO);
  assert (ptr);

  //
  // set up for ppEnum
  //
  ImplEnumAAFMobSlots * internalppEnum = NULL;
  ImplEnumAAFMobSlots ** pinternalppEnum = NULL;
  if (ppEnum)
    {
      pinternalppEnum = &internalppEnum;
    }

  try
    {
      hr = ptr->Clone
       (pinternalppEnum);
    }
  catch (OMException& e)
    {
      // OMExceptions should be handled by the impl code. However, if an
      // unhandled OMException occurs, control reaches here. We must not
      // allow the unhandled exception to reach the client code, so we
      // turn it into a failure status code.
      //
      // If the OMException contains an HRESULT, it is returned to the
      // client, if not, AAFRESULT_UNEXPECTED_EXCEPTION is returned.
      //
      hr = OMExceptionToResult(e, AAFRESULT_UNEXPECTED_EXCEPTION);
    }
  catch (OMAssertionViolation &)
    {
      // Control reaches here if there is a programming error in the
      // impl code that was detected by an assertion violation.
      // We must not allow the assertion to reach the client code so
      // here we turn it into a failure status code.
      //
      hr = AAFRESULT_ASSERTION_VIOLATION;
    }
  catch (...)
    {
      // We CANNOT throw an exception out of a COM interface method!
      // Return a reasonable exception code.
      //
      hr = AAFRESULT_UNEXPECTED_EXCEPTION;
    }

  //
  // cleanup for ppEnum
  //
  if (SUCCEEDED(hr))
    {
      IUnknown *pUnknown;
      HRESULT hStat;

      if (internalppEnum)
        {
          pUnknown = static_cast<IUnknown *> (internalppEnum->GetContainer());
          hStat = pUnknown->QueryInterface(IID_IEnumAAFMobSlots, (void **)ppEnum);
          assert (SUCCEEDED (hStat));
          //pUnknown->Release();
          internalppEnum->ReleaseReference(); // We are through with this pointer.
        }
    }
  return hr;
}

//
// 
// 
inline int EQUAL_UID(const GUID & a, const GUID & b)
{
  return (0 == memcmp((&a), (&b), sizeof (aafUID_t)));
}
HRESULT CEnumAAFMobSlots::InternalQueryInterface
(
    REFIID riid,
    void **ppvObj)
{
    if (NULL == ppvObj)
        return E_INVALIDARG;

    // We only support the IClassFactory interface 
    if (EQUAL_UID(riid,IID_IEnumAAFMobSlots)) 
    { 
        *ppvObj = (IEnumAAFMobSlots *)this; 
        ((IUnknown *)*ppvObj)->AddRef();
        return S_OK;
    }

    // Always delegate back to base implementation.
    return CAAFRoot::InternalQueryInterface(riid, ppvObj);
}

//
// Define the contrete object support implementation.
// 
AAF_DEFINE_FACTORY(EnumAAFMobSlots)

