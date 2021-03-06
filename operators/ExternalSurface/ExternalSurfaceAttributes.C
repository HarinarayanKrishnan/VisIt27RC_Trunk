/*****************************************************************************
*
* Copyright (c) 2000 - 2013, Lawrence Livermore National Security, LLC
* Produced at the Lawrence Livermore National Laboratory
* LLNL-CODE-442911
* All rights reserved.
*
* This file is  part of VisIt. For  details, see https://visit.llnl.gov/.  The
* full copyright notice is contained in the file COPYRIGHT located at the root
* of the VisIt distribution or at http://www.llnl.gov/visit/copyright.html.
*
* Redistribution  and  use  in  source  and  binary  forms,  with  or  without
* modification, are permitted provided that the following conditions are met:
*
*  - Redistributions of  source code must  retain the above  copyright notice,
*    this list of conditions and the disclaimer below.
*  - Redistributions in binary form must reproduce the above copyright notice,
*    this  list of  conditions  and  the  disclaimer (as noted below)  in  the
*    documentation and/or other materials provided with the distribution.
*  - Neither the name of  the LLNS/LLNL nor the names of  its contributors may
*    be used to endorse or promote products derived from this software without
*    specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT  HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR  IMPLIED WARRANTIES, INCLUDING,  BUT NOT  LIMITED TO, THE
* IMPLIED WARRANTIES OF MERCHANTABILITY AND  FITNESS FOR A PARTICULAR  PURPOSE
* ARE  DISCLAIMED. IN  NO EVENT  SHALL LAWRENCE  LIVERMORE NATIONAL  SECURITY,
* LLC, THE  U.S.  DEPARTMENT OF  ENERGY  OR  CONTRIBUTORS BE  LIABLE  FOR  ANY
* DIRECT,  INDIRECT,   INCIDENTAL,   SPECIAL,   EXEMPLARY,  OR   CONSEQUENTIAL
* DAMAGES (INCLUDING, BUT NOT  LIMITED TO, PROCUREMENT OF  SUBSTITUTE GOODS OR
* SERVICES; LOSS OF  USE, DATA, OR PROFITS; OR  BUSINESS INTERRUPTION) HOWEVER
* CAUSED  AND  ON  ANY  THEORY  OF  LIABILITY,  WHETHER  IN  CONTRACT,  STRICT
* LIABILITY, OR TORT  (INCLUDING NEGLIGENCE OR OTHERWISE)  ARISING IN ANY  WAY
* OUT OF THE  USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH
* DAMAGE.
*
*****************************************************************************/

#include <ExternalSurfaceAttributes.h>
#include <DataNode.h>

// ****************************************************************************
// Method: ExternalSurfaceAttributes::ExternalSurfaceAttributes
//
// Purpose: 
//   Init utility for the ExternalSurfaceAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

void ExternalSurfaceAttributes::Init()
{
    removeGhosts = false;
    edgesIn2D = true;

    ExternalSurfaceAttributes::SelectAll();
}

// ****************************************************************************
// Method: ExternalSurfaceAttributes::ExternalSurfaceAttributes
//
// Purpose: 
//   Copy utility for the ExternalSurfaceAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

void ExternalSurfaceAttributes::Copy(const ExternalSurfaceAttributes &obj)
{
    removeGhosts = obj.removeGhosts;
    edgesIn2D = obj.edgesIn2D;

    ExternalSurfaceAttributes::SelectAll();
}

// Type map format string
const char *ExternalSurfaceAttributes::TypeMapFormatString = EXTERNALSURFACEATTRIBUTES_TMFS;
const AttributeGroup::private_tmfs_t ExternalSurfaceAttributes::TmfsStruct = {EXTERNALSURFACEATTRIBUTES_TMFS};


// ****************************************************************************
// Method: ExternalSurfaceAttributes::ExternalSurfaceAttributes
//
// Purpose: 
//   Default constructor for the ExternalSurfaceAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

ExternalSurfaceAttributes::ExternalSurfaceAttributes() : 
    AttributeSubject(ExternalSurfaceAttributes::TypeMapFormatString)
{
    ExternalSurfaceAttributes::Init();
}

// ****************************************************************************
// Method: ExternalSurfaceAttributes::ExternalSurfaceAttributes
//
// Purpose: 
//   Constructor for the derived classes of ExternalSurfaceAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

ExternalSurfaceAttributes::ExternalSurfaceAttributes(private_tmfs_t tmfs) : 
    AttributeSubject(tmfs.tmfs)
{
    ExternalSurfaceAttributes::Init();
}

// ****************************************************************************
// Method: ExternalSurfaceAttributes::ExternalSurfaceAttributes
//
// Purpose: 
//   Copy constructor for the ExternalSurfaceAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

