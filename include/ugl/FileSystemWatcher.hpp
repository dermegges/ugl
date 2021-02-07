/** @file FileSystemWatcher.hpp



Copyright 2016 Computational Topology Group, University of Kaiserslautern

   Licensed under the Apache License, Version 2.0 (the "License");
   you may not use this file except in compliance with the License.
   You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

   Unless required by applicable law or agreed to in writing, software
   distributed under the License is distributed on an "AS IS" BASIS,
   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
   See the License for the specific language governing permissions and
   limitations under the License.

    Author(s): C.Garth, T.Biedert
*/

#ifndef __FileSystemWatcher_hpp
#define __FileSystemWatcher_hpp

#include <string>
#include <functional>
#include <map>
#include <vector>

#include <FileWatcher/FileWatcher.h>


namespace ugl
{

/**
 * @brief Notifications about file changes.
 */
class FileSystemWatcher : public FW::FileWatchListener
{
public:
    class Listener
    {
    public:
        virtual void fileEvent(const std::string& path) = 0;
    };

public:
    /**
     * @brief Returns the singleton instance.
     * @return
     */
    static FileSystemWatcher& getInstance()
    {
        static FileSystemWatcher instance;
        return instance;
    }

    virtual ~FileSystemWatcher() {}

    void watch(const std::string& path, Listener* listener);
    void remove(Listener* listener);
    void update();

private:
    void handleFileAction(FW::WatchID watchID, const FW::String& dir, const FW::String& filename, FW::Action action);

private:
    FileSystemWatcher() {}

    FileSystemWatcher(const FileSystemWatcher&) = delete;
    void operator=(const FileSystemWatcher&) = delete;

private:
    FW::FileWatcher watcher;

    struct Entry
    {
        std::string path;
        bool isDir;
        std::string watchDir;
        Listener* listener;
    };

    std::vector<Entry> registry;
};

}
#endif
