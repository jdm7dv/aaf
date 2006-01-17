//=---------------------------------------------------------------------=
//
// $Id: RequirementXMLException.h,v 1.3 2005/10/18 17:02:42 ajakowpa Exp $ $Name:  $
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
//=---------------------------------------------------------------------=

#ifndef __REQUIREMENTXMLEXCEPTION_H_
#define __REQUIREMENTXMLEXCEPTION_H_

//Analyzer Base files
#include <AnalyzerException.h>

namespace aafanalyzer {

using namespace std;

class RequirementXMLException : public AnalyzerException {

  public:

    RequirementXMLException( const wchar_t* what );
    virtual ~RequirementXMLException() throw();
    
  private:
  
    // prohibited
    RequirementXMLException();
    RequirementXMLException& operator=( const RequirementXMLException& );
    
};

} // end of namespace diskstream

#endif /*__REQUIREMENTXMLEXCEPTION_H_*/