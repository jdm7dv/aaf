/******************************************\
*                                          *
* Advanced Authoring Format                *
*                                          *
* Copyright (c) 1998 Avid Technology, Inc. *
* Copyright (c) 1998 Microsoft Corporation *
*                                          *
\******************************************/

#include "ImplAAFSourceClip.h"
#include "ImplAAFSourceMob.h"
#include "ImplAAFComponent.h"
#include "ImplAAFDataDef.h"
#include "ImplAAFFindSourceInfo.h"
#include "ImplAAFMasterMob.h"
#include "ImplAAFHeader.h"
#include "ImplAAFContentStorage.h"
#include "ImplAAFObjectCreation.h"
#include "ImplEnumAAFMobSlots.h"

#include <assert.h>
#include "AAFResult.h"
#include "aafCvt.h"

extern "C" const aafClassID_t CLSID_AAFSourceClip;

ImplAAFMasterMob::ImplAAFMasterMob ()
{}


ImplAAFMasterMob::~ImplAAFMasterMob ()
{}


AAFRESULT STDMETHODCALLTYPE
ImplAAFMasterMob::Initialize ()
{
  return AAFRESULT_SUCCESS;
}


//***********************************************************
//
// AddMasterSlot()
//
// This function adds a slot to the specified Master Mob that
// references the specified a slot in the specified Source Mob. The
// new slot in the Master Mob contains a Source Clip that specifies
// the Source Mob in its source reference properties.
//
// The pDataDef parameter requires a data kind valid for a media
// stream. Valid data kinds are:
//
// - DDEF_Video
// - DDEF_Audio
//
// Note: If pSlotName is passed in with zero length, then the
// slot is not assigned a name.  Slot names are not used by the
// SDK, and exist only so the user can name slots.
// 
// Succeeds if all of the following are true:
// (more conditions here)
// 
// If this method fails no state is changed.
// 
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - One or more of the following parameters are NULL pSourceMob,
//     pDataDef, and pSlotName.
//
// AAFRESULT_INVALID_DATADEF
//   - The data kind of the source MOB slot to be added to the Master
//     Mob does not match what is specfied in pDataDef.
//
// AAFRESULT_SLOT_NOTFOUND
//   - The specified Source Mob slot was not found.
//
// AAFRESULT_SLOT_EXISTS
//   - The specified Master slot ID already exists.
//
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::AddMasterSlot (aafUID_t*			pDataDef,
									 aafSlotID_t		sourceSlotID,
									 ImplAAFSourceMob*	pSourceMob,
									 aafSlotID_t		masterSlotID,
									 aafWChar*			pSlotName)
{
	aafLength_t	slotLength;
	aafUID_t	sourceMobID;
	HRESULT		hr = AAFRESULT_SUCCESS;
	ImplAAFMobSlot*	pMobSlot;
	aafUID_t	DataDef;
	ImplAAFSegment*	pSegment = NULL;
	ImplAAFSourceClip*	pSrcClip = NULL;
	aafSourceRef_t		ref;
	aafPosition_t		zeroPos;
	ImplAAFMobSlot	*pNewSlot = NULL;

	if (!pDataDef || !pSourceMob || !pSlotName)
		return AAFRESULT_NULL_PARAM;

	XPROTECT()
	{
		// Get the slot length and mob id.  Verify that data kind
		// of the slot is the same as pDataDef
		CHECK(pSourceMob->GetMobID(&sourceMobID));

		CHECK(pSourceMob->FindSlotBySlotID(sourceSlotID, &pMobSlot));

		CHECK(pMobSlot->GetSegment(&pSegment));

		pSegment->GetLength(&slotLength);
		pSegment->GetDataDef(&DataDef);
		pSegment->ReleaseReference();
		pSegment = NULL;

		// Make sure the slot contains the expected media type.
		if (memcmp(&DataDef, pDataDef, sizeof(aafUID_t)) != 0)
			RAISE(AAFRESULT_INVALID_DATADEF);

		pMobSlot->ReleaseReference();
		pMobSlot = NULL;

	// Add the master slot

		CvtInt32toPosition(0, zeroPos);
		ref.sourceID = sourceMobID;
		ref.sourceSlotID = sourceSlotID;
		ref.startTime = zeroPos;

		pSrcClip = (ImplAAFSourceClip *)CreateImpl(CLSID_AAFSourceClip);
		if(pSrcClip == NULL)
			RAISE(E_FAIL);

		CHECK(pSrcClip->Initialize(pDataDef, &slotLength, ref));
		CHECK(AppendNewSlot(pSrcClip, masterSlotID, pSlotName, &pNewSlot));

		pNewSlot->ReleaseReference();
		pNewSlot = NULL;

		if(pSrcClip)
			pSrcClip->ReleaseReference();
		pSrcClip = NULL;
	}
	XEXCEPT
	{
		if(pSegment != NULL)
			pSegment->ReleaseReference();
		if(pNewSlot != NULL)
			pNewSlot->ReleaseReference();
		if(pSrcClip != NULL)
			pSrcClip->ReleaseReference();
	}
	XEND;
	return hr;
}

