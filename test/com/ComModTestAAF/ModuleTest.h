//=---------------------------------------------------------------------=
//
// $Id: ModuleTest.h,v 1.5.2.4 2004/06/08 13:46:07 stuart_hc Exp $ $Name:  $
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

#ifndef MODULE_TEST_COMMON_H
#define MODULE_TEST_COMMON_H

#include "AAF.h"


typedef enum unitTestMode
{
	kAAFUnitTestReadWrite, kAAFUnitTestReadOnly
} testMode_t;



#ifdef _cplusplus
extern "C" {
#endif

//
// Feature support
//

// Return the current version of the AAF toolkit shared library and
// the version of the toolkit used to create the file.
// NOTE: The parameter fileAAFVersion may be NULL.
HRESULT GetAAFVersions(IAAFHeader * pHeader, 
                       aafProductVersion_t * currentAAFVersion, 
                       aafProductVersion_t * fileAAFVersion);


// Support for Sets (strong reference sets) added in DR4
bool StrongReferenceSetsSupported(aafProductVersion_constref toolkitVersion);

// Support for weak references was added after DR4
bool WeakReferencesSupported(aafProductVersion_constref toolkitVersion);

// Support for weak reference sets was added after DR4
bool WeakReferenceSetsSupported(aafProductVersion_constref toolkitVersion);

// Support for extending AAFObject was added after DR4
bool ExtendingAAFObjectSupported(aafProductVersion_constref toolkitVersion);

// Return true is greater than DR3
bool DR4TestSupported(aafProductVersion_constref toolkitVersion);

// Return true is greater than DR4 (a.k.a. RC1)
bool RC1TestSupported(aafProductVersion_constref toolkitVersion);

#ifdef _cplusplus
}
#endif

//
// Comparison operators
//
bool operator ==( const aafUID_t uid1, const aafUID_t uid2 );
bool operator !=( const aafUID_t uid1, const aafUID_t uid2 );

bool operator ==( const aafRational_t& a, const aafRational_t& b );
bool operator !=( const aafRational_t& a, const aafRational_t& b );

bool operator ==( const aafTimeStamp_t& a, const aafTimeStamp_t& b );
bool operator !=( const aafTimeStamp_t& a, const aafTimeStamp_t& b );


#endif  // MODULE_TEST_COMMON_H
