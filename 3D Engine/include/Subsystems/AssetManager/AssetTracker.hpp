#ifndef SATURN_ASSET_TRACKER_HPP_
#define SATURN_ASSET_TRACKER_HPP_

#include <efsw/efsw.hpp>

#include <string>
#include <vector>

#include "AssetManager.hpp"
#include "Editor/EditorLog.hpp"
#include "Editor/ProjectFile.hpp"

namespace Saturn {

template<typename A>
class AssetTracker;

template<typename R>
class AssetManager;

template<typename A>
class AssetListener : public efsw::FileWatchListener {
public:
    AssetListener(AssetTracker<A>* tracker);

    void handleFileAction(efsw::WatchID watchid,
                          const std::string& dir,
                          const std::string& filename,
                          efsw::Action action,
                          std::string oldFilename /* = "" */) override;

private:
    AssetTracker<A>* tracker;
};

template<typename A>
class AssetTracker {
public:
    AssetTracker() : listener(this) {
        fs::path assets_abs = Editor::ProjectFile::root_path() / "assets";
        fs::path assets_rel = fs::relative(assets_abs);
        // start the file watcher
        watcher.addWatch(assets_rel.generic_string(), &listener, true);
        watcher.watch();
    }
    using asset_data_t = typename AssetManager<A>::Asset;

    struct TrackedAsset {
        asset_data_t* asset;
        std::vector<fs::path> associated_paths;
    };

    void start_tracking(asset_data_t& asset,
                        std::vector<fs::path> associated_paths) {
        tracked_assets.push_back(
            TrackedAsset{&asset, std::move(associated_paths)});
    }

    void stop_tracking(asset_data_t& asset) {
        auto find_asset_f = [&asset](TrackedAsset const& tracked) -> bool {
            return tracked.asset->id == asset.id;
        };
        tracked_assets.erase(std::find_if(tracked_assets.begin(),
                                          tracked_assets.end(), find_asset_f),
                             tracked_assets.end());
    }

    std::vector<TrackedAsset> tracked_assets;

private:
    AssetListener<A> listener;
    efsw::FileWatcher watcher;
    efsw::WatchID asset_folder;
};

template<typename A>
AssetListener<A>::AssetListener(AssetTracker<A>* tracker) : tracker(tracker) {}

template<typename A>
void AssetListener<A>::handleFileAction(efsw::WatchID watchid,
                                        const std::string& dir,
                                        const std::string& filename,
                                        efsw::Action action,
                                        std::string oldFilename) {

    if (action == efsw::Actions::Modified) {
        const fs::path modified_abs = fs::absolute(dir + '/' + filename);
        for (auto& tracked_asset : tracker->tracked_assets) {
            for (auto const& path : tracked_asset.associated_paths) {
                if (path == modified_abs) {
                    AssetManager<A>::queue_reload(tracked_asset.asset);
                }
            }
        }
    }
}

} // namespace Saturn

#endif // ifndef SATURN_ASSET_TRACKER_HPP_