//***********************************************************
//
// GetTapeName()
//
// Finds the videotape Source Mob associated with a Master Mob slot
// and writes the name of the videotape, which is stored in the
// Mobs Name property, into the pTapeName buffer.  The buffer is
// allocated by the caller.  The size of the buffer is given by
// bufSize.  If the property name has not yet been set, a
// zero-length string will be written (that is, only the trailing
// null character).
// 
// Caller may call GetTapeNameBufLen() to determine the required
// buffer size.
// 
// Succeeds if all of the following are true:
// - the pTapeName pointer is valid.
// - bufSize indicates the buffer is large enough to hold the name.
// 
// If this method fails nothing will be written to *pTapeName.
// 
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - pTapeName arg is NULL.
//
// AAFRESULT_SLOT_NOTFOUND
//   - The specified Master Slot was not found.
//
// AAFRESULT_NOT_TAPEMOB
//   - The specified Master Slot does not contain a Tape MOB.
//
// AAFRESULT_SMALLBUF
//   - bufSize indicates the buffer is too small to hold the string.
//
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::GetTapeName (aafInt32		masterSlotID,
								   aafWChar*	pTapeName,
								   aafInt32		bufSize)
{
	ImplAAFFindSourceInfo	*info = NULL;
	ImplAAFMob				*mob = NULL;

	XPROTECT()
	{
		CHECK(SearchSource (masterSlotID, 0, kTapeMob, NULL, NULL,
									&info));
		CHECK(info->GetMob(&mob));
		CHECK(mob->GetName(pTapeName, bufSize));
		info->ReleaseReference();
		info = NULL;
		mob->ReleaseReference();
		mob = NULL;
	}
	XEXCEPT
	{
		if(info != NULL)
			info->ReleaseReference();
		if(mob != NULL)
			mob->ReleaseReference();
	}
	XEND;

	return AAFRESULT_SUCCESS;
}


//***********************************************************
//
// GetTapeNameBufLen()
//
// Returns the length of buffer required for the GetTapeName()
// method.  The value is placed into the location specified by pLen.
// The value will include space required for the trailing null
// character.
//
// Succeeds if all of the following are true:
// - the pLen pointer is valid.
//
// If this method fails nothing will be written to *pLen.
//
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - pLen arg is NULL.
//
// AAFRESULT_SLOT_NOTFOUND
//   - The specified Master Slot was not found.
//
// AAFRESULT_NOT_TAPEMOB
//   - The specified Master Slot does not contain a Tape MOB.
//
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::GetTapeNameBufLen (aafInt32	masterSlotID,
										 aafInt32*  pLen)
{
	return AAFRESULT_NOT_IMPLEMENTED;
}

