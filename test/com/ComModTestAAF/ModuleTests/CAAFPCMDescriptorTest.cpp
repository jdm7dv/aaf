//=---------------------------------------------------------------------=
//
// $Id: CAAFPCMDescriptorTest.cpp,v 1.1.2.1 2004/03/11 18:58:44 stuart_hc Exp $ $Name:  $
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

#include <iostream>

#include "AAF.h"
#include "AAFResult.h"

#include "CAAFBuiltinDefs.h"
#include "ModuleTest.h"

using namespace std;

const aafUInt32  peakEnvelopeDataSize = 16;
struct CAAFPCMDescriptorTestFixture
{
    aafMobID_t              sourceMobID;
    aafUInt16               blockAlign;
    aafUInt8                sequenceOffset;
    aafUInt32               averageBPS;
    aafUInt32               peakEnvelopeVersion;
    aafUInt32               peakEnvelopeFormat;
    aafUInt32               pointsPerPeakValue;
    aafUInt32               peakEnvelopeBlockSize;
    aafUInt32               peakChannelCount;
    aafUInt32               peakFrameCount;
    aafUInt32               peakOfPeaksPosition;
    aafTimeStamp_t          peakEnvelopeTimestamp;
    char                    peakEnvelopeData[ peakEnvelopeDataSize ];
};

static const CAAFPCMDescriptorTestFixture  gTestData = 
{
    { { 0x06, 0x0c, 0x2b, 0x34,
        0x02, 0x05, 0x11, 0x01,
        0x01, 0x00, 0x10, 0x00 },
      0x13, 0x00, 0x00, 0x00,
      { 0xf6d43ded, 0xa5aa, 0x4a05,
        0xa2, 0x84, 0x37, 0x8f,
        0x94, 0x16, 0xd0, 0x56 } },

    8976,
    51,
    55372,
    17234,
    12,
    17,
    29,
    35,
    41,
    54,
    { {1979, 12, 31}, {11, 30, 0, 0} },

    { '\x001', '\x002', '\x003', '\x004',
      '\x005', '\x006', '\x007', '\x008',
      '\x009', '\x00a', '\x00b', '\x00c',
      '\x00d', '\x00e', '\x00f', '\x010' }
};



// Convenient error handlers.
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



