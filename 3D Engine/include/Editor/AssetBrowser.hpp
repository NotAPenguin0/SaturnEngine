#ifndef SATURN_ASSET_BROWSER_HPP_
#define SATURN_ASSET_BROWSER_HPP_

#include "Editor/EditorWidget.hpp"

#include <string_view>
#include <vector>

namespace Saturn::Editor {

class AssetBrowser : public EditorWidget {
public:
    AssetBrowser();
    void show();

private:
    std::vector<std::string_view> asset_types;
    int preview_size = 128;
	bool show_editor_assets = false;
};

} // namespace Saturn::Editor

#endif // ifndef SATURN_ASSET_BROWSER_HPP_
