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
 *  prior written permission of Avid Technology, Inc.
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


#include "extensionRead.h"
#include "extensionUtils.h"
#include "AAFResult.h"

#include <iostream.h>
#include <assert.h>


//
// This example code is intended to show how AAF may be extended to
// store personnel information relating to the staff which worked on a
// project.
//
// See comments in extensionWrite.cpp for a discussion of what we're
// trying to do and how we're doing it.
//

bool extensionRead (const aafCharacter * filename)
{
  assert (filename);

  cout << "***Reading file " << filename << "***" << endl;

  IAAFFile *pFile=NULL;
  check (AAFFileOpenExistingRead ((aafCharacter*) filename,
								  0,
								  &pFile));
  
  IAAFHeader *pHead=NULL;
  check (pFile->GetHeader(&pHead));

  IAAFDictionary *pDict=NULL;
  check (pHead->GetDictionary(&pDict));

  IAAFTypeDef *ptd=NULL;
  IAAFClassDef *pcd=NULL;

  cout << "Verifying role enum type has been registered." << endl;
  check (pDict->LookupType (kTypeID_eRole, &ptd));
  ptd->Release();
  ptd=NULL;

  cout << "Verifying PersonnelResource class has been registered." << endl;
  check (pDict->LookupClass (kClassID_PersonnelResource, &pcd));
  pcd->Release();
  pcd=NULL;

  cout << "Verifying PersonnelMob class has been registered." << endl;
  check (pDict->LookupClass (kClassID_PersonnelMob, &pcd));
  pcd->Release();
  pcd=NULL;

  cout << "Verifying PersonnelResourceReference type has been"
	   << " registered." << endl;
  check (pDict->LookupType (kTypeID_PersonnelResourceStrongReference,
							&ptd));
  ptd->Release();
  ptd=NULL;

  cout << "Verifying PersonnelResourceReferenceVector type has been"
	   << " registered." << endl; 
  check (pDict->LookupType (kTypeID_PersonnelResourceStrongReferenceVector,
							&ptd));
  ptd->Release();
  ptd=NULL;

  cout << "Verifying AdminMob instance has been created and added" 
	   << " to header." << endl;
  // Need to find AdminMob, temp iterate through Mobs to find one.
  IEnumAAFMobs*	pMobIter = NULL;
  IAAFMob *pMob=NULL;
  bool foundAdmin=false;
  check(pHead->EnumAAFAllMobs(NULL, &pMobIter));
  while(!foundAdmin && (AAFRESULT_SUCCESS == pMobIter->NextOne(&pMob)))
  {
	//Check if Mob is a Admin Mob
	IAAFObject *pObject;
	check(pMob->QueryInterface(IID_IAAFObject,(void **)&pObject));
	IAAFClassDef *pMobCD=NULL;
	check (pObject->GetDefinition(&pMobCD));
	pObject->Release();
	pObject=NULL;
	if (classDefinitionIsA(pMobCD, kClassID_PersonnelMob))
	{
		foundAdmin = true;
	} else
	{
		// will need to release pMob when it is not the right mob
		pMob->Release();
		pMob=NULL;
	}
 	pMobCD->Release();
	pMobCD=NULL;
  }


  if (foundAdmin) 
  {
	  cout << "Printing contents of Admin Mob." << endl;
	  PrintPersonnelResources (pDict, pMob);
	  pMob->Release();
	  pMob=NULL;
  } else
  {
	  cout << "Did not find Admin Mob" << endl;
  }
  // done.
  pFile->Close ();
  pFile->Release();
  pFile=NULL;
  pHead->Release();
  pHead=NULL;
  pDict->Release();
  pDict=NULL;
  pMobIter->Release();
  pMobIter=NULL;
  return true;
}