// Required function prototype.
extern "C" HRESULT CAAFPCMDescriptor_test(
    testMode_t mode,
    aafUID_t fileKind,
//    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID);

static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
//    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID);

static HRESULT ReadAAFFile(
    aafWChar * pFileName);

static HRESULT Test_IAAFPCMDescriptor_Uninitialized( IAAFPCMDescriptor* );
static HRESULT Test_IAAFPCMDescriptor_NotPersistent( IAAFPCMDescriptor*, testMode_t );

static HRESULT Test_IAAFPCMDescriptor( IAAFPCMDescriptor*, testMode_t );


//
// The public entry for this module test,
//
HRESULT CAAFPCMDescriptor_test(
    testMode_t mode,
    aafUID_t fileKind,
//    testRawStorageType_t rawStorageType,
    aafProductIdentification_t productID )
{
    // None of the tests are implemented. Do not run them to
    // avoid unnecessary exceptions.
    // Once the tests implemented remove this line.
    return AAFRESULT_NOT_IMPLEMENTED;


    HRESULT  hr = AAFRESULT_NOT_IMPLEMENTED;
    const size_t fileNameBufLen = 128;
    aafWChar pFileName[ fileNameBufLen ] = L"";
    //GenerateTestFileName( productID.productName, fileKind, fileNameBufLen, pFileName );


    try
    {
        if(mode == kAAFUnitTestReadWrite)
            ; //hr = CreateAAFFile(pFileName, fileKind, rawStorageType, productID);
        else
            hr = AAFRESULT_SUCCESS;
        if(hr == AAFRESULT_SUCCESS)
            hr = ReadAAFFile( pFileName );
    }
    catch (...)
    {
        cerr << "CAAFSoundDescriptor_test...Caught general C++"
             << " exception!" << endl; 
        hr = AAFRESULT_TEST_FAILED;
    }


    return hr;
}

#if 0
static HRESULT CreateAAFFile(
    aafWChar * pFileName,
    aafUID_constref fileKind,
    testRawStorageType_t rawStorageType,
    aafProductIdentification_constref productID)
{
    IAAFFile*               pFile = 0;
    IAAFHeader*             pHeader = 0;
    IAAFDictionary*         pDictionary = 0;
    IAAFSourceMob*          pSourceMob = 0;
    IAAFMob*                pMob = 0;
    IAAFEssenceDescriptor*  pEssenceDesc = 0;
    IAAFPCMDescriptor*      pPCMDesc = 0;

    HRESULT hr = S_OK;

    try
    {
        // Remove the previous test file if any.
        RemoveTestFile(pFileName);


        // Create the test file
        checkResult(CreateTestFile( pFileName,
                                    fileKind,
                                    rawStorageType,
                                    productID,
                                    &pFile ));


        // Get the AAF Dictionary
        checkResult(pFile->GetHeader(&pHeader));
        checkResult(pHeader->GetDictionary(&pDictionary));
        CAAFBuiltinDefs  defs (pDictionary);


        // Create a mob, set its ID
        checkResult(defs.cdSourceMob()->CreateInstance( IID_IAAFSourceMob,
                                                        (IUnknown **)&pSourceMob) );
        checkResult(pSourceMob->QueryInterface (IID_IAAFMob, (void **)&pMob));

        checkResult(pMob->SetMobID(gTestData.sourceMobID));
        checkResult(pMob->SetName(L"PCMDescriptorTest"));


        // Create a PCMDescriptor,
        // add the descriptor to the source mob.
        checkResult(defs.cdPCMDescriptor()->CreateInstance(
            IID_IAAFPCMDescriptor,
            (IUnknown **)&pPCMDesc));


        // Before initialization test if the SoundDescriptor
        // methods return valid error values.
        checkResult(Test_IAAFPCMDescriptor_Uninitialized (pPCMDesc));



        // Initialize PCMDescriptor object.
        checkResult(pPCMDesc->Initialize());


        checkResult(Test_IAAFPCMDescriptor_NotPersistent(pPCMDesc,
                                                         kAAFUnitTestReadWrite));


        // Attach the descriptor to the source mob and
        // then attach the source mob to the file.
        checkResult(pPCMDesc->QueryInterface(IID_IAAFEssenceDescriptor, (void **) &pEssenceDesc));
        checkResult(pSourceMob->SetEssenceDescriptor(pEssenceDesc));

        checkResult(pHeader->AddMob(pMob));


        // Test initialized SoundDescriptor.
        checkResult(Test_IAAFPCMDescriptor(pPCMDesc,
                                           kAAFUnitTestReadWrite));


        // Save the file
        checkResult(pFile->Save());
    }
    catch (HRESULT& rResult)
    {
        hr = rResult;
    }

    if (pPCMDesc) 
    {
        pPCMDesc->Release();
    }
    if (pEssenceDesc) 
    {
        pEssenceDesc->Release();
    }
    if (pMob) 
    {
        pMob->Release();
    }
    if (pSourceMob) 
    {
        pSourceMob->Release();
    }
    if (pDictionary) 
    {
        pDictionary->Release();
    }
    if (pHeader) 
    {
        pHeader->Release();
    }
    if (pFile) 
    {
        pFile->Close();
        pFile->Release();
    }

    return hr;
}

#endif

static HRESULT ReadAAFFile(
    aafWChar * pFileName )
{
    HRESULT                 hr = S_OK;
    IAAFFile*               pFile = 0;
    IAAFHeader*             pHeader = 0;
    IAAFMob*                pMob = 0;
    IAAFSourceMob*          pSourceMob = 0;
    IAAFEssenceDescriptor*  pEssenceDesc = 0;
    IAAFPCMDescriptor*      pPCMDesc = 0;

    try
    {
        // Open the test file
        checkResult(AAFFileOpenExistingRead(pFileName, 0, &pFile));
        checkResult(pFile->GetHeader(&pHeader));

        // Get the source mob
        checkResult(pHeader->LookupMob(gTestData.sourceMobID, &pMob));
        checkResult(pMob->QueryInterface(IID_IAAFSourceMob,
                                         (void **)&pSourceMob));

        // Get the sound descriptor
        checkResult(pSourceMob->GetEssenceDescriptor(&pEssenceDesc));
        checkResult(pEssenceDesc->QueryInterface(IID_IAAFPCMDescriptor,
                                                 (void **)&pPCMDesc));


        checkResult(Test_IAAFPCMDescriptor(pPCMDesc,
                                           kAAFUnitTestReadOnly));
    }
    catch (HRESULT& rResult)
    {
        hr = rResult;
    }


    if (pPCMDesc) 
    {
        pPCMDesc->Release();
    }
    if (pEssenceDesc) 
    {
        pEssenceDesc->Release();
    }
    if (pSourceMob) 
    {
        pSourceMob->Release();
    }
    if (pMob) 
    {
        pMob->Release();
    }
    if (pHeader) 
    {
        pHeader->Release();
    }
    if (pFile) 
    {
        pFile->Close();
        pFile->Release();
    }


    return hr;
}



static HRESULT Test_IAAFPCMDescriptor_Uninitialized(
    IAAFPCMDescriptor* )
{
    return AAFRESULT_NOT_IMPLEMENTED;
}



static HRESULT Test_IAAFPCMDescriptor_NotPersistent(
    IAAFPCMDescriptor*,
    testMode_t )
{
    return AAFRESULT_NOT_IMPLEMENTED;
}



static HRESULT Test_IAAFPCMDescriptor(
    IAAFPCMDescriptor*,
    testMode_t )
{
    return AAFRESULT_NOT_IMPLEMENTED;
}



static HRESULT Test_IAAFPCMDescriptor_BlockAlign(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        const aafUInt16 bogusBlockAlign = 39;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetBlockAlign() should succeed because Channels
            // is a required property.
            aafUInt16  blockAlign = bogusBlockAlign;
            hr = pPCMDesc->GetBlockAlign( &blockAlign );
            checkExpression( hr == AAFRESULT_SUCCESS,
                             AAFRESULT_TEST_FAILED );
            checkExpression( blockAlign != bogusBlockAlign,
                             AAFRESULT_TEST_FAILED );


            // GetBlockAlign() should fail if function arguments are invalid.
            blockAlign = bogusBlockAlign;
            hr = pPCMDesc->GetBlockAlign( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( blockAlign == bogusBlockAlign,
                             AAFRESULT_TEST_FAILED );


            // SetBlockAlign() should always succeed
            hr = pPCMDesc->SetBlockAlign( gTestData.blockAlign );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetBlockAlign() should succeed.
            aafUInt16  blockAlign = bogusBlockAlign;
            hr = pPCMDesc->GetBlockAlign( &blockAlign );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( blockAlign == gTestData.blockAlign,
                             AAFRESULT_TEST_FAILED );
        }


        // If we got to this point none of the tests above
        // failed and the status can be set to 'success'.
        hr = S_OK;
    }
    catch( HRESULT e )
    {
        hr = e;
    }


    return hr;
}



