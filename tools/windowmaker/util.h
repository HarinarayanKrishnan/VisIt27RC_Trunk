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

#include <stdio.h>
#include <string>

std::string
i2s(int i)
{
    char tmp[1000];
    sprintf(tmp, "%d", i);
    return std::string(tmp);
}

char
toLower(char c)
{
    if (c>='A' && c<='Z')
        return c + (int('a')-int('A'));
    else
        return c;
}

char
toUpper(char c)
{
    if (c>='a' && c<='z')
        return c + (int('A')-int('a'));
    else
        return c;
}

std::string
toUpper(std::string s)
{
    for (int i=0; i<s.length(); i++)
        s[i] = toUpper(s[i]);
    return s;
}

std::string
findWord(const std::string &s)
{
    int i=0;
    std::string foo("");
    while (s.length() > i && (s[i] == ' ' || s[i] == 0x08 || s[i] == '\t' || s[i] == '\n'))
        i++;
    while (s.length() > i && (s[i] != ' ' && s[i] != 0x08 && s[i] != '\t' && s[i] != '\n'))
    {
        foo += s[i];
        i++;
    }
    return foo;
}

std::string
stripWord(std::string &s)
{
    int i = 0;
    std::string foo("");
    while (s.length() > i && (s[i] == ' ' || s[i] == 0x08 || s[i] == '\t' || s[i] == '\n'))
        i++;
    int len=0;
    while (s.length() > i && (s[i] != ' ' && s[i] != 0x08 && s[i] != '\t' && s[i] != '\n'))
    {
        foo += s[i];
        i++;
        len++;
    }
    if (len && s.length() > i)
        s = s.substr(i, s.length() - i);
    return foo;
}

