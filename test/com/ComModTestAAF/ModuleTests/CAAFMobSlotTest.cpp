// @doc INTERNAL
// @com This file implements the module test for CAAFMobSlot
/***********************************************************************
 *
 *              Copyright (c) 1998-1999 Avid Technology, Inc.
 *
 * Permission to use, copy and modify this software and accompanying 
 * documentation, and to distribute and sublicense application software
 * incorporating this software for any purpose is hereby granted, 
 * provided that (i) the above copyright notice and this permission
 * notice appear in all copies of the software and related documentation,
 * and (ii) the name Avid Technology, Inc. may not be used in any
 * advertising or publicity relating to the software without the specific,
 * prior written permission of Avid Technology, Inc.
 *
 * THE SOFTWARE IS PROVIDED AS-IS AND WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS, IMPLIED OR OTHERWISE, INCLUDING WITHOUT LIMITATION, ANY
 * WARRANTY OF MERCHANTABILITY OR FITNESS FOR A PARTICULAR PURPOSE.
 * IN NO EVENT SHALL AVID TECHNOLOGY, INC. BE LIABLE FOR ANY DIRECT,
 * SPECIAL, INCIDENTAL, PUNITIVE, INDIRECT, ECONOMIC, CONSEQUENTIAL OR
 * OTHER DAMAGES OF ANY KIND, OR ANY DAMAGES WHATSOEVER ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE AND
 * ACCOMPANYING DOCUMENTATION, INCLUDING, WITHOUT LIMITATION, DAMAGES
 * RESULTING FROM LOSS OF USE, DATA OR PROFITS, AND WHETHER OR NOT
 * ADVISED OF THE POSSIBILITY OF DAMAGE, REGARDLESS OF THE THEORY OF
 * LIABILITY.
 *
 ************************************************************************/


#include "AAF.h"

#include <iostream.h>
#include <stdio.h>

#include "AAFStoredObjectIDs.h"
#include "AAFResult.h"
#include "AAFDefUIDs.h"
#include "AAFDataDefs.h"

#include "CAAFBuiltinDefs.h"

static aafWChar *slotNames[5] = { L"SLOT1", L"SLOT2", L"SLOT3", L"SLOT4", L"SLOT5" };

static const 	aafMobID_t	TEST_MobID =
{{0x06, 0x0c, 0x2b, 0x34, 0x02, 0x05, 0x11, 0x01, 0x01, 0x00, 0x10, 0x00},
0x13, 0x00, 0x00, 0x00,
{0xbcf9fda0, 0x0403, 0x11d4, 0x8e, 0x3d, 0x00, 0x90, 0x27, 0xdf, 0xca, 0x7c}};

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