static HRESULT Test_IAAFPCMDescriptor_SequenceOffset(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafUInt8  bogusSequenceOffset = 67;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetSequenceOffset() should fail if the property not present.
            aafUInt8  sequenceOffset = bogusSequenceOffset;
            hr = pPCMDesc->GetSequenceOffset( &sequenceOffset );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( sequenceOffset == bogusSequenceOffset,
                             AAFRESULT_TEST_FAILED );


            // GetSequenceOffset() should fail if function arguments are invalid.
            sequenceOffset = bogusSequenceOffset;
            hr = pPCMDesc->GetSequenceOffset( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( sequenceOffset == bogusSequenceOffset,
                             AAFRESULT_TEST_FAILED );


            // SetSequenceOffset() should always succeed
            hr = pPCMDesc->SetSequenceOffset( gTestData.sequenceOffset );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetSequenceOffset() should succeed.
            aafUInt8  sequenceOffset = bogusSequenceOffset;
            hr = pPCMDesc->GetSequenceOffset( &sequenceOffset );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( sequenceOffset == gTestData.sequenceOffset,
                             AAFRESULT_TEST_FAILED );
        }


        // If we got to this point none of the tests above
        // failed and the status can be set to 'success'.
        hr = S_OK;
    }
    catch( HRESULT e )
    {
        hr = e;
    }


    return hr;
}