//***********************************************************
//
// GetNumRepresentations()
//
// This function returns the number of media representations
// available for the specified SlotID on a specified Master
// Mob.
//
// Succeeds if all of the following are true:
// - the pNumReps pointer is valid.
//
// If this method fails nothing will be written to *pLen.
//
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - pNumReps arg is NULL.
//
// AAFRESULT_SLOT_NOTFOUND
//   - The specified Master Slot was not found.
//
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::GetNumRepresentations (aafSlotID_t	slotID,
											 aafNumSlots_t*	pNumReps)
{
	ImplAAFMobSlot	*pSlot = NULL;
  ImplAAFSegment	*pSegment = NULL;
	HRESULT			hr;
	
	if (!pNumReps)
		return AAFRESULT_NULL_PARAM;

	*pNumReps = 0;
	
	hr = FindSlotBySlotID(slotID, &pSlot);
	if (SUCCEEDED(hr))
	{
		hr = pSlot->GetSegment(&pSegment);
		if (SUCCEEDED(hr))
		{
			hr = pSegment->NumRepresentations(pNumReps);
			pSegment->ReleaseReference();
			pSegment = NULL;
		}
		pSlot->ReleaseReference();
		pSlot = NULL;
	}

	return hr;
}

//***********************************************************
//
// GetRepresentationSourceClip()
//
// This method returns the indexed media representation for the specified
// Master Mob, SlotID, and index.  This call is meant to work
// with GetNumRepresentations, so that you can iterate through all
// of the choices yourself.  This method uses an integer index, not
// an iterator.  The function GetRepresentationSourceClip takes an
// index between 1 and the number of representations [inclusive],
// and returns the indexed Source Mob.
//
// Succeeds if all of the following are true:
// - the ppSourceClip pointer is valid.
//
// If this method fails nothing will be written to *ppSourceClip.
//
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - ppSourceClip arg is NULL.
//
// AAFRESULT_SLOT_NOTFOUND
//   - The specified Master Slot was not found.
//
// AAFRESULT_BADINDEX
//   - No Source Mob at specified index.
//
// AAFRESULT_MISSING_MEDIA_REP
//   - TODO: add this to idl
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::GetRepresentationSourceClip (aafSlotID_t			slotID,
												   aafInt32				index,
												   ImplAAFSourceClip**	ppSourceClip)
{
	return AAFRESULT_NOT_IMPLEMENTED;
}

//***********************************************************
//
// GetCriteriaSourceClip()
//
// Returns the Source Clip on the specified slot of a Master Mob
// that references the Source Mob that best meets the specified
// criteria.  This function will work whether multiple media
// representations exist or not.
//
// Note:
// The aafMediaCriteria_t is defined as the following structure:
// typedef struct
// {
//	aafCriteriaType_t type;
//	aafCriteriaProc_t proc;
// } aafMediaCriteria_t;
// 
// Note:
// The type field can have one of the following values: |
// typedef enum
// {
//	kAAFAnyRepresentation = 0,
//	kAAFFastestRepresentation,
//	kAAFBestFidelityRepresentation,
//	kAAFSmallestRepresentation,
//	kAAFUseRepresentationProc
// } aafCriteriaType_t;
//
// Succeeds if all of the following are true:
// - the ppSourceClip pointer is valid.
//
// If this method fails nothing will be written to *ppSourceClip.
//
// This method will return the following codes.  If more than one of
// the listed errors is in effect, it will return the first one
// encountered in the order given below:
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - ppSourceClip arg is NULL.
//
// AAFRESULT_SLOT_NOTFOUND
//   - The specified Master Slot was not found.
//
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::GetCriteriaSourceClip (aafSlotID_t			slotID,
											 aafMediaCriteria_t*	pCriteria,
											 ImplAAFSourceClip**	ppSourceClip)
{
	return AAFRESULT_NOT_IMPLEMENTED;
}



//***********************************************************
//
// SearchSource()
//
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - ppSourceClip arg is NULL.
//
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::SearchSource (aafSlotID_t				slotID,
									aafPosition_t			offset,
									aafMobKind_t			mobKind,
									aafMediaCriteria_t*		pMediaCrit,
									aafEffectChoice_t*		pEffectChoice,
									ImplAAFFindSourceInfo**	ppSourceInfo)
{
	return(InternalSearchSource(slotID, offset, mobKind, pMediaCrit, pEffectChoice,
										   ppSourceInfo));
}

//***********************************************************
//
// GetMobKind()
//
// Returns kMasterMob in *pMobKind.
// 
// AAFRESULT_SUCCESS
//   - succeeded.  (This is the only code indicating success.)
//
// AAFRESULT_NULL_PARAM
//   - ppSourceClip arg is NULL.
//
// 
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::GetMobKind (aafMobKind_t *pMobKind)
{
	if(pMobKind == NULL)
		return(AAFRESULT_NULL_PARAM);
	*pMobKind = kMasterMob;

	return AAFRESULT_SUCCESS;
}