ExternalSurfaceAttributes::ExternalSurfaceAttributes(const ExternalSurfaceAttributes &obj) : 
    AttributeSubject(ExternalSurfaceAttributes::TypeMapFormatString)
{
    ExternalSurfaceAttributes::Copy(obj);
}

// ****************************************************************************
// Method: ExternalSurfaceAttributes::ExternalSurfaceAttributes
//
// Purpose: 
//   Copy constructor for derived classes of the ExternalSurfaceAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

ExternalSurfaceAttributes::ExternalSurfaceAttributes(const ExternalSurfaceAttributes &obj, private_tmfs_t tmfs) : 
    AttributeSubject(tmfs.tmfs)
{
    ExternalSurfaceAttributes::Copy(obj);
}

// ****************************************************************************
// Method: ExternalSurfaceAttributes::~ExternalSurfaceAttributes
//
// Purpose: 
//   Destructor for the ExternalSurfaceAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

ExternalSurfaceAttributes::~ExternalSurfaceAttributes()
{
    // nothing here
}

// ****************************************************************************
// Method: ExternalSurfaceAttributes::operator = 
//
// Purpose: 
//   Assignment operator for the ExternalSurfaceAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

ExternalSurfaceAttributes& 
ExternalSurfaceAttributes::operator = (const ExternalSurfaceAttributes &obj)
{
    if (this == &obj) return *this;

    ExternalSurfaceAttributes::Copy(obj);

    return *this;
}

// ****************************************************************************
// Method: ExternalSurfaceAttributes::operator == 
//
// Purpose: 
//   Comparison operator == for the ExternalSurfaceAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

bool
ExternalSurfaceAttributes::operator == (const ExternalSurfaceAttributes &obj) const
{
    // Create the return value
    return ((removeGhosts == obj.removeGhosts) &&
            (edgesIn2D == obj.edgesIn2D));
}

// ****************************************************************************
// Method: ExternalSurfaceAttributes::operator != 
//
// Purpose: 
//   Comparison operator != for the ExternalSurfaceAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

bool
ExternalSurfaceAttributes::operator != (const ExternalSurfaceAttributes &obj) const
{
    return !(this->operator == (obj));
}

// ****************************************************************************
// Method: ExternalSurfaceAttributes::TypeName
//
// Purpose: 
//   Type name method for the ExternalSurfaceAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

const std::string
ExternalSurfaceAttributes::TypeName() const
{
    return "ExternalSurfaceAttributes";
}

// ****************************************************************************
// Method: ExternalSurfaceAttributes::CopyAttributes
//
// Purpose: 
//   CopyAttributes method for the ExternalSurfaceAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

bool
ExternalSurfaceAttributes::CopyAttributes(const AttributeGroup *atts)
{
    if(TypeName() != atts->TypeName())
        return false;

    // Call assignment operator.
    const ExternalSurfaceAttributes *tmp = (const ExternalSurfaceAttributes *)atts;
    *this = *tmp;

    return true;
}

// ****************************************************************************
// Method: ExternalSurfaceAttributes::CreateCompatible
//
// Purpose: 
//   CreateCompatible method for the ExternalSurfaceAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

AttributeSubject *
ExternalSurfaceAttributes::CreateCompatible(const std::string &tname) const
{
    AttributeSubject *retval = 0;
    if(TypeName() == tname)
        retval = new ExternalSurfaceAttributes(*this);
    // Other cases could go here too. 

    return retval;
}

// ****************************************************************************
// Method: ExternalSurfaceAttributes::NewInstance
//
// Purpose: 
//   NewInstance method for the ExternalSurfaceAttributes class.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

AttributeSubject *
ExternalSurfaceAttributes::NewInstance(bool copy) const
{
    AttributeSubject *retval = 0;
    if(copy)
        retval = new ExternalSurfaceAttributes(*this);
    else
        retval = new ExternalSurfaceAttributes;

    return retval;
}

// ****************************************************************************
// Method: ExternalSurfaceAttributes::SelectAll
//
// Purpose: 
//   Selects all attributes.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

void
ExternalSurfaceAttributes::SelectAll()
{
    Select(ID_removeGhosts, (void *)&removeGhosts);
    Select(ID_edgesIn2D,    (void *)&edgesIn2D);
}

///////////////////////////////////////////////////////////////////////////////
// Persistence methods
///////////////////////////////////////////////////////////////////////////////

// ****************************************************************************
// Method: ExternalSurfaceAttributes::CreateNode
//
// Purpose: 
//   This method creates a DataNode representation of the object so it can be saved to a config file.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