static HRESULT Test_IAAFPCMDescriptor_AverageBPS(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafUInt32  bogusAverageBPS = 12082;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetAverageBPS() should fail if the property not present.
            aafUInt32  averageBPS = bogusAverageBPS;
            hr = pPCMDesc->GetAverageBPS( &averageBPS );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( averageBPS == bogusAverageBPS,
                             AAFRESULT_TEST_FAILED );


            // GetAverageBPS() should fail if function arguments are invalid.
            averageBPS = bogusAverageBPS;
            hr = pPCMDesc->GetAverageBPS( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( averageBPS == bogusAverageBPS,
                             AAFRESULT_TEST_FAILED );


            // SetAverageBPS() should always succeed
            hr = pPCMDesc->SetAverageBPS( gTestData.averageBPS );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetAverageBPS() should succeed.
            aafUInt32  averageBPS = bogusAverageBPS;
            hr = pPCMDesc->GetAverageBPS( &averageBPS );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( averageBPS == gTestData.averageBPS,
                             AAFRESULT_TEST_FAILED );
        }


        // If we got to this point none of the tests above
        // failed and the status can be set to 'success'.
        hr = S_OK;
    }
    catch( HRESULT e )
    {
        hr = e;
    }


    return hr;
}



static HRESULT Test_IAAFPCMDescriptor_AreAllPeakEnvelopePropertiesPresent(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        // AreAllPeakEnvelopePropertiesPresent() should fail if function
        // arguments are invalid.
        hr = pPCMDesc->AreAllPeakEnvelopePropertiesPresent( 0 );
        checkExpression( hr == AAFRESULT_NULL_PARAM,
                         AAFRESULT_TEST_FAILED );


        // See if all the needed properties are present
        aafUInt32  dummyUInt32 = 0;
        aafPosition_t  dummyPosition = 0;
        aafTimeStamp_t  dummyTimeStamp = { {0, 0, 0}, {0, 0, 0, 0} };
        aafBoolean_t  allPeakEnvelopePropsPresent = kAAFTrue;

        if( pPCMDesc->GetPeakEnvelopeVersion( &dummyUInt32 ) ==
            AAFRESULT_PROP_NOT_PRESENT )
        {
            allPeakEnvelopePropsPresent = kAAFFalse;
        }

        if( pPCMDesc->GetPeakEnvelopeFormat( &dummyUInt32 ) ==
            AAFRESULT_PROP_NOT_PRESENT )
        {
            allPeakEnvelopePropsPresent = kAAFFalse;
        }

        if( pPCMDesc->GetPointsPerPeakValue( &dummyUInt32 ) ==
            AAFRESULT_PROP_NOT_PRESENT )
        {
            allPeakEnvelopePropsPresent = kAAFFalse;
        }

        if( pPCMDesc->GetPeakEnvelopeBlockSize( &dummyUInt32 ) ==
            AAFRESULT_PROP_NOT_PRESENT )
        {
            allPeakEnvelopePropsPresent = kAAFFalse;
        }

        if( pPCMDesc->GetPeakChannelCount( &dummyUInt32 ) ==
            AAFRESULT_PROP_NOT_PRESENT )
        {
            allPeakEnvelopePropsPresent = kAAFFalse;
        }

        if( pPCMDesc->GetPeakFrameCount( &dummyUInt32 ) ==
            AAFRESULT_PROP_NOT_PRESENT )
        {
            allPeakEnvelopePropsPresent = kAAFFalse;
        }

        if( pPCMDesc->GetPeakOfPeaksPosition( &dummyPosition ) ==
            AAFRESULT_PROP_NOT_PRESENT )
        {
            allPeakEnvelopePropsPresent = kAAFFalse;
        }

        if( pPCMDesc->GetPeakEnvelopeTimestamp( &dummyTimeStamp ) ==
            AAFRESULT_PROP_NOT_PRESENT )
        {
            allPeakEnvelopePropsPresent = kAAFFalse;
        }

        if( pPCMDesc->GetPeakEnvelopeDataPosition( &dummyPosition ) ==
            AAFRESULT_PROP_NOT_PRESENT )
        {
            allPeakEnvelopePropsPresent = kAAFFalse;
        }


        aafBoolean_t  arePresent = ~allPeakEnvelopePropsPresent;
        hr = pPCMDesc->AreAllPeakEnvelopePropertiesPresent( &arePresent );
        checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        checkExpression( allPeakEnvelopePropsPresent == arePresent,
                         AAFRESULT_TEST_FAILED );


        // If we got to this point none of the tests above
        // failed and the status can be set to 'success'.
        hr = S_OK;
    }
    catch( HRESULT e )
    {
        hr = e;
    }


    return hr;
}



