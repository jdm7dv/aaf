// @doc INTERNAL
// @com This file implements the module test for CAAFHTMLClip
/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#include "CAAFHTMLClip.h"
#include "CAAFHTMLClip.h"
#ifndef __CAAFHTMLClip_h__
#error - improperly defined include guard
#endif


#include <iostream.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#if defined(macintosh) || defined(_MAC)
#include <wstring.h>
#endif

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"



// Cross-platform utility to delete a file.
static void RemoveTestFile(const wchar_t* pFileName)
{
  const size_t kMaxFileName = 512;
  char cFileName[kMaxFileName];

  size_t status = wcstombs(cFileName, pFileName, kMaxFileName);
  if (status != (size_t)-1)
  { // delete the file.
    remove(cFileName);
  }
}


// convenient error handlers.
inline void checkResult(HRESULT r)
{
  if (FAILED(r))
    throw r;
}
inline void checkExpression(bool expression, HRESULT r)
{
  if (!expression)
    throw r;
}

class HTMLClipTest
{
public:
  HTMLClipTest();
  ~HTMLClipTest();

  void Create(wchar_t *pFileName, aafProductIdentification_t* pinfo);
  void Open(wchar_t *pFileName);
  void Close();

  void CreateHTMLClip();
  void OpenHTMLClip();

private:
  IAAFFile *_pFile;
  bool _bWritableFile;
  IAAFHeader *_pHeader;
  IAAFDictionary *_pDictionary;
  aafUID_t _referencedMobID; // save id for validation in the open test...
  static const wchar_t *_beginAnchor; // sample anchor values for validation.
  static const wchar_t *_endAnchor;
};


HRESULT CAAFHTMLClip::test()
{
  HRESULT hr = S_OK;
  aafProductIdentification_t	ProductInfo = {0};
  aafWChar * pFileName = L"HTMLClipTest.aaf";

  // Initialize the product info for this module test
  ProductInfo.companyName = L"AAF Developers Desk";
  ProductInfo.productName = L"AAFHTMLClip Test";
  ProductInfo.productVersion.major = 1;
  ProductInfo.productVersion.minor = 0;
  ProductInfo.productVersion.tertiary = 0;
  ProductInfo.productVersion.patchLevel = 0;
  ProductInfo.productVersion.type = kVersionUnknown;
  ProductInfo.productVersionString = NULL;
  ProductInfo.productID = -1;
  ProductInfo.platform = NULL;

  // Create an instance of our html clip test class and run the
  // tests...
  HTMLClipTest test;

  try
  {
    // Attempt to create a test file
    test.Create(pFileName, &ProductInfo);

    // Attempt to read the test file.
    test.Open(pFileName);
  }
  catch (HRESULT& rHR)
  {
    hr = rHR;
  }
  catch (...)
  {
    cerr << "CAAFHTMLClip::test...Caught general C++ exception!" << endl;
    hr = AAFRESULT_TEST_FAILED;
  }

  return hr;
}

// Initialize private constants.
const wchar_t *HTMLClipTest::_beginAnchor = L"HTML Begin Anchor Text";
const wchar_t *HTMLClipTest::_endAnchor = L"HTML End Anchor Text";

HTMLClipTest::HTMLClipTest() :
  _pFile(NULL),
  _bWritableFile(false),
  _pHeader(NULL),
  _pDictionary(NULL)
{
  memset(&_referencedMobID, 0, sizeof(_referencedMobID));
}

HTMLClipTest::~HTMLClipTest()
{
  Close();
}


void HTMLClipTest::Create(wchar_t *pFileName, aafProductIdentification_t* pinfo)
{
  assert(NULL == _pFile);

  // Remove the previous test file if any.
  RemoveTestFile(pFileName);
    
  // Create the file
  checkResult(AAFFileOpenNewModify(pFileName, 0, pinfo, &_pFile));
  _bWritableFile = true;

  // We can't really do anthing in AAF without the header.
  checkResult(_pFile->GetHeader(&_pHeader));

  // Get the AAF Dictionary so that we can create valid AAF objects.
  checkResult(_pHeader->GetDictionary(&_pDictionary));

  // Make a html clip.
  CreateHTMLClip();

  // cleanup
  Close();
}


void HTMLClipTest::Open(wchar_t *pFileName)
{
  assert(NULL == _pFile);
    
  // Create the file
  checkResult(AAFFileOpenExistingRead(pFileName, 0, &_pFile));
  _bWritableFile = false;

  // We can't really do anthing in AAF without the header.
  checkResult(_pFile->GetHeader(&_pHeader));

  // Open and validate the html clip.
  OpenHTMLClip();

  // cleanup
  Close();
}


void HTMLClipTest::Close()
{
  if (_pDictionary)
  {
    _pDictionary->Release();
    _pDictionary = NULL;
  }

  if (_pHeader)
  {
    _pHeader->Release();
    _pHeader = NULL;
  }
  
  if (_pFile)
  {
    if (_bWritableFile)
      checkResult(_pFile->Save());

    checkResult(_pFile->Close());

    _pFile->Release();
    _pFile = NULL;
  }  
}