bool
ExternalSurfaceAttributes::CreateNode(DataNode *parentNode, bool completeSave, bool forceAdd)
{
    if(parentNode == 0)
        return false;

    ExternalSurfaceAttributes defaultObject;
    bool addToParent = false;
    // Create a node for ExternalSurfaceAttributes.
    DataNode *node = new DataNode("ExternalSurfaceAttributes");

    if(completeSave || !FieldsEqual(ID_removeGhosts, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("removeGhosts", removeGhosts));
    }

    if(completeSave || !FieldsEqual(ID_edgesIn2D, &defaultObject))
    {
        addToParent = true;
        node->AddNode(new DataNode("edgesIn2D", edgesIn2D));
    }


    // Add the node to the parent node.
    if(addToParent || forceAdd)
        parentNode->AddNode(node);
    else
        delete node;

    return (addToParent || forceAdd);
}

// ****************************************************************************
// Method: ExternalSurfaceAttributes::SetFromNode
//
// Purpose: 
//   This method sets attributes in this object from values in a DataNode representation of the object.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

void
ExternalSurfaceAttributes::SetFromNode(DataNode *parentNode)
{
    if(parentNode == 0)
        return;

    DataNode *searchNode = parentNode->GetNode("ExternalSurfaceAttributes");
    if(searchNode == 0)
        return;

    DataNode *node;
    if((node = searchNode->GetNode("removeGhosts")) != 0)
        SetRemoveGhosts(node->AsBool());
    if((node = searchNode->GetNode("edgesIn2D")) != 0)
        SetEdgesIn2D(node->AsBool());
}

///////////////////////////////////////////////////////////////////////////////
// Set property methods
///////////////////////////////////////////////////////////////////////////////

void
ExternalSurfaceAttributes::SetRemoveGhosts(bool removeGhosts_)
{
    removeGhosts = removeGhosts_;
    Select(ID_removeGhosts, (void *)&removeGhosts);
}

void
ExternalSurfaceAttributes::SetEdgesIn2D(bool edgesIn2D_)
{
    edgesIn2D = edgesIn2D_;
    Select(ID_edgesIn2D, (void *)&edgesIn2D);
}

///////////////////////////////////////////////////////////////////////////////
// Get property methods
///////////////////////////////////////////////////////////////////////////////

bool
ExternalSurfaceAttributes::GetRemoveGhosts() const
{
    return removeGhosts;
}

bool
ExternalSurfaceAttributes::GetEdgesIn2D() const
{
    return edgesIn2D;
}

///////////////////////////////////////////////////////////////////////////////
// Keyframing methods
///////////////////////////////////////////////////////////////////////////////

// ****************************************************************************
// Method: ExternalSurfaceAttributes::GetFieldName
//
// Purpose: 
//   This method returns the name of a field given its index.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

std::string
ExternalSurfaceAttributes::GetFieldName(int index) const
{
    switch (index)
    {
    case ID_removeGhosts: return "removeGhosts";
    case ID_edgesIn2D:    return "edgesIn2D";
    default:  return "invalid index";
    }
}

// ****************************************************************************
// Method: ExternalSurfaceAttributes::GetFieldType
//
// Purpose: 
//   This method returns the type of a field given its index.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

AttributeGroup::FieldType
ExternalSurfaceAttributes::GetFieldType(int index) const
{
    switch (index)
    {
    case ID_removeGhosts: return FieldType_bool;
    case ID_edgesIn2D:    return FieldType_bool;
    default:  return FieldType_unknown;
    }
}

// ****************************************************************************
// Method: ExternalSurfaceAttributes::GetFieldTypeName
//
// Purpose: 
//   This method returns the name of a field type given its index.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

std::string
ExternalSurfaceAttributes::GetFieldTypeName(int index) const
{
    switch (index)
    {
    case ID_removeGhosts: return "bool";
    case ID_edgesIn2D:    return "bool";
    default:  return "invalid index";
    }
}

// ****************************************************************************
// Method: ExternalSurfaceAttributes::FieldsEqual
//
// Purpose: 
//   This method compares two fields and return true if they are equal.
//
// Note:       Autogenerated by xml2atts.
//
// Programmer: xml2atts
// Creation:   omitted
//
// Modifications:
//   
// ****************************************************************************

bool
ExternalSurfaceAttributes::FieldsEqual(int index_, const AttributeGroup *rhs) const
{
    const ExternalSurfaceAttributes &obj = *((const ExternalSurfaceAttributes*)rhs);
    bool retval = false;
    switch (index_)
    {
    case ID_removeGhosts:
        {  // new scope
        retval = (removeGhosts == obj.removeGhosts);
        }
        break;
    case ID_edgesIn2D:
        {  // new scope
        retval = (edgesIn2D == obj.edgesIn2D);
        }
        break;
    default: retval = false;
    }

    return retval;
}

///////////////////////////////////////////////////////////////////////////////
// User-defined methods.
///////////////////////////////////////////////////////////////////////////////