static HRESULT Test_IAAFPCMDescriptor_PeakEnvelopeVersion(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafUInt32  bogusPeakEnvelopeVersion = 283;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetPeakEnvelopeVersion() should fail if the property
            // not present.
            aafUInt32  peakEnvelopeVersion = bogusPeakEnvelopeVersion;
            hr = pPCMDesc->GetPeakEnvelopeVersion( &peakEnvelopeVersion );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakEnvelopeVersion == bogusPeakEnvelopeVersion,
                             AAFRESULT_TEST_FAILED );


            // GetPeakEnvelopeVersion() should fail if function
            // arguments are invalid.
            peakEnvelopeVersion = bogusPeakEnvelopeVersion;
            hr = pPCMDesc->GetPeakEnvelopeVersion( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakEnvelopeVersion == bogusPeakEnvelopeVersion,
                             AAFRESULT_TEST_FAILED );


            // SetPeakEnvelopeVersion() should always succeed
            hr = pPCMDesc->SetPeakEnvelopeVersion(
                gTestData.peakEnvelopeVersion );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetPeakEnvelopeVersion() should succeed.
            aafUInt32  peakEnvelopeVersion = bogusPeakEnvelopeVersion;
            hr = pPCMDesc->GetPeakEnvelopeVersion( &peakEnvelopeVersion );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( peakEnvelopeVersion == gTestData.peakEnvelopeVersion,
                             AAFRESULT_TEST_FAILED );
        }


        // If we got to this point none of the tests above
        // failed and the status can be set to 'success'.
        hr = S_OK;
    }
    catch( HRESULT e )
    {
        hr = e;
    }


    return hr;
}



static HRESULT Test_IAAFPCMDescriptor_PeakEnvelopeFormat(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafUInt32  bogusPeakEnvelopeFormat = 134;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetPeakEnvelopeFormat() should fail if the property not present.
            aafUInt32  peakEnvelopeFormat = bogusPeakEnvelopeFormat;
            hr = pPCMDesc->GetPeakEnvelopeFormat( &peakEnvelopeFormat );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakEnvelopeFormat == bogusPeakEnvelopeFormat,
                             AAFRESULT_TEST_FAILED );


            // GetPeakEnvelopeFormat() should fail if function
            // arguments are invalid.
            peakEnvelopeFormat = bogusPeakEnvelopeFormat;
            hr = pPCMDesc->GetPeakEnvelopeFormat( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakEnvelopeFormat == bogusPeakEnvelopeFormat,
                             AAFRESULT_TEST_FAILED );


            // SetPeakEnvelopeFormat() should always succeed
            hr = pPCMDesc->SetPeakEnvelopeFormat( gTestData.peakEnvelopeFormat );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetPeakEnvelopeFormat() should succeed.
            aafUInt32  peakEnvelopeFormat = bogusPeakEnvelopeFormat;
            hr = pPCMDesc->GetPeakEnvelopeFormat( &peakEnvelopeFormat );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( peakEnvelopeFormat == gTestData.peakEnvelopeFormat,
                             AAFRESULT_TEST_FAILED );
        }


        // If we got to this point none of the tests above
        // failed and the status can be set to 'success'.
        hr = S_OK;
    }
    catch( HRESULT e )
    {
        hr = e;
    }


    return hr;
}