void HTMLClipTest::CreateHTMLClip()
{
  assert(_pHeader && _pDictionary);

  HRESULT hr = S_OK;
  IAAFMasterMob *pMasterMob = NULL;
  IAAFMob *pReferencedMob = NULL;
  IAAFHTMLClip *pHTMLClip = NULL;
  IAAFSourceReference *pSourceReference = NULL;
  IAAFCompositionMob *pCompositionMob = NULL;
  IAAFMob *pReferencingMob = NULL;
  IAAFSegment *pSegment = NULL;
  IAAFMobSlot *pMobSlot = NULL;


  try
  {
    //Make the MOB to be referenced
    checkResult(_pDictionary->CreateInstance(&AUID_AAFMasterMob,
					    IID_IAAFMasterMob, 
					    (IUnknown **)&pMasterMob));
  
    checkResult(pMasterMob->QueryInterface(IID_IAAFMob, (void **)&pReferencedMob));
    checkResult(pReferencedMob->GetMobID(&_referencedMobID));
    checkResult(pReferencedMob->SetName(L"HTMLClipTest::ReferencedMob"));

    // Save the master mob.
    checkResult(_pHeader->AppendMob(pReferencedMob));

    // Use EssenceAccess to write some html essence
      // Create a file mob for the html essence.
      // Create the corresponding html essence.
      // Write some html essence.


    // Create a HTMLClip
    checkResult(_pDictionary->CreateInstance(&AUID_AAFHTMLClip,
					    IID_IAAFHTMLClip, 
					    (IUnknown **)&pHTMLClip));
    checkResult(pHTMLClip->SetBeginAnchor(const_cast<wchar_t *>(_beginAnchor)));
    checkResult(pHTMLClip->SetEndAnchor(const_cast<wchar_t *>(_endAnchor)));


    // Initialize the source reference data.
    checkResult(pHTMLClip->QueryInterface(IID_IAAFSourceReference, (void **)&pSourceReference));
    checkResult(pSourceReference->SetSourceID(_referencedMobID));
    checkResult(pSourceReference->SetSourceMobSlotID(0));

    // Create a composition mob to hold the html clip.
    checkResult(_pDictionary->CreateInstance(&AUID_AAFCompositionMob,
					    IID_IAAFCompositionMob, 
					    (IUnknown **)&pCompositionMob));
    checkResult(pCompositionMob->QueryInterface(IID_IAAFMob, (void **)&pReferencingMob));
    checkResult(pReferencingMob->SetName(L"CompositionMob_HTMLClipTest"));
 
  
    checkResult(pHTMLClip->QueryInterface(IID_IAAFSegment, (void **)&pSegment));
    IAAFMobSlot *pSlot = NULL;
    checkResult(pReferencingMob->AppendNewSlot(pSegment, 1, L"HTMLClipTest", &pMobSlot));


    // Save the referencing mob.
    checkResult(_pHeader->AppendMob(pReferencingMob));
  }
  catch (HRESULT& rHR)
  {
    hr = rHR;
    // fall through and handle cleanup
  }

  // Cleanup local references
  if (pMobSlot)
  {
    pMobSlot->Release();
    pMobSlot = NULL;
  }

  if (pSegment)
  {
    pSegment->Release();
    pSegment = NULL;
  }

  if (pReferencingMob)
  {
    pReferencingMob->Release();
    pReferencingMob = NULL;
  }

  if (pCompositionMob)
  {
    pCompositionMob->Release();
    pCompositionMob = NULL;
  }

  if (pSourceReference)
  {
    pSourceReference->Release();
    pSourceReference = NULL;
  }

  if (pHTMLClip)
  {
    pHTMLClip->Release();
    pHTMLClip = NULL;
  }

  if (pReferencedMob)
  {
    pReferencedMob->Release();
    pReferencedMob = NULL;
  }

  if (pMasterMob)
  {
    pMasterMob->Release();
    pMasterMob = NULL;
  }


  // Propogate the error if necessary.
  checkResult(hr);
}