static HRESULT CreateAAFFile(aafWChar * pFileName)
{
	IAAFFile *					pFile = NULL;
	bool bFileOpen = false;
	IAAFHeader *        pHeader = NULL;
	IAAFDictionary*  pDictionary = NULL;
	IAAFMob						*pMob = NULL;
	IAAFMobSlot		*newSlot = NULL;
	IAAFSegment		*seg = NULL;
	IAAFSourceClip	*sclp = NULL;
	IAAFComponent	*pComp = NULL;
	aafProductIdentification_t	ProductInfo;
	HRESULT						hr = S_OK;
	
	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFMobSlot Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.productID = UnitTestProductID;
	ProductInfo.platform = NULL;
	
	
	try
	{
		// Remove the previous test file if any.
		RemoveTestFile(pFileName);
		
		// Create the file.
		checkResult(AAFFileOpenNewModify(pFileName, 0, &ProductInfo, &pFile));
		bFileOpen = true;
		
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));
		
		// Get the AAF Dictionary so that we can create valid AAF objects.
		checkResult(pHeader->GetDictionary(&pDictionary));
		CAAFBuiltinDefs defs (pDictionary);
		
		//Make the first mob
		long	test;
		aafRational_t	audioRate = { 44100, 1 };
		
		// Create a Mob
		checkResult(defs.cdMob()->
					CreateInstance(IID_IAAFMob, 
								   (IUnknown **)&pMob));
		
		checkResult(pMob->SetMobID(TEST_MobID));
		checkResult(pMob->SetName(L"MOBTest"));
		
		// Add some slots
		for(test = 0; test < 5; test++)
		{
			checkResult(defs.cdSourceClip()->
						CreateInstance(IID_IAAFSourceClip, 
									   (IUnknown **)&sclp));		
			
			checkResult(sclp->QueryInterface (IID_IAAFComponent, (void **)&pComp));
			checkResult(pComp->SetDataDef (defs.ddPicture()));
			pComp->Release();
			pComp = NULL;
			checkResult(sclp->QueryInterface (IID_IAAFSegment, (void **)&seg));
			
			checkResult(defs.cdMobSlot()->
						CreateInstance(IID_IAAFMobSlot, 
									   (IUnknown **)&newSlot));		
			
			checkResult(newSlot->SetSegment(seg));
			checkResult(newSlot->SetSlotID(test+1));
			checkResult(newSlot->SetPhysicalNum(test+2));
			checkResult(newSlot->SetName(slotNames[test]));
			checkResult(pMob->AppendSlot (newSlot));
			
			newSlot->Release();
			newSlot = NULL;
			
			seg->Release();
			seg = NULL;
			
			sclp->Release();
			sclp = NULL;
		}
		
		// Add the mob to the file.
		checkResult(pHeader->AddMob(pMob));
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	
	// Cleanup and return
	if (newSlot)
		newSlot->Release();
	
	if (seg)
		seg->Release();
	
	if (sclp)
		sclp->Release();
	
	if (pComp)
		pComp->Release();
	
	if (pMob)
		pMob->Release();
	
	if (pDictionary)
		pDictionary->Release();
	
	if (pHeader)
		pHeader->Release();
	
	if (pFile)
	{  // Close file
		if (bFileOpen)
		{
			pFile->Save();
			pFile->Close();
		}
		pFile->Release();
	}
	
	return hr;
}