static HRESULT Test_IAAFPCMDescriptor_PointsPerPeakValue(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafUInt32  bogusPointsPerPeakValue = 139;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetPointsPerPeakValue() should fail if the property not present.
            aafUInt32  pointsPerPeakValue = bogusPointsPerPeakValue;
            hr = pPCMDesc->GetPointsPerPeakValue( &pointsPerPeakValue );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( pointsPerPeakValue == bogusPointsPerPeakValue,
                             AAFRESULT_TEST_FAILED );


            // GetPointsPerPeakValue() should fail if function
            // arguments are invalid.
            pointsPerPeakValue = bogusPointsPerPeakValue;
            hr = pPCMDesc->GetPointsPerPeakValue( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( pointsPerPeakValue == bogusPointsPerPeakValue,
                             AAFRESULT_TEST_FAILED );


            // SetPointsPerPeakValue() should always succeed
            hr = pPCMDesc->SetPointsPerPeakValue( gTestData.pointsPerPeakValue );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetPointsPerPeakValue() should succeed.
            aafUInt32  pointsPerPeakValue = bogusPointsPerPeakValue;
            hr = pPCMDesc->GetPointsPerPeakValue( &pointsPerPeakValue );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( pointsPerPeakValue == gTestData.pointsPerPeakValue,
                             AAFRESULT_TEST_FAILED );
        }


        // If we got to this point none of the tests above
        // failed and the status can be set to 'success'.
        hr = S_OK;
    }
    catch( HRESULT e )
    {
        hr = e;
    }


    return hr;
}



static HRESULT Test_IAAFPCMDescriptor_PeakEnvelopeBlockSize(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafUInt32  bogusPeakEnvelopeBlockSize = 142;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetPeakEnvelopeBlockSize() should fail if the property not present.
            aafUInt32  peakEnvelopeBlockSize = bogusPeakEnvelopeBlockSize;
            hr = pPCMDesc->GetPeakEnvelopeBlockSize( &peakEnvelopeBlockSize );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakEnvelopeBlockSize == bogusPeakEnvelopeBlockSize,
                             AAFRESULT_TEST_FAILED );


            // GetPeakEnvelopeBlockSize() should fail if function
            // arguments are invalid.
            peakEnvelopeBlockSize = bogusPeakEnvelopeBlockSize;
            hr = pPCMDesc->GetPeakEnvelopeBlockSize( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakEnvelopeBlockSize == bogusPeakEnvelopeBlockSize,
                             AAFRESULT_TEST_FAILED );


            // SetPeakEnvelopeBlockSize() should always succeed
            hr = pPCMDesc->SetPeakEnvelopeBlockSize( gTestData.peakEnvelopeBlockSize );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetPeakEnvelopeBlockSize() should succeed.
            aafUInt32  peakEnvelopeBlockSize = bogusPeakEnvelopeBlockSize;
            hr = pPCMDesc->GetPeakEnvelopeBlockSize( &peakEnvelopeBlockSize );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( peakEnvelopeBlockSize == gTestData.peakEnvelopeBlockSize,
                             AAFRESULT_TEST_FAILED );
        }


        // If we got to this point none of the tests above
        // failed and the status can be set to 'success'.
        hr = S_OK;
    }
    catch( HRESULT e )
    {
        hr = e;
    }


    return hr;
}