void HTMLClipTest::OpenHTMLClip()
{
  assert(_pHeader);

  HRESULT hr = S_OK;
  aafInt32 compositionMobs = 0;
  IEnumAAFMobs *pEnumMobs = NULL;
  IAAFMob *pReferencingMob = NULL;
  IAAFCompositionMob *pCompositionMob = NULL;
  aafInt32 mobSlots = 0;
  IEnumAAFMobSlots *pEnumSlots = NULL;
  IAAFMobSlot *pMobSlot = NULL;
  IAAFSegment *pSegment = NULL;
  IAAFHTMLClip *pHTMLClip = NULL;
  aafUInt32 bufSize = 0;
  aafUInt32 actualBufSize = 0;
  aafUInt32 actualLength = 0;
  wchar_t *pBeginAnchor = NULL;
  wchar_t *pEndAnchor = NULL;
  IAAFSourceReference *pSourceReference = NULL;
  aafUID_t masterMobID = {0};
  IAAFMasterMob *pMasterMob = NULL;
  IAAFMob *pReferencedMob = NULL;


  try
  {
    // Get the number of composition mobs in the file (should be one)
    checkResult(_pHeader->GetNumMobs(kCompMob, &compositionMobs));
    checkExpression(1 == compositionMobs, AAFRESULT_TEST_FAILED);

    // Get the composition mob. There should only be one.
    aafSearchCrit_t criteria;
    criteria.searchTag = kByMobKind;
    criteria.tags.mobKind = kCompMob;
    checkResult(_pHeader->EnumAAFAllMobs(&criteria, &pEnumMobs));
    checkResult(pEnumMobs->NextOne(&pReferencingMob));
    checkResult(pReferencingMob->QueryInterface(IID_IAAFCompositionMob, (void **)&pCompositionMob));

    // Get the html clip in the slot. There should be only one.
    checkResult(pReferencingMob->GetNumSlots(&mobSlots));
    checkExpression(1 == mobSlots, AAFRESULT_TEST_FAILED);
    checkResult(pReferencingMob->EnumAAFAllMobSlots(&pEnumSlots));
    checkResult(pEnumSlots->NextOne(&pMobSlot));
    checkResult(pMobSlot->GetSegment(&pSegment));
    checkResult(pSegment->QueryInterface(IID_IAAFHTMLClip, (void **)&pHTMLClip));

    // Validate the properties...
    checkResult(pHTMLClip->GetBeginAnchorBufLen(&bufSize));
    actualLength = wcslen(_beginAnchor) + 1;
    actualBufSize = actualLength * 2;
    checkExpression(bufSize == actualBufSize, AAFRESULT_TEST_FAILED);
    pBeginAnchor = new wchar_t[actualLength];
    checkExpression(NULL != pBeginAnchor, AAFRESULT_NOMEMORY);
    checkResult(pHTMLClip->GetBeginAnchor(pBeginAnchor, actualBufSize));
    checkExpression(0 == wcscmp(pBeginAnchor, _beginAnchor), AAFRESULT_TEST_FAILED);

    checkResult(pHTMLClip->GetEndAnchorBufLen(&bufSize));
    actualLength = wcslen(_endAnchor) + 1;
    actualBufSize = actualLength * 2;
    checkExpression(bufSize == actualBufSize, AAFRESULT_TEST_FAILED);
    pEndAnchor = new wchar_t[actualLength];
    checkExpression(NULL != pEndAnchor, AAFRESULT_NOMEMORY);
    checkResult(pHTMLClip->GetEndAnchor(pEndAnchor, actualBufSize));
    checkExpression(0 == wcscmp(pEndAnchor, _endAnchor), AAFRESULT_TEST_FAILED);
    

    // Attempt to "resolve" the reference and find the master mob.
    checkResult(pHTMLClip->QueryInterface(IID_IAAFSourceReference, (void **)&pSourceReference));  
    checkResult(pSourceReference->GetSourceID(&masterMobID));
    checkExpression(0 == memcmp(&masterMobID, &_referencedMobID, sizeof(masterMobID)), AAFRESULT_TEST_FAILED);
    checkResult(_pHeader->LookupMob(&masterMobID, &pReferencedMob));
    checkResult(pReferencedMob->QueryInterface(IID_IAAFMasterMob, (void **)&pMasterMob));

    
    // Use EssenceAccess to read some html essence
      // Find the file mob for the html essence.
      // Find the corresponding html essence.
      // Read some html essence.
  }
  catch (HRESULT& rHR)
  {
    hr = rHR;
    // fall through and handle cleanup
  }

  // Cleanup local references
  if (pMasterMob)
  {
    pMasterMob->Release();
    pMasterMob = NULL;
  }

  if (pReferencedMob)
  {
    pReferencedMob->Release();
    pReferencedMob = NULL;
  }

  if (pSourceReference)
  {
    pSourceReference->Release();
    pSourceReference = NULL;
  }
  
  if (pEndAnchor)
  {
    delete [] pEndAnchor;
    pEndAnchor = NULL;
  }
  
  if (pBeginAnchor)
  {
    delete [] pBeginAnchor;
    pBeginAnchor = NULL;
  }

  if (pHTMLClip)
  {
    pHTMLClip->Release();
    pHTMLClip = NULL;
  }

  if (pSegment)
  {
    pSegment->Release();
    pSegment = NULL;
  }

  if (pMobSlot)
  {
    pMobSlot->Release();
    pMobSlot = NULL;
  }

  if (pEnumSlots)
  {
    pEnumSlots->Release();
    pEnumSlots = NULL;
  }

  if (pCompositionMob)
  {
    pCompositionMob->Release();
    pCompositionMob = NULL;
  }

  if (pReferencingMob)
  {
    pReferencingMob->Release();
    pReferencingMob = NULL;
  }

  if (pEnumMobs)
  {
    pEnumMobs->Release();
    pEnumMobs = NULL;
  }

  // Propogate the error if necessary.
  checkResult(hr);
}

