//****************
// NewPhysSourceRef()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::NewPhysSourceRef (aafRational_t  editrate,
                           aafSlotID_t  aMobSlot,
                           aafUID_t *pEssenceKind,
							aafSourceRef_t  ref,
                           aafLength_t  srcRefLength)
{
	return(ImplAAFMob::AddPhysSourceRef(kAAFForceOverwrite, editrate, aMobSlot,
							pEssenceKind, ref, srcRefLength));
}

//****************
// AppendPhysSourceRef()
//
AAFRESULT STDMETHODCALLTYPE
    ImplAAFMasterMob::AppendPhysSourceRef (aafRational_t  editrate,
                           aafSlotID_t  aMobSlot,
                           aafUID_t *pEssenceKind,
							aafSourceRef_t  ref,
                           aafLength_t  srcRefLength)
{
	return(ImplAAFMob::AddPhysSourceRef(kAAFAppend, editrate, aMobSlot,
							pEssenceKind, ref, srcRefLength));
}
extern "C" const aafClassID_t CLSID_AAFMasterMob;

OMDEFINE_STORABLE(ImplAAFMasterMob, CLSID_AAFMasterMob);

// Cheat!  We're using this object's CLSID instead of object class...
AAFRESULT STDMETHODCALLTYPE
ImplAAFMasterMob::GetObjectClass(aafUID_t * pClass)
{
  if (! pClass)
	{
	  return AAFRESULT_NULL_PARAM;
	}
  memcpy (pClass, &CLSID_AAFMasterMob, sizeof (aafClassID_t));
  return AAFRESULT_SUCCESS;
}

/************************
 * Function: omfsReconcileMasterMobLength (INTERNAL)
 *
 * 	Called from omfsReconcileMobLength to handle the master mob case.
 *		Given a master mob, make sure that all fields which contain the
 *		length of the mob are in agreement.  Currently only makes sure
 *		that mob length references are >= each of the track lengths.
 *
 *		Since 2.0 does not contain a redundant mob length field, this
 *		function simply returns on 2.x era files.
 *
 * Argument Notes:
 *		<none>.
 *
 * ReturnValue:
 *		Error code (see below).
 *
 * Possible Errors:
 *		Standard errors (see top of file).
 */
AAFRESULT ImplAAFMasterMob::ReconcileMobLength(void)
{
	ImplAAFMob			*fileMob;
	aafInt32			loop;
	aafInt32			numSlots, fileNumSlots;
	aafPosition_t		endPos;
	ImplEnumAAFMobSlots	*slotIter = NULL, *fileSlotIter = NULL;
	ImplAAFMobSlot		*fileSlot, *slot;
	ImplAAFSegment		*fileSeg, *seg;

	XPROTECT()
	{
		/* Adjust the SCLP length from the master mob to the length of
		 * the file mob, in the units of the master mob
		 */
		CHECK(EnumAAFAllMobSlots (&slotIter));
		CHECK(GetNumSlots(&numSlots));
		for (loop = 1; loop <= numSlots; loop++)
		{
			CHECK(slotIter->NextOne(&slot));
			CHECK(slot->GetSegment(&seg));
			CHECK(((ImplAAFSourceClip *)seg)->ResolveRef( &fileMob));	//!!!
			CHECK(fileMob->GetNumSlots(&fileNumSlots));
			if(fileNumSlots >= 1)
			{
				CHECK(fileMob->EnumAAFAllMobSlots (&slotIter));
				CHECK(slotIter->NextOne(&fileSlot));
				CHECK(fileSlot->GetSegment(&fileSeg));
				CHECK(fileSeg->GetLength(&endPos));
				delete fileSlotIter;
			}
			
			CHECK(slot->ConvertToMyRate(endPos, fileSlot, &endPos));
			CHECK(seg->SetLength(&endPos));
		}			
//!!!			delete slotIter;
//!!!			slotIter = NULL;
	}
	XEXCEPT
	XEND
		
	return (AAFRESULT_SUCCESS);
}

