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
* THE SOFTWARE IS PROVIDED "AS-IS" AND WITHOUT WARRANTY OF ANY KIND,
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

// @doc OMEXTERNAL
#ifndef OMWIDESTRINGPROPERTY_H
#define OMWIDESTRINGPROPERTY_H

#include "OMVariableSizeProperty.h"

  // @class Abstract base class for persistent character string
  //        properties supported by the Object Manager.
  //   @tcarg class | CharacterType | The type of the characters that
  //                                  comprise the string.
  //   @base public | <c OMVariableSizeProperty>
template <typename CharacterType>
class OMCharacterStringProperty :
                                 public OMVariableSizeProperty<CharacterType> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMCharacterStringProperty(const OMPropertyId propertyId, const char * name);

    // @cmember Destructor.
  virtual ~OMCharacterStringProperty(void);

    // @cmember Type conversion. Convert an
    //          <c OMCharacterStringProperty> into a
    //          string of <p CharacterType> characters.
  operator const CharacterType* ();

    // @cmember Type conversion. Convert an
    //          <c OMCharacterStringProperty> into a
    //          string of <p CharacterType> characters.
  operator const CharacterType* () const;

    // @cmember Assign the string <p characterString> to this
    //          <c OMCharacterStringProperty>.
  void assign(const CharacterType* characterString);

    // @cmember The length of this <c OMCharacterStringProperty> in
    //          characters (not counting the null terminating character).
  size_t length(void) const;

    // @cmember Utility function for computing the length, in
    //          characters, of the string of <p CharacterType>
    //          characters <p characterString>.
  static size_t stringLength(const CharacterType* characterString);

private:
  // hide, declare but don't define
  operator CharacterType* (void);
};

  // @class Persistent character strings supported by the Object Manager.
  //   @base public | <c OMCharacterStringProperty>
class OMStringProperty : public OMCharacterStringProperty<char> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMStringProperty(const OMPropertyId propertyId, const char* name);

    // @cmember Destructor.
  virtual ~OMStringProperty(void);

    // @cmember Assignment operator.
  OMStringProperty& operator = (const char* value);

};

  // @class Persistent wide character strings supported by the Object Manager.
  //   @base public | <c OMCharacterStringProperty>
class OMWideStringProperty : public OMCharacterStringProperty<wchar_t> {
public:
  // @access Public members.

    // @cmember Constructor.
  OMWideStringProperty(const OMPropertyId propertyId, const char* name);

    // @cmember Destructor.
  virtual ~OMWideStringProperty(void);

    // @cmember Assignment operator.
  OMWideStringProperty& operator = (const wchar_t * value);

};

#include "OMWideStringPropertyT.h"

#endif
