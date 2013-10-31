// =============================================================================
//
// Copyright (c) 2009-2013 Christopher Baker <http://christopherbaker.net>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//
// =============================================================================


#pragma once


#include <set>
#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/String.h"
#include "Poco/UTF8String.h"
#include "ofx/IO/AbstractTypes.h"


namespace ofx {
namespace IO {


class FileFilterCollection: public AbstractFileFilter
{
public:
    FileFilterCollection()
    {
    }

    virtual ~FileFilterCollection()
    {
    }

    bool accept(const Poco::File& file) const
    {
        std::set<AbstractFileFilter*>::iterator iter = _filters.begin();
        while(iter != _filters.end())
        {
            if(!(*iter)->accept(file))
            {
                return false;
            }
            ++iter;
        }
        return true;
    }

    void addFilter(AbstractFileFilter* filter)
    {
        _filters.insert(filter);
    }

    void removeFilter(AbstractFileFilter* filter)
    {
        _filters.erase(filter);
    }

private:
    std::set<AbstractFileFilter*> _filters;
};


class FileExtensionFilter: public AbstractFileFilter
{
public:

    FileExtensionFilter(): _ignoreCase(true), _includeMatches(true)
    {
    }

    FileExtensionFilter(bool ignoreCase, bool includeMatches = true):
        _ignoreCase(ignoreCase),
        _includeMatches(includeMatches)
    {
    }

    virtual ~FileExtensionFilter()
    {
    }

    bool accept(const Poco::File& file) const
    {
        
        Poco::Path path(file.path());
        std::string extension = path.getExtension();

        std::set<std::string>::iterator iter = _extensions.begin();

        while(iter != _extensions.end())
        {
            bool match = false;

            if(_ignoreCase)
            {
                match = (Poco::UTF8::icompare(extension,*iter) == 0);
            }
            else
            {
                match = (extension == *iter);
            }

            if(_includeMatches && match)
            {
                return true;
            }

            ++iter;
        }

        return !_includeMatches;

    }

    void addExtension(const std::string& extension)
    {
        _extensions.insert(extension);
    }

    void removeExtension(const std::string& extension)
    {
        _extensions.erase(extension);
    }

    void setIgnoreCase(bool ignoreCase)
    {
        _ignoreCase = ignoreCase;
    }

    bool getIgnoreCase() const
    {
        return _ignoreCase;
    }

private:
    bool _ignoreCase;
    bool _includeMatches;
    std::set<std::string> _extensions;
    
};


class HiddenFileFilter: public AbstractFileFilter
{
public:
    HiddenFileFilter()
    {
    }

    virtual ~HiddenFileFilter()
    {
    }

    bool accept(const Poco::File& file) const
    {
        return !file.isHidden();
    }
    
};


class DirectoryFilter: public AbstractFileFilter
{
public:
    DirectoryFilter()
    {
    }

    virtual ~DirectoryFilter()
    {
    }

    bool accept(const Poco::File& file) const
    {
        return !file.isDirectory();
    }
    
};


} } // namespace ofx::IO