static HRESULT ReadAAFFile(aafWChar * pFileName)
{
	IAAFFile				*pFile = NULL;
	bool					bFileOpen = false;
	IAAFHeader				*pHeader = NULL;
	IEnumAAFMobs			*mobIter = NULL;
	IAAFMob					*aMob = NULL;
	IEnumAAFMobSlots		*slotIter = NULL;
	IAAFMobSlot				*slot = NULL;
	IAAFSegment				*pSeg = NULL;
	IAAFSourceClip			*pSourceClip = NULL;
	IAAFDataDef *            pDataDef = 0;
	IAAFDefObject *          pDefObj = 0;
	aafProductIdentification_t	ProductInfo;
	aafNumSlots_t			numMobs, n;
	aafSlotID_t				s;
	aafUInt32				length;
	HRESULT					hr = S_OK;
	aafUID_t				readUID, typeUID = DDEF_Picture;
	
	aafProductVersion_t v;
	v.major = 1;
	v.minor = 0;
	v.tertiary = 0;
	v.patchLevel = 0;
	v.type = kAAFVersionUnknown;
	ProductInfo.companyName = L"AAF Developers Desk";
	ProductInfo.productName = L"AAFMobSlot Test";
	ProductInfo.productVersion = &v;
	ProductInfo.productVersionString = NULL;
	ProductInfo.platform = NULL;
	
	try
	{
		// Open the file
		checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
		bFileOpen = true;
		
		// We can't really do anthing in AAF without the header.
		checkResult(pFile->GetHeader(&pHeader));
		
		
		checkResult(pHeader->CountMobs(kAAFAllMob, &numMobs));
		checkExpression(1 == numMobs, AAFRESULT_TEST_FAILED);
		
		
		aafSearchCrit_t		criteria;
		criteria.searchTag = kAAFNoSearch;
		checkResult(pHeader->GetMobs (&criteria, &mobIter));
		
		for(n = 0; n < numMobs; n++)
		{
			aafWChar		name[500], slotName[500];
			aafNumSlots_t	numSlots;
			aafMobID_t		mobID;
			aafSlotID_t		trackID;
			
			checkResult(mobIter->NextOne (&aMob));
			checkResult(aMob->GetName (name, sizeof(name)));
			checkResult(aMob->GetMobID (&mobID));
			
			checkResult(aMob->CountSlots (&numSlots));
			checkExpression(5 == numSlots, AAFRESULT_TEST_FAILED);
			
			checkResult(aMob->GetSlots(&slotIter));
			
			for(s = 0; s < (aafSlotID_t)numSlots; s++)
			{
				checkResult(slotIter->NextOne (&slot));
				checkResult(slot->GetNameBufLen(&length));
				checkResult(slot->GetName (slotName, length));
				checkExpression (wcscmp(slotName, slotNames[s]) == 0, AAFRESULT_TEST_FAILED);
				checkResult(slot->GetSlotID(&trackID));
				checkExpression (trackID == s+1, AAFRESULT_TEST_FAILED);
				checkResult(slot->GetPhysicalNum(&trackID));
				checkExpression (trackID == s+2, AAFRESULT_TEST_FAILED);
				checkResult(slot->GetPhysicalNum(&trackID));
				checkResult(slot->GetDataDef(&pDataDef));
				checkResult(pDataDef->QueryInterface (IID_IAAFDefObject, (void **)&pDefObj));
				checkResult(pDefObj->GetAUID(&readUID));
				checkExpression (memcmp(&typeUID, &readUID, sizeof(typeUID)) == 0, AAFRESULT_TEST_FAILED);
				checkResult(slot->GetSegment(&pSeg));
				checkResult(pSeg->QueryInterface (IID_IAAFSourceClip, (void **)&pSourceClip));
				pDataDef->Release();
				pDataDef = 0;
				pDefObj->Release ();
				pDefObj = 0;
				pSourceClip->Release();
				pSourceClip = NULL;
				pSeg->Release();
				pSeg = NULL;
				slot->Release();
				slot = NULL;
			}
			
			aMob->Release();
			aMob = NULL;
		}
	}
	catch (HRESULT& rResult)
	{
		hr = rResult;
	}
	
	// Cleanup object references
	if (slot)
	  {
		slot->Release();
		slot = 0;
	  }
	
	if (pSeg)
	  {
		pSeg->Release();
		pSeg = 0;
	  }
	
	if (pSourceClip)
	  {
		pSourceClip->Release();
		pSourceClip = 0;
	  }
	
	if (slotIter)
	  {
		slotIter->Release();
		slotIter = 0;
	  }
	
	if (aMob)
	  {
		aMob->Release();
		aMob = 0;
	  }
	
	if (mobIter)
	  {
		mobIter->Release();
		mobIter = 0;
	  }
	
	if (pHeader)
	  {
		pHeader->Release();
		pHeader = 0;
	  }
	
	if (pDataDef)	
	  {
		pDataDef->Release();
		pDataDef = 0;
	  }

	if (pDefObj)
	  {
		pDefObj->Release ();
		pDefObj = 0;
	  }

	if (pFile)
	{  // Close file
		if (bFileOpen)
			pFile->Close();
		pFile->Release();
		pFile = 0;
	}
	
	return hr;
}
 

extern "C" HRESULT CAAFMobSlot_test()
{
	HRESULT hr = AAFRESULT_NOT_IMPLEMENTED;
	aafWChar * pFileName = L"AAFMobSlotTest.aaf";
	
	try
	{
		hr = CreateAAFFile(	pFileName );
		if(hr == AAFRESULT_SUCCESS)
			hr = ReadAAFFile( pFileName );
	}
	catch (...)
	{
		cerr << "CAAFMobSlot_test...Caught general C++"
			 << " exception!" << endl; 
		hr = AAFRESULT_TEST_FAILED;
	}
	
	// Cleanup our object if it exists.
	
	// When all of the functionality of this class is tested, we can return success.
	// When a method and its unit test have been implemented, remove it from the list.
//	if (SUCCEEDED(hr))
//	{
//		cout << "The following AAFMobSlot tests have not been implemented:" << endl; 
//		cout << "     GetDataDef" << endl; 
//		hr = AAFRESULT_TEST_PARTIAL_SUCCESS;
//	}
	
	return hr;
}