static HRESULT Test_IAAFPCMDescriptor_PeakChannelCount(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafUInt32  bogusPeakChannelCount = 11198;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetPeakChannelCount() should fail if the property not present.
            aafUInt32  peakChannelCount = bogusPeakChannelCount;
            hr = pPCMDesc->GetPeakChannelCount( &peakChannelCount );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakChannelCount == bogusPeakChannelCount,
                             AAFRESULT_TEST_FAILED );


            // GetPeakChannelCount() should fail if function
            // arguments are invalid.
            peakChannelCount = bogusPeakChannelCount;
            hr = pPCMDesc->GetPeakChannelCount( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakChannelCount == bogusPeakChannelCount,
                             AAFRESULT_TEST_FAILED );


            // SetPeakChannelCount() should always succeed
            hr = pPCMDesc->SetPeakChannelCount( gTestData.peakChannelCount );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetPeakChannelCount() should succeed.
            aafUInt32  peakChannelCount = bogusPeakChannelCount;
            hr = pPCMDesc->GetPeakChannelCount( &peakChannelCount );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( peakChannelCount == gTestData.peakChannelCount,
                             AAFRESULT_TEST_FAILED );
        }


        // If we got to this point none of the tests above
        // failed and the status can be set to 'success'.
        hr = S_OK;
    }
    catch( HRESULT e )
    {
        hr = e;
    }


    return hr;
}



static HRESULT Test_IAAFPCMDescriptor_PeakFrameCount(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafUInt32  bogusPeakFrameCount = 1200;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetPeakFrameCount() should fail if the property not present.
            aafUInt32  peakFrameCount = bogusPeakFrameCount;
            hr = pPCMDesc->GetPeakFrameCount( &peakFrameCount );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakFrameCount == bogusPeakFrameCount,
                             AAFRESULT_TEST_FAILED );


            // GetPeakFrameCount() should fail if function
            // arguments are invalid.
            peakFrameCount = bogusPeakFrameCount;
            hr = pPCMDesc->GetPeakFrameCount( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakFrameCount == bogusPeakFrameCount,
                             AAFRESULT_TEST_FAILED );


            // SetPeakFrameCount() should always succeed
            hr = pPCMDesc->SetPeakFrameCount( gTestData.peakFrameCount );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetPeakFrameCount() should succeed.
            aafUInt32  peakFrameCount = bogusPeakFrameCount;
            hr = pPCMDesc->GetPeakFrameCount( &peakFrameCount );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( peakFrameCount == gTestData.peakFrameCount,
                             AAFRESULT_TEST_FAILED );
        }


        // If we got to this point none of the tests above
        // failed and the status can be set to 'success'.
        hr = S_OK;
    }
    catch( HRESULT e )
    {
        hr = e;
    }


    return hr;
}



static HRESULT Test_IAAFPCMDescriptor_PeakOfPeaksPosition(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafPosition_t  bogusPeakOfPeaksPosition = -49100;


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetPeakOfPeaksPosition() should fail if the property not present.
            aafPosition_t  peakOfPeaksPosition = bogusPeakOfPeaksPosition;
            hr = pPCMDesc->GetPeakOfPeaksPosition( &peakOfPeaksPosition );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakOfPeaksPosition == bogusPeakOfPeaksPosition,
                             AAFRESULT_TEST_FAILED );


            // GetPeakOfPeaksPosition() should fail if function
            // arguments are invalid.
            peakOfPeaksPosition = bogusPeakOfPeaksPosition;
            hr = pPCMDesc->GetPeakOfPeaksPosition( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            checkExpression( peakOfPeaksPosition == bogusPeakOfPeaksPosition,
                             AAFRESULT_TEST_FAILED );


            // SetPeakOfPeaksPosition() should always succeed
            hr = pPCMDesc->SetPeakOfPeaksPosition( gTestData.peakOfPeaksPosition );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetPeakOfPeaksPosition() should succeed.
            aafPosition_t  peakOfPeaksPosition = bogusPeakOfPeaksPosition;
            hr = pPCMDesc->GetPeakOfPeaksPosition( &peakOfPeaksPosition );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            checkExpression( peakOfPeaksPosition == gTestData.peakOfPeaksPosition,
                             AAFRESULT_TEST_FAILED );
        }


        // If we got to this point none of the tests above
        // failed and the status can be set to 'success'.
        hr = S_OK;
    }
    catch( HRESULT e )
    {
        hr = e;
    }


    return hr;
}



