/** @file FileSystemWatcher.cpp



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

#include "ugl/FileSystemWatcher.hpp"

#include <iostream>

#include <boost/filesystem.hpp>
#include <boost/algorithm/string/predicate.hpp>

namespace ugl
{

/**
 * @brief Adds a path or directory to be watched.
 * @param path
 * @param callback
 */
void FileSystemWatcher::watch(const std::string& path, Listener *listener)
{
    try
    {
        // Extract directory from path
        boost::filesystem::path p(path);
        bool isDir = boost::filesystem::is_directory(p);
        std::string watchDir = isDir ? path : p.parent_path().string();

        // Check if we already have a watcher for this directory
        if (std::find_if(this->registry.begin(), this->registry.end(), [&](const Entry& e) { return e.watchDir == watchDir; }) == this->registry.end())
            this->watcher.addWatch(watchDir, this);

        // Add registry entry for this path
        this->registry.push_back( Entry { path, isDir, watchDir, listener } );
    }
    catch (const FW::FileNotFoundException&)
    {
         std::cerr << "Error: Could not watch directory \"" << path << "\"." << std::endl;
    }
}


/**
 * @brief Removes all registry entries for this listener.
 * @param listener
 */
void FileSystemWatcher::remove(Listener* listener)
{
    std::remove_if(this->registry.begin(), this->registry.end(), [&](const Entry& e) { return e.listener == listener; } );
}


/**
 * @brief Update file watcher. This should be called each frame.
 */
void FileSystemWatcher::update()
{
    this->watcher.update();
}


/**
 * @brief FileWatcher event handler.
 * @param watchid
 * @param dir
 * @param filename
 * @param action
 */
void FileSystemWatcher::handleFileAction(FW::WatchID /*watchID*/, const FW::String& dir, const FW::String& filename, FW::Action /*action*/)
{
    for (auto&& e : this->registry)
        if (e.watchDir == dir && (e.isDir || boost::algorithm::ends_with(e.path, filename)))
            e.listener->fileEvent(e.path);
}

}