static HRESULT Test_IAAFPCMDescriptor_PeakEnvelopeTimestamp(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        static const aafTimeStamp_t  bogusPeakEnvelopeTimestamp =
                                        { {1941, 7, 22}, {4, 0, 0, 0} };


        if( mode == kAAFUnitTestReadWrite )
        {
            // GetPeakEnvelopeTimestamp() should fail if the property not present.
            aafTimeStamp_t  peakEnvelopeTimestamp = bogusPeakEnvelopeTimestamp;
            hr = pPCMDesc->GetPeakEnvelopeTimestamp( &peakEnvelopeTimestamp );
            checkExpression( hr == AAFRESULT_PROP_NOT_PRESENT,
                             AAFRESULT_TEST_FAILED );
            //checkExpression( peakEnvelopeTimestamp == bogusPeakEnvelopeTimestamp,
            //                 AAFRESULT_TEST_FAILED );


            // GetPeakEnvelopeTimestamp() should fail if function
            // arguments are invalid.
            peakEnvelopeTimestamp = bogusPeakEnvelopeTimestamp;
            hr = pPCMDesc->GetPeakEnvelopeTimestamp( 0 );
            checkExpression( hr == AAFRESULT_NULL_PARAM,
                             AAFRESULT_TEST_FAILED );
            //checkExpression( peakEnvelopeTimestamp == bogusPeakEnvelopeTimestamp,
            //                 AAFRESULT_TEST_FAILED );


            // SetPeakEnvelopeTimestamp() should always succeed
            hr = pPCMDesc->SetPeakEnvelopeTimestamp( gTestData.peakEnvelopeTimestamp );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
        }


        if( mode == kAAFUnitTestReadWrite || mode == kAAFUnitTestReadOnly )
        {
            // GetPeakEnvelopeTimestamp() should succeed.
            aafTimeStamp_t  peakEnvelopeTimestamp = bogusPeakEnvelopeTimestamp;
            hr = pPCMDesc->GetPeakEnvelopeTimestamp( &peakEnvelopeTimestamp );
            checkExpression( hr == AAFRESULT_SUCCESS, AAFRESULT_TEST_FAILED );
            //checkExpression( peakEnvelopeTimestamp == gTestData.peakEnvelopeTimestamp,
            //                 AAFRESULT_TEST_FAILED );
        }


        // If we got to this point none of the tests above
        // failed and the status can be set to 'success'.
        hr = S_OK;
    }
    catch( HRESULT e )
    {
        hr = e;
    }


    return hr;
}



static HRESULT Test_IAAFSoundDescriptor_WritePeakEnvelopeData(
    IAAFPCMDescriptor* pPCMDesc,
    testMode_t mode )
{
    HRESULT  hr = S_OK;


    try
    {
        /*
        // () should fail if function arguments are invalid.
        const aafUInt32  bogusBytesWritten = 21;
        aafUInt32  bytesWritten = bogusBytesWritten;
        hr = pPCMDesc->WritePeakEnvelopeData( 0,
                                              gTestData.peakEnvelopeData,
                                              &bytesWritten );
        checkExpression( hr == AAFRESULT_INVALID_PARAM,
                         AAFRESULT_TEST_FAILED );
        checkExpression( bytesWritten == bogusBytesWritten,
                         AAFRESULT_TEST_FAILED );

        bytesWritten = bogusBytesWritten;
        hr = pPCMDesc->WritePeakEnvelopeData( peakEnvelopeDataSize,
                                              0,
                                              &bytesWritten );
        checkExpression( hr == AAFRESULT_NULL_PARAM,
                         AAFRESULT_TEST_FAILED );
        checkExpression( bytesWritten == bogusBytesWritten,
                         AAFRESULT_TEST_FAILED );

        hr = pPCMDesc->WritePeakEnvelopeData( peakEnvelopeDataSize,
                                              gTestData.peakEnvelopeData,
                                              0 );
        checkExpression( hr == AAFRESULT_NULL_PARAM,
                         AAFRESULT_TEST_FAILED );
                         */


        // If we got to this point none of the tests above
        // failed and the status can be set to 'success'.
        hr = S_OK;
    }
    catch( HRESULT e )
    {
        hr = e;
    }


    return hr;
}



